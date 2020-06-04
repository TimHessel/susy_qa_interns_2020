# Importing everything
import os
os.environ['TF_CPP_MIN_LOG_LEVEL']='2'
import numpy as np
import pandas
from sklearn.metrics import confusion_matrix, cohen_kappa_score
from commonFunctions import StopDataLoader, getYields, getYields_diogo, FullFOM
import matplotlib.pyplot as plt
#from scipy.stats import ks_2samp
import localConfig as cfg
#import pickle
from false_prediction import prediction_maker
import root_numpy

if __name__ == "__main__":
    import sys
    # CAlculating FOM
    inputBranches = list(["Jet1Pt", "Met", "mt", "LepPt", "LepEta", "LepChg", "HT", "NbLoose", "Njet", "JetHBpt", "DrJetHBLep", "JetHBCSV", "XS"," weight"])
    preselection = "(DPhiJet1Jet2 < 2.5 || Jet2Pt < 60) && (Met > 280) && (HT > 200) && (isTight == 1) && (Jet1Pt > 110)"
    suffix =""# "_skimmed"
    luminosity = 35866
    number_of_events_print = 1
    test_point = "550_520"
    train_DM = "DM30"
    fraction=1
    dataDev, dataVal = StopDataLoader(cfg.loc, inputBranches, selection=preselection,
                    suffix=suffix, signal=train_DM, test=test_point,
                    fraction=fraction, useSF=True)
    dataVal["NN"] = prediction_maker(dataVal.category, 20)

    tmpSig, tmpBkg = getYields(dataVal)
    sigYield, sigYieldUnc = tmpSig
    bkgYield, bkgYieldUnc = tmpBkg

    sigDataVal = dataVal[dataVal.category==1]
    bkgDataVal = dataVal[dataVal.category==0]

    fomEvo = []
    fomEvo_diogo = []
    fomCut = []

    for cut in np.arange(0.0, 0.9999999, 0.001):
      sig, bkg = getYields(dataVal, cut=cut)
      sign, bkgr = getYields_diogo(dataVal, cut=cut)
      if sig[0] > 0 and bkg[0] > 0:
        fom, fomUnc = FullFOM(sig, bkg)
        fom_diogo, fomUnc_diogo = FullFOM(sign, bkgr)
        fomEvo.append(fom)
        fomEvo_diogo.append(fom_diogo)
        fomCut.append(cut)


    f= open("FOM_evo_data.txt","w+")
    f.write("\n".join(map(str,fomEvo)))
    f.close()

    f= open("FOM_cut_data.txt","w+")
    f.write("\n".join(map(str,fomCut)))
    f.close()

    f= open("FOM_evo_diogo_data.txt","w+")
    f.write("\n".join(map(str,fomEvo_diogo)))
    f.close()

    f= open("FOM_cut_data.txt","w+")
    f.write("\n".join(map(str,fomCut)))
    f.close()
