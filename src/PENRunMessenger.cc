#include "PENRunMessenger.hh"

#include "PENRunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PENPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

PENRunMessenger::PENRunMessenger(PENRunAction* run)
    : G4UImessenger(),
    cmdRefresh(0)
{
    fSrcDir = new G4UIdirectory("/PEN/run/");
    fSrcDir->SetGuidance("Run control");

    cmdRefresh = new G4UIcmdWithABool("/PEN/run/refresh", this);
    cmdRefresh->SetGuidance("Choose the type of source");
    cmdRefresh->SetParameterName("IfRefresh", true);
    cmdRefresh->SetDefaultValue("false");
    cmdRefresh->AvailableForStates(G4State_PreInit, G4State_Idle);

    fAction = run;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENRunMessenger::~PENRunMessenger()
{
    delete cmdRefresh;
    delete fSrcDir;
    //delete fSourceEnergy;
    //delete fSourcePositionX;
    //delete fSourcePositionY;
    //delete fSourcePositionZ;
    //delete fAction;//debug
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENRunMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == cmdRefresh) {
        fAction->RefreshOutput(cmdRefresh->GetNewBoolValue(newValue));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
