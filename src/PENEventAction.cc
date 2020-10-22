//PENEventAction.cc

#include "PENEventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENEventAction::PENEventAction(PENRunAction* runaction)
	: edepBulk(0.),
	SiPM_0(0),
	SiPM_1(0),
	SiPM_2(0),
	SiPM_3(0),
	SiPM_4(0),
	SiPM_5(0),
	SiPM_6(0),
	SiPM_7(0),
	SiPM_8(0),
	SiPM_9(0),
	SiPM_10(0),
	SiPM_11(0),
	SiPM_12(0),
	SiPM_13(0),
	SiPM_14(0),
	SiPM_15(0),
	Total(0),
	ID(0),
    SiPMPhotonCount(0),
    EscapedPhotonCount(0),
	ifSiPM(false),
	ifBulk(false),
    ifDetectable(false),
	run(runaction)
    //ResultFile("Distribution_Results_NTuple.root","RECREATE"),
    //Distribution_Results("Distribution_Results","Distribution_Results")
{
  /*
  Distribution_Results.Branch("SiPM_0",&SiPM_0, "SiPM_0/I");
  Distribution_Results.Branch("SiPM_1",&SiPM_1, "SiPM_1/I");
  Distribution_Results.Branch("SiPM_2",&SiPM_2, "SiPM_2/I");
  Distribution_Results.Branch("SiPM_3",&SiPM_3, "SiPM_3/I");
  Distribution_Results.Branch("SiPM_4",&SiPM_4, "SiPM_4/I");
  Distribution_Results.Branch("SiPM_5",&SiPM_5, "SiPM_5/I");
  Distribution_Results.Branch("SiPM_6",&SiPM_6, "SiPM_6/I");
  Distribution_Results.Branch("SiPM_7",&SiPM_7, "SiPM_7/I");
  Distribution_Results.Branch("SiPM_8",&SiPM_8, "SiPM_8/I");
  Distribution_Results.Branch("SiPM_9",&SiPM_9, "SiPM_9/I");
  Distribution_Results.Branch("SiPM_10",&SiPM_10, "SiPM_10/I");
  Distribution_Results.Branch("SiPM_11",&SiPM_11, "SiPM_11/I");
  Distribution_Results.Branch("SiPM_12",&SiPM_12, "SiPM_12/I");
  Distribution_Results.Branch("SiPM_13",&SiPM_13, "SiPM_13/I");
  Distribution_Results.Branch("SiPM_14",&SiPM_14, "SiPM_14/I");
  Distribution_Results.Branch("SiPM_15",&SiPM_15, "SiPM_15/I");
  Distribution_Results.Branch("Total",&Total, "Total/I");
  */
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENEventAction::~PENEventAction()
{
  //Distribution_Results.Write();
  //ResultFile.Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENEventAction::BeginOfEventAction(const G4Event* evt)
{
  edepBulk = 0;
  SiPM_0 = 0;
  SiPM_1 = 0;
  SiPM_2 = 0;
  SiPM_3 = 0;
  SiPM_4 = 0;
  SiPM_5 = 0;
  SiPM_6 = 0;
  SiPM_7 = 0;
  SiPM_8 = 0;
  SiPM_9 = 0;
  SiPM_10 = 0;
  SiPM_11 = 0;
  SiPM_12 = 0;
  SiPM_13 = 0;
  SiPM_14 = 0;
  SiPM_15 = 0;
  Total = 0;
  SiPMPhotonCount = 0;
  EscapedPhotonCount = 0;
  ifSiPM = false;
  ifBulk = false;
  ifDetectable = false;
  // G4cout<<ID<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENEventAction::EndOfEventAction(const G4Event* evt)
{

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(0, edepBulk);
  analysisManager->FillH1(1, SiPMPhotonCount);
  //G4cout << SiPMPhotonCount << G4endl;
	if (edepBulk > 0 && SiPMPhotonCount > 0) {
		run->CountVetoEvent();
    }

	if (edepBulk > 0) {
		run->CountBulkEvent();
	}

	if (SiPMPhotonCount > 0) {
		run->CountSiPMEvent();
	}

    if (ifDetectable == true) {
        run->CountDetectableEvent();
    }

    if (ifDetectable == true && edepBulk > 0) {
        run->CountVetoPossibleEvent();
    }

    if (edepBulk > 2200 * keV && edepBulk < 2400 * keV) {
        run->CountROIEvent();

        if (SiPMPhotonCount > 0) {
            run->CountROIVetoEvent();
        }

        if (ifDetectable == true) {
            run->CountROIVetoPossibleEvent();
        }
    }

    G4int evtID = evt->GetEventID();

    if (evtID % 5000 == 0) {
        G4cout << evtID << G4endl;
    }
  //G4cout<<TE<<G4endl;

  // analysisManager -> FillH1(0, Energy_0);
  // analysisManager -> FillH1(1, SiPM_0);
  // analysisManager -> FillH1(2, SiPM_1);
  // analysisManager -> FillH1(3, SiPM_2);
  // analysisManager -> FillH1(4, SiPM_3);
  // analysisManager -> FillH1(5, SiPM_4);
  // analysisManager -> FillH1(6, SiPM_5);
  // analysisManager -> FillH1(7, SiPM_6);
  // analysisManager -> FillH1(8, SiPM_7);
  // analysisManager -> FillH1(9, SiPM_8);
  // analysisManager -> FillH1(10, SiPM_9);
  // analysisManager -> FillH1(11, SiPM_10);
  // analysisManager -> FillH1(12, SiPM_11);
  // analysisManager -> FillH1(13, SiPM_12);
  // analysisManager -> FillH1(14, SiPM_13);
  // analysisManager -> FillH1(15, SiPM_14);
  // analysisManager -> FillH1(16, SiPM_15);
  // analysisManager -> FillH1(17, Total);
  //Distribution_Results.Fill();

  ID++;

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENEventAction::AddBulkEnergy(G4double de)
{

    edepBulk += de;

}

void PENEventAction::AddToSiPM(G4int i){
  Total++;
  switch (i){
    case 0:
    SiPM_0 ++;
    break;
    case 1:
    SiPM_1 ++;
    break;
    case 2:
    SiPM_2 ++;
    break;
    case 3:
    SiPM_3 ++;
    break;
    case 4:
    SiPM_4 ++;
    break;
    case 5:
    SiPM_5 ++;
    break;
    case 6:
    SiPM_6 ++;
    break;
    case 7:
    SiPM_7 ++;
    break;
    case 8:
    SiPM_8 ++;
    break;
    case 9:
    SiPM_9 ++;
    break;
    case 10:
    SiPM_10 ++;
    break;
    case 11:
    SiPM_11 ++;
    break;
    case 12:
    SiPM_12 ++;
    break;
    case 13:
    SiPM_13 ++;
    break;
    case 14:
    SiPM_14 ++;
    break;
    case 15:
    SiPM_15 ++;
    break;
    default:
    break;
  }
}