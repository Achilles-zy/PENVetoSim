#ifndef PENPrimaryGeneratorAction_h
#define PENPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4Event;

class PENPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
private:
    G4GeneralParticleSource* PENGPS;
public:
    PENPrimaryGeneratorAction();
    ~PENPrimaryGeneratorAction();
    void GeneratePrimaries(G4Event* anEvent);

};

#endif