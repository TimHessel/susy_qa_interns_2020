import root_numpy
import numpy as np
#from sklearn.preprocessing import StandardScaler
#from sklearn.externals import joblib
#from sklearn import decomposition

import localConfig as cfg
from commonFunctions import StopDataLoader

fraction = 1

myFeatures = ["Jet1Pt", "Met", "mt", "LepPt", "LepEta", "LepChg", "HT", "NbLoose","Njet", "JetHBpt", "DrJetHBLep", "JetHBCSV"]
inputBranches = list(myFeatures)
inputBranches.append("XS")
inputBranches.append("weight")
preselection = "(DPhiJet1Jet2 < 2.5 || Jet2Pt < 60) && (Met > 280) && (HT > 200) && (isTight == 1) && (Jet1Pt > 110)"
suffix =""# "_skimmed"
luminosity = 35866
number_of_events_print = 1
test_point = "550_520"
train_DM = "DM30"

#print "Loading datasets..."
dataDev, dataVal = StopDataLoader(cfg.loc, inputBranches, selection=preselection,
                    suffix=suffix, signal=train_DM, test=test_point,
                    fraction=fraction, useSF=True)
