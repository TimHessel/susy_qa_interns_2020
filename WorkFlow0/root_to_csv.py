##############################################
#Converter program from root file to csv file
#Converting program in numpy array and calculating
#weights according to cross sections and number of
#events. Then preparing final set by randomly
#picking events according to their weights and
#interpolating values between -1 and 1.
#WORKING
#23/05/2020
##############################################

import numpy as np
import root_numpy
import os
import random

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
data_bkg = [#"Wjets_70to100.root",
            #"Wjets_100to200.root",
            "Wjets_200to400.root",
            "Wjets_400to600.root",
            "Wjets_600to800.root",
            "Wjets_800to1200.root",
            "Wjets_1200to2500.root",
            #"Wjets_2500toInf.root",
            "TT_pow.root"#,
#            "ZJetsToNuNu_HT100to200.root",
#            "ZJetsToNuNu_HT200to400.root",
#            "ZJetsToNuNu_HT400to600.root",
#            "ZJetsToNuNu_HT600to800.root",
#            "ZJetsToNuNu_HT800to1200.root",
#            "ZJetsToNuNu_HT1200to2500.root",
#            "ZJetsToNuNu_HT2500toInf.root"
            ]

def data_loader(data_path, data) :
	#Convert root data into matrix
	loaded_data = None
	n_event = []
	data_label = []
	for name in data :
		print("Loading " + name)
		#Converting root data as matrixes
		data_tmp = root_numpy.root2array(data_path + "truncated_" +name,treename="bdttree")
		n_tmp = data_tmp.shape[0]
		#Appending event number array
		for evt in range(n_tmp) :
			n_event.append(n_tmp)
		#Concatening loaded data
		if loaded_data is None :
			loaded_data = np.copy(data_tmp)
		else :
			loaded_data = np.concatenate((loaded_data, data_tmp))
		#Building an array of corresponding original data set for each event
		j = 0
		for j in range(data_tmp.shape[0]):
			data_label.append(name)
	nevent_indice = 0 #indice of number of events in numpy matrices
	xs_indice = 1 #indice of cross section in numpy matrices
	i = 0
	weights = np.zeros(loaded_data.shape[0])
	#Weights calculation
	for i in range(loaded_data.shape[0]) :
		weights[i] = loaded_data[i][xs_indice]*1000000/loaded_data[i][nevent_indice]
	return loaded_data, weights, data_label

def data_crafter(data, data_weights, data_label, hist_name) :
	#Randomly picking a large amount of events from data according to corresponding weights
	tot_weights = sum(data_weights)
	step_weights = max(data_weights)
	set_size = int(tot_weights/step_weights)
	#Preparing shuffled list of indices
	shuffle_indices = list(range(len(data_weights)))
	random.shuffle(shuffle_indices)
	shuffle_weights = [0]*len(data_weights)
	data_histo = {}
	#Shuffling weights in a new array
	i = 0
	for i in range(len(data_weights)) :
		shuffle_weights[i] = data_weights[shuffle_indices[i]]
		#Preparing data set histogram
		if (i == 0) or (data_label[i] != data_label[i-1]) :
			data_histo.update({data_label[i] : 0})
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
	picked_data = []
	k = 0
	#Appending array with picked data
	for k in picked_list :
		picked_data.append(data[k])
		#Calculating data histogram
		data_histo[data_label[k]] += 1
	hist_file = open(hist_name,"w")
	hist_file.write( str(data_histo) )
	hist_file.close()
	return picked_data

def interpolating(data1, data2) :
	#Interpolation function of two data set to be distributed between -1 and 1
	output_data1 = np.copy(data1)
	output_data2 = np.copy(data2)
	#Loop accross each variables
	var = 0
	for var in range(len(data1[0])) :
		print("Interpolating variable" + str(val))
		#Collecting values from all data set
		tmp_values = []
		i = 0
		j = 0
		for i in range(len(data1)) :
			tmp_values.append(data1[i][var])
		for j in range(len(data2)) :
			tmp_values.append(data2[j][var])
		#Calculating max and min values
		max_value = max(tmp_values)
		min_value = min(tmp_values)
		#Applying linear transformation to all events
		k = 0
		l = 0
		for k in range(len(data1)) :
			output_data1[k][var] = (2*data1[k][var]-max_value-min_value)/(max_value-min_value)
		for l in range(len(data2)) :
			output_data2[l][var] = (2*data2[l][var]-max_value-min_value)/(max_value-min_value)
	return output_data1, output_data2

#Creating target folder
csv_folder = "stop_data/"
os.mkdir(csv_folder)

#Converting data to numpy
print("Loading data")
sig, sig_weights, sig_label = data_loader(stop_path, data_sig)
bkg, bkg_weights, bkg_label = data_loader(stop_path, data_bkg)
print("Data loaded")
#Picking events according to calculated weights
print("Preparing signal sample")
picked_sig = data_crafter(sig, sig_weights, sig_label, csv_folder+"stop_sig_histo.txt")
print("Preparing background sample")
picked_bkg = data_crafter(bkg, bkg_weights, bkg_label, csv_folder+"stop_bkg_histo.txt")
#Interpolating variables in final files
print("Interpolating values")
output_sig, output_bkg = interpolating(picked_sig, picked_bkg)
#Writing CSV file in folder
print("Writing CSV file")
np.savetxt(csv_folder + 'stop_sig.csv', output_sig, delimiter = ',')
np.savetxt(csv_folder + 'stop_bkg.csv', output_bkg, delimiter = ',')
print("CSV files saved")
