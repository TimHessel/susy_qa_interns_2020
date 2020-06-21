#################################################################################################################
#21/06/2020 - Working
#Takes mus and computes the strong classifier's predictions with the test sample
#has to be launched above the mus folder
#A csv file is created for each fold (for the last iteration).
#Those cvs are organised in the following way: 4 columns : Tag/Nevt/XS/Strong Classifier's predictions/label and one line per event
################################################################################################################
import numpy as np
import os

print("Please enter the total number of folds : ")
n_folds = int(input())
print("Please enter the total number of train size : ")
train_size = int(input())

print("loading data")
sig = np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_sig.csv",delimiter=",", usecols=(3,4,5,6,7,8,9,10,11,12,13,14))
sig_tag = np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_sig.csv",delimiter=",",dtype="str", usecols=(0,))
sig_nevt = np.ndarray.tolist(np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_sig.csv",delimiter=",", usecols=(1,)))
sig_XS =np.ndarray.tolist(np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_sig.csv",delimiter=",", usecols=(2,)))
bkg = np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_bkg.csv",delimiter=",", usecols=(3,4,5,6,7,8,9,10,11,12,13,14))
bkg_tag = np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_bkg.csv",delimiter=",",dtype="str", usecols=(0,))
bkg_nevt = np.ndarray.tolist(np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_bkg.csv",delimiter=",", usecols=(1,)))
bkg_XS = np.ndarray.tolist(np.loadtxt("/home/t3cms/thessel/stop_data/stop_test_bkg.csv",delimiter=",", usecols=(2,)))
mus =[]
for i in range(n_folds) :
    mus.append(np.load("/home/t3cms/oudot/QAZ/WorkFlow1/qamlz/NO_AUGMENT/New-Csv/qaz_train1000_2/mus/mus"+str(train_size)+"_fold"+str(i)+"_iter7.npy"))
print("data loaded")

def get_data(sig, bkg, sig_label, bkg_label):
    #Concatenating data and creating data, label and tag lists
    n_classifiers = sig.shape[1]
    data = np.concatenate((np.sign(sig), np.sign(bkg)))/float(n_classifiers)
    y = np.concatenate((np.ones(len(sig)), -np.ones(len(bkg))))
    tag = np.concatenate((sig_label , bkg_label))
    return np.ndarray.tolist(data), np.ndarray.tolist(y), np.ndarray.tolist(tag)

def get_classifier(data, weights) :
    #Computing the strong classifier predictions by multiplying weak classifiers and mus
    r =[]
    for i in range(len(data)) :
        tmp = 0
        for var in range(len(weights[0])) :
            tmp += data[i][var]*float(weights[0][var])
        r.append(tmp)
    print(type(tmp))
    return r
        
#creating lists of weak classifiers, labels and tags
data_test, y_test , tag_test = get_data(sig, bkg, sig_tag, bkg_tag)

#creating new directory to store predictions
if os.path.isdir("./strong_test_predictions/") == False :
    os.mkdir("./strong_test_predictions/")

#Creating and saving prediction
predictions = []
for j in range(n_folds) :
     #print("fold"+str(j))
     final_predictions = []
     predictions = get_classifier(data_test, mus[j]) 
     #print("tag shape :", len(tag_test))
     final_predictions.append(tag_test)
     #print("nevt shape :", len(sig_nevt+bkg_nevt))
     final_predictions.append(sig_nevt+bkg_nevt)
     #print("XS shape :", len(sig_XS+bkg_XS))
     final_predictions.append(sig_XS+bkg_XS)
     #print("prediction shape :", len(predictions))
     final_predictions.append(predictions)
     #print("labels shape :", len(y_test))
     final_predictions.append(y_test)
     np.savetxt("./strong_test_predictions/fold"+str(j)+"_test_predictions.csv", np.transpose(final_predictions), delimiter = ',', fmt = '%s')
