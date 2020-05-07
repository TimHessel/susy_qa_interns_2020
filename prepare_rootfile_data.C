#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <sstream>

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

int main void (){
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
              "T2DegStop_800_770.root"
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
                      "HT", "NbLoose", "Njet", "JetHBpt", "DrJetHPLep", "JetHBCSV","XS", "Nevt"};

  //Preselection parameters
  string preselection = "(DPhiJet1Jet2 < 2.5 || Jet2Pt < 60) && (Met > 280) && (HT > 200) && (isTight == 1) && (Jet1Pt > 110)";
  //creating new root file test number 1
  int i = 0;
  for (i = 0; i < sizeof(data_bkg); i++) {
    string filename = stop_path + data_bkg[i];
    TFile oldfile(filename);
    TTree *bdttree;
    oldfile.GetObject("t", bdttree);
    bdttree->SetBranchStatus("*",0);

    for(auto Branch : stop_branches){
      bdttree->SetBranchStatus(Branch,1);
    }
    TFile newfile("skimmed_" + data_bkg[i], recreate);
    newtree->Print();
    newfile.Write();
  }
}
