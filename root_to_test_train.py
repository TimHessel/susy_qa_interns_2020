###########################
#Converter from root to CSV
#22/06/2020
#Using pandas dataframes, load every root tuple and prepare
#a training sample and a testing sample for signal and
#background. Normalising training signal by number of event
#and training background by number of event and cross section.
#WORKING
##########################


import numpy as np
import pandas as pd
import root_numpy
import os
import random

#Path to data
stop_path = "truncated_data/"
#Signal data to be imported
data_train_sig = ["T2DegStop_250_220.root",
            "T2DegStop_275_245.root",
            "T2DegStop_300_270.root",
            "T2DegStop_325_295.root",
            "T2DegStop_350_320.root",
            "T2DegStop_375_345.root",
            "T2DegStop_400_370.root",
            "T2DegStop_425_395.root",
            "T2DegStop_450_420.root",
            "T2DegStop_475_445.root",
            "T2DegStop_500_470.root",
            "T2DegStop_525_495.root",
            #"T2DegStop_550_520.root",
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

data_test_sig = ["T2DegStop_550_520.root"]
#Background data to be imported
data_main_bkg = [#"Wjets_70to100.root",
        #"Wjets_100to200.root",
        "Wjets_200to400.root",
        "Wjets_400to600.root",
        "Wjets_600to800.root",
        "Wjets_800to1200.root",
        "Wjets_1200to2500.root",
        #"Wjets_2500toInf.root",
        "TT_pow.root",#,
        #"ZJetsToNuNu_HT100to200.root",
        "ZJetsToNuNu_HT200to400.root",
        "ZJetsToNuNu_HT400to600.root",
        "ZJetsToNuNu_HT600to800.root",
        "ZJetsToNuNu_HT800to1200.root",
        "ZJetsToNuNu_HT1200to2500.root",
        "ZJetsToNuNu_HT2500toInf.root"
        ]
data_minor_bkg = ["Wjets_70to100.root",
        "Wjets_100to200.root",
        "Wjets_2500toInf.root",
        "WW.root",
        "WZ.root",
        "ZZ.root",
        "DYJetsToLL_M50_HT100to200.root",
        "DYJetsToLL_M50_HT200to400.root",
        "DYJetsToLL_M50_HT400to600.root",
        "DYJetsToLL_M50_HT600to800.root",
        "DYJetsToLL_M50_HT800to1200.root",
        "DYJetsToLL_M50_HT1200to2500.root",
        "DYJetsToLL_M50_HT2500toInf.root",
        "DYJetsToLL_M5to50_HT100to200.root",
        "DYJetsToLL_M5to50_HT200to400.root",
        "DYJetsToLL_M5to50_HT400to600.root",
        "DYJetsToLL_M5to50_HT600toInf.root",
        #"TBar_tch_powheg.root",
        "TBar_tWch_ext.root",
        "T_tch_powheg.root",
        "T_tWch_ext.root"
        ]


def data_loader(data_path, data, XS_norm) :
    #Convert root data into matrix
    data_tag = []
    numbers_of_letters = 4
    for name in data :
        #Converting root data as matrixes
        data_tmp = root_numpy.root2array(data_path + "truncated_" +name,treename="bdttree")
        data_tmp = pd.DataFrame(data_tmp)
        #Concatening loaded data
        if name == data[0] :
            loaded_data = data_tmp
        else :
            loaded_data = loaded_data.append(data_tmp, ignore_index = True)
        #Building an array of corresponding original data set for each event
        j = 0
        for j in range(len(data_tmp)):
            data_tag.append(name[:numbers_of_letters])
    #nevent_indice = 1 #indice of number of events in numpy matrices
    #xs_indice = 2 #indice of cross section in numpy matrices
    i = 0
    weights = [[0]]*len(loaded_data)
    #Weights calculation
    for i in range(len(loaded_data)) :
        if XS_norm == True :
            weights[i] = loaded_data.XS[i]*1000000/loaded_data.Nevt[i]
        else :
            weights[i] = 1000000/loaded_data.Nevt[i]
    loaded_data["Tag"] = data_tag
    return loaded_data, weights

def data_crafter(data, data_weights) :
        #Randomly picking a large amount of events from data according to corresponding weights
        tot_weights = sum(data_weights)
        step_weights = max(data_weights)
        set_size = int(tot_weights/step_weights)
        #Preparing shuffled list of indices
        shuffle_indices = list(range(len(data_weights)))
        random.shuffle(shuffle_indices)
        shuffle_weights = [0]*len(data_weights)
        #Shuffling weights in a new array
        i = 0
        for i in range(len(data_weights)) :
                shuffle_weights[i] = data_weights[shuffle_indices[i]]
        picked_list = []
        j = 0
        evt = 1
        cumul = shuffle_weights[0]
        #Taking indices from shuffled weights array
        for j in range(set_size) :
                picked_list.append(shuffle_indices[evt-1])
                while (j*step_weights >= cumul) or (evt == 1) :
                        cumul += shuffle_weights[evt]
                        evt += 1
                j+=1
        k = 0
        picked_data = data.loc[picked_list].reset_index(drop=True)
        return picked_data

def interpolating(data1, data2, data3, data4) :
        #Interpolation function of two data set to be distributed between -1 and 1
        data = [data1, data2, data3, data4]
	output_data = [data1.copy(), data2.copy(), data3.copy(), data4.copy()]
        full_data = data1.append(data2, ignore_index=True)
        full_data = full_data.append(data3, ignore_index=True)
        full_data = full_data.append(data4, ignore_index=True)
        #Loop accross each variables
        var_ind = 0
        for var_ind in range(2,len(data1.columns)-1) :
                #Calculating max and min values
		var = data1.columns[var_ind]
                max_value = max(full_data[var])
                min_value = min(full_data[var])
                #Applying linear transformation to all events
                i = 0
		print("Interpolating " + var)
                for i in range(len(data)) :
                    j = 0
                    for j in range(len(data[i])) :
			old_val = data[i].at[j, var]
                        new_val = (2*old_val-max_value-min_value)/(max_value-min_value)
			output_data[i].at[j, var] = new_val/12
        return tuple(output_data)

#Creating target folder
csv_folder = "stop_data/"
if not os.path.exists(csv_folder) :
	os.mkdir(csv_folder)

#Converting data to numpy
print("Loading train signal data")
train_sig, train_sig_weights = data_loader(stop_path, data_train_sig, False)
print("Loading test signal data")
test_sig, _ = data_loader(stop_path, data_test_sig, False)
print("Loading main background data")
bkg_main, bkg_main_weights = data_loader(stop_path, data_main_bkg, True)
print("Loading minor background data")
bkg_minor, _ = data_loader(stop_path, data_minor_bkg, False)
print("All data loaded")
#Picking signal events according to calculated weights
print("Preparing signal sample")
picked_train_sig = data_crafter(train_sig, train_sig_weights)
print("Train sig : " + str(len(picked_train_sig)))
picked_test_sig = test_sig.sample(frac=1).reset_index(drop=True)
print("Test sig : " + str(len(picked_test_sig)))
print("Preparing background sample")
bkg_main = bkg_main.sample(frac=1).reset_index(drop=True)
bkg_limit = len(bkg_main)/2
print("Main bkg : " + str(len(bkg_main)))
picked_train_bkg = bkg_main[:bkg_limit].copy()
picked_train_bkg = data_crafter(picked_train_bkg, bkg_main_weights[:bkg_limit])
print("Train bkg : " + str(len(picked_train_bkg)))
picked_test_bkg = bkg_main[bkg_limit:].copy().reset_index(drop=True)
picked_test_bkg = picked_test_bkg.append(bkg_minor, ignore_index=True)
print("Minor bkg : " + str(len(bkg_minor)))
picked_test_bkg = picked_test_bkg.sample(frac=1).reset_index(drop=True)
print("Test bkg : " + str(len(picked_test_bkg)))
#Interpolating variables in final files
output_train_sig, output_train_bkg, output_test_sig, output_test_bkg = interpolating(picked_train_sig, picked_train_bkg, picked_test_sig, picked_test_bkg)
#Writing CSV file in folder
print("Writing CSV files")
output_train_sig.to_csv(csv_folder + 'stop_train_sig.csv', index=False, header=False)
output_train_bkg.to_csv(csv_folder + 'stop_train_bkg.csv', index=False, header=False)
output_test_sig.to_csv(csv_folder + 'stop_test_sig.csv', index=False, header=False)
output_test_bkg.to_csv(csv_folder + 'stop_test_bkg.csv', index=False, header=False)
print("CSV files saved")
