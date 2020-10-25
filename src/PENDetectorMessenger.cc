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
	cmdSetRunInfo(0),
	cmdSetLayerNb(0),
	cmdSetReadoutAngle(0),
	cmdSetPENPropertiesID(0)
{
	fDetDir = new G4UIdirectory("/PEN/det/set");
	fDetDir->SetGuidance("Set detector construction parameters");

	fPENDir = new G4UIdirectory("/PEN/sim/set");
	fPENDir->SetGuidance("Set simulation parameters");

	cmdSetWireType = new G4UIcmdWithAString("/PEN/det/set/wiretype", this);
	cmdSetWireType->SetGuidance("Select wire type.");
	cmdSetWireType->SetParameterName("WireType", false);
	cmdSetWireType->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetWireType->SetToBeBroadcasted(false);

	cmdSetConfine = new G4UIcmdWithAString("/PEN/det/set/confine", this);
	cmdSetConfine->SetGuidance("Set confine name in file name.");
	cmdSetConfine->SetParameterName("ConfineInfo", false);
	cmdSetConfine->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetConfine->SetToBeBroadcasted(false);

	cmdSetRunInfo = new G4UIcmdWithAString("/PEN/det/set/runinfo", this);
	cmdSetRunInfo->SetGuidance("Add run info");
	cmdSetRunInfo->SetParameterName("RunInfo", false);
	cmdSetRunInfo->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetRunInfo->SetToBeBroadcasted(false);

	cmdSetMode = new G4UIcmdWithAString("/PEN/sim/set/mode", this);
	cmdSetMode->SetGuidance("Select simulation mode.");
	cmdSetMode->SetParameterName("SimulationMode", false);
	cmdSetMode->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetMode->SetToBeBroadcasted(false);

	cmdSetLayerNb = new G4UIcmdWithAnInteger("/PEN/det/set/layernumber", this);
	cmdSetLayerNb->SetGuidance("Set PEN shell layer number");
	cmdSetLayerNb->SetParameterName("LayerNumber", false);
	cmdSetLayerNb->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetLayerNb->SetToBeBroadcasted(false);

	cmdSetReadoutAngle = new G4UIcmdWithADouble("/LEGEND/readout/angle", this);
	cmdSetReadoutAngle->SetGuidance("Set readout angle");
	cmdSetReadoutAngle->SetParameterName("Angle", false);
	cmdSetReadoutAngle->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetReadoutAngle->SetToBeBroadcasted(false);

	cmdSetPENPropertiesID = new G4UIcmdWithAnInteger("/PEN/mat/set/PENpropertiesID", this);
	cmdSetPENPropertiesID->SetGuidance("Set PEN properties ID");
	cmdSetPENPropertiesID->SetParameterName("ID", false);
	cmdSetPENPropertiesID->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetPENPropertiesID->SetToBeBroadcasted(false);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENDetectorMessenger::~PENDetectorMessenger()
{
	delete fDetDir;
	delete fPENDir;
	delete cmdSetWireType;
	delete cmdSetConfine;
	delete cmdSetLayerNb;
	delete cmdSetRunInfo;
	delete cmdSetPENPropertiesID;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	if (command == cmdSetWireType) {
		fDetCons->SetWireType(newValue);
	}

	if (command == cmdSetRunInfo) {
		fDetCons->SetRunInfo(newValue);
	}

	if (command == cmdSetConfine) {
		fDetCons->SetConfine(newValue);
	}

	if (command == cmdSetMode) {
		fDetCons->SetMode(newValue);
	}

	if (command == cmdSetLayerNb) {
		fDetCons->SetLayerNb(cmdSetLayerNb->GetNewIntValue(newValue));
	}

	if (command == cmdSetReadoutAngle) {
		fDetCons->SetReadoutAngle(cmdSetReadoutAngle->GetNewDoubleValue(newValue));
	}

	if (command == cmdSetPENPropertiesID) {
		fDetCons->SetPENPropertiesID(cmdSetPENPropertiesID->GetNewIntValue(newValue));
	}
}