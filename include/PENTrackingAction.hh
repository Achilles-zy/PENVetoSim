#ifndef PENTrackingAction_h
#define PENTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class G4Track;
class PENDetectorConstruction;
class PENEventAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PENTrackingAction : public G4UserTrackingAction
{
  public:
    PENTrackingAction(PENDetectorConstruction*, PENEventAction*);
   ~PENTrackingAction();

  public:
    void PreUserTrackingAction(const G4Track*);
    void PostUserTrackingAction(const G4Track*);

  private:
    PENDetectorConstruction* PENDetCons;
    PENEventAction*      PENEvent;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
