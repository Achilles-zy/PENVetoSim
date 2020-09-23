#include "PENPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalVolumeStore.hh"

//#include "TMath.h"
#include "Randomize.hh"

PENPrimaryGeneratorAction::PENPrimaryGeneratorAction()
{
    PENGPS = new G4GeneralParticleSource();

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
    PENGPS->SetParticleDefinition(particleTable->FindParticle(particleName));
	PENGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
	PENGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(particleEnergy);
	PENGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
	PENGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
	PENGPS->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
	PENGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0, 0, 0));
	PENGPS->GetCurrentSource()->GetPosDist()->SetRadius(60 * mm);
	PENGPS->GetCurrentSource()->GetPosDist()->SetHalfZ(61 * mm);

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
}