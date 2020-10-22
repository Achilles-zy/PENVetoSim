#pragma once
#ifndef PENRunMessenger_h
#define PENRunMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PENRunAction;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PENRunMessenger : public G4UImessenger
{
public:
    PENRunMessenger(PENRunAction*);
    virtual ~PENRunMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

private:
    PENRunAction* fAction;
    G4UIdirectory* fSrcDir;

    G4UIcmdWithABool* cmdRefresh;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
