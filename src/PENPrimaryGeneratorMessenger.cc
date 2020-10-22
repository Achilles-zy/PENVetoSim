#include "PENPrimaryGeneratorMessenger.hh"

#include "PENPrimaryGeneratorAction.hh"
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

PENPrimaryGeneratorMessenger::PENPrimaryGeneratorMessenger(PENPrimaryGeneratorAction* Gun)
  : G4UImessenger(),
    fAction(Gun),
    fSrcDir(0),
    cmdSetSrcType(0),
    cmdLENGENDSetSrcPos(0)
{
  fSrcDir = new G4UIdirectory("/PEN/src/");
  fSrcDir->SetGuidance("PrimaryGenerator control");

  cmdSetSrcType = new G4UIcmdWithAString("/PEN/src/type", this);
  cmdSetSrcType->SetGuidance("Choose the type of source");
  cmdSetSrcType->SetParameterName("SrcType",true);
  cmdSetSrcType->SetDefaultValue("Wire");
  cmdSetSrcType->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  cmdLENGENDSetSrcPos = new G4UIcmdWithAString("/LEGEND/src/pos", this);
  cmdLENGENDSetSrcPos->SetGuidance("Choose the position of source");
  cmdLENGENDSetSrcPos->SetParameterName("SrcPos", true);
  cmdLENGENDSetSrcPos->SetDefaultValue("Side");
  cmdLENGENDSetSrcPos->AvailableForStates(G4State_PreInit, G4State_Idle);
  /*
  fSourcePositionX = new G4UIcmdWithADoubleAndUnit("/PEN/gun/sourcePositionX",this);
  fSourcePositionX->SetGuidance("Set Source x position");
  fSourcePositionX->SetParameterName("fPositionX",true);
  fSourcePositionX->SetDefaultValue(0.*mm);
  fSourcePositionX->AvailableForStates(G4State_Idle);

  fSourcePositionY = new G4UIcmdWithADoubleAndUnit("/PEN/gun/sourcePositionY",this);
  fSourcePositionY->SetGuidance("Set Source y position");
  fSourcePositionY->SetParameterName("fPositionY",true);
  fSourcePositionY->SetDefaultValue(0.*mm);
  fSourcePositionY->AvailableForStates(G4State_Idle);

  fSourcePositionZ = new G4UIcmdWithADoubleAndUnit("/PEN/gun/sourcePositionZ",this);
  fSourcePositionZ->SetGuidance("Set Source z position");
  fSourcePositionZ->SetParameterName("fPositionZ",true);
  fSourcePositionZ->SetDefaultValue(30.*mm);
  fSourcePositionZ->AvailableForStates(G4State_Idle);
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENPrimaryGeneratorMessenger::~PENPrimaryGeneratorMessenger()
{
  delete cmdSetSrcType;
  delete cmdLENGENDSetSrcPos;
  delete fSrcDir;
  //delete fSourceEnergy;
  //delete fSourcePositionX;
  //delete fSourcePositionY;
  //delete fSourcePositionZ;
  //delete fAction;//debug
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == cmdSetSrcType) {
  	fAction->SetSrcType(newValue);
  }

  if (command == cmdLENGENDSetSrcPos) {
      fAction->SetLEGENDSrcPos(newValue);
  }

  //if(command == fSourceEnergy){
  //	fAction->SetSourceType(3);
  //	fAction->SetSourceEnergy(fSourceEnergy->GetNewDoubleValue(newValue));
  //}
  //if(command == fSourcePositionX){
  //	fAction->SetSourcePositionX(fSourcePositionX->GetNewDoubleValue(newValue));
  //}
  //if(command == fSourcePositionY){
  //	fAction->SetSourcePositionY(fSourcePositionY->GetNewDoubleValue(newValue));
  //}
  //if(command == fSourcePositionZ){
  //	fAction->SetSourcePositionZ(fSourcePositionZ->GetNewDoubleValue(newValue));
  //}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
