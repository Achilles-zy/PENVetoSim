#ifndef PENEventAction_h
#define PENEventAction_h 1

#include "G4UserEventAction.hh"
#include "PENRunAction.hh"
#include "globals.hh"
//#include "TROOT.h"
//#include "TFile.h"
//#include "TNtuple.h"
//#include "Rtypes.h"

class TNtuple;
class TFile;
class G4Event;

class PENEventAction : public G4UserEventAction
{
  public:
    PENEventAction(PENRunAction* runaction);
   ~PENEventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
    void AddBulkEnergy(G4double);
    void AddToSiPM(G4int);
	void SiPMTrue() { ifSiPM == true; }
	void BulkTrue() { ifBulk == true; }
	void CountSiPMPhoton(G4int ph) { PhotonCount = PhotonCount + ph; }

  private:
    G4double edepBulk;
    G4int SiPM_0;
    G4int SiPM_1;
    G4int SiPM_2;
    G4int SiPM_3;
    G4int SiPM_4;
    G4int SiPM_5;
    G4int SiPM_6;
    G4int SiPM_7;
    G4int SiPM_8;
    G4int SiPM_9;
    G4int SiPM_10;
    G4int SiPM_11;
    G4int SiPM_12;
    G4int SiPM_13;
    G4int SiPM_14;
    G4int SiPM_15;
    G4int Total;

    G4int ID;
	G4int PhotonCount;
    
	G4bool ifSiPM;
	G4bool ifBulk;

	PENRunAction* run;
    //TFile ResultFile;
    //TTree Distribution_Results;
};

#endif