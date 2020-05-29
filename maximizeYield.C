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

using std::string;

string fileName =  "CutTable";
string directory = "plots/SET3112_ttLO_Zinv/";
string basedirectory = "SET3112_ttLO_Zinv/";

string to_string_with_precision(double a_value, int n = 3) {
  std::ostringstream out;
  out << std::setprecision(n) << a_value;
  return out.str();
}

class variable{

public:

  variable(string name="", string expression = "", int bins=30, double xmin=0, double xmax=100, string leg = "", int Logy=0);

  string GetName() {return fname;}
  string GetExpression() {return fexpression;}
  int GetBins() {return fbins;}
  double GetXMin() {return fxmin;}
  double GetXMax() {return fxmax;}
  string GetLeg() {return fleg;}
  int GetLogy() {return fLogy;}

  void SetName(string name) {fname=name;}
  void SetExpression(string expression) {fexpression=expression;}
  void SetBins(int bins) {fbins=bins;}
  void SetXMin(double xmin) {fxmin=xmin;}
  void SetXMax(double xmax) {fxmax=xmax;}
  void SetLeg(string leg) {fleg=leg;}

private:

  string fname;
  string fexpression;
  int    fbins;
  double fxmin;
  double fxmax;
  string fleg;
  int fLogy;
};

variable::variable(string name, string expression, int bins, double xmin, double xmax, string leg, int Logy): fname(name), fexpression(expression), fbins(bins), fxmin(xmin), fxmax(xmax), fleg(leg), fLogy(Logy) {}

class process{
public:
  process(TChain* chain, string name=""):fchain(chain),fname(name),Nevt(0),Nexp_nosel(0),Nexp_sel(0)
  {fselection = "";}

  string GetName() {return fname;}
  TChain* GetChain() {return fchain;}
  int GetNevt() {return Nevt;}
  int GetNexp_nosel() {return Nexp_nosel;}
  int GetNexp_sel() {return Nexp_sel;}
  string GetSelection() {return fselection;}

  void SetNevt_nosel(int evt_nosel) {Nexp_nosel = evt_nosel;}
  void SetNevt_sel(int evt_sel) {Nexp_sel = evt_sel;}
  void SetSelection(string selection) {fselection = selection;}

private:
  TChain* fchain;
  string fname;
  string fselection;
  int Nevt;
  int Nexp_nosel;
  int Nexp_sel;
};

double countEvt(process &process, TCut Cut){

  TH1D* tmp = new TH1D("tmp", "tmp", 30, 0, 1000);
  // Get number of events after a cut
  double Sel = 0;
  // Valid w splitting: Include a factor 2 @ numerator b/c denominator is the same while selected events have been halfed
  //  process.GetChain()->Draw("Njet>>tmp","XS*filterEfficiency*12900.*2./Nevt"*Cut, "goff");
  process.GetChain()->Draw("Njet>>tmp","weight*12900.*splitFactor"*Cut, "goff");

  for(int i=0; i<=(tmp->GetNbinsX()+1); i++)
    Sel += tmp->GetBinContent(i);

  delete tmp;
  process.SetSelection(Cut.GetName());
  process.SetNevt_sel(Sel);
  return Sel;
}

void YieldMaximize(vector<process> vprocess, variable variable, TCut initial_cut, ofstream &yieldFile, double* &xpointsR, double* &ypointsR, double* &errorR, double* &effS, double* &effB){

  int max = variable.GetXMin();
  int intervals = variable.GetBins();
  double step = (variable.GetXMax() - variable.GetXMin())/intervals;
  TCut bestCut = "1";
  TCut bestCutR = "1";

  double BG = 0, BGPS = 0;
  double EffB = 0;
  double wjets1 = 0, wjets2 = 0, wjets3 = 0, wjets4 = 0,  wjets5 = 0, wjets6 = 0, wjets7 = 0, ttbar = 0;
  double zinv1 = 0, zinv2 = 0, zinv3 = 0, zinv4 = 0,  zinv5 = 0, zinv6 = 0, zinv7 = 0;
  double ww = 0, wz = 0, zz = 0, tch = 0, twch = 0, tbch = 0, tbwch = 0;
  double dym50ht1 = 0, dym50ht2 = 0, dym50ht3 = 0, dym50ht4 = 0;
  double dy5m50ht1 = 0, dy5m50ht2 = 0, dy5m50ht3 = 0, dy5m50ht4 = 0;
  double signal = 0;
  double EffS = 0;

  double ratio = 0, ratioR = 0;
  double ratio_Error = 0, ratioR_Error = 0;

  double f = 0.2, fom = 0, fom1 = 0, fom2 = 0;

  for(int i=0; i<intervals; i++) {
    BG = 0;
    TCut cut = (variable.GetExpression() + ">" + std::to_string(variable.GetXMin() + i*step)).c_str();
    cut.SetName((variable.GetExpression() + " $>$ " + to_string_with_precision(variable.GetXMin() + i*step, 3)).c_str());
      
    for(int i=0; i<vprocess.size()-1; i++) {
      BG += countEvt(vprocess[i],cut*initial_cut);
      BGPS += countEvt(vprocess[i],initial_cut);
    }
    EffB = BG/BGPS;
    effB[i] = EffB;
      
    signal = countEvt(vprocess[30],cut*initial_cut);
    EffS = signal/countEvt(vprocess[30],initial_cut);
    effS[i] = EffS;
      
    fom1 = (signal + BG)*log(((signal+BG)*(BG+f*f*BG*BG))/(BG*BG+(signal + BG)*f*f*BG*BG));
    fom2 = (1/(f*f))*log(1 + (f*f*BG*BG*signal)/(BG*(BG+f*f*BG*BG)));
    fom = sqrt(2*(fom1-fom2));

    double fom_error = 0.;

    xpointsR[i] = variable.GetXMin() + i*step;
    
    if(BG != 0) {
      ypointsR[i] = fom;
      errorR[i] = fom_error;
    }
    else {
      ypointsR[i] = 0;
      errorR[i] = 0;
    }

    if(ypointsR[i] > ratioR) {
      ratioR = fom;
      bestCutR = cut;
      ratioR_Error = fom_error;
    }
  }

  bestCut = bestCutR;
  ratio = ratioR;
  ratio_Error = ratioR_Error;
  wjets1 = countEvt(vprocess[0],bestCutR*initial_cut);
  wjets2 = countEvt(vprocess[1],bestCutR*initial_cut);
  wjets3 = countEvt(vprocess[2],bestCutR*initial_cut);
  wjets4 = countEvt(vprocess[3],bestCutR*initial_cut);
  wjets5 = countEvt(vprocess[4],bestCutR*initial_cut);
  wjets6 = countEvt(vprocess[5],bestCutR*initial_cut);
  wjets7 = countEvt(vprocess[6],bestCutR*initial_cut);
  ttbar  = countEvt(vprocess[7], bestCutR*initial_cut);
  zinv1  = countEvt(vprocess[8],bestCutR*initial_cut);
  zinv2  = countEvt(vprocess[9],bestCutR*initial_cut);
  zinv3  = countEvt(vprocess[10],bestCutR*initial_cut);
  zinv4  = countEvt(vprocess[11],bestCutR*initial_cut);
  zinv5  = countEvt(vprocess[12],bestCutR*initial_cut);
  zinv6  = countEvt(vprocess[13],bestCutR*initial_cut);
  zinv7  = countEvt(vprocess[14],bestCutR*initial_cut);
  ww     = countEvt(vprocess[15],bestCutR*initial_cut);
  wz     = countEvt(vprocess[16],bestCutR*initial_cut);
  zz     = countEvt(vprocess[17],bestCutR*initial_cut);
  tch    = countEvt(vprocess[18],bestCutR*initial_cut);
  twch   = countEvt(vprocess[19],bestCutR*initial_cut);
  tbch   = countEvt(vprocess[20],bestCutR*initial_cut);
  tbwch  = countEvt(vprocess[21],bestCutR*initial_cut);
  dym50ht1 = countEvt(vprocess[22],bestCutR*initial_cut);
  dym50ht2 = countEvt(vprocess[23],bestCutR*initial_cut);
  dym50ht3 = countEvt(vprocess[24],bestCutR*initial_cut);
  dym50ht4 = countEvt(vprocess[25],bestCutR*initial_cut);
  dy5m50ht1 = countEvt(vprocess[26],bestCutR*initial_cut);
  dy5m50ht2 = countEvt(vprocess[27],bestCutR*initial_cut);
  dy5m50ht3 = countEvt(vprocess[28],bestCutR*initial_cut);
  dy5m50ht4 = countEvt(vprocess[29],bestCutR*initial_cut);
  signal = countEvt(vprocess[30], bestCutR*initial_cut);


  double WJtot = wjets1+wjets2+wjets3+wjets4+wjets5+wjets6+wjets7;
  double ZItot = zinv1+zinv2+zinv3+zinv4+zinv5+zinv6+zinv7;
  double STtot = tch+twch+tbch+tbwch;
  double VVtot = ww+wz+zz;
  double DYtot = dym50ht1+dym50ht2+dym50ht3+dym50ht4+dy5m50ht1+dy5m50ht2+dy5m50ht3+dy5m50ht4;
  double Backg = WJtot+ttbar+ZItot+STtot+VVtot+DYtot;

  std::cout << bestCut << std::endl;

  string FOMname = "FOM";
  string Sname   = "(300,270)";
  string Bname   = "Background";
  string W1name  = "Wjets [100,200]";
  string W2name  = "Wjets [200,400]";
  string W3name  = "Wjets [400,600]";
  string W4name  = "Wjets [600,800]";
  string W5name  = "Wjets [800,1200]";
  string W6name  = "Wjets [1200,2500]";
  string W7name  = "Wjets [2500,Inf]";
  string WJname  = "Wjets";
  string TTname  = "tt";
  string Z1name  = "Zinv [100,200]";
  string Z2name  = "Zinv [200,400]";
  string Z3name  = "Zinv [400,600]";
  string Z4name  = "Zinv [600,800]";
  string Z5name  = "Zinv [800,1200]";
  string Z6name  = "Zinv [1200,2500]";
  string Z7name  = "Zinv [2500,Inf]";
  string ZIname  = "Zinv";
  string WWname  = "WW";
  string WZname  = "WZ";
  string ZZname  = "ZZ";
  string VVname  = "VV";
  string STname  = "Single tops";
  string DYname  = "DY";

  yieldFile << bestCut.GetName() << "&" <<           "\\\\" << std::endl;
  yieldFile << FOMname           << "&" << std::setprecision(3) << ratio  << "\\\\" << std::endl;
  yieldFile << Sname             << "&" << std::setprecision(3) << signal << "\\\\" << std::endl;
  yieldFile << Bname             << "&" << std::setprecision(3) << Backg  << "\\\\" << std::endl;
  yieldFile << "\\hline" << std::endl;
  yieldFile << WJname            << "&" << std::setprecision(3) << WJtot << "\\\\" << std::endl;
  /*
  yieldFile << W1name            << "&" << std::setprecision(2) << wjets1 << "\\\\" << std::endl;
  yieldFile << W2name            << "&" << std::setprecision(2) << wjets2 << "\\\\" << std::endl;
  yieldFile << W3name            << "&" << std::setprecision(2) << wjets3 << "\\\\" << std::endl;
  yieldFile << W4name            << "&" << std::setprecision(2) << wjets4 << "\\\\" << std::endl;
  yieldFile << W5name            << "&" << std::setprecision(2) << wjets5 << "\\\\" << std::endl;
  yieldFile << W6name            << "&" << std::setprecision(2) << wjets6 << "\\\\" << std::endl;
  yieldFile << W7name            << "&" << std::setprecision(2) << wjets7 << "\\\\" << std::endl;
  */
  yieldFile << TTname            << "&" << std::setprecision(3) << ttbar  << "\\\\" << std::endl;
  yieldFile << ZIname            << "&" << std::setprecision(3) << ZItot  << "\\\\" << std::endl;
  /*
  yieldFile << Z1name            << "&" << std::setprecision(2) << zinv1  << "\\\\" << std::endl;
  yieldFile << Z2name            << "&" << std::setprecision(2) << zinv2  << "\\\\" << std::endl;
  yieldFile << Z3name            << "&" << std::setprecision(2) << zinv3  << "\\\\" << std::endl;
  yieldFile << Z4name            << "&" << std::setprecision(2) << zinv4  << "\\\\" << std::endl;
  yieldFile << Z5name            << "&" << std::setprecision(2) << zinv5  << "\\\\" << std::endl;
  yieldFile << Z6name            << "&" << std::setprecision(2) << zinv6  << "\\\\" << std::endl;
  yieldFile << Z7name            << "&" << std::setprecision(2) << zinv7  << "\\\\" << std::endl;
  */
  yieldFile << VVname            << "&" << std::setprecision(3) << VVtot  << "\\\\" << std::endl;
  yieldFile << STname            << "&" << std::setprecision(3) << STtot  << "\\\\" << std::endl;
  yieldFile << DYname            << "&" << std::setprecision(3) << DYtot  << "\\\\" << std::endl;

  //  return bestCut;
}

void StartPrint(vector<process> vprocess, ofstream &yieldFile){
  yieldFile.open((fileName + ".tex").c_str());
  yieldFile << "\\documentclass{article}" << std::endl;
  yieldFile << "\\usepackage[utf8]{inputenc}" << std::endl;
  yieldFile << "\\usepackage{cancel}" << std::endl;
  yieldFile << "\\begin{document}" << std::endl;
  yieldFile << "\\begin{table}[!h]" << std::endl;
  yieldFile << "\\centering" << std::endl;
  string col = "ll";
  yieldFile << "\\begin{tabular}{" << col << "}" << std::endl;
  yieldFile << "\\hline" << std::endl;
  yieldFile << "\\hline" << std::endl;
}

void EndPrint(ofstream &yieldFile){
  yieldFile << "\\hline" << std::endl;
  yieldFile << "\\hline" << std::endl;
  yieldFile << "\\end{tabular}" << std::endl;
  yieldFile << "\\caption{FOM}" << std::endl;
  yieldFile << "\\end{table}" << std::endl;
  yieldFile << "\\end{document}" << std::endl;

  system(("pdflatex " + fileName + ".tex").c_str());
  system(("gnome-open " + fileName + ".pdf").c_str());
}

int maximizeYield(){

  // Open input file(s)
  TChain* wjetsChain1 = new TChain("bdttree");
  wjetsChain1->Add((basedirectory + "Wjets_100to200_bdt.root").c_str());
  TChain* wjetsChain2 = new TChain("bdttree");
  wjetsChain2->Add((basedirectory + "Wjets_200to400_bdt.root").c_str());
  TChain* wjetsChain3 = new TChain("bdttree");
  wjetsChain3->Add((basedirectory + "Wjets_400to600_bdt.root").c_str());
  TChain* wjetsChain4 = new TChain("bdttree");
  wjetsChain4->Add((basedirectory + "Wjets_600to800_bdt.root").c_str());
  TChain* wjetsChain5 = new TChain("bdttree");
  wjetsChain5->Add((basedirectory + "Wjets_800to1200_bdt.root").c_str());
  TChain* wjetsChain6 = new TChain("bdttree");
  wjetsChain6->Add((basedirectory + "Wjets_1200to2500_bdt.root").c_str());
  TChain* wjetsChain7 = new TChain("bdttree");
  wjetsChain7->Add((basedirectory + "Wjets_2500toInf_bdt.root").c_str());

  TChain* ttbarChain = new TChain("bdttree");
  //  ttbarChain->Add((basedirectory + "TTJets_bdt.root").c_str());
  ttbarChain->Add((basedirectory + "TTJets_LO_bdt.root").c_str());

  TChain* zinvChain1 = new TChain("bdttree");
  zinvChain1->Add((basedirectory + "ZJetsToNuNu_HT100to200_bdt.root").c_str());
  TChain* zinvChain2 = new TChain("bdttree");
  zinvChain2->Add((basedirectory + "ZJetsToNuNu_HT200to400_bdt.root").c_str());
  TChain* zinvChain3 = new TChain("bdttree");
  zinvChain3->Add((basedirectory + "ZJetsToNuNu_HT400to600_bdt.root").c_str());
  TChain* zinvChain4 = new TChain("bdttree");
  zinvChain4->Add((basedirectory + "ZJetsToNuNu_HT600to800_bdt.root").c_str());
  TChain* zinvChain5 = new TChain("bdttree");
  zinvChain5->Add((basedirectory + "ZJetsToNuNu_HT800to1200_bdt.root").c_str());
  TChain* zinvChain6 = new TChain("bdttree");
  zinvChain6->Add((basedirectory + "ZJetsToNuNu_HT1200to2500_bdt.root").c_str());
  TChain* zinvChain7 = new TChain("bdttree");
  zinvChain7->Add((basedirectory + "ZJetsToNuNu_HT2500toInf_bdt.root").c_str());

  TChain* wwChain = new TChain("bdttree");
  wwChain->Add((basedirectory + "WW_bdt.root").c_str());
  TChain* wzChain = new TChain("bdttree");
  wzChain->Add((basedirectory + "WZ_bdt.root").c_str());
  TChain* zzChain = new TChain("bdttree");
  zzChain->Add((basedirectory + "ZZ_bdt.root").c_str());

  TChain* tchChain = new TChain("bdttree");
  tchChain->Add((basedirectory + "T_tch_bdt.root").c_str());
  TChain* twchChain = new TChain("bdttree");
  twchChain->Add((basedirectory + "T_tWch_bdt.root").c_str());
  TChain* tbtchChain = new TChain("bdttree");
  tbtchChain->Add((basedirectory + "TBar_tch_bdt.root").c_str());
  TChain* tbwchChain = new TChain("bdttree");
  tbwchChain->Add((basedirectory + "TBar_tWch_bdt.root").c_str());

  TChain* dyChain1 = new TChain("bdttree");
  dyChain1->Add((basedirectory + "DYJetsToLL_M50_HT100to200_bdt.root").c_str());
  TChain* dyChain2 = new TChain("bdttree");
  dyChain2->Add((basedirectory + "DYJetsToLL_M50_HT200to400_bdt.root").c_str());
  TChain* dyChain3 = new TChain("bdttree");
  dyChain3->Add((basedirectory + "DYJetsToLL_M50_HT400to600_bdt.root").c_str());
  TChain* dyChain4 = new TChain("bdttree");
  dyChain4->Add((basedirectory + "DYJetsToLL_M50_HT600toInf_bdt.root").c_str());
  TChain* dyChain5 = new TChain("bdttree");
  dyChain5->Add((basedirectory + "DYJetsToLL_M5to50_HT100to200_bdt.root").c_str());
  TChain* dyChain6 = new TChain("bdttree");
  dyChain6->Add((basedirectory + "DYJetsToLL_M5to50_HT200to400_bdt.root").c_str());
  TChain* dyChain7 = new TChain("bdttree");
  dyChain7->Add((basedirectory + "DYJetsToLL_M5to50_HT400to600_bdt.root").c_str());
  TChain* dyChain8 = new TChain("bdttree");
  dyChain8->Add((basedirectory + "DYJetsToLL_M5to50_HT600toInf_bdt.root").c_str());


  TChain* stopChain = new TChain("bdttree");
  //  stopChain->Add((basedirectory + "T2DegStop_300_290_bdt.root").c_str());
  //  stopChain->Add((basedirectory + "T2DegStop_300_280_bdt.root").c_str());
  stopChain->Add((basedirectory + "T2DegStop_300_270_bdt.root").c_str());
  //  stopChain->Add((basedirectory + "T2DegStop_300_260_bdt.root").c_str());
  //  stopChain->Add((basedirectory + "T2DegStop_300_250_bdt.root").c_str());
  //  stopChain->Add((basedirectory + "T2DegStop_300_240_bdt.root").c_str());
  //  stopChain->Add((basedirectory + "T2DegStop_300_230_bdt.root").c_str());
  //  stopChain->Add((basedirectory + "T2DegStop_350_270_bdt.root").c_str());

  vector<process> vprocess;
  process pwjets1(wjetsChain1,"wjets1");
  process pwjets2(wjetsChain2,"wjets2");
  process pwjets3(wjetsChain3,"wjets3");
  process pwjets4(wjetsChain4,"wjets4");
  process pwjets5(wjetsChain5,"wjets5");
  process pwjets6(wjetsChain6,"wjets6");
  process pwjets7(wjetsChain7,"wjets7");
  process pttbar(ttbarChain,"ttbar");
  process pzinv1(zinvChain1,"zinv1");
  process pzinv2(zinvChain2,"zinv2");
  process pzinv3(zinvChain3,"zinv3");
  process pzinv4(zinvChain4,"zinv4");
  process pzinv5(zinvChain5,"zinv5");
  process pzinv6(zinvChain6,"zinv6");
  process pzinv7(zinvChain7,"zinv7");
  process pww(wwChain,"ww");
  process pwz(wzChain,"wz");
  process pzz(zzChain,"zz");
  process ptch(tchChain,"tch");
  process ptwch(twchChain,"twch");
  process ptbch(tbtchChain,"tbch");
  process ptbwch(tbwchChain,"tbwch");

  process pdy1(dyChain1,"dym50ht1");
  process pdy2(dyChain2,"dym50ht2");
  process pdy3(dyChain3,"dym50ht3");
  process pdy4(dyChain4,"dym50ht4");
  process pdy5(dyChain5,"dy5m50ht1");
  process pdy6(dyChain6,"dy5m50ht2");
  process pdy7(dyChain7,"dy5m50ht3");
  process pdy8(dyChain8,"dy5m50ht4");

  process psignal(stopChain,"signal");
  vprocess.push_back(pwjets1);
  vprocess.push_back(pwjets2);
  vprocess.push_back(pwjets3);
  vprocess.push_back(pwjets4);
  vprocess.push_back(pwjets5);
  vprocess.push_back(pwjets6);
  vprocess.push_back(pwjets7);
  vprocess.push_back(pttbar);
  vprocess.push_back(pzinv1);
  vprocess.push_back(pzinv2);
  vprocess.push_back(pzinv3);
  vprocess.push_back(pzinv4);
  vprocess.push_back(pzinv5);
  vprocess.push_back(pzinv6);
  vprocess.push_back(pzinv7);
  vprocess.push_back(pww);
  vprocess.push_back(pwz);
  vprocess.push_back(pzz);
  vprocess.push_back(ptch);
  vprocess.push_back(ptwch);
  vprocess.push_back(ptbch);
  vprocess.push_back(ptbwch);
  vprocess.push_back(pdy1);
  vprocess.push_back(pdy2);
  vprocess.push_back(pdy3);
  vprocess.push_back(pdy4);
  vprocess.push_back(pdy5);
  vprocess.push_back(pdy6);
  vprocess.push_back(pdy7);
  vprocess.push_back(pdy8);
  vprocess.push_back(psignal);

  // Create Variables

  vector<variable> vvariable;

  //  variable BDToutput("BDTout", "BDT", 100, -0.25, 0.75, "BDT output",0);
  variable BDToutput("BDTout", "BDT", 65, 0., 0.65, "BDT output",0);
  //  variable BDToutput("BDTout", "BDT", 10, 0.30, 0.40, "BDT output",0);

  vvariable.push_back(BDToutput);

  // Create TCuts
  TCut null = "1";
  TCut lept = "LepPt<30.";
  //  TCut lept = "LepPt<10000.";
  TCut lepton = lept;
  TCut ISRjet = "Jet1Pt > 110";
  TCut dphij1j2 = "(DPhiJet1Jet2 < 2.5)||(Jet2Pt<60.)";
  TCut met = "Met > 280";
  TCut ht = "HT30>200.";
  TCut preSel = lepton && ISRjet && dphij1j2 && met && ht;

  //Set names of TCuts
  lepton.SetName("lepton");
  ISRjet.SetName("$p_T$(Jet1)$ > 110$");
  met.SetName("$\\cancel{E_T} > 300$");
  preSel.SetName("PreSelection");

  TCut selection = preSel && "BDT > 0.23";
  selection.SetName("Selection");

  // Maximize

  vector<double*> xFOMR;
  vector<double*> yFOMR;
  vector<double*> eFOMR;
  vector<double*> EffSgn;
  vector<double*> EffBkg;

  ofstream BestCuts;
  StartPrint(vprocess, BestCuts);
  for(int i=0; i<int(vvariable.size()); i++) {
    TCanvas *c1 = new TCanvas("FOM","FOM",800,600);

    xFOMR.push_back(new double[vvariable[i].GetBins()]);
    yFOMR.push_back(new double[vvariable[i].GetBins()]);
    eFOMR.push_back(new double[vvariable[i].GetBins()]);
    EffSgn.push_back(new double[vvariable[i].GetBins()]);
    EffBkg.push_back(new double[vvariable[i].GetBins()]);

    YieldMaximize(vprocess, vvariable[i], preSel, BestCuts, xFOMR[i], yFOMR[i], eFOMR[i], EffSgn[i], EffBkg[i]);

    TGraphErrors *graphR = new TGraphErrors(vvariable[i].GetBins(), xFOMR[i], yFOMR[i], 0, eFOMR[i]);

    TGraphErrors *graphSgn = new TGraphErrors(vvariable[i].GetBins(), xFOMR[i], EffSgn[i], 0, 0);
    TGraphErrors *graphBkg = new TGraphErrors(vvariable[i].GetBins(), xFOMR[i], EffBkg[i], 0, 0);
    
    graphR->SetTitle((vvariable[i].GetExpression() + " > x").c_str());
    graphR->GetXaxis()->SetTitle((vvariable[i].GetExpression()).c_str());
    graphR->GetYaxis()->SetTitle("FOM");
    graphR->SetMarkerStyle(kFullDotLarge);
    graphR->SetMarkerColor(1);
    graphR->SetMinimum(0.);
    graphR->SetMaximum(10.);
    gPad->SetGridy(kTRUE);
    gPad->SetGridx(kTRUE);
    
    graphSgn->SetTitle((vvariable[i].GetExpression() + " > x").c_str());
    graphSgn->GetXaxis()->SetTitle((vvariable[i].GetExpression()).c_str());
    graphSgn->GetYaxis()->SetTitle("Efficiency");
    graphSgn->SetMarkerStyle(kFullTriangleUp);
    graphSgn->SetMarkerColor(4);
    graphSgn->SetMinimum(1.e-07);
    graphBkg->SetTitle((vvariable[i].GetExpression() + " > x").c_str());
    graphBkg->GetXaxis()->SetTitle((vvariable[i].GetExpression()).c_str());
    graphBkg->GetYaxis()->SetTitle("Efficiency");
    graphBkg->SetMarkerStyle(kFullTriangleUp);
    graphBkg->SetMarkerColor(2);

    graphR->Draw();
    
    c1->SaveAs((directory + vvariable[i].GetExpression() + "_" + fileName + ".png").c_str());
//      delete c1;
//      delete graphR;
    TCanvas *c2 = new TCanvas("Efficiency","Efficiency",800,600);
    graphSgn->Draw("ap");
    graphBkg->Draw("psame");
    gPad->SetGridy(kTRUE);
    gPad->SetGridx(kTRUE);
    gPad->SetLogy(kTRUE);
    c2->SaveAs((directory + vvariable[i].GetExpression() + "_Eff.png").c_str());

  }
  EndPrint(BestCuts);

  return 0;
}

