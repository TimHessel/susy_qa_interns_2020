import os
import pandas as pd
from sklearn.decomposition import PCA
import matplotlib.pyplot as plt 

name="/home/t3cms/thessel/Workflow2/stop_data/stop_train_sig_wc_A.csv"
name_bkg="/home/t3cms/thessel/Workflow2/stop_data/stop_train_bkg_wc_A.csv"
drop_cols=[0,1,2,15]
names = [i for i in range(21)]
#columns=[] #list of columns we want to take
file_df_sig=pd.read_csv(name, sep=",",names=names)
tmp_df_sig = file_df_sig.drop(drop_cols, axis=1)

file_df_bkg = pd.read_csv(name_bkg, sep=",",names=names)
tmp_df_bkg  = file_df_bkg.drop(drop_cols, axis=1)

tmp_df = pd.concat([tmp_df_sig , tmp_df_bkg] , ignore_index=True)



#fig , ax = plt.subplots()
#tmp_df.hist(bins=10,ax=ax)
#fig.savefig("before_pca.pdf")

pca=PCA(n_components=len(tmp_df.columns)).fit_transform(tmp_df)



pca_df = pd.DataFrame(data=pca, columns=tmp_df.columns)

#fig , ax = plt.subplots()
#df.hist(bins=10,ax=ax)
#fig.savefig("after_pca.pdf")

final_df= pd.concat([file_df_sig , file_df_bkg] , ignore_index=True)

#print("Before PCA" , final_df)

for i in pca_df.columns :
	final_df[i]=pca_df[i]
	
#print("After PCA" , final_df)

cut=len(file_df_sig.index)


final_df.iloc[:cut].to_csv("pca_stop_train_sig_wc_A.csv",header= False,index=False)
final_df.iloc[cut:].to_csv("pca_stop_train_bkg_wc_A.csv",header= False , index =False)
