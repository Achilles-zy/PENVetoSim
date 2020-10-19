#include "PENActionInitialization.hh"
#include "PENPrimaryGeneratorAction.hh"
#include "PENRunAction.hh"
#include "PENEventAction.hh"
#include "PENSteppingAction.hh"
#include "PENTrackingAction.hh"
#include "PENStackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENActionInitialization::PENActionInitialization(PENDetectorConstruction* det)
	: G4VUserActionInitialization()
{
	fDetCons = det;
	fPrimaryGen = new PENPrimaryGeneratorAction(fDetCons);
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENActionInitialization::~PENActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENActionInitialization::BuildForMaster() const
{
	PENPrimaryGeneratorAction* gen = new PENPrimaryGeneratorAction(fDetCons);
	PENRunAction* runAction = new PENRunAction(fPrimaryGen, fDetCons);
	SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENActionInitialization::Build() const
{
	PENPrimaryGeneratorAction* gen = new PENPrimaryGeneratorAction(fDetCons);
	//fPrimaryGen = gen;
	SetUserAction(fPrimaryGen);

	PENRunAction* runAction = new PENRunAction(gen, fDetCons);
	SetUserAction(runAction);
	
	PENEventAction* eventAction = new PENEventAction(runAction);
	SetUserAction(eventAction);

	SetUserAction(new PENSteppingAction(eventAction));
	//SetUserAction(new PENTrackingAction(eventAction));
	//SetUserAction(new PENTrackingAction(eventAction));
	SetUserAction(new PENStackingAction);
}