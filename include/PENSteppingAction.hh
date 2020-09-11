// PENSteppingAction.hh

#ifndef PENSteppingAction_h
#define PENSteppingAction_h 1

#include "G4UserSteppingAction.hh"

#include "G4Types.hh"

class PENDetectorConstruction;
class PENEventAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PENSteppingAction : public G4UserSteppingAction
{
  public:
    PENSteppingAction(PENEventAction*);
   ~PENSteppingAction(){};

    void UserSteppingAction(const G4Step*);

  private:

    PENEventAction*      PENEvent;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
