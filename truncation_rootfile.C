/* 
Truncating program for each skimmed root file.
Creating truncated file in truncated_data/ according to furnished criteria of each root file.
TO BE TESTED
11/05/2020
*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "string"
#include "vector"

int array_size(string *tab){
  //calculating size of a string array that has to end with "0"
  int size = 0;
  while(tab[size] != "0"){
    size++;
  }
  return size;
}

void truncate(string data[], string path, const char* dir, string selection[]){
  //Creating root file with selected branched in a target directory
  int i = 0;
  int n_data = array_size(data);
  for (i = 0; i < n_data; i++){
    //Loading root data
    string filename = (path + "skimmed_" + (data[i]).c_str());
    const char * c1 = filename.c_str();
    printf("Loading %s...\n", data[i].c_str());
    TFile oldfile(c1, "READ");
    TTree* full_tree = static_cast <TTree*>(oldfile.Get("bdttree"));
    //Writting new tree in a new root file 
    string file_out = string(dir) + "truncated_" + data[i];
    const char * c2 = file_out.c_str();
    TFile newfile(c2, "RECREATE");
    //Applying corresponding truncation criteria to data
    const char* selec = selection[i].c_str();
    TTree* truncated_tree = full_tree->CopyTree(selec);
    truncated_tree->Write();
  }
}

void truncation_rootfile(){
  //Path to data
  string stop_path = "/home/t3cms/thessel/skimmed_data/";
  //Signal data to be imported
  string data_sig[]= {"T2DegStop_250_220.root",
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
              "T2DegStop_800_770.root",
	      "0"
            };
  //Background data to be imported, regrouped by crossections
  string data_bkg[] = {"Wjets_70to100.root",
              "Wjets_100to200.root",
              "Wjets_200to400.root",
              "Wjets_400to600.root",
              "Wjets_600to800.root",
              "Wjets_800to1200.root",
              "Wjets_1200to2500.root",
              "Wjets_2500toInf.root",
              "TT_pow.root",
	      "0"
              /*"ZJetsToNuNu_HT100to200.root",
              "ZJetsToNuNu_HT200to400.root",
              "ZJetsToNuNu_HT400to600.root",
              "ZJetsToNuNu_HT600to800.root",
              "ZJetsToNuNu_HT800to1200.root",
              "ZJetsToNuNu_HT1200to2500.root",
              "ZJetsToNuNu_HT2500toInf.root"*/
            };

  //Variables to be imported
  string stop_branches[] = {"Jet1Pt", "Met", "mt", "LepPt", "LepEta", "LepChg",
                      "HT", "NbLoose", "Njet", "JetHBpt", "DrJetHBLep", "JetHBCSV","XS", "Nevt", "0"};

  //Truncation criteria arrays for signal and background
  string sig_truncation[]  = {"",
			"",
			""};
  string bkg_truncation[]  = {"",
                        "",
                        "(Jet1PT<1100)&&(Met<800)&&(mt<700)&&(LepPt<500)&&(HT<2000)&&(JetHBpt<900)&&(DRJet<4,6)&&(DRJet>0,1)&&(JetHBCSV>0,04)"}; 
  //Creating target directory
  const char* directory_name = "truncated_data/";
  mkdir(directory_name, 0777);
  //Creating new root file
  printf("TRUNCATING SIGNAL :\n");
  truncate(data_sig, stop_path, directory_name, sig_truncation);
  printf("TRUNCATING BACKGROUND :\n");
  truncate(data_bkg, stop_path, directory_name, bkg_truncation);
  printf("TRUNCATION FINISH\n");
}


