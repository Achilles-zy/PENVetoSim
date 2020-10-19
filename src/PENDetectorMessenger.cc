#include "PENDetectorMessenger.hh"

#include "PENDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"

#include "G4RunManager.hh"

class PENDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

PENDetectorMessenger::PENDetectorMessenger(PENDetectorConstruction* Det)
	:G4UImessenger(),
	fDetCons(Det),
	fPENDir(0),
	fDetDir(0),
	cmdSetWireType(0),
	cmdSetConfine(0),
	cmdSetLayerNb(0)
{
	fDetDir = new G4UIdirectory("/PEN/det/set");
	fDetDir->SetGuidance("detector construction commands");

	cmdSetWireType = new G4UIcmdWithAString("/PEN/det/set/wiretype", this);
	cmdSetWireType->SetGuidance("Select wire type.");
	cmdSetWireType->SetParameterName("choice", false);
	cmdSetWireType->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetWireType->SetToBeBroadcasted(false);

	cmdSetConfine = new G4UIcmdWithAString("/PEN/det/set/confine", this);
	cmdSetConfine->SetGuidance("Set confine name in file name.");
	cmdSetConfine->SetParameterName("choice", false);
	cmdSetConfine->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetConfine->SetToBeBroadcasted(false);

	cmdSetLayerNb = new G4UIcmdWithAnInteger("/PEN/det/set/layernumber", this);
	cmdSetLayerNb->SetGuidance("Set PEN shell layer number");
	cmdSetLayerNb->SetParameterName("LayerNumber", false);
	cmdSetLayerNb->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetLayerNb->SetToBeBroadcasted(false);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENDetectorMessenger::~PENDetectorMessenger()
{
	delete fDetDir;
	delete fPENDir;
	delete cmdSetWireType;
	delete cmdSetConfine;
	delete cmdSetLayerNb;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	if (command == cmdSetWireType) {
		fDetCons->SetWireType(newValue);
	}

	if (command == cmdSetConfine) {
		fDetCons->SetConfine(newValue);
	}

	if (command == cmdSetLayerNb) {
		fDetCons->SetLayerNb(cmdSetLayerNb->GetNewIntValue(newValue));
	}

}