####################################################
#Program to evaluate correlation between root_tuples.
#Asks the two files and variable to be compared and
#plots the 2D histogram.
#TO BE TESTED
#09/06/2020
###################################################

import numpy as np
import root_numpy
import matplotlib.pyplot as plt

variables_list = {"Jet1Pt" : 0, "Met" : 1, "mt" : 2, "LepPt" : 3, "LepEta" : 4,
                "LepChg" : 5, "HT" : 6, "NbLoose" : 7, "Njet" : 8, "JetHBpt" : 9,
                "DrJetHPLep" : 10, "JetHBCSV" : 11}

def data_loader(data, var) :
    #Import the specified variable from all events of the root fix_variables
    print("Loading" + data)
    #Converting root file as a matrix
    data_imported = root_numpy.root2array(data, treename = "bdttree")
    #Building output table
    variable_tab = []
    for i in range(data_imported.shape[0]) :
        variable_tab.append(data_imported[i][var])
    return variable_tab

def histo_plotter(tab1, tab2) :
    #Plotting 2D histogram
    plt.hist2d(tab1, tab2)
    plt.xlabel(axis1)
    plt.ylabel(axis2)
    plt.show()

#Querying data paths and variable
print("Enter first data path :")
path1 = input()
print("Enter second data path :")
path2 = input()
print("Enter variable to be compared :")
variable = variables_list[input()]
#Loading data
data1 = data_loader(path1, variable)
data2 = data_loader(path2, variable)
#Plotting histogram
histo_plotter(data1, data2)
