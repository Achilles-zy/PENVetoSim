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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENTrackingAction::PENTrackingAction(PENDetectorConstruction* det,
				     PENEventAction* evt):
    PENDetCons(det),PENEvent(evt)
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
    G4double trackTime = track->GetGlobalTime();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENTrackingAction::PostUserTrackingAction(const G4Track* trk)
{
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
