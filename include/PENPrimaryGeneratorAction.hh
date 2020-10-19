#ifndef PENPrimaryGeneratorAction_h
#define PENPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "PENDetectorConstruction.hh"
#include "PENPrimaryGeneratorMessenger.hh"

class G4GeneralParticleSource;
class G4Event;
class PENDetectorConstruction;
class PENPrimaryGeneratorMessenger;

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

    void SetSrcType(G4String type) {
        SrcType = type;
    }

private:
    G4double PrimaryE;
    G4String PrimaryName;
    G4String SrcType;

    G4GeneralParticleSource* fPENGPS;
    PENDetectorConstruction* fDetCons;
    PENPrimaryGeneratorMessenger* fPrimaryMessenger;
};

#endif