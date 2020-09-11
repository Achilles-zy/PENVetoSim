#ifndef PENActionInitialization_h
#define PENActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class PENActionInitialization : public G4VUserActionInitialization
{
public:
	PENActionInitialization();
	virtual ~PENActionInitialization();

	virtual void BuildForMaster() const;
	virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif