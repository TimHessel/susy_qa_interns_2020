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

data_test_sig = ["T2Deg_Stop_550_520.root"]
#Background data to be imported
data_main_bkg = [#"Wjets_70to100.root",
        #"Wjets_100to200.root",
        "Wjets_200to400.root",
        "Wjets_400to600.root",
        "Wjets_600to800.root",
        "Wjets_800to1200.root",
        "Wjets_1200to2500.root",
        #"Wjets_2500toInf.root",
        "TT_pow.root"#,
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
        "TBar_tch_powheg.root",
        "TBar_tWch_ext.root",
        "T_tch_powheg.root",
        "T_tWch_ext.root"
        ]


def data_loader(data_path, data, XS_norm) :
    #Convert root data into matrix
    loaded_data = None
    n_event = []
    data_tag = []
    numbers_of_letters = 4
    for name in data :
        print("Loading " + name)
        #Converting root data as matrixes
        data_tmp = root_numpy.root2array(data_path + "truncated_" +name,treename="bdttree")
        data_tmp = pd.DataFrame(data_tmp, ignore_index=True)
        n_tmp = len(data_tmp)
        #Appending event number array
        for evt in range(n_tmp) :
            n_event.append(n_tmp)
            #Concatening loaded data
            if loaded_data is None :
                loaded_data = data_tmp
            else :
                loaded_data.append(data_tmp)
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
        if XS_norm = True :
            weights[i] = loaded_data.XS[i]*1000000/loaded_data.NEvent[i]
        else :
            weights[i] = 1000000/loaded_data.NEvent[i]
    loaded_data["Tag"] = data_tag
    return loaded_data, weights, data_label

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
        picked_list = None
        #Appending array with picked data
        for k in picked_list :
                if picked_list == None :
                    picked_list = data[k]
                else :
                    picked_data.append(data[k], ignore_index=True)
        return picked_data

def interpolating(data1, data2, data3, data4) :
        #Interpolation function of two data set to be distributed between -1 and 1
        data = [data1, data2, data3, data4]
        full_data = data1.append(data2, data3, data4, ignore_index=True)
        #Loop accross each variables
        var = 0
        for var in range(3,len(data1[0])) :
                #Calculating max and min values
                max_value = max(full_data[var])
                min_value = min(full_data[var])
                #Applying linear transformation to all events
                i = 0
                for i in range(len(data))
                    j = 0
                    for j in range(len(data1)) :
                        data[var][k] = (2*data[var][k]-max_value-min_value)/(max_value-min_value)
        return tuple(data)

#Creating target folder
csv_folder = "stop_data/"
os.mkdir(csv_folder)

#Converting data to numpy
print("Loading data")
train_sig, train_sig_weights = data_loader(stop_path, data_train_sig, False)
test_sig, _ = data_loader(stop_path, data_test_sig, False)
bkg_main, bkg_main_weights = data_loader(stop_path, data_main_bkg, True)
bkg_minor, _ = data_loader(stop_path, data_minor_bkg, False)
print("Data loaded")
#Picking signal events according to calculated weights
print("Preparing signal sample")
picked_train_sig = data_crafter(train_sig, train_sig_weights)
picked_test_sig = test_sig.sample(frac=1).reset_index(drop=True)
print("Preparing background sample")
bkg_main = bkg_main.sample(frac=1).reset_index(drop=True)
bkg_limit = len(bkg_main)/2
picked_train_bkg = bkg_main[:bkg_limit]
picked_train_bkg = data_crafter(picked_train_bkg, bkg_main_weights[:bkg_limit])
picked_test_bkg = bkg_main[bkg_limit:].append(bkg_minor, ignore_index=True)
picked_test_bkg = picked_test_bkg.sample(frac=1).reset_index(drop=True)
#Interpolating variables in final files
print("Interpolating values")
output_train_sig, output_train_bkg, output_test_sig, output_test_bkg = interpolating(picked_train_sig, picked_train_bkg, picked_test_sig, picked_test_bkg)
#Writing CSV file in folder
print("Writing CSV file")
output_train_sig.to_csv(csv_folder + 'stop_train_sig.csv')
output_train_bkg.to_csv(csv_folder + 'stop_train_bkg.csv')
output_test_sig.to_csv(csv_folder + 'stop_test_sig.csv')
output_test_bkg.to_csv(csv_folder + 'stop_test_bkg.csv')
print("CSV files saved")
-- INSERT --                                                                       58,37         14%
