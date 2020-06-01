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

stepcut = 0.01
FOM_matrix = np.zeros(2, int(2/stepcut))
ind = 0

if sig_tot+bkg_tot != len(predictions) :
    print("Error : Number of events missmatch")
else :
    for cut in range(-1, 1, stepcut) :
        s = 0
        b = 0
        i = 0
        for i in range(len(predictions)) :
            if prediction[i]>cut :
                selected_data.append(i) #List of indices of the selected data
                if i <= sig_tot :
                    selected_sig.append(i)
                else :
                    selected_bkg.append(i)
        #Computing Yields for fom calculation
        j = 0
        for j in range(selected_sig) :
            s += sig[j, 1]*35866*2/sig[j, 0] #weight = XS*Luminosity*Splitfactor/Nevt
        k = 0
        for k in range(selected_bkg) :
            b += bkg[k, 1]*35866*2/bkg[k, 0] #weight = XS*Luminosity*Splitfactor/Nevt
        #Calculating FOM
        FOM1 =  2*(s+b)*log(((s+b)*(b + (0.2*b)**2))/(b**2 + (s + b) * (0.2*b)**2))
        FOM2 =  log(1 + (s*b*b*0.2*0.2)/(b*(b+(0.2*b)**2)))/(0.2**2)
        FOM_tot = (FOM1 - FOM2)**0.5
        FOM_matrix[1, ind] = FOM_tot
        FOM_matrix[0, ind] = cut
        ind += 1
    fom_folder = path + "fom/"
    os.mkdir(fom_folder)
    np.savetxt(fom_folder + "FOM.csv", FOM_matrix, delimiter=",")
