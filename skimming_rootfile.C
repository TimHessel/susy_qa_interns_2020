/* 
Extracting data from root files.
Preselection and branches selection.
10/05/2020
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

void selecting_rootfile(string data[], string path, const char* selection, const char* dir){
  //Applying selection on root file and creating a temporary tree in targeted directory
  int i = 0;
  int n_data = array_size(data);
  for (i = 0; i < n_data; i++){
    //Loading root data
    string filename = (path + (data[i]).c_str());
    const char * c1 = filename.c_str();
    printf("Applying selection on %s...\n", data[i].c_str());
    TFile oldfile(c1, "READ");
    TTree* full_tree = static_cast <TTree*>(oldfile.Get("bdttree"));
    //Writting temporary tree in a new root file
    string file_out = (string(dir) + "tmp_" + data[i]);
    const char * c3 = file_out.c_str();
    TFile newfile(c3, "RECREATE");
    //Applying preselection criteria
    TTree* selected_tree = full_tree->CopyTree(selection);
    selected_tree->Write();
  }
}

void branching_rootfile(string data[], string branches[], const char* dir){
  //Selecting specified branches from 
  int i = 0;
  int n_data = array_size(data);
  int n_branches = array_size(branches);
  for (i = 0; i < n_data; i++){
    //Loading root data
    string filename = (string(dir) + "tmp_" + (data[i]).c_str());
    const char * c1 = filename.c_str();
    printf("Selecting branches from %s...\n", data[i].c_str());
    TFile oldfile(c1, "READ");
    TTree* tmp_tree = static_cast <TTree*>(oldfile.Get("bdttree"));
    //Muting unselected branches
    tmp_tree->SetBranchStatus("*",0);
    int j=0;
    for(j=0; j<n_branches; j++){
     const char * c2 = branches[j].c_str();
     tmp_tree->SetBranchStatus(c2, 1);
    }
    //Writting new tree in a new root file 
    string file_out = (string(dir) + "skimmed_" + data[i]);
    const char * c3 = file_out.c_str();
    TFile newfile(c3, "RECREATE");
    TTree* branched_tree = tmp_tree->CloneTree();
    branched_tree->Write();
    //Deleting temporary file
    remove(c1);
  }
}

void skimming_rootfile(){
  //Path to data
  string stop_path = "/home/t3cms/dbastos/LSTORE/Stop4Body/nTuples16_v2017-10-19/";
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

  //Preselection parameters
  const char* preselection = "(DPhiJet1Jet2 < 2.5 || Jet2Pt < 60) && (Met > 280) && (HT > 200) && (isTight == 1) && (Jet1Pt > 110)";
  //Creating target directory
  const char* directory_name = "skimmed_data/";
  mkdir(directory_name, 0777);
  //Creating new root file
  printf("LOADING SIGNAL :\n");
  selecting_rootfile(data_sig, stop_path, preselection, directory_name);
  branching_rootfile(data_sig, stop_branches, directory_name);
  printf("LOADING BACKGROUND :\n");
  selecting_rootfile(data_bkg, stop_path, preselection, directory_name);
  branching_rootfile(data_bkg, stop_branches, directory_name);
}


