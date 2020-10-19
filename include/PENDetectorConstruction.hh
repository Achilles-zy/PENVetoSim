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
#include "PENDetectorMessenger.hh"
#include "PENDetectorConstruction.hh"
//#include "TMath.h"
//#include "G4GDMLParser.hh"

class PENMaterials;
class G4VPhysicalVolume;
class G4LogicalVolume;
class PENDeterctorMessenger;

class PENDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        PENDetectorConstruction();
        ~PENDetectorConstruction();
        const G4VPhysicalVolume* GetPENShell() const;
		const G4VPhysicalVolume* GetBulk() const;
        const G4VPhysicalVolume* GetSiPM(G4int i) const;
        const G4VPhysicalVolume* GetEnv() const;

        G4VPhysicalVolume* Construct();
        void DefineMat();

        void SetABS(G4double);
        void SetWireType(G4String);
        void SetConfine(G4String);
        void SetLayerNb(G4int);
        //void SetLayerNbS(G4String);

        G4ThreeVector GetWirePos() {
            return fWirePos;
        }

        G4double GetWireRadius() {
            return fWireRadius;
        }

        G4double GetWireLength() {
            return fWireLength;
        }

        G4String GetWireType() {
            return fType;
        }

        G4String GetConfine() {
            return fConfine;
        }

        G4int GetLayerNb() {
            return fLayerNb;
        }

        G4double GetPENShellLength() {
            return fPENShellLength;
        }

        G4double GetPENShellRadius() {
            return fPENShellRadius;
        }

    private:
        G4VPhysicalVolume* PENShell;
		G4VPhysicalVolume* Bulk;
        G4VPhysicalVolume* Env;
        G4VPhysicalVolume* physSiPM0;
        G4VPhysicalVolume* physSiPM1;
        G4VPhysicalVolume* physSiPM2;
        G4VPhysicalVolume* physSiPM3;
        G4VPhysicalVolume* physSiPM4;
        G4VPhysicalVolume* physSiPM5;
        G4VPhysicalVolume* physSiPM6;
        G4VPhysicalVolume* physSiPM7;
        G4VPhysicalVolume* physSiPM8;
        G4VPhysicalVolume* physSiPM9;
        G4VPhysicalVolume* physSiPM10;
        G4VPhysicalVolume* physSiPM11;
        G4VPhysicalVolume* physSiPM12;
        G4VPhysicalVolume* physSiPM13;
        G4VPhysicalVolume* physSiPM14;
        G4VPhysicalVolume* physSiPM15;
		//G4VPhysicalVolume* SiPM_0;
		//G4VPhysicalVolume* SiPM_1;
		//G4VPhysicalVolume* SiPM_2;
		//G4VPhysicalVolume* SiPM_3;
		//G4VPhysicalVolume* SiPM_4;
  //      G4VPhysicalVolume* SiPM_5;
		//G4VPhysicalVolume* SiPM_6;
		//G4VPhysicalVolume* SiPM_7;
		//G4VPhysicalVolume* SiPM_8;
		//G4VPhysicalVolume* SiPM_9;
		//G4VPhysicalVolume* SiPM_10;
		//G4VPhysicalVolume* SiPM_11;
  //      G4VPhysicalVolume* SiPM_12;
  //      G4VPhysicalVolume* SiPM_13;
  //      G4VPhysicalVolume* SiPM_14;
  //      G4VPhysicalVolume* SiPM_15;
        G4VPhysicalVolume* physWire;
        G4VPhysicalVolume* physPENShell;
        G4LogicalVolume* logicPENShell;
        G4Tubs* solidSideSiPM;

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
        G4String fType;
        G4String fConfine;
        G4int fLayerNb;
        G4ThreeVector fWirePos;
        G4double fWireCentDist;
        G4double fWireRadius;
        G4double fWireLength;
        G4double fPENShellRadius;
        G4double fPENShellLength;
        PENDetectorMessenger* fDetectorMessenger;
        
};

inline const G4VPhysicalVolume* PENDetectorConstruction::GetPENShell() const
{
    return PENShell;
}

inline const G4VPhysicalVolume* PENDetectorConstruction::GetBulk() const
{
	return Bulk;
}

inline const G4VPhysicalVolume* PENDetectorConstruction::GetEnv() const
{
    return Env;
}

inline const G4VPhysicalVolume* PENDetectorConstruction::GetSiPM(G4int i) const
{
    switch  (i){
        case 0:
        return physSiPM0;
        break;
        case 1:
        return physSiPM1;
        break;
        case 2:
        return physSiPM2;
        break;
        case 3:
        return physSiPM3;
        break;
        case 4:
        return physSiPM4;
        break;
        case 5:
        return physSiPM5;
        break;
        case 6:
        return physSiPM6;
        break;
        case 7:
        return physSiPM7;
        break;
        case 8:
        return physSiPM8;
        break;
        case 9:
        return physSiPM9;
        break;
        case 10:
            return physSiPM10;
            break;
        case 11:
            return physSiPM11;
            break;
        case 12:
            return physSiPM12;
            break;
        case 13:
            return physSiPM13;
            break;
        case 14:
            return physSiPM14;
            break;
        case 15:
            return physSiPM15;
            break;
        default:
        break;
    }
}

#endif