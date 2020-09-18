#include "PENDetectorConstruction.hh"
#include "PENPhysicsList.hh"
#include "PENRunAction.hh"
#include "PENEventAction.hh"
#include "PENTrackingAction.hh"
#include "PENSteppingAction.hh"
#include "PENActionInitialization.hh"
#include "PENPrimaryGeneratorAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#define G4MT_USE

#ifdef G4MT_USE
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4UImanager.hh"

//#define G4VIS_USE
//#define G4UI_USE


//#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
//#endif

//#ifdef G4UI_USE
#include "G4UIExecutive.hh"
//#endif

#include "Randomize.hh"

int main(int argc, char** argv)
{
#ifdef G4MT_USE
	G4MTRunManager* PENRunManager = new G4MTRunManager;
#else
	G4RunManager* PENRunManager = new G4RunManager;
#endif

    PENDetectorConstruction* PENDetCons = new PENDetectorConstruction();
    PENRunManager -> SetUserInitialization(PENDetCons);

	G4PhysicalVolumeStore* PVStore = G4PhysicalVolumeStore::GetInstance();
	G4int i = 0;
	G4VPhysicalVolume* tempPV = NULL;
	while (i < G4int(PVStore->size())) {
		tempPV = (*PVStore)[i];
		G4cout << i << " " << " " << tempPV->GetName() << " " << G4endl;
		i++;
	}

	G4LogicalVolumeStore* LVStore = G4LogicalVolumeStore::GetInstance();
	i = 0;
	G4LogicalVolume* tempLV = NULL;
	while (i < G4int(PVStore->size())) {
		tempLV = (*LVStore)[i];
		G4cout << i << " " << " " << tempLV->GetName() << " " << G4endl;
		i++;
	}
	auto physicslist = new PENPhysicsList();
	physicslist->SetVerboseLevel(0);
    PENRunManager -> SetUserInitialization(physicslist);

    PENRunManager -> SetUserInitialization(new PENActionInitialization());

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/run/verbose 0");
    UImanager->ApplyCommand("/event/verbose 0");
    UImanager->ApplyCommand("/tracking/verbose 0");
    UImanager->ApplyCommand("/process/em/fluo true");
    UImanager->ApplyCommand("/process/em/auger true");
    UImanager->ApplyCommand("/process/em/augerCascade true");
    UImanager->ApplyCommand("/process/em/pixe true");

    //#ifdef G4VIS_USE
    G4VisManager* PENVis = new G4VisExecutive;
    PENVis -> Initialize();
    //#endif


    if (argc != 1){
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager -> ApplyCommand(command+fileName);
    }
    else {
        //#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        //#ifdef G4VIS_USE
        //UImanager -> ApplyCommand("/control/execute gps_def_03.mac");
        UImanager -> ApplyCommand("/control/execute init_vis.mac");
        //#endif
        ui -> SessionStart();
        delete ui;
        //#endif
    }

    //#ifdef G4VIS_USE
    delete PENVis;
    //#endif
    
    delete PENRunManager;
    return 0;
}