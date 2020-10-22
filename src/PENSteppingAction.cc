// PENSteppingAction.cc

#include "PENSteppingAction.hh"

#include "PENDetectorConstruction.hh"
#include "PENEventAction.hh"

#include "G4Track.hh"
#include "G4SteppingManager.hh"

#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4HadronicProcessType.hh"
#include "G4EmProcessSubType.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENSteppingAction::PENSteppingAction(
				     PENEventAction* evt)
:PENEvent(evt)
{ }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENSteppingAction::UserSteppingAction(const G4Step* aStep)
{
	auto volume = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
	auto particle_name = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();

	//G4cout << particle_name << G4endl;
	auto edep = aStep->GetTotalEnergyDeposit();
	if (PENEvent->GetSiPMPhotonCnt() > 3 && particle_name == "opticalphoton") {
		//aStep->GetTrack()->SetTrackStatus(fStopAndKill);
	}

	const PENDetectorConstruction* detectorConstruction
		= static_cast<const PENDetectorConstruction*>
		(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

	if (volume == detectorConstruction->GetBulk() && particle_name != "opticalphoton") {
		//PENEvent->BulkTrue();
		PENEvent->AddBulkEnergy(edep);
	}
	
	if (volume == detectorConstruction->GetEnv() && particle_name == "opticalphoton") {
		PENEvent->DetectableTrue();
	}

	for (int i = 0; i <= 11; i++) {
		//G4cout << detectorConstruction->GetSiPM(i)->GetName()<<G4endl;
		//G4cout << i << G4endl;
		if (volume == detectorConstruction->GetSiPM(i) && particle_name == "opticalphoton" && detectorConstruction->GetSiPM(i) != nullptr) {
			aStep->GetTrack()->SetTrackStatus(fStopAndKill);
			//G4cout << i << G4endl;
			//G4cout << detectorConstruction->GetSiPM(i)->GetName() << G4endl;
			PENEvent->AddToSiPM(i);
			PENEvent->CountSiPMPhoton(1);
		}
	}

	G4int processtype = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessSubType();
	G4int creatorprocess = aStep->GetTrack()->GetCreatorProcess()->GetProcessSubType();
	G4int parentID = aStep->GetTrack()->GetParentID();

	if (parentID == 1 ) {
		if (processtype == fScintillation || processtype == fRadioactiveDecay) {
			//G4cout << "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBb" << G4endl;
			//aStep->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
		}
	}

	if (processtype == fRadioactiveDecay) {
		//G4cout << "Parent ID =" <<parentID << G4endl;

	}

	if (parentID == 1) {
		
		//G4cout << "Parent ID =" << parentID << "Particle =" << aStep->GetTrack()->GetParticleDefinition()->GetParticleName() << "Process =" << aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() <<aStep->GetTotalEnergyDeposit()<< G4endl;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

