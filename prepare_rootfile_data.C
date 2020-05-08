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
#include "TObject.h"
#include "TMVA/Types.h"

int size_str_array(string *tab){
  //Calculates the size of an array of string AS LONG AS it ends with "0"
  int n = 0;
  while(tab[n]!="0"){
    n++;
  }
  return n;
}

void skimming_rootfile(string *data, string path, string *branches, const char* directory){
  //Creates new root files with selected branches in a specified directory
  //Size of data
  int n_data = size_str_array(data);
  int n_branches = size_str_array(branches);
  int i = 0;
  for (i = 0; i < n_data; i++){
    //Loading file
    string filename = (path + data[i]);
    const char * c1 = filename.c_str();
    TFile oldfile(c1, "READ");
    TTree* full_tree = static_cast <TTree*>(oldfile.Get("bdttree"));
    //Muting unselected branches
    full_tree->SetBranchStatus("*",0);
    int j=0;
    for(j=0; j<n_branches; j++){
    //Selecting wanted branches for the new root files
     const char * c2 = branches[j].c_str();
     full_tree->SetBranchStatus(c2, 1);
    }
    //Number of events in a root files 
    Long64_t n_events = full_tree->GetEntries();
    //Declaring event pointer and new tree
    Event *event = new Event();
    full_tree->SetBranchAddress("event", &event);
    TTree* skimmed_tree = full_tree->CloneTree();
    //Preselection
    Long64_t ent = 0;
    for(ent=0; ent<n_events; ent++){
      full_tree->GetEntry(ent);
      if((event->GetDPhiJet1Jet2 < 2.5 || event->GetJet2Pt < 60) && event->GetMET > 280 && event->GetHT > 200 && event->GetisTight == 1 && event->GetJet1Pt > 110){
        skimmed_tree->Fill();
      }
      event->clear();
    }

    //Writing new files in the specified directory
    string file_out = (string(directory) + "trimmed_" + data[i]);
    const char * c3 = file_out.c_str();
    TFile newfile(c3, "RECREATE");
    skimmed_tree->Write();
  }
}

void prepare_rootfile_data(){
  //gSysytem->Load("path_to_file/libEvent") -- We do not know the path_to_file/libEvent
  //Path to data
  string stop_path = "/home/t3cms/dbastos/LSTORE/Stop4Body/nTuples16_v2017-10-19/";
  //Signal data to be imported, needs to end by "0" so that size can be calculated
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
  //Background data to be imported, regrouped by crossections, needs to end with "0" so that size can be calculated
  string data_bkg[] = {"Wjets_70to100.root",
              "Wjets_100to200.root",
              "Wjets_200to400.root",
              "Wjets_400to600.root",
              "Wjets_600to800.root",
              "Wjets_800to1200.root",
              "Wjets_1200to2500.root",
              "Wjets_2500toInf.root",
              "TT_pow.root",
              "ZJetsToNuNu_HT100to200.root",
              "ZJetsToNuNu_HT200to400.root",
              "ZJetsToNuNu_HT400to600.root",
              "ZJetsToNuNu_HT600to800.root",
              "ZJetsToNuNu_HT800to1200.root",
              "ZJetsToNuNu_HT1200to2500.root",
              "ZJetsToNuNu_HT2500toInf.root",
	      "0"
            };

  //Variables to be imported, needs to end with "0" so that size can be calculated
  string stop_branches[] = {"Jet1Pt", "Met", "mt", "LepPt", "LepEta", "LepChg",
                      "HT", "NbLoose", "Njet", "JetHBpt", "DrJetHBLep", "JetHBCSV","XS", "Nevt", "0"};

  //Preselection parameters
  string preselection = "(DPhiJet1Jet2 < 2.5 || Jet2Pt < 60) && (Met > 280) && (HT > 200) && (isTight == 1) && (Jet1Pt > 110)";
  //Creating new root file in a new directory
  const char* directory_name = "skimmed_data/";
  mkdir(directory_name, 0777);
  printf("Loading signal...\n");
  skimming_rootfile(data_sig, stop_path, stop_branches, directory_name);
  printf("Signal loaded\nLoading background...\n");
  skimming_rootfile(data_bkg, stop_path, stop_branches, directory_name);
  printf("Background loaded\n"); 
}
