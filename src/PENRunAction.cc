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
	ifRefresh(false)
{
  fPrimaryMessenger = new PENRunMessenger(this);
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
  analysisManager->CreateNtupleIColumn("ROIEventCount");
  analysisManager->CreateNtupleIColumn("ROIVetoEventCount");
  analysisManager->CreateNtupleIColumn("ROIVetoPossibleEvtCount");
  analysisManager->FinishNtuple();
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
  

  // delete G4AnalysisManager::Instance();
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
	  fileName = "LEGEND_" + fDetCons->GetRunInfo() + "_" + std::to_string(RunID);
	  filename = fileName;
	  txtname = "LEGEND_" + fDetCons->GetRunInfo();
  }

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
  analysisManager->FillNtupleIColumn(5, ROIEventCount.GetValue());
  analysisManager->FillNtupleIColumn(6, ROIVetoEventCount.GetValue());
  analysisManager->FillNtupleIColumn(7, ROIVetoPossibleEvtCount.GetValue());
  analysisManager->AddNtupleRow();
  // analysisManager -> SetH1Plotting(0,true);
  // analysisManager -> SetH1Plotting(1,true);
  analysisManager->Write();
  analysisManager->CloseFile();
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
				<< std::setw(10) << "Run ID" << '\t'
				<< std::setw(40) << "Number of Event" << '\t'
				<< std::setw(40) << "Primary Particle" << '\t'
				<< std::setw(40) << "Primary Energy(MeV)" << '\t'
				<< std::setw(40) << "SiPMEventCount" << '\t'
				<< std::setw(40) << "VetoEventCount" << '\t'
				<< std::setw(40) << "BulkEventCount" << '\t'
				<< std::setw(40) << "DetectableEventCount" << '\t'
				<< std::setw(40) << "VetoPossibleEventCount" << '\t'
				<< std::setw(40) << "VetoEfficiency" << '\t'
				<< std::setw(40) << "ROIEventCount" << '\t'
				<< std::setw(40) << "ROIVetoEventCount" << '\t'
				<< std::setw(40) << "ROIVetoPossibleEventCount" << '\t'
				<< std::setw(40) << "ROIVetoEfficiency" << '\t' << G4endl;
		}

		output
			<< std::setw(10) << aRun->GetRunID() << '\t'
			<< std::setw(40) << aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << fPrimaryGenerator->GetPrimaryName() << '\t'
			<< std::setw(40) << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fPrimaryGenerator->GetPrimaryE() << '\t'
			<< std::setw(40) << SiPMEventCount.GetValue() << '\t'
			<< std::setw(40) << VetoEventCount.GetValue() << '\t'
			<< std::setw(40) << BulkEventCount.GetValue() << '\t'
			<< std::setw(40) << DetectableEventCount.GetValue() << '\t'
			<< std::setw(40) << VetoPossibleEvtCount.GetValue() << '\t'
			<< std::setw(40) << eff << '\t'
			<< std::setw(40) << ROIEventCount.GetValue() << '\t'
			<< std::setw(40) << ROIVetoEventCount.GetValue() << '\t'
			<< std::setw(40) << ROIVetoPossibleEvtCount.GetValue() << '\t'
			<< std::setw(40) << ROIeff << G4endl;
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
		G4cout << G4endl;
		G4cout << G4endl;
		G4cout << "===============================================================" << G4endl;

		std::ofstream output;
		if (ifRefresh==true) {
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
				<< std::setw(10) << "Run ID" << '\t'
				<< std::setw(10) << "Readout angle" << '\t'
				<< std::setw(40) << "Number of Event" << '\t'
				<< std::setw(40) << "Primary Particle" << '\t'
				<< std::setw(40) << "Primary Energy(MeV)" << '\t'
				<< std::setw(40) << "SiPMEventCount" << '\t'
				<< std::setw(40) << "VetoEventCount" << '\t'
				<< std::setw(40) << "BulkEventCount" << '\t'
				<< std::setw(40) << "DetectableEventCount" << '\t'
				<< std::setw(40) << "VetoPossibleEventCount" << '\t'
				<< std::setw(40) << "VetoEfficiency" << '\t'
				<< std::setw(40) << "ROIEventCount" << '\t'
				<< std::setw(40) << "ROIVetoEventCount" << '\t'
				<< std::setw(40) << "ROIVetoPossibleEventCount" << '\t'
				<< std::setw(40) << "ROIVetoEfficiency" << '\t' << G4endl;
		}

		output
			<< std::setw(10) << aRun->GetRunID() << '\t'
			<< std::setw(40) << fDetCons->GetReadoutAngle() << '\t'
			<< std::setw(40) << aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << fPrimaryGenerator->GetPrimaryName() << '\t'
			<< std::setw(40) << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fPrimaryGenerator->GetPrimaryE() << '\t'
			<< std::setw(40) << SiPMEventCount.GetValue() << '\t'
			<< std::setw(40) << VetoEventCount.GetValue() << '\t'
			<< std::setw(40) << BulkEventCount.GetValue() << '\t'
			<< std::setw(40) << DetectableEventCount.GetValue() << '\t'
			<< std::setw(40) << VetoPossibleEvtCount.GetValue() << '\t'
			<< std::setw(40) << eff << '\t'
			<< std::setw(40) << ROIEventCount.GetValue() << '\t'
			<< std::setw(40) << ROIVetoEventCount.GetValue() << '\t'
			<< std::setw(40) << ROIVetoPossibleEvtCount.GetValue() << '\t'
			<< std::setw(40) << ROIeff << G4endl;
		output.close();
		//std::DecimalFormat df1 = new DecimalFormat("0.0");
	}
}