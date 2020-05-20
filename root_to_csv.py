##############################################
#Converter program from root file to csv file
#Converting program in numpy array and calculating
#weights according to cross sections and number of
#events. Then preparing final set by randomly
#picking events according to their weights.
#TO BE TESTED
#20/05/2020
##############################################

import numpy as np
import root_numpy
import os

#Path to data
stop_path = "truncated_data/"
#Signal data to be imported
data_sig = ["T2DegStop_250_220.root",
            "T2DegStop_275_245.root",
            "T2DegStop_300_270.root",
            "T2DegStop_325_295.root",
            "T2DegStop_350_320.root",
            "T2DegStop_375_345.root",
            "T2DegStop_400_370.root",
            "T2DegStop_425_395.root",
            "T2DegStop_450_420.root",
            "T2DegStop_475_445.root",
            "T2DegStop_400_370.root",
            "T2DegStop_425_395.root",
            "T2DegStop_450_420.root",
            "T2DegStop_475_445.root",
            "T2DegStop_500_470.root",
            "T2DegStop_525_495.root",
            "T2DegStop_550_520.root",
            "T2DegStop_575_545.root",
            "T2DegStop_600_570.root",
            "T2DegStop_625_595.root",
            "T2DegStop_650_620.root",
            "T2DegStop_675_645.root",
            "T2DegStop_700_670.root",
            "T2DegStop_725_695.root",
            "T2DegStop_750_720.root",
            "T2DegStop_775_745.root",
            "T2DegStop_800_770.root"
            ]
#Background data to be imported
data_bkg = ["Wjets_70to100.root",
            "Wjets_100to200.root",
            "Wjets_200to400.root",
            "Wjets_400to600.root",
            "Wjets_600to800.root",
            "Wjets_800to1200.root",
            "Wjets_1200to2500.root",
            "Wjets_2500toInf.root",
            "TT_pow.root",
            "ZJetsToNuNu_HT100to200.root",
            "ZJetsToNuNu_HT200to400.root",
            "ZJetsToNuNu_HT400to600.root",
            "ZJetsToNuNu_HT600to800.root",
            "ZJetsToNuNu_HT800to1200.root",
            "ZJetsToNuNu_HT1200to2500.root",
            "ZJetsToNuNu_HT2500toInf.root"
            ]

def data_loader(data_path, data, pre_selection) :
    #Convert root data into matrixes
    loaded_data = []
    n_event = []
    for name in data :
        #Converting root data as matrixes
        data_tmp = root_numpy.root2array(data_path + "truncated_" +name,treename=bdttree)
        n_tmp = data_tmp.shape[0]
        #Appending event number array
        for evt in range (n_tmp) :
            n_event.append(n_tmp)
        #Concatening loaded data
        if loaded_data is None :
            loaded_data = np.copy(data_tmp)
        else :
            loaded_data = np.append(loaded_data, data_tmp)
    xs_indice = 1 #indice of cross section in numpy arrays
    #Normalizing by number of event inside each cross section
    i=0
    XS_tot=0
    weights = np.zeros(loaded_data.shape[1])
    while i<loaded_data[xs_indice].size :
        #XS loop, each time i changes, jump to another cross section group of events
        j=i
        #calculating total cross section
        XS_tot += loaded_data[xs_indice, i]
        while loaded_data[xs_indice, i] == loaded_data[xs_indice, j] :
            j+=1
        #weights calculation
        for k in range (i,j) :
            weights[k] = loaded_data[xs_indice, k]/n_event[k]
        i=j+1
    #Optimising the number of events so that the minimun of Nevt is 1
    return loaded_data, weights

def data_crafter(data, data_weights) :
    #Randomly picking a large amount of events from data according to corresponding weights
    tot_weights = sum(data_weights)
    set_size = len(data_weights)
    #Evaluation of minimal size of the set
    min_size = len(data_weights)*0.1
    while set_size > min_size :
        picked_data = []
        picked_indices = []
        for i in range(set_size) :
            x = random.uniform(0, tot_weights) #random variable
            evt = 0
            #Rolling events until cumuled weights reaches x
            cumul = data_weights[0]
            while cumul < x :
                evt++
                cumul += data_weights[evt]
            #Checking if the event has already been picked
            if evt in picked_indices :
                break
            else :
                picked_indices.append(evt)
            #appending output numpy array
            if picked_data is None :
                picked_data = np.copy(data[evt])
            else :
                picked_data = np.append(picked_data, data[evt])
        #Checking if output is large enough
        if len(picked_indices) = set_size :
            return picked_data
            break
        #Otherwise reducing size and redoing the randomization
        else :
            set_size = int(0.95*set_size)

#Creating target folder
csv_folder = "stop_data"
os.mkdir(csv_folder, 0777)

#Converting data to numpy
sig, sig_weights = data_loader(stop_path, data_sig, stop_branches, preselection)
bkg, bkg_weights = data_loader(stop_path, bkg_sig, stop_branches, preselection)

#Picking events according to calculated weights
picked_sig = data_crafter(sig, sig_weights)
picked_bkg = data_crafter(bkg, bkg_weights)

#Writing CSV file in folder
savetxt(csv_folder + 'stop_sig.csv', picked_sig, delimiter = ',')
savetxt(csv_folder + 'stop_bkg.csv', picked_bkg, delimiter = ',')
