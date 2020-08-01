############################################################################
#11/07/2020 -  WORKING
#train_size20000
#cutoff = 98
#2 folds
#AUGMENT
#Fix_variable = True
############################################################################

#Python imports
import numpy as np
from scipy.optimize import basinhopping
from contextlib import closing
from multiprocessing import Pool
import os
import datetime
import time
#Dwave imports
from dwave.system.samplers import DWaveSampler
from dwave.cloud import Client
import dimod
import dwave_networkx as dnx
from dwave.system.composites import EmbeddingComposite


a_time = 5
nreads = 200
train_size = 1000
start_num = 0
end_num = 10

zoom_factor = 0.5
n_iterations = 8

flip_probs = np.array([0.16, 0.08, 0.04, 0.02] + [0.01]*(n_iterations - 4))
flip_others_probs = np.array([0.16, 0.08, 0.04, 0.02] + [0.01]*(n_iterations - 4))/2
flip_state = -1

AUGMENT_CUTOFF_PERCENTILE = 98
AUGMENT_SIZE = 7   # must be an odd number (since augmentation includes original value in middle)
AUGMENT_OFFSET = 0.007

AUGMENT = True
UPDATING_HAMILTONIAN = True
FIXING_VARIABLES = True

def total_hamiltonian(s, C_i, C_ij):
    bits = len(s)
    h = 0 - np.dot(s, C_i)
    for i in range(bits):
        h += s[i] * np.dot(s[i+1:], C_ij[i][i+1:])
    return h


def anneal(C_i, C_ij, mu, sigma, l, strength_scale, energy_fraction, ngauges, max_excited_states):
    #Initialising h and J as dictionnaries
    h = {}
    J = {}

    for i in range(len(C_i)):
        h_i = -2*sigma[i]*C_i[i]
        for j in range(len(C_ij[0])):
            if j > i:
                J[(i, j)] = float(2*C_ij[i][j]*sigma[i]*sigma[j])
            h_i += 2*(sigma[i]*C_ij[i][j]*mu[j])
        h[i] = h_i

    #applying cutoff
    print("Number of J before : "+str(len(J))) #J before cutoff
    float_vals = []
    for i in J.values():
        float_vals.append(i)
    cutoff = np.percentile(float_vals, AUGMENT_CUTOFF_PERCENTILE)
    to_delete = []
    for k, v in J.items():
        if v < cutoff:
            to_delete.append(k)
    for k in to_delete:
        del J[k]
    print("Number of J after : "+str(len(J))) # J after cutof
    new_Q = {}
    isingpartial = {}

    if FIXING_VARIABLES:
        #Optimising heuristically the number of coupling terms
        Q, _ = dimod.ising_to_qubo(h, J, offset = 0.0)
        bqm = dimod.BinaryQuadraticModel.from_qubo(Q, offset = 0.0)
        simple =  dimod.fix_variables(bqm, sampling_mode = False)
        if simple == {} :
            new_Q = Q
        else :
            Q_indices = []
            for i in Q :
                if i in simple.keys() :
                    continue
                else :
                    Q_indices.append(i)
            new_Q = {key : Q[key] for key in Q_indices}
        print('new length', len(new_Q))
        isingpartial = simple

    if (not FIXING_VARIABLES) or len(new_Q) > 0:
        mapping = []
        offset = 0
        for i in range(len(C_i)):
            if i in isingpartial:
                mapping.append(None)
                offset += 1
            else:
                mapping.append(i - offset)
        if FIXING_VARIABLES:
            new_Q_mapped = {}
            for (first, second), val in new_Q.items():
                new_Q_mapped[(mapping[first], mapping[second])] = val
            h, J, _ = dimod.qubo_to_ising(new_Q_mapped)

        #Run gauges
        qaresults = [] 
        print("Number of variables to anneal :"+str(len(h)))
        for g in range(ngauges):
            #Finding embedding
            qaresult = []
            embedded = False
            for attempt in range(5):
                a = np.sign(np.random.rand(len(h)) - 0.5)
                float_h = []
                for i in h.values():
                    float_h.append(i)
                h_gauge = float_h*a
                J_gauge = {}
                for i in range(len(h)):
                    for j in range(len(h)):
                        if (i, j) in J:
                            J_gauge[(i, j)] = J[(i, j)]*a[i]*a[j]
                try:
                    print("Trying to find embeding")
                    sampler = EmbeddingComposite(DWaveSampler(token='secret_token') )
                    embedded = True
                    break
                except ValueError:      # no embedding found
                    print('no embedding found')
                    embedded = False
                    continue

            if not embedded:
                continue
            print("emebeding found")

            print("Quantum annealing")
            try_again = True
            while try_again:
                try:
                    #Annealing, saving energy and sample list
                    sampleset = sampler.sample_ising(h_gauge, J_gauge, chain_strength = strength_scale, num_reads=200, annealing_time = 20)
                    try_again = False
                except:
                    print('runtime or ioerror, trying again')
                    time.sleep(10)
                    try_again = True
            print("Quantum done")
            
            qaresult.append(sampleset.record[0][0].tolist())
            qaresult = np.asarray(qaresult)
            qaresult = qaresult * a
            qaresults[g*nreads:(g+1)*nreads] = qaresult


        full_strings= np.zeros((len(qaresults),len(C_i)))
        full_strings = np.asarray(full_strings)
        qaresults = np.asarray(qaresults)
        if FIXING_VARIABLES:
            j = 0
            for i in range(len(C_i)):
                if i in isingpartial:
                    full_strings[:, i] = 2*isingpartial[i] - 1
                else:
                    full_strings[:, i] = qaresults[:, j]
                    j += 1
        else:
            full_strings = qaresults

        s = np.asarray(full_strings)
        energies = np.zeros(len(qaresults))
        s[np.where(s > 1)] = 1.0
        s[np.where(s < -1)] = -1.0
        bits = len(s[0])
        for i in range(bits):
            energies += 2*s[:, i]*(-sigma[i]*C_i[i])
            for j in range(bits):
                if j > i:
                    energies += 2*s[:, i]*s[:, j]*sigma[i]*sigma[j]*C_ij[i][j]
                energies += 2*s[:, i]*sigma[i]*C_ij[i][j] * mu[j]

        unique_energies, unique_indices = np.unique(energies, return_index=True)
        ground_energy = np.amin(unique_energies)
        if ground_energy < 0:
            threshold_energy = (1 - energy_fraction) * ground_energy
        else:
            threshold_energy = (1 + energy_fraction) * ground_energy
        lowest = np.where(unique_energies < threshold_energy)
        unique_indices = unique_indices[lowest]
        if len(unique_indices) > max_excited_states:
            sorted_indices = np.argsort(energies[unique_indices])[-max_excited_states:]
            unique_indices = unique_indices[sorted_indices]
        print("unique indices : ", unique_indices)
        print(type(unique_indices[0]))
        print(type(full_strings))
        final_answers = full_strings[unique_indices]
        print('number of selected excited states', len(final_answers))

        return final_answers

    else:
        final_answer = []
        print("Evrything resolved by FIXING_VARIABLES")
        for i in range(len(C_i)):
            if i in isingpartial:
                final_answer.append(2*isingpartial[i] - 1)
        final_answer = np.array(final_answer)
        return np.array([final_answer])


def create_data(sig, bkg, sig_label, bkg_label):
    n_classifiers = sig.shape[1]
    predictions = np.concatenate((np.sign(sig), np.sign(bkg)))
    predictions = np.transpose(predictions) / float(n_classifiers)
    y = np.concatenate((np.ones(len(sig)), -np.ones(len(bkg))))
    tag = np.concatenate((sig_label , bkg_label))

    return predictions, y , tag

def create_augmented_data(sig, bkg, sig_label, bkg_label):
    offset = AUGMENT_OFFSET
    scale = AUGMENT_SIZE

    n_samples = len(sig) + len(bkg)
    n_classifiers = sig.shape[1]
    predictions_raw = np.concatenate((sig, bkg))
    predictions_raw = np.transpose(predictions_raw)
    predictions = np.zeros((n_classifiers * scale, n_samples))
    for i in range(n_classifiers):
        for j in range(scale):
            predictions[i*scale + j] = np.sign(predictions_raw[i] + (j-scale//2)*offset) / (n_classifiers * scale)
    y = np.concatenate((np.ones(len(sig)), -np.ones(len(bkg))))
    tag = np.concatenate((sig_label , bkg_label))
    #print('predictions', predictions)
    return predictions, y , tag

def strong_classifier(predictions, weights):
    return np.dot(predictions.T, weights) 

print('loading data')
sig = np.loadtxt("/workspace/susy_qa_interns_2020/WorkFlow2/AB/CSV/stop_train_sig_wc_AB.csv",delimiter=",", usecols=(3,4,5,6,7,8,9,10,11,12,13,14,16,17,18,19,20,21,22,23,24))
sig_tag = np.loadtxt("/workspace/susy_qa_interns_2020/WorkFlow2/AB/CSV/stop_train_sig_wc_AB.csv",delimiter=",",dtype="str", usecols=(15)) 
bkg = np.loadtxt("/workspace/susy_qa_interns_2020/WorkFlow2/AB/CSV/stop_train_bkg_wc_AB.csv",delimiter=",", usecols=(3,4,5,6,7,8,9,10,11,12,13,14,16,17,18,19,20,21,22,23,24))
bkg_tag = np.loadtxt("/workspace/susy_qa_interns_2020/WorkFlow2/AB/CSV/stop_train_bkg_wc_AB.csv",delimiter=",",dtype="str", usecols=(15))
sig_pct = float(len(sig)) / (len(sig) + len(bkg))
bkg_pct = float(len(bkg)) / (len(sig) + len(bkg))
print('loaded data')

if not os.path.exists("./mus/") :
    os.mkdir("./mus/")
    print("created mus directory")

if not os.path.exists("./energies/") :
    os.mkdir("./energies/")
    print("created energies directory")

if not os.path.exists("./strong_train_predictions/") :
    os.mkdir("./strong_train_predictions/")
    print("created predictions directory")


n_folds = 2
num = 0

sig_indices = np.arange(len(sig))
bkg_indices = np.arange(len(bkg))

remaining_sig = sig_indices
remaining_bkg = bkg_indices
fold_generator = np.random.RandomState(0)
ground_energies=np.zeros((n_folds,n_iterations))
ground_energies_test=np.zeros((n_folds,n_iterations))
for f in range(n_folds):
    if num >= end_num:
        break
    print('fold', f)
    train_sig = fold_generator.choice(remaining_sig, size=int(train_size*sig_pct), replace=False)
    train_bkg = fold_generator.choice(remaining_bkg, size=int(train_size*bkg_pct), replace=False)

    remaining_sig = np.delete(remaining_sig, train_sig)
    remaining_bkg = np.delete(remaining_bkg, train_bkg)

    test_sig = np.delete(sig_indices, train_sig)
    test_bkg = np.delete(bkg_indices, train_bkg)

    if AUGMENT:
        predictions_train, y_train, tag_train = create_augmented_data(sig[train_sig], bkg[train_bkg], sig_tag[train_sig], bkg_tag[train_bkg])
        predictions_test, y_test , tag_test = create_augmented_data(sig[test_sig], bkg[test_bkg], sig_tag[test_sig], bkg_tag[test_bkg])
    else:
        predictions_train, y_train , tag_train= create_data(sig[train_sig], bkg[train_bkg], sig_tag[train_sig], bkg_tag[train_bkg])
        predictions_test, y_test , tag_test = create_data(sig[test_sig], bkg[test_bkg], sig_tag[test_sig], bkg_tag[test_bkg])
    print('Data splited intro train ({} events) and test ({} events)'.format(train_size , len(y_test)))

    if num < start_num:
        num += 1
        continue

    # create C_ij and C_i matrices
    n_classifiers = len(predictions_train)
    test_size = len(y_test)
    C_ij = np.zeros((n_classifiers, n_classifiers))
    C_ij_test= np. zeros ((n_classifiers, n_classifiers))
    C_i = np.dot(predictions_train, y_train)
    C_i_test = np.dot(predictions_test, y_test)
    for i in range(n_classifiers):
        for j in range(n_classifiers):
            C_ij[i][j] = np.dot(predictions_train[i], predictions_train[j])
            C_ij_test[i][j] = np.dot(predictions_test[i], predictions_test[j])
    print('created C_ij and C_i matrices')


    mu0 = np.zeros(n_classifiers)
    sigma0 = np.ones(n_classifiers)
    mu = np.copy(mu0)
    sigma = np.copy(sigma0)
    reg = 0.0
    l0 = reg*np.amax(np.diagonal(C_ij)*sigma*sigma - 2*sigma*C_i)
    strengths = [3.0, 1.0, 0.5, 0.2] + [0.1]*(n_iterations - 4)
    energy_fractions = [0.08, 0.04, 0.02] + [0.01]*(n_iterations - 3)
    gauges = [50, 10] + [10]*(n_iterations - 2)
    max_states =  [1]*(n_iterations)     # cap the number of excited states accepted per iteration

    if UPDATING_HAMILTONIAN:
        mus = [np.zeros(n_classifiers)]
        iterations = n_iterations
        for i in range(iterations):
            print('iteration', i)
            l = reg*np.amax(np.diagonal(C_ij)*sigma*sigma - 2*sigma*C_i)
            new_mus = []
            for mu in mus:
                excited_states = anneal(C_i, C_ij, mu, sigma, l, strengths[i], energy_fractions[i], gauges[i], max_states[i])
                for s in excited_states:
                    new_energy = total_hamiltonian(mu + s*sigma*zoom_factor, C_i, C_ij) / (train_size - 1)
                    flips = np.ones(len(s))
                    for a in range(len(s)):
                        temp_s = np.copy(s)
                        temp_s[a] = 0
                        old_energy = total_hamiltonian(mu + temp_s*sigma*zoom_factor, C_i, C_ij) / (train_size - 1)
                        energy_diff = new_energy - old_energy
                        if energy_diff > 0:
                            flip_prob = flip_probs[i]
                            flip = np.random.choice([1, flip_state], size=1, p=[1-flip_prob, flip_prob])[0]
                            flips[a] = flip
                        else:
                            flip_prob = flip_others_probs[i]
                            flip = np.random.choice([1, flip_state], size=1, p=[1-flip_prob, flip_prob])[0]
                            flips[a] = flip
                    flipped_s = s * flips
                    new_mus.append(mu + flipped_s*sigma*zoom_factor)
            sigma *= zoom_factor
            mus = new_mus
            ground_energies[f,i]=total_hamiltonian(mus[0],C_i,C_ij)/(train_size-1)
            ground_energies_test[f,i] = total_hamiltonian(mus[0],C_i_test,C_ij_test)/(test_size-1)

            np.save('./mus/' +'mus' + str(train_size) + "_fold" + str(f) + '_iter' + str(i) + '.npy', np.array(mus))


    final_predictions_train=[]
    final_predictions_test=[]
    strong_classifier_train = strong_classifier(predictions_train , mus[0])
    strong_classifier_test = strong_classifier(predictions_test , mus[0])
    for i in range(len(tag_train)) :
        final_predictions_train.append([strong_classifier_train[i] , tag_train[i]])

    for i in range(len(tag_test)) :
        final_predictions_test.append([strong_classifier_test[i], tag_test[i]])

    np.save("./strong_train_predictions/prediction_lables_train_f"+str(f)+".npy", final_predictions_train)
    np.save("./strong_train_predictions/prediction_lables_test_f"+str(f)+".npy", final_predictions_test)


    num += 1

np.save("./energies/ground_energies.npy",ground_energies)
np.save("./energies/ground_energies_test.npy",ground_energies_test)



