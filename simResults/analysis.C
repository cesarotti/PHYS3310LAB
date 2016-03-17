#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TRandom3.h"
#include <iostream>
#include <sstream>
#include <map>
#include "TF1.h"
#include <vector>

void analysis()
{
  TFile * file1 = new TFile("./1p33MeV50k/complete.root");
  TTree * tree1 = (TTree *) file1->Get("CalorimeterData"); 
  int nEvents1 = tree1->GetEntries();
  double eng1, eng2;
  tree1->SetBranchAddress("Energy_Dep", &eng1);

  TFile * file2 = new TFile("./1p17MeV50k/complete.root");
  TTree * tree2 = (TTree *) file2->Get("CalorimeterData");
  int nEvents2 = tree2->GetEntries();
  tree2->SetBranchAddress("Energy_Dep", &eng2);

  TH1D * spec = new TH1D("spec", "Energy Spectrum", 300, 0, 1.4);
  
  for (int i=0; i< nEvents1; i++)
    {
      tree1->GetEntry(i);
      spec->Fill(eng1);
    }

  for (int i=0; i< nEvents2; i++)
    {
      tree2->GetEntry(i);
      spec->Fill(eng2);
    }

  TCanvas * canvas = new TCanvas("canvas", "canvas", 1000, 500);
  canvas->cd();
  canvas->SetLogy();
  spec->Draw();

}
