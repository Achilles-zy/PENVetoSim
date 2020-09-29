#include "PENActionInitialization.hh"
#include "PENPrimaryGeneratorAction.hh"
#include "PENRunAction.hh"
#include "PENEventAction.hh"
#include "PENSteppingAction.hh"
#include "PENTrackingAction.hh"
#include "PENStackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENActionInitialization::PENActionInitialization()
	: G4VUserActionInitialization()
{
	fPrimaryGenerator = new PENPrimaryGeneratorAction();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENActionInitialization::~PENActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENActionInitialization::BuildForMaster() const
{
	PENRunAction* runAction = new PENRunAction(fPrimaryGenerator);
	SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENActionInitialization::Build() const
{
	SetUserAction(fPrimaryGenerator);

	PENRunAction* runAction = new PENRunAction(fPrimaryGenerator);
	SetUserAction(runAction);

	PENEventAction* eventAction = new PENEventAction(runAction);
	SetUserAction(eventAction);

	SetUserAction(new PENSteppingAction(eventAction));
	//SetUserAction(new PENTrackingAction(eventAction));
	SetUserAction(new PENStackingAction);
}