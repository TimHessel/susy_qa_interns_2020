####################################################
#Program to evaluate correlation between root_tuples.
#Asks the two files and variable to be compared and
#plots the 2D histogram.
#TO BE TESTED
#10/06/2020
###################################################

import numpy as np
import root_numpy
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm

def data_loader(data, var1, var2) :
    #Import the specified variable from all events of the root fix_variables
    print("Loading" + data)
    #Converting root file as a matrix
    data_imported = root_numpy.root2array(data,branches=[var1,var2], treename = "bdttree")
    #Building output table
    var_tab1 = []
    var_tab2 = []
    for i in range(data_imported.shape[0]) :
        var_tab1.append(data_imported[i][0])
        var_tab2.append(data_imported[i][1])
    variable_tab = [var_tab1, var_tab2]
    return variable_tab

def histo_plotter(tab1, tab2, var1, var2) :
    #Plotting 2D histogram
    fig, (ax1, ax2) = plt.subplots(1, 2)
    nbins=100
    ax1.hist2d(tab1[0], tab1[1],bins=nbins,norm=LogNorm())
    ax2.hist2d(tab2[0], tab2[1],bins=nbins,norm=LogNorm())
    plt.xlabel(var1)
    plt.ylabel(var2)
    plt.show()

#Querying data paths and variable
path = "/home/t3cms/dbastos/LSTORE/Stop4Body/nTuples16_v2017-10-19/"
print("Path to data :" + path)
print("Enter background :")
path1=str(input())
path1 = path + path1 +"*.root"
print("Enter signal :")
path2 = str(input())
path2 = path + path2 +"*.root"
print("Enter first variable :")
variable1 = input()
print("Enter second variable :")
variable2 = input()
#Loading data
data1 = data_loader(path1, variable1, variable2)
data2 = data_loader(path2, variable1, variable2)
#Plotting histogram
histo_plotter(data1, data2, variable1, variable2)
