#ifndef PENDetectorMessenger_h
#define PENDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PENDetectorConstruction;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PENDetectorMessenger : public G4UImessenger
{
public:

    PENDetectorMessenger(PENDetectorConstruction*);
    ~PENDetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

private:

    PENDetectorConstruction* fDetCons;

    G4UIdirectory* fPENDir;
    G4UIdirectory* fDetDir;

    G4UIcmdWithAString* cmdSetWireType;
    G4UIcmdWithAString* cmdSetConfine;
    G4UIcmdWithAnInteger* cmdSetLayerNb;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
