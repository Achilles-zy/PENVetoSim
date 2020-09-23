//PENRunAction.cc
 
#include "PENRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <time.h>
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENRunAction::PENRunAction():
	BulkEventCount(0),
	SiPMEventCount(0),
	VetoEventCount(0)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->CreateH1("edepBulk", "edepBulk", 200, 0 * keV, 20 * keV);
  analysisManager->CreateH1("PhotonCount", "PhotonCount", 200, 0, 200);

  analysisManager->CreateNtuple("Data", "EventCount");
  analysisManager->CreateNtupleIColumn("VetoCount");
  analysisManager->CreateNtupleIColumn("SiPMCount");
  analysisManager->CreateNtupleIColumn("BulkCount");
  analysisManager->FinishNtuple();
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(SiPMEventCount);
  accumulableManager->RegisterAccumulable(VetoEventCount);
  accumulableManager->RegisterAccumulable(BulkEventCount);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENRunAction::~PENRunAction()
{
  

  // delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENRunAction::BeginOfRunAction(const G4Run* aRun)
{
  auto analysisManager = G4AnalysisManager::Instance();
  G4String fileName = "Result";
  analysisManager->SetFileName(fileName);
  analysisManager->OpenFile();

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
  if (G4RunManager::GetRunManager()->GetRunManagerType() == 1) {
	  G4cout << "Begin Running ..." << G4endl;
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(0, VetoEventCount.GetValue());
  analysisManager->FillNtupleIColumn(1, SiPMEventCount.GetValue());
  analysisManager->FillNtupleIColumn(2, BulkEventCount.GetValue());
  analysisManager->AddNtupleRow();
  // analysisManager -> SetH1Plotting(0,true);
  // analysisManager -> SetH1Plotting(1,true);

  if (G4RunManager::GetRunManager()->GetRunManagerType() == 1) {
	  G4cout << "End Running ..." << G4endl;
	  G4cout << "==========================Run Summary==========================" << G4endl;
	  G4cout << G4endl;
	  G4cout << G4endl;
	  G4cout << "VetoEventCount =" << VetoEventCount.GetValue() << G4endl;
	  G4cout << "SiPMEventCount =" << SiPMEventCount.GetValue() << G4endl;
	  G4cout << "BulkEventCount =" << BulkEventCount.GetValue() << G4endl;
	  G4cout << G4endl;
	  G4cout << G4endl;
	  G4cout << "===============================================================" << G4endl;

	  std::ofstream output;
	  if (aRun->GetRunID() == 0) {
		  output.open("Simulation Result.txt", std::ios::ate);
	  }
	  else
	  {
		  output.open("Simulation Result.txt", std::ios::app);
	  }
	  output
		  << "Run ID:\t" << std::setw(5) << aRun->GetRunID() << '\t'
		  << "VetoEventCount =\t" << std::left << std::setw(10) << VetoEventCount.GetValue() << '\t'
		  << "SiPMEventCount =\t" << std::left << std::setw(10) << SiPMEventCount.GetValue() << '\t'
		  << "BulkEventCount =\t" << std::left << std::setw(10) << BulkEventCount.GetValue() << G4endl;
	  output.close();
	  //std::DecimalFormat df1 = new DecimalFormat("0.0");
  }

  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
