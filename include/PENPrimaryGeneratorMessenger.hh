#ifndef PENPrimaryGeneratorMessenger_h
#define PENPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PENPrimaryGeneratorAction;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PENPrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PENPrimaryGeneratorMessenger(PENPrimaryGeneratorAction* );
    virtual ~PENPrimaryGeneratorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    PENPrimaryGeneratorAction* 		fAction;
    G4UIdirectory*                  fSrcDir;

    G4UIcmdWithAString* cmdSetSrcType;
    G4UIcmdWithAString* cmdLENGENDSetSrcPos;
    //G4UIcmdWithADoubleAndUnit* fSourceEnergy;
    //G4UIcmdWithADoubleAndUnit* fSourcePositionX;
    //G4UIcmdWithADoubleAndUnit* fSourcePositionY;
    //G4UIcmdWithADoubleAndUnit* fSourcePositionZ;
    //G4UIcmdWithADoubleAndUnit*      fPolarCmd;
    //G4UIcmdWithAnInteger*			fSourceType;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
