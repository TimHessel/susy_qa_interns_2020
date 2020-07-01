/*
Truncating program for each skimmed root file.
Creating truncated file in truncated_data/ according to furnished criteria of each root file.
Fill with truncation critera, with symmetric cuts on LepEta
WORKING
15/05/2020
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
  int n_selection = array_size(selection);
  if (n_data!=n_selection){
    printf("Error : not matching data set and selection\n");
    return;
  }
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
  string sig_truncation[]  =
  {"(Jet1Pt<1000)&&(Met<920)&&(mt<210)&&(LepPt<78)&&(HT<1600)&&(JetHBpt<800)&&(DrJetHBLep<4.8)&&(DrJetHBLep>0)&&(JetHBCSV>0.045)&&(LepEta>-2.4)&&(LepEta<2.4)", //Stop_250_220
	"(Jet1Pt<1000)&&(Met<1050)&&(mt<228)&&(LepPt<72)&&(HT<1550)&&(JetHBpt<900)&&(DrJetHBLep<4.8)&&(DrJetHBLep>0.008)&&(JetHBCSV>0.04)&&(LepEta>-2.4)&&(LepEta<2.4)", //Stop_275_245
  "(Jet1Pt<1060)&&(Met<1050)&&(mt<370)&&(LepPt<78)&&(HT<1560)&&(JetHBpt<865)&&(DrJetHBLep<4.9)&&(DrJetHBLep>0)&&(JetHBCSV>0.4)&&(LepEta>-2.4)&&(LepEta<2.4)", //Stop_300_270
  "(Jet1Pt<1250)&&(Met<1050)&&(mt<330)&&(LepPt<75)&&(HT<1640)&&(JetHBpt<1000)&&(DrJetHBLep<5)&&(DrJetHBLep>0)&&(JetHBCSV>0)&&(LepEta>-2.5)&&(LepEta<2.5)", //Stop_325_295
  "(Jet1Pt<1240)&&(Met<1340)&&(mt<285)&&(LepPt<74)&&(HT<1900)&&(JetHBpt<1200)&&(DrJetHBLep<5)&&(DrJetHBLep>0)&&(JetHBCSV>0.05)&&(LepEta>-2.35)&&(LepEta<2.35)", //Stop_350_320
  "(Jet1Pt<1400)&&(Met<1300)&&(mt<400)&&(LepPt<69)&&(HT<1900)&&(JetHBpt<1100)&&(DrJetHBLep<5.1)&&(DrJetHBLep>0)&&(JetHBCSV>0.052)&&(LepEta>-2.4)&&(LepEta<2.4)", //Stop_375_345
  "(Jet1Pt<1200)&&(Met<1350)&&(mt<350)&&(LepPt<74)&&(HT<1700)&&(JetHBpt<1000)&&(DrJetHBLep<5)&&(DrJetHBLep>0)&&(JetHBCSV>0.04)&&(LepEta>-2.5)&&(LepEta<2.5)", //Stop_400_370
  "(Jet1Pt<1300)&&(Met<1375)&&(mt<250)&&(LepPt<60)&&(HT<1900)&&(JetHBpt<1080)&&(DrJetHBLep<4.7)&&(DrJetHBLep>0)&&(JetHBCSV>0.04)&&(LepEta>-2.3)&&(LepEta<2.3)", //Stop_425_395
  "(Jet1Pt<1200)&&(Met<1300)&&(mt<260)&&(LepPt<53)&&(HT<1575)&&(JetHBpt<1000)&&(DrJetHBLep<4.9)&&(DrJetHBLep>0.15)&&(JetHBCSV>0.04)&&(LepEta>-2.4)&&(LepEta<2.4)", //Stop_450_420
  "(Jet1Pt<1060)&&(Met<1100)&&(mt<190)&&(LepPt<45)&&(HT<1400)&&(JetHBpt<900)&&(DrJetHBLep<4.5)&&(DrJetHBLep>0.4)&&(JetHBCSV>0.045)&&(LepEta>-2.5)&&(LepEta<2.5)", //Stop_475_445
  "(Jet1Pt<1050)&&(Met<1000)&&(mt<200)&&(LepPt<49)&&(HT<1450)&&(JetHBpt<900)&&(DrJetHBLep<4.9)&&(DrJetHBLep>0.4)&&(JetHBCSV>0.04)&&(LepEta>-2.5)&&(LepEta<2.5)", //Stop_500_470
  "(Jet1Pt<1000)&&(Met<1000)&&(mt<220)&&(LepPt<48)&&(HT<1400)&&(JetHBpt<800)&&(DrJetHBLep<4.5)&&(DrJetHBLep>0.4)&&(JetHBCSV>0.04)&&(LepEta>-2.5)&&(LepEta<2.5)", //Stop_525_495
  "(Jet1Pt<1050)&&(Met<1125)&&(mt<195)&&(LepPt<43)&&(HT<1375)&&(JetHBpt<820)&&(DrJetHBLep<4.5)&&(DrJetHBLep>0.4)&&(JetHBCSV>0.045)&&(LepEta>-2.3)&&(LepEta<2.3)", //Stop_550_520
  "(Jet1Pt<1050)&&(Met<1125)&&(mt<195)&&(LepPt<43)&&(HT<1375)&&(JetHBpt<820)&&(DrJetHBLep<4.3)&&(DrJetHBLep>0.1)&&(JetHBCSV>0.0575)&&(LepEta>-2.3)&&(LepEta<2.3)", //Stop_575_545
  "(Jet1Pt<1000)&&(Met<1150)&&(mt<250)&&(LepPt<45)&&(HT<1300)&&(JetHBpt<950)&&(DrJetHBLep<4.4)&&(DrJetHBLep>0.2)&&(JetHBCSV>0.04)&&(LepEta>-2)&&(LepEta<2)", //Stop_600_570
  "(Jet1Pt<1100)&&(Met<1200)&&(mt<250)&&(LepPt<38)&&(HT<1700)&&(JetHBpt<800)&&(DrJetHBLep<4.2)&&(DrJetHBLep>0.3)&&(JetHBCSV>0)&&(LepEta>-2)&&(LepEta<2)", //Stop_625_595
  "(Jet1Pt<975)&&(Met<1100)&&(mt<260)&&(LepPt<40)&&(HT<1600)&&(JetHBpt<975)&&(DrJetHBLep<4)&&(DrJetHBLep>0.2)&&(JetHBCSV>0)&&(LepEta>-2)&&(LepEta<2)", //Stop_650_620
  "(Jet1Pt<950)&&(Met<1150)&&(mt<250)&&(LepPt<46)&&(HT<1350)&&(JetHBpt<1025)&&(DrJetHBLep<4)&&(DrJetHBLep>0.2)&&(JetHBCSV>0)&&(LepEta>-1.7)&&(LepEta<1.7)", //Stop_675_645
  "(Jet1Pt<1000)&&(Met<1250)&&(mt<250)&&(LepPt<42)&&(HT<1550)&&(JetHBpt<830)&&(DrJetHBLep<4.2)&&(DrJetHBLep>0.3)&&(JetHBCSV>0.04)&&(LepEta>-1.7)&&(LepEta<1.7)", //Stop_700_670
  "(Jet1Pt<1100)&&(Met<1300)&&(mt<225)&&(LepPt<32)&&(HT<1490)&&(JetHBpt<830)&&(DrJetHBLep<4.2)&&(DrJetHBLep>0)&&(JetHBCSV>0.04)&&(LepEta>-2)&&(LepEta<2)", //Stop_725_695
  "(Jet1Pt<925)&&(Met<1280)&&(mt<250)&&(LepPt<42)&&(HT<1500)&&(JetHBpt<950)&&(DrJetHBLep<4.1)&&(DrJetHBLep>0)&&(JetHBCSV>0.03)&&(LepEta>-2.1)&&(LepEta<2.1)", //Stop_750_720
  "(Jet1Pt<975)&&(Met<980)&&(mt<250)&&(LepPt<40)&&(HT<1400)&&(JetHBpt<1000)&&(DrJetHBLep<3.8)&&(DrJetHBLep>0.2)&&(JetHBCSV>0)&&(LepEta>-2)&&(LepEta<2)", //Stop_775_745
  "(Jet1Pt<1025)&&(Met<1200)&&(mt<250)&&(LepPt<42)&&(HT<1450)&&(JetHBpt<900)&&(DrJetHBLep<4.1)&&(DrJetHBLep>0.15)&&(JetHBCSV>0.05)&&(LepEta>-2)&&(LepEta<2)", //Stop_800_770
  "0"};

  string bkg_truncation[]  =
  {"", //Wjets_70to100
	"(Jet1Pt<700)&&(Met<800)&&(mt<700)&&(LepPt<130)&&(HT<800)&&(JetHBpt<750)&&(JetHBCSV>0)", //Wjets_100to200
  "(Jet1Pt<505)&&(Met<575)&&(mt<900)&&(LepPt<220)&&(HT<640)&&(JetHBpt<505)&&(JetHBCSV>0)&&(DrJetHBLep>0.02)", //Wjets_200to400
  "(Jet1Pt<700)&&(Met<675)&&(mt<650)&&(LepPt<355)&&(HT<900)&&(JetHBpt<680)&&(DrJetHBLep<5.3)&&(DrJetHBLep>0)&&(JetHBCSV>0)&&(LepEta>-2.5)&&(LepEta<2.5)", //Wjets_400to600
  "(Jet1Pt<925)&&(Met<925)&&(mt<1050)&&(LepPt<560)&&(HT<1300)&&(JetHBpt<960)&&(DrJetHBLep<5.6)&&(DrJetHBLep>0.2)&&(JetHBCSV>0)&&(LepEta>-2.5)&&(LepEta<2.5)", //Wjets_600to800
  "(Jet1Pt<1250)&&(Met<1200)&&(mt<1275)&&(LepPt<880)&&(HT<1675)&&(JetHBpt<1300)&&(DrJetHBLep<5.1)&&(DrJetHBLep>0.2)&&(JetHBCSV>0)&&(LepEta>-2.5)&&(LepEta<2.5)", //Wjets_800to1200
  "(Jet1Pt>180)&&(Jet1Pt<2300)&&(Met<1860)&&(mt<1450)&&(LepPt<1450)&&(HT<2700)&&(JetHBpt<2250)&&(DrJetHBLep<5.1)&&(DrJetHBLep>0.2)&&(JetHBCSV>0)&&(LepEta>-2.5)&&(LepEta<2.5)", //Wjets_1200to2500
  "(Jet1Pt>540)&&(Jet1Pt<2800)&&(Met<2700)&&(mt<1550)&&(LepPt<2175)&&(HT>700)&&(HT<4700)&&(JetHBpt<3100)&&(DrJetHBLep<5)&&(DrJetHBLep>0)&&(JetHBCSV>0)&&(LepEta>-2.4)&&(LepEta<2.4)", //Wjets_2500toInf
  "(Jet1Pt<1100)&&(Met<800)&&(mt<700)&&(LepPt<500)&&(HT<2000)&&(JetHBpt<900)&&(DrJetHBLep<4.6)&&(DrJetHBLep>0.1)&&(JetHBCSV>0.04)", //TT_pow
  "(JetHBCSV>0)", //ZJetsToNuNu_HT100to200
  "(Jet1Pt<420)&&(Met<425)&&(mt<145)&&(LepPt<24)&&(HT<450)&&(JetHBpt<410)&&(DrJetHBLep<4.8)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //ZJetsToNuNu_HT200to400
  "(Jet1Pt<570)&&(Met<600)&&(mt<540)&&(LepPt<230)&&(HT<720)&&(JetHBpt<600)&&(DrJetHBLep<5.2)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //ZJetsToNuNu_HT400to600
  "(Jet1Pt<740)&&(Met<800)&&(mt<860)&&(LepPt<430)&&(HT<950)&&(JetHBpt<750)&&(DrJetHBLep<5.2)&&(DrJetHBLep>0)&&(JetHBCSV>0.04)", //ZJetsToNuNu_HT600to800
  "(Jet1Pt<1000)&&(Met<1020)&&(mt<1080)&&(LepPt<580)&&(HT<1300)&&(JetHBpt<980)&&(DrJetHBLep<5.2)&&(DrJetHBLep>0)&&(JetHBCSV>0.04)", //ZJetsToNuNu_HT800to1200
  "(Jet1Pt>200)&&(Jet1Pt<1125)&&(Met<930)&&(mt<1000)&&(LepPt<720)&&(HT>300)&&(HT<1950)&&(JetHBpt<1100)&&(DrJetHBLep<4.8)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //ZJetsToNuNu_HT1200to2500
  "(Jet1Pt>600)&&(Jet1Pt<1900)&&(Met<1000)&&(mt<2200)&&(LepPt<1600)&&(HT>800)&&(HT<4000)&&(JetHBpt<1800)&&(DrJetHBLep<4)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //ZJetsToNuNu_HT2500toInf
  "(Jet1Pt<940)&&(Met<740)&&(mt<740)&&(LepPt<450)&&(HT<1350)&&(JetHBpt<930)&&(DrJetHBLep<5)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //WW
  "(Jet1Pt<920)&&(Met<640)&&(mt<350)&&(LepPt<270)&&(HT<950)&&(JetHBpt<700)&&(DrJetHBLep<5)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //WZ
  "(Jet1Pt<530)&&(Met<550)&&(mt<600)&&(LepPt<250)&&(HT<800)&&(JetHBpt<600)&&(DrJetHBLep<5)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //ZZ
  "(Jet1Pt<5000)&&(Met<4000)&&(mt<400)&&(LepPt<200)&&(HT<5000)&&(JetHBpt<5000)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //DYJetsToLL_M50_HT100to200
  "(Jet1Pt<430)&&(Met<380)&&(mt<100)&&(LepPt<110)&&(HT<480)&&(JetHBpt<420)&&(DrJetHBLep<4.6)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //DYJetsToLL_M50_HT200to400
  "(Jet1Pt<700)&&(Met<600)&&(mt<230)&&(LepPt<300)&&(HT<820)&&(JetHBpt<700)&&(DrJetHBLep<5.4)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //DYJetsToLL_M50_HT400to600
  "(Jet1Pt<840)&&(Met<750)&&(mt<280)&&(LepPt<480)&&(HT<1120)&&(JetHBpt<850)&&(DrJetHBLep<5.4)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //DYJetsToLL_M50_HT400to800
  "(Jet1Pt<1200)&&(Met<1030)&&(mt<160)&&(LepPt<670)&&(HT<1600)&&(JetHBpt<1150)&&(DrJetHBLep<5.4)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //DYJetsToLL_M50_HT800to1200
  "(Jet1Pt<1600)&&(Met<1250)&&(mt<125)&&(LepPt<800)&&(HT>860)&&(HT<2430)&&(JetHBpt<1425)&&(DrJetHBLep<4.8)&&(DrJetHBLep>0)&&(JetHBCSV>0)", //DYJetsToLL_M50_HT1200to2500
  "(Jet1Pt>600)&&(Jet1Pt<2300)&&(Met<1450)&&(mt<350)&&(LepPt<800)&&(HT>1700)&&(HT<4200)&&(JetHBpt<2500)&&(DrJetHBLep<3.7)&&(DrJetHBLep>0)&&(JetHBCSV>0)&&(LepEta>-2)&&(LepEta<2)", //DYJetsToLL_M50_HT2500toInf
  "(JetHBCSV>0)", //DYJetsToLL_M5to50_HT100to200
  "(Jet1Pt<430)&&(Met<340)&&(mt<40)&&(LepPt<100)&&(JetHBCSV>0)&&(LepEta>-2)&&(LepEta<2)", //DYJetsToLL_M5to50_HT200to400
  "(Jet1Pt<550)&&(Met<470)&&(mt<35)&&(LepPt<180)&(HT>280)&&(HT<620)&&(JetHBpt<530)&&(DrJetHBLep<4.4)&&(JetHBCSV>0)", //DYJetsToLL_M5to50_HT400to600
  "(Jet1Pt>160)&&(Jet1Pt<910)&&(Met<760)&&(mt<60)&&(LepPt<400)&(HT>380)&&(HT<1275)&&(JetHBpt<900)&&(DrJetHBLep<4.5)&&(JetHBCSV>0.06)", //DYJetsToLL_M5to50_HT600toInf
  "(Jet1Pt<670)&&(Met<510)&&(mt<120)&&(LepPt<220)&&(HT>1300)&&(HT<650)&&(JetHBpt<650)&&(DrJetHBLep<4.8)&&(JetHBCSV>0)", //TBar_tch_powheg
  "(Jet1Pt<1000)&&(Met<780)&&(mt<425)&&(LepPt<560)&&(HT<2080)&&(JetHBpt<900)&&(DrJetHBLep<4.5)&&(JetHBCSV>0)", //TBar_tWch_ext
  "(Jet1Pt<800)&&(Met<650)&&(mt<120)&&(LepPt<290)&&(HT<1470)&&(JetHBpt<670)&&(DrJetHBLep<4.8)&&(JetHBCSV>0)", //T_tch_powheg
  "(Jet1Pt<1000)&&(Met<850)&&(mt<520)&&(LepPt<530)&&(HT<2000)&&(JetHBpt<880)&&(DrJetHBLep<4.8)&&(JetHBCSV>0.12)", //T_tWch_ext
"0"
};

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
