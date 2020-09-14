#ifndef PENDetectorConstruction_h
#define PENDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4String.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "PENMaterials.hh"
//#include "TMath.h"
//#include "G4GDMLParser.hh"

class PENMaterials;
class G4VPhysicalVolume;
class G4LogicalVolume;

class PENDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        PENDetectorConstruction();
        ~PENDetectorConstruction();
        const G4VPhysicalVolume* GetPENShell() const;
		const G4VPhysicalVolume* GetBulk() const;
        const G4VPhysicalVolume* GetSiPM(G4int i) const;

        G4VPhysicalVolume* Construct();
        void DefineMat();
        void SetABS(G4double);
    
    private:
        G4VPhysicalVolume* PENShell;
		G4VPhysicalVolume* Bulk;
		G4VPhysicalVolume* SiPM_0;
		G4VPhysicalVolume* SiPM_1;
		G4VPhysicalVolume* SiPM_2;
		G4VPhysicalVolume* SiPM_3;
		G4VPhysicalVolume* SiPM_4;
        G4VPhysicalVolume* SiPM_5;
		G4VPhysicalVolume* SiPM_6;
		G4VPhysicalVolume* SiPM_7;
		G4VPhysicalVolume* SiPM_8;
		G4VPhysicalVolume* SiPM_9;
		G4VPhysicalVolume* SiPM_10;
		G4VPhysicalVolume* SiPM_11;

        PENMaterials* matconstructor;

        G4double fLY;
        G4double fRES;
        G4double AbsorptionLength;
        G4double pmtReflectivity;
        G4double fRI;

        G4Material* fWorldMaterial;
        G4Material* fTargetMaterial;
        G4Material* fGlassMaterialPMT;
        G4Material* fPhotoCathodeMaterial;

        G4OpticalSurface* AirTarget;
        G4OpticalSurface* surfaceCathodeSupport;
        G4OpticalSurface* surfaceCathodeSupportBottom;

        G4MaterialPropertiesTable* MPT_PEN;
        G4MaterialPropertiesTable* MPT_GlassPMT;
        G4MaterialPropertiesTable* MPT_Target;
        G4MaterialPropertiesTable* SMPT_AirTarget;
        G4MaterialPropertiesTable* MPT_World;

        G4Material* matPEN;
        G4Material* matBialkali;
        G4Material* matSi;
        G4Material* fGe;
        G4Material* matAir;
        G4Material* fVacuum;
        G4Material* matTriggerFoilEJ212;
        G4Material* Pstyrene;
        G4Material* fGlass;
        G4Material* fPOM;
        G4Material* fABS;
        G4Material* matPMMA;
        G4Material* matEnGe;
        G4Material* matNaGe;
        G4Material* matGreaseEJ550;
        G4Material* matTeflon;
        G4Material* matVikuiti;
        G4Material* matPolyethylene;
        G4Material* matTitanium;
        G4Material* matLN2;
        G4Material* matLAr;
        G4Material* matGAGG;
		G4Material* matPTFE;
		G4Material* matCu;

        G4String fABSFile;


        
};

inline const G4VPhysicalVolume* PENDetectorConstruction::GetPENShell() const
{
    return PENShell;
}

inline const G4VPhysicalVolume* PENDetectorConstruction::GetBulk() const
{
	return Bulk;
}

inline const G4VPhysicalVolume* PENDetectorConstruction::GetSiPM(G4int i) const
{
    switch  (i){
        case 0:
        return SiPM_0;
        break;
        case 1:
        return SiPM_1;
        break;
        case 2:
        return SiPM_2;
        break;
        case 3:
        return SiPM_3;
        break;
        case 4:
        return SiPM_4;
        break;
        case 5:
        return SiPM_5;
        break;
        case 6:
        return SiPM_6;
        break;
        case 7:
        return SiPM_7;
        break;
        case 8:
        return SiPM_8;
        break;
        case 9:
        return SiPM_9;
        break;
        case 10:
        return SiPM_10;
        break;
        default:
        break;
    }
}

#endif