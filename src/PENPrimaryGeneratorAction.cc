#include "PENPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RunManager.hh"

//#include "TMath.h"
#include "Randomize.hh"

PENPrimaryGeneratorAction::PENPrimaryGeneratorAction(PENDetectorConstruction* det):
	G4VUserPrimaryGeneratorAction(),
	PrimaryE(0),
	PrimaryName("")
{
    PENGPS = new G4GeneralParticleSource();
	fDetCons = det;
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "e-";
	G4double particleEnergy = 0 * MeV;
	/*
	G4PhysicalVolumeStore* PVStore = G4PhysicalVolumeStore::GetInstance();
	G4int i = 0;
	G4VPhysicalVolume* tempPV = NULL;
	while (i < G4int(PVStore->size())) {
		tempPV = (*PVStore)[i];
		G4cout << i << " " << " " << tempPV->GetName() << " " << G4endl;
		i++;
	}
	*/
	G4double Radius = fDetCons->GetWireRadius();
	G4double Length = fDetCons->GetWireLength();
	G4ThreeVector WirePos = fDetCons->GetWirePos();
	G4cout << "==========================Primary Info==========================" << G4endl;
	G4cout << "Wire Position: " << WirePos << G4endl;
	G4cout << "Wire Radius: " << Radius << G4endl;
	G4cout << "Wire Length: " << Length << G4endl;
	G4cout << "================================================================" << G4endl;
    PENGPS->SetParticleDefinition(particleTable->FindParticle(particleName));
	PENGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
	PENGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(particleEnergy);
	PENGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
	PENGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
	PENGPS->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
	PENGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(WirePos);
	PENGPS->GetCurrentSource()->GetPosDist()->SetRadius(Radius);
	PENGPS->GetCurrentSource()->GetPosDist()->SetHalfZ(Length / 2);

	//PENGPS->GetCurrentSource()->GetPosDist()->ConfineSourceToVolume("PENShell");
	//PENGPS->GetCurrentSource()->GetPosDist()->ConfineSourceToVolume("PENShell");

}

PENPrimaryGeneratorAction::~PENPrimaryGeneratorAction()
{
    delete PENGPS;
}

void PENPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    PENGPS->GeneratePrimaryVertex(anEvent);
	anEvent->GetEventID();
	if (anEvent->GetEventID() == 0) {
		PrimaryE = PENGPS->GetCurrentSource()->GetParticleEnergy();
		PrimaryName = PENGPS->GetCurrentSource()->GetParticleDefinition()->GetParticleName();
	}
}