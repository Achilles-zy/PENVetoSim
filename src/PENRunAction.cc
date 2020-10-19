//PENRunAction.cc
 
#include "PENRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <time.h>
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENRunAction::PENRunAction(PENPrimaryGeneratorAction* gen, PENDetectorConstruction* det):
	BulkEventCount(0),
	SiPMEventCount(0),
	VetoEventCount(0),
	DetectableEventCount(0),
	VetoPossibleEvtCount(0)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->CreateH1("edepBulk", "Edep in Bulk", 200, 0 * keV, 20 * keV);
  analysisManager->CreateH1("PhotonCount", "Photon Count", 200, 0, 200);

  analysisManager->CreateNtuple("Data", "EventCount");
  analysisManager->CreateNtupleIColumn("VetoCount");
  analysisManager->CreateNtupleIColumn("SiPMCount");
  analysisManager->CreateNtupleIColumn("BulkCount");
  analysisManager->CreateNtupleIColumn("DetectableCount");
  analysisManager->CreateNtupleIColumn("VetoPossibleCount");
  analysisManager->FinishNtuple();
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(SiPMEventCount);
  accumulableManager->RegisterAccumulable(VetoEventCount);
  accumulableManager->RegisterAccumulable(BulkEventCount);
  accumulableManager->RegisterAccumulable(DetectableEventCount);
  accumulableManager->RegisterAccumulable(VetoPossibleEvtCount);
  fDetCons = det;
  fPrimaryGenerator = gen;
  filename = "Simulation Result";
  txtname = "Simulation Result";

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
  G4int RunID = aRun->GetRunID();
  G4String fileName = fDetCons->GetWireType() + "_" + fDetCons->GetConfine() + "_" + std::to_string(fDetCons->GetLayerNb()) + "_" + std::to_string(RunID);
  filename = fileName;
  txtname = fDetCons->GetWireType() + "_" + fDetCons->GetConfine() + "_" + std::to_string(fDetCons->GetLayerNb());
  //analysisManager->SetFileName(fileName);
  analysisManager->OpenFile(fileName);

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
  if (G4RunManager::GetRunManager()->GetRunManagerType() == 1) {
	  G4cout << "Run " << aRun->GetRunID() << " started." << G4endl;
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
  analysisManager->FillNtupleIColumn(3, DetectableEventCount.GetValue());
  analysisManager->FillNtupleIColumn(4, VetoPossibleEvtCount.GetValue());
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
	  G4cout << "DetectableEventCount =" << DetectableEventCount.GetValue() << G4endl;
	  G4cout << "VetoPossibleEventCount =" << VetoPossibleEvtCount.GetValue() << G4endl;
	  G4cout << G4endl;
	  G4cout << G4endl;
	  G4cout << "===============================================================" << G4endl;

	  std::ofstream output;
	  if (aRun->GetRunID() == 0) {
		  output.open(txtname + ".txt", std::ios::ate);
		  output
			  << "Wire Type:\t" << fDetCons->GetWireType() << G4endl
			  << "Confine Info:\t" << fDetCons->GetConfine() << G4endl
			  << "Shell Layer Number:\t"  << std::to_string(fDetCons->GetLayerNb()) << G4endl
			  << "Simulation result:" << G4endl;
	  }
	  else
	  {
		  output.open(txtname + ".txt", std::ios::app);
	  }
	  G4double eff;
	  if (BulkEventCount.GetValue() != 0) {
		  eff = VetoEventCount.GetValue() / BulkEventCount.GetValue();
	  }
	  else
	  {
		  eff = 0;
	  }
	  output
		  << "Run ID:\t" << std::setw(5) << aRun->GetRunID() << '\t'
		  << "Number of Event is\t" << std::setw(10) << aRun->GetNumberOfEvent() << '\t'
		  << "Primary Particle is\t" << std::setw(5) << fPrimaryGenerator->GetPrimaryName() << '\t'
		  << "Primary Energy(MeV) =\t" << std::setw(5) << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fPrimaryGenerator->GetPrimaryE() << '\t'
		  << "SiPMEventCount =\t" << std::left << std::setw(10) << SiPMEventCount.GetValue() << '\t'
		  << "VetoEventCount =\t" << std::left << std::setw(10) << VetoEventCount.GetValue() << '\t'
		  << "BulkEventCount =\t" << std::left << std::setw(10) << BulkEventCount.GetValue() << '\t'
		  << "DetectableEventCount =\t" << std::left << std::setw(10) << DetectableEventCount.GetValue() << '\t'
		  << "VetoPossibleEventCount =\t" << std::left << std::setw(10) << VetoPossibleEvtCount.GetValue() << '\t'
		  << "VetoEfficiency =\t" << std::left << std::setw(5) << eff << G4endl;

	  output.close();
	  //std::DecimalFormat df1 = new DecimalFormat("0.0");
  }

  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
