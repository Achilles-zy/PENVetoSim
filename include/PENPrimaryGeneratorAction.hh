#ifndef PENPrimaryGeneratorAction_h
#define PENPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "PENDetectorConstruction.hh"

class G4GeneralParticleSource;
class G4Event;
class PENDetectorConstruction;

class PENPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{

public:
    PENPrimaryGeneratorAction(PENDetectorConstruction*);
    ~PENPrimaryGeneratorAction();
    void GeneratePrimaries(G4Event* anEvent);
    G4double GetPrimaryE() {
        return PrimaryE;
    }
    G4String GetPrimaryName() {
        return PrimaryName;
    }

private:
    G4GeneralParticleSource* PENGPS;
    G4double PrimaryE;
    G4String PrimaryName;
    PENDetectorConstruction* fDetCons;

};

#endif