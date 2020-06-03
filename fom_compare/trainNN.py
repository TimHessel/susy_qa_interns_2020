# Importing everything
import os
os.environ['TF_CPP_MIN_LOG_LEVEL']='2'
import numpy
import time
import pandas
from sklearn.metrics import confusion_matrix, cohen_kappa_score
from commonFunctions import getYields, FullFOM, myClassifier, gridClassifier, getDefinedClassifier, assure_path_exists, plotter, NNarch
import matplotlib.pyplot as plt
#from scipy.stats import ks_2samp
import localConfig as cfg
#import pickle


if __name__ == "__main__":
    import sys
    # CAlculating FOM
    dataVal["NN"] = valPredict

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
        fomEvo_diogo.append(sign, bkgr)
        fomCut.append(cut)


    f= open(plots_path+"FOM_evo_data.txt","w+")
    f.write("\n".join(map(str,fomEvo)))
    f.close()

    f= open(plots_path+"FOM_cut_data.txt","w+")
    f.write("\n".join(map(str,fomCut)))
    f.close()

    f= open(plots_path+"FOM_evo_diogo_data.txt","w+")
    f.write("\n".join(map(str,fomEvo_diogo)))
    f.close()

    f= open(plots_path+"FOM_cut_diogo_data.txt","w+")
    f.write("\n".join(map(str,fomCut)))
    f.close()
