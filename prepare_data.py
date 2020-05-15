import numpy as np
import root_numpy
import pandas
import os

#Path to data
stop_path = "/home/t3cms/thessel/truncated_data/"
#Signal data to be imported
data_sig = ["truncated_T2DegStop_250_220.root",
            "truncated_T2DegStop_275_245.root",
            "truncated_T2DegStop_300_270.root",
            "truncated_T2DegStop_325_295.root",
            "truncated_T2DegStop_350_320.root",
            "truncated_T2DegStop_375_345.root",
            "truncated_T2DegStop_400_370.root",
            "truncated_T2DegStop_425_395.root",
            "truncated_T2DegStop_450_420.root",
            "truncated_T2DegStop_475_445.root",
            "truncated_T2DegStop_400_370.root",
            "truncated_T2DegStop_425_395.root",
            "truncated_T2DegStop_450_420.root",
            "truncated_T2DegStop_475_445.root",
            "truncated_T2DegStop_500_470.root",
            "truncated_T2DegStop_525_495.root",
            "truncated_T2DegStop_550_520.root",
            "truncated_T2DegStop_575_545.root",
            "truncated_T2DegStop_600_570.root",
            "truncated_T2DegStop_625_595.root",
            "truncated_T2DegStop_650_620.root",
            "truncated_T2DegStop_675_645.root",
            "truncated_T2DegStop_700_670.root",
            "truncated_T2DegStop_725_695.root",
            "truncated_T2DegStop_750_720.root",
            "truncated_T2DegStop_775_745.root",
            "truncated_T2DegStop_800_770.root"
            ]
#Background data to be imported
data_bkg = ["truncated_Wjets_70to100.root",
            "truncated_Wjets_100to200.root",
            "truncated_Wjets_200to400.root",
            "truncated_Wjets_400to600.root",
            "truncated_Wjets_600to800.root",
            "truncated_Wjets_800to1200.root",
            "truncated_Wjets_1200to2500.root",
            "truncated_Wjets_2500toInf.root",
            "truncated_TT_pow.root",
            "truncated_ZJetsToNuNu_HT100to200.root",
            "truncated_ZJetsToNuNu_HT200to400.root",
            "truncated_ZJetsToNuNu_HT400to600.root",
            "truncated_ZJetsToNuNu_HT600to800.root",
            "truncated_ZJetsToNuNu_HT800to1200.root",
            "truncated_ZJetsToNuNu_HT1200to2500.root",
            "truncated_ZJetsToNuNu_HT2500toInf.root"
            ]

#Variables to be imported
inputs_variables = ["Jet1Pt", "Met", "mt", "LepPt", "LepEta", "LepChg",
                    "HT", "NbLoose", "Njet", "DrJetHPLep", "JetHBCSV"]
event_caracters = ["XS", "Nevt"] #Cross section and number of events

stop_branches = inputs_variables + event_caracters


def data_loader(data_path, data, data_features, XS_norm) :
    #Convert root data into DataFrame
    loaded_data = []
    for name in data :
        #Converted root data as DataFrame
        data_tmp = root_numpy.root2array(data_path + name,
                                        treename=bdttree,
                                        branches = data_features
                                        )
        #Updating DataFrame with converted data
        if loaded_data is None :
            loaded_data = pandas.DataFrame(data_tmp)
        else
            loaded_data = loaded_data.append(pandas.DataFrame(data_tmp), ignore_index=True)
    #Normalizing by number of events
    n_tot = loaded_data.sum()["Nevt"]
    loaded_data["Weights"] = loaded_data["Nevt"]/n_tot
    if XS_norm= True :
        #Normalizing by cross section
        XS_tot = loaded_data.sum()["XS"]
        loaded_data["Weights"] = loaded_data["Weights"]*loaded_data["XS"]/XS_tot
    #Selecting data by taking account of weights
    min_weight = loaded_data.min()["Weights"]
    loaded_data["Nevt"] = loaded_data["Nevt"]*loaded_data["Weights"]/min_weight
    return loaded_data


sig = data_loader(stop_path, data_sig, stop_branches, False)
loaded_data.to_csv("signal.csv" , index =False, encoding ="utf-8")
bkg = data_loader(stop_path, bkg_sig, stop_branches, True)
loaded_data.to_csv("background.csv" , index =False, encoding ="utf-8")
