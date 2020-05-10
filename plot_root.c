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


void plot_root(){

  // Signal Data DM=30
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
  //Background data
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
              "ZJetsToNuNu_HT2500toInf.root"
            };

  // Creating Tchain = tree
  TChain plot("bdttree");

  // access bdtt
  plot.Add("/home/t3cms/thessel/skimmed_data/TT_pow.root");

  // plot create
  TH1D* pl_lepPt = new TH1D("pl_lepPt", "lepPT", 200,0,200);
  TH1D* pl_xs = new TH1D("pl_xs", "XS", 200,0,25);
  TH1D* pl_jet_pt = new TH1D("pl_jet_pt", "Jet1PT", 800,0,800);
  TH1D* pl_met = new TH1D("pl_met", "Met", 1500,0,1500);
  TH1D* pl_mt = new TH1D("pl_mt", "mt", 200,0,200);
  TH1D* pl_Lep_Eta = new TH1D("pl_Lep_Eta", "lepEta", 100,-2.5,2.500);
  TH1D* pl_lep_chg = new TH1D("pl_lep_chg", "lepChg", 4,-1.50,1.50);
  TH1D* pl_ht = new TH1D("pl_ht", "ht", 800,200,1000);
  TH1D* pl_Nb = new TH1D("pl_Nb", "Nbloose", 6,0,6);
  TH1D* pl_Njet = new TH1D("pl_Njet", "Njet", 10,0,10);
  TH1D* pl_jet_HB = new TH1D("pl_jet_HB", "JetHBpt", 700,0,700);
  TH1D* pl_dr = new TH1D("pl_dr", "DrJetHblep", 100,0,4);
  TH1D* pl_jet_HB_csv = new TH1D("pl_jet_HB_csv", "JetHBCSV", 50,0,1);

  //Draw histograms
  plot.Draw("LepPt>>pl_lepPt");
  plot.Draw("XS>>pl_xs");
  plot.Draw("Jet1Pt>>pl_jet_pt");
  plot.Draw("Met>>pl_met");
  plot.Draw("mt>>pl_mt");
  plot.Draw("LepEta>>pl_Lep_Eta");
  plot.Draw("LepChg>>pl_lep_chg");
  plot.Draw("HT>>pl_ht");
  plot.Draw("NbLoose>>pl_Nb");
  plot.Draw("Njet>>pl_Njet");
  plot.Draw("JetHBpt>>pl_jet_HB");
  plot.Draw("DrJetHBLep>>pl_dr");
  plot.Draw("JetHBCSV>>pl_jet_HB_csv");


  //Create and divide TCanvas

  TCanvas *c1 = new TCanvas("c1","",8000,8000);
  c1->Divide(4,4) ;

  //Plot those graph!
  c1->cd(1);
  pl_lepPt->Draw("");
  c1->cd(2);
  pl_dr->Draw("");
  c1->cd(3);
  pl_ht->Draw("");
  c1->cd(4);
  pl_xs->Draw("");
  c1->cd(5);
  pl_Nb->Draw("");
  c1->cd(6);
  pl_mt->Draw("");
  c1->cd(7);
  pl_met->Draw("");
  c1->cd(8);
  pl_Njet->Draw("");
  c1->cd(9);
  pl_jet_pt->Draw("");
  c1->cd(10);
  pl_jet_HB->Draw("");
  c1->cd(11);
  pl_Lep_Eta->Draw("");
  c1->cd(12);
  pl_lep_chg->Draw("");
  c1->cd(13);
  pl_jet_HB_csv->Draw("");

  //SAVE
  c1->SaveAs("compare_histograms_TT_pow.pdf");


}
