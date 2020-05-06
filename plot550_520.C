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


void plot550_520(){
  TChain plot_550_520("bdttree");

  // access bdtt
  plot_550_520.Add("/home/t3cms/dbastos/LSTORE/Stop4Body/nTuples16_v2017-10-19_test/T2DegStop_550_520.root");

  // plot create
  TH1D* pl_lepPt = new TH1D("pl_lepPt", "lepPT", 200,0,200);
  TH1D* pl_xs = new TH1D("pl_xs", "XS", 200,0,200);
  TH1D* pl_jet_pt = new TH1D("pl_jet_pt", "Jet1PT", 200,0,200);
  TH1D* pl_met = new TH1D("pl_met", "Met", 200,0,200);
  TH1D* pl_mt = new TH1D("pl_mt", "mt", 200,0,200);
  TH1D* pl_Lep_Eta = new TH1D("pl_Lep_Eta", "lepEta", 200,0,200);
  TH1D* pl_lep_chg = new TH1D("pl_lep_chg", "lepChg", 200,0,200);
  TH1D* pl_ht = new TH1D("pl_ht", "ht", 200,0,200);
  TH1D* pl_Nb = new TH1D("pl_Nb", "Nbloose", 200,0,200);
  TH1D* pl_Njet = new TH1D("pl_Njet", "Njet", 200,0,200);
  TH1D* pl_jet_HB = new TH1D("pl_jet_HB", "JetHBpt", 200,0,200);
  TH1D* pl_dr = new TH1D("pl_dr", "DrJetHblep", 200,0,200);
  TH1D* pl_jet_HB_csv = new TH1D("pl_jet_HB_csv", "JetHBCSV", 200,0,200);

  //Draw histograms
  plot_550_520.Draw("LepPt>>pl_lepPt");
  plot_550_520.Draw("XS>>pl_xs");
  plot_550_520.Draw("Jet1Pt>>pl_jet_pt");
  plot_550_520.Draw("Met>>pl_met");
  plot_550_520.Draw("mt>>pl_mt");
  plot_550_520.Draw("LepEta>>pl_Lep_Eta");
  plot_550_520.Draw("LepChg>>pl_lep_chg");
  plot_550_520.Draw("HT>>pl_ht");
  plot_550_520.Draw("NbLoose>>pl_Nb");
  plot_550_520.Draw("Njet>>pl_Njet");
  plot_550_520.Draw("JetHBpt>>pl_jet_HB");
  plot_550_520.Draw("DrJetHBLep>>pl_dr");
  plot_550_520.Draw("JetHBCSV>>pl_jet_HB_csv");


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
  c1->SaveAs("compare_histograms_550_520.pdf");


}
