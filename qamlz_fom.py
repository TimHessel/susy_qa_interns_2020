##############################################
#Calculating FOM with the QAMLZ (strong) classifier predictions
#sig/bkg events must be separated at one giving indice in the predictions list e.g. : i_sep=sig_tot
#=> predictions[i<=i_sep]<=>sig events and predictions[i>i_sep]<=>bkg events
#NOT COMPLETE, Has to be tested
#29/05/2020
##############################################

import numpy as np
import os

path = "/home/t3cms/thessel/"
sig = np.loadtxt("sig.csv")
bkg = np.loadtxt("bkg_full.csv")         
predictions = np.loadtxt("predictions_strong.csv")
sig_tot = sig.shape[0]
bkg_tot = bkg.shape[0]
if sig_tot+bkg_tot != len(predictions) :
    print("Error : Number of events missmatch")
else :
    for cut in range(-1, 1, 0.01) :
        for i in range(len(predictions)) :
            if prediction[i]>cut :
                selected_data += 1
                if i <= sig_tot :
                    selected_sig += 1
                else :
                    selected_bkg += 1
        #Calculating FOM
        s = selected_sig #???
        b = selected_bkg #???
        FOM1 =  2*(s+b)*log(((s+b)*(b + (0.2*b)**2))/(b**2 + (s + b) * (0.2*b)**2))
        FOM2 =  log(1 + (s*b*b*0.2*0.2)/(b*(b+(0.2*b)**2)))/(0.2**2)
        FOM_tot = (FOM1 - FOM2)**0.5
        FOM.append(FOM_tot)
        FOM_cut.append(cut)
    fom_folder = path + "fom/"
    os.mkdir(fom_folder)
    np.savetxt(fom_folder + "FOM.csv", FOM, delimiter=",")
    np.savetxt(fom_folder + "cut.csv", FOM_cut, delimiter=",")
