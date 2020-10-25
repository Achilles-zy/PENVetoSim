//PENRunAction.cc
 
#include "PENRunAction.hh"
#include "PENRunMessenger.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <time.h>
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENRunAction::PENRunAction(PENPrimaryGeneratorAction* gen, PENDetectorConstruction* det):
	SiPMEventCount(0),
	BulkEventCount(0),
	VetoEventCount(0),
	DetectableEventCount(0),
	VetoPossibleEvtCount(0),
	ROIEventCount(0),
	ROIVetoEventCount(0),
	ROIVetoPossibleEvtCount(0),
	ifRefresh(false),
	runID(0)
{
  fPrimaryMessenger = new PENRunMessenger(this);
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetVerboseLevel(1);
  analysisManager->CreateH1("edepBulk", "Edep in Bulk", 200, 0 * keV, 20 * keV);
  analysisManager->CreateH1("PhotonCount", "Photon Count", 1000, 0, 3000);
  analysisManager->CreateH1("PhotonCount2", "Photon Count", 100, 0, 100);


  analysisManager->CreateNtuple("RunRes", "EventCount");
  analysisManager->CreateNtupleIColumn(0, "VetoCount");
  analysisManager->CreateNtupleIColumn(0, "SiPMCount");
  analysisManager->CreateNtupleIColumn(0, "BulkCount");
  analysisManager->CreateNtupleIColumn(0, "DetectableCount");
  analysisManager->CreateNtupleIColumn(0, "VetoPossibleCount");
  analysisManager->CreateNtupleIColumn(0, "ROIEventCount");
  analysisManager->CreateNtupleIColumn(0, "ROIVetoEventCount");
  analysisManager->CreateNtupleIColumn(0, "ROIVetoPossibleEvtCount");
  analysisManager->FinishNtuple(0);

  analysisManager->CreateNtuple("PhotonCount", "PhotonCoutnt");
  analysisManager->CreateNtupleIColumn(1, "SiPMPhotonCount");
  analysisManager->FinishNtuple(1);
  //analysisManager->SetFirstNtupleId(0);

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(SiPMEventCount);
  accumulableManager->RegisterAccumulable(VetoEventCount);
  accumulableManager->RegisterAccumulable(BulkEventCount);
  accumulableManager->RegisterAccumulable(DetectableEventCount);
  accumulableManager->RegisterAccumulable(VetoPossibleEvtCount);
  accumulableManager->RegisterAccumulable(ROIEventCount);
  accumulableManager->RegisterAccumulable(ROIVetoEventCount);
  accumulableManager->RegisterAccumulable(ROIVetoPossibleEvtCount);
  fDetCons = det;
  fPrimaryGenerator = gen;
  filename = "Simulation Result";
  txtname = "Simulation Result";

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENRunAction::~PENRunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENRunAction::BeginOfRunAction(const G4Run* aRun)
{
  auto analysisManager = G4AnalysisManager::Instance();

  G4int RunID = aRun->GetRunID();
  G4String fileName;
  if (fDetCons->GetMode() == "CDEX") {
	  fileName = fDetCons->GetWireType() + "_" + fDetCons->GetConfine() + "_" + std::to_string(RunID);
	  filename = fileName;
	  txtname = fDetCons->GetWireType() + "_" + fDetCons->GetConfine();
  }
  else if (fDetCons->GetMode() == "LEGEND") {
	  fileName = "LEGEND_" + fDetCons->GetRunInfo() + "_" + std::to_string((int)fDetCons->GetReadoutAngle());
	  filename = fileName;
	  txtname = "LEGEND_" + fDetCons->GetRunInfo();
  }

  //analysisManager->SetFileName(fileName);
  analysisManager->OpenFile(fileName);

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
  if (G4RunManager::GetRunManager()->GetRunManagerType() == 1) {
	  G4cout << "Run started." << G4endl;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  auto analysisManager = G4AnalysisManager::Instance();
  if (G4RunManager::GetRunManager()->GetRunManagerType() != 1) {
	  analysisManager->FillNtupleIColumn(0, 0, VetoEventCount.GetValue());
	  analysisManager->FillNtupleIColumn(0, 1, SiPMEventCount.GetValue());
	  analysisManager->FillNtupleIColumn(0, 2, BulkEventCount.GetValue());
	  analysisManager->FillNtupleIColumn(0, 3, DetectableEventCount.GetValue());
	  analysisManager->FillNtupleIColumn(0, 4, VetoPossibleEvtCount.GetValue());
	  analysisManager->FillNtupleIColumn(0, 5, ROIEventCount.GetValue());
	  analysisManager->FillNtupleIColumn(0, 6, ROIVetoEventCount.GetValue());
	  analysisManager->FillNtupleIColumn(0, 7, ROIVetoPossibleEvtCount.GetValue());
	  analysisManager->AddNtupleRow(0);
  }


  if (fDetCons->GetMode() == "CDEX") {
	  CDEXOutput(aRun);
  }
  else if (fDetCons->GetMode() == "LEGEND") {
	  LEGENDOutput(aRun);
  }
  else {
	  G4cout << "ERRRO! Mode does not exsist, nothing to output!" << G4endl;
  }
  ifRefresh = false;
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENRunAction::CDEXOutput(const G4Run* aRun) {
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
		G4cout << "ROIEventCount =" << ROIEventCount.GetValue() << G4endl;
		G4cout << "ROIVetoEventCount =" << ROIVetoEventCount.GetValue() << G4endl;
		G4cout << "ROIVetoPossibleEventCount =" << ROIVetoPossibleEvtCount.GetValue() << G4endl;
		G4cout << G4endl;
		G4cout << G4endl;
		G4cout << "===============================================================" << G4endl;
	
		std::ofstream output;
		if (aRun->GetRunID() == 0) {
			output.open(txtname + ".txt", std::ios::ate);
			output
				<< "Wire Type:\t" << fDetCons->GetWireType() << G4endl
				<< "Confine Info:\t" << fDetCons->GetConfine() << G4endl
				<< "Shell Layer Number:\t" << std::to_string(fDetCons->GetLayerNb()) << G4endl
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

		G4double ROIeff;
		if (ROIEventCount.GetValue() != 0) {
			ROIeff = ROIVetoEventCount.GetValue() / ROIEventCount.GetValue();
		}
		else
		{
			ROIeff = 0;
		}

		if (aRun->GetRunID() == 0) {
			output
				<< std::setw(10) << std::left << "Run ID" << '\t'
				<< std::setw(40) << std::left << "Number of Event" << '\t'
				<< std::setw(40) << std::left << "Primary Particle" << '\t'
				<< std::setw(40) << std::left << "Primary Energy(MeV)" << '\t'
				<< std::setw(40) << std::left << "SiPMEventCount" << '\t'
				<< std::setw(40) << std::left << "VetoEventCount" << '\t'
				<< std::setw(40) << std::left << "BulkEventCount" << '\t'
				<< std::setw(40) << std::left << "DetectableEventCount" << '\t'
				<< std::setw(40) << std::left << "VetoPossibleEventCount" << '\t'
				<< std::setw(40) << std::left << "VetoEfficiency" << '\t'
				<< std::setw(40) << std::left << "ROIEventCount" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEventCount" << '\t'
				<< std::setw(40) << std::left << "ROIVetoPossibleEventCount" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEfficiency" << '\t' << G4endl;
		}

		output
			<< std::setw(10) << std::left << aRun->GetRunID() << '\t'
			<< std::setw(40) << std::left << aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << fPrimaryGenerator->GetPrimaryName() << '\t'
			<< std::setw(40) << std::left << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fPrimaryGenerator->GetPrimaryE() << '\t'
			<< std::setw(40) << std::left << SiPMEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << BulkEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << DetectableEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoPossibleEvtCount.GetValue() << '\t'
			<< std::setw(40) << std::left << eff << '\t'
			<< std::setw(40) << std::left << ROIEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoPossibleEvtCount.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIeff << G4endl;
		output.close();
		//std::DecimalFormat df1 = new DecimalFormat("0.0");
	}
}


void PENRunAction::LEGENDOutput(const G4Run* aRun) {
	if (G4RunManager::GetRunManager()->GetRunManagerType() == 1) {
		G4cout << "End Running ..." << G4endl;
		G4cout << "==========================Run Summary==========================" << G4endl;
		G4cout << G4endl;
		G4cout << G4endl;
		G4cout << "ReadoutAngle =" << fDetCons->GetReadoutAngle() << G4endl;
		G4cout << "VetoEventCount =" << VetoEventCount.GetValue() << G4endl;
		G4cout << "SiPMEventCount =" << SiPMEventCount.GetValue() << G4endl;
		G4cout << "BulkEventCount =" << BulkEventCount.GetValue() << G4endl;
		G4cout << "DetectableEventCount =" << DetectableEventCount.GetValue() << G4endl;
		G4cout << "VetoPossibleEventCount =" << VetoPossibleEvtCount.GetValue() << G4endl;
		G4cout << "ROIEventCount =" << ROIEventCount.GetValue() << G4endl;
		G4cout << "ROIVetoEventCount =" << ROIVetoEventCount.GetValue() << G4endl;
		G4cout << "ROIVetoPossibleEventCount =" << ROIVetoPossibleEvtCount.GetValue() << G4endl;
		G4cout << "PropertiesID =" << fDetCons->GetPENPropertiesID() << G4endl;
		G4cout << G4endl;
		G4cout << G4endl;
		G4cout << "===============================================================" << G4endl;

		std::ofstream output;
		if (ifRefresh == true) {
			output.open(txtname + ".txt", std::ios::ate);
			output
				<< "Run Info:\t" << fDetCons->GetRunInfo() << G4endl;
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

		G4double ROIeff;
		if (ROIEventCount.GetValue() != 0) {
			ROIeff = ROIVetoEventCount.GetValue() / ROIEventCount.GetValue();
		}
		else
		{
			ROIeff = 0;
		}

		if (ifRefresh == true) {
			output
				<< std::setw(10) << std::left << "Run ID" << '\t'
				<< std::setw(40) << std::left << "Readout angle" << '\t'
				<< std::setw(40) << std::left << "Number of Event" << '\t'
				<< std::setw(40) << std::left << "Primary Particle" << '\t'
				<< std::setw(40) << std::left << "Primary Energy(MeV)" << '\t'
				<< std::setw(40) << std::left << "SiPMEventCount" << '\t'
				<< std::setw(40) << std::left << "VetoEventCount" << '\t'
				<< std::setw(40) << std::left << "BulkEventCount" << '\t'
				<< std::setw(40) << std::left << "DetectableEventCount" << '\t'
				<< std::setw(40) << std::left << "VetoPossibleEventCount" << '\t'
				<< std::setw(40) << std::left << "VetoEfficiency" << '\t'
				<< std::setw(40) << std::left << "ROIEventCount" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEventCount" << '\t'
				<< std::setw(40) << std::left << "ROIVetoPossibleEventCount" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEfficiency" << '\t' << G4endl;
		}

		output
			<< std::setw(10) << std::left << aRun->GetRunID() << '\t'
			<< std::setw(40) << std::left << fDetCons->GetReadoutAngle() << '\t'
			<< std::setw(40) << std::left << aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << fPrimaryGenerator->GetPrimaryName() << '\t'
			<< std::setw(40) << std::left << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fPrimaryGenerator->GetPrimaryE() << '\t'
			<< std::setw(40) << std::left << SiPMEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << BulkEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << DetectableEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoPossibleEvtCount.GetValue() << '\t'
			<< std::setw(40) << std::left << eff << '\t'
			<< std::setw(40) << std::left << ROIEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoPossibleEvtCount.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIeff << G4endl;
		output.close();
		//std::DecimalFormat df1 = new DecimalFormat("0.0");
	}
}