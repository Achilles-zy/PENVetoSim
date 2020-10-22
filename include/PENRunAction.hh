//PENRunAction.hh

#ifndef PENRunAction_h
#define PENRunAction_h

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4AccumulableManager.hh"
#include "G4Accumulable.hh"
#include "PENPrimaryGeneratorAction.hh"
#include "PENDetectorConstruction.hh"

class G4Run;
class PENPrimaryGeneratorAction;
class PENDetectorConstruction;
class PENRunMessenger;

class PENRunAction : public G4UserRunAction
{
    public:
    PENRunAction(PENPrimaryGeneratorAction*, PENDetectorConstruction*);
    ~PENRunAction();

    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);

	void CDEXOutput(const G4Run* aRun);
	void LEGENDOutput(const G4Run* aRun);

	void RefreshOutput(G4bool b) {
		ifRefresh = b;
	}
	void CountSiPMEvent() { 
		SiPMEventCount += 1;
	};

	inline 	void CountBulkEvent() {
		BulkEventCount += 1;
	};
	inline	void CountVetoEvent() {
		VetoEventCount += 1;
	}
	inline 	void CountDetectableEvent() {
		DetectableEventCount += 1;
	}
	inline 	void CountVetoPossibleEvent() {
		VetoPossibleEvtCount += 1;
	}


	inline 	void CountROIVetoEvent() {
		VetoEventCount += 1;
	}
	inline 	void CountROIVetoPossibleEvent() {
		VetoPossibleEvtCount += 1;
	}
	inline 	void CountROIEvent() {
		ROIEventCount += 1;
	}

private:
	//number of events that generate signals in bulk
	//G4int BulkEventCount;
	//number of events that generate signals in SiPMs
	//G4int SiPMEventCount;
	//G4int VetoEventCount;
	PENPrimaryGeneratorAction* fPrimaryGenerator;
	PENDetectorConstruction* fDetCons;
	G4Accumulable<G4int> SiPMEventCount;

	G4Accumulable<G4int> VetoEventCount;
	G4Accumulable<G4int> BulkEventCount;
	G4Accumulable<G4int> DetectableEventCount;
	G4Accumulable<G4int> VetoPossibleEvtCount;

	G4Accumulable<G4int> ROIVetoEventCount;
	G4Accumulable<G4int> ROIEventCount;
	G4Accumulable<G4int> ROIVetoPossibleEvtCount;


	PENRunMessenger* fPrimaryMessenger;
	G4String filename;
	G4String txtname;
	G4bool ifRefresh;
};

#endif