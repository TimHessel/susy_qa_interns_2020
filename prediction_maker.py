#################################################
#20/06/2020
#Takes mus and computes the strong classifier's predictions with the test sample
#has to be launched above the mus folder
#cvs per fold file columns to be organised as Tag/Nevt/XS/Predictions/label
#################################################
import numpy as np
import os

print("Please enter the total number of folds: ")
n_folds = int(input())

print("loading data")
sig = np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_sig.csv",delimiter=",", usecols=(3,4,5,6,7,8,9,10,11,12,13,14))
sig_tag = np.loadtxt("/home/t3cms/thessel/stop_data/test_test_sig.csv",delimiter=",",dtype="str", usecols=(0))
sig_nevt = np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_sig.csv",delimiter=",", usecols=(1))
sig_XS = np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_sig.csv",delimiter=",", usecols=(2))
bkg = np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_bkg.csv",delimiter=",", usecols=(3,4,5,6,7,8,9,10,11,12,13,14))
bkg_tag = np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_bkg.csv",delimiter=",",dtype="str", usecols=(0))
bkg_nevt = np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_sig.csv",delimiter=",", usecols=(1))
bkg_XS = np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_sig.csv",delimiter=",", usecols=(2))
mus =[]
for i in range(n_folds) :
    mus.append(np.load("./mus/*_fold"+str(i)+"iter_8.npy"))
print("data loaded")


def get_data(sig, bkg, sig_label, bkg_label):
    #Concatenating data and creating label and tag lists
    n_classifiers = sig.shape[1]
    data = np.concatenate((np.sign(sig), np.sign(bkg)))
    data = np.transpose(data) / float(n_classifiers)
    y = np.concatenate((np.ones(len(sig)), -np.ones(len(bkg))))
    tag = np.concatenate((sig_label , bkg_label))
    return data, y, tag

data_test, y_test , tag_test = get_data(sig, bkg, sig_tag, bkg_tag)

#creating new directory to store predictions
if os.path.isdir("./strong_test_predictions/") == False :
    os.mkdir("./strong_test_predictions/")

#Creating and saving prediction
for j in range(n_folds) :
     final_predictions = []
     predictions = np.dot(predictions, mu[j])
     final_predictions.append(tag_test)
     final_predictions.append(sig_nevt+bkg_nevt)
     final_predictions.append(sig_XS+bkg_XS)
     final_predictions.append(predictions)
     final_predictions.append(y_test)
     np.savetxt("./strong_test_predictions/fold_"+str(j)+"_test_predictions.csv", np.transpose(final_predictions))
