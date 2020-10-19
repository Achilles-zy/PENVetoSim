#ifndef PENActionInitialization_h
#define PENActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

#include "PENPrimaryGeneratorAction.hh"
#include "PENDetectorConstruction.hh"

class PENPrimaryGeneratorAction;
class PENDetectorConstruction;
/// Action initialization class.

class PENActionInitialization : public G4VUserActionInitialization
{
public:
	PENActionInitialization(PENDetectorConstruction*);
	virtual ~PENActionInitialization();

	virtual void BuildForMaster() const;
	virtual void Build() const;

private:
	PENPrimaryGeneratorAction* fPrimaryGen;
	PENDetectorConstruction* fDetCons;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif