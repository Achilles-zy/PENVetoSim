//PENTrackingAction.cc

#include "PENTrackingAction.hh"
#include "PENDetectorConstruction.hh"
#include "PENEventAction.hh"

#include "G4Event.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENTrackingAction::PENTrackingAction(PENEventAction* evt)
    :PENEvent(evt)
{}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENTrackingAction::~PENTrackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENTrackingAction::PreUserTrackingAction(const G4Track* track)
{

    
    G4double charge = track->GetDefinition()->GetPDGCharge();
    G4int ID = track->GetTrackID();
    G4int parentID = track->GetParentID();
    //G4cout << "photoncnt = " << PENEvent->GetPhotonCnt() << G4endl;
    G4double trackTime = track->GetGlobalTime();
    //if (PENEvent->GetPhotonCnt() > 3) {
        //track->GetStep()->GetTrack()->SetTrackStatus(fStopAndKill);
    //}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENTrackingAction::PostUserTrackingAction(const G4Track* trk)
{
    const PENDetectorConstruction* detectorConstruction
        = static_cast<const PENDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4String particlename = trk->GetParticleDefinition()->GetParticleName();
    if (trk->GetVolume() == detectorConstruction->GetEnv() && particlename == "opticalphoton") {
        //PENEvent->CountEscapedPhoton(1);
        //G4cout << trk->GetVolume()->GetName() << G4endl;
        //G4cout << "Next" << G4endl;
    }

    //G4cout << trk->GetVolume()->GetName() << G4endl;
    //G4cout << detectorConstruction->GetEnv()->GetName() << G4endl;
    //G4cout << trk->GetVolume()->GetName() << G4endl;
    //G4cout << trk->GetNextVolume()->GetName() << G4endl;
    //G4cout << "" <<G4endl;
    /*
    auto next_volume = trk->GetNextVolume();
    for(G4int i=0; i<16; i++)
    {
        auto sipm = PENDetCons->GetSiPM(i);
        if(sipm == next_volume){
            PENEvent->AddToSiPM(i);
        }
    }
    */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
