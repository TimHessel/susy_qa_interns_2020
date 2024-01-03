import os
import sys
import ROOT

main_bkg = ["TT_pow.root",
        "Wjets_100to200.root",
        "Wjets_200to400.root",
        "Wjets_400to600.root",
        "Wjets_600to800.root",
        "Wjets_800to1200.root",
        "Wjets_1200to2500.root",
        "Wjets_2500toInf.root",
        "ZJetsToNuNu_HT100to200.root",
        "ZJetsToNuNu_HT200to400.root",
        "ZJetsToNuNu_HT400to600.root",
        "ZJetsToNuNu_HT600to800.root",
        "ZJetsToNuNu_HT800to1200.root",
        "ZJetsToNuNu_HT1200to2500.root",
        "ZJetsToNuNu_HT2500toInf.root"]

small_bkg = ["DYJetsToLL_M50_HT100to200.root",
             "DYJetsToLL_M50_HT1200to2500.root",
             "DYJetsToLL_M50_HT200to400.root",
             "DYJetsToLL_M50_HT2500toInf.root",
             "DYJetsToLL_M50_HT400to600.root",
             "DYJetsToLL_M50_HT600to800.root",
             "DYJetsToLL_M50_HT800to1200.root",
             "DYJetsToLL_M5to50_HT100to200.root",
             "DYJetsToLL_M5to50_HT200to400.root",
             "DYJetsToLL_M5to50_HT400to600.root",
             "DYJetsToLL_M5to50_HT600toInf.root",
             "TBar_tch_powheg.root",
             "TBar_tWch_ext.root",
             "T_tch_powheg.root",
             "T_tWch_ext.root",
             "WW.root",
             "WZ.root",
             "ZZ.root"]

signal = "T2DegStop_550_520.root"

signal_train = ["T2DegStop_250_220.root",
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
                "T2DegStop_575_545.root",
                "T2DegStop_600_570.root",
                "T2DegStop_625_595.root",
                "T2DegStop_650_620.root",
                "T2DegStop_675_645.root",
                "T2DegStop_700_670.root",
                "T2DegStop_725_695.root",
                "T2DegStop_750_720.root",
                "T2DegStop_775_745.root",
                "T2DegStop_800_770.root"]

def assure_path_exists(path):
  if not os.path.exists(path):
    os.mkdir(path)

def half_split(source, target1, target2):
  full = ROOT.TFile.Open(source)
  full_tree = full.Get('bdttree')
  nev = full_tree.GetEntries()

  file1 = ROOT.TFile(target1, "recreate")
  tree1 = full_tree.CloneTree(0)
  for i in range(nev//2):
    full_tree.GetEntry(i)
    tree1.Fill()
  tree1.AutoSave()
  file1.Close()

  file2 = ROOT.TFile(target2, "recreate")
  tree2 = full_tree.CloneTree(0)
  for i in range(nev//2, nev):
    full_tree.GetEntry(i)
    tree2.Fill()
  tree2.AutoSave()
  file2.Close()

def main():
  assure_path_exists("test")
  assure_path_exists("train")
  assure_path_exists("validation")
  assure_path_exists("tmp")

  for sig_train in signal_train:
    print(f"Copy full {sig_train} to train...")
    os.system(f"cp truncated_data/truncated_{sig_train} train")

  for s_bkg in small_bkg:
    print(f"Copy full {s_bkg} to test...")
    os.system(f"cp truncated_data/truncated_{s_bkg} test")

  print(f"Split {signal} between test and validation...")
  half_split("truncated_data/truncated_"+signal, "test/"+signal, "validation/"+signal)

  for m_bkg in main_bkg:
    print(f"Split {m_bkg} between test, train and validation...")
    half_split("truncated_data/truncated_"+m_bkg, "test/"+m_bkg, "tmp/"+m_bkg)
    half_split("tmp/"+m_bkg, "train/"+m_bkg, "validation/"+m_bkg)

  print("Remove temporary directory...")
  os.system("rm -r tmp")

  print("Split done!")

if __name__=="__main__":
  sys.exit(main())
