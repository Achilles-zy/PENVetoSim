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

class PENRunAction : public G4UserRunAction
{
    public:
    PENRunAction(PENPrimaryGeneratorAction*, PENDetectorConstruction*);
    ~PENRunAction();

    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);


	void CountSiPMEvent() { 
		SiPMEventCount += 1;
	};

	void CountBulkEvent() {
		BulkEventCount += 1;
	};
	void CountVetoEvent() {
		VetoEventCount += 1;
	}
	void CountDetectableEvent() {
		DetectableEventCount += 1;
	}
	void CountVetoPossibleEvent() {
		VetoPossibleEvtCount += 1;
	}

	void CountROIBulkEvent() {
		BulkEventCount += 1;
	};
	void CountROIVetoEvent() {
		VetoEventCount += 1;
	}
	void CountROIDetectableEvent() {
		DetectableEventCount += 1;
	}
	void CountROIVetoPossibleEvent() {
		VetoPossibleEvtCount += 1;
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
	G4Accumulable<G4int> ROIBulkEventCount;
	G4Accumulable<G4int> ROIDetectableEventCount;
	G4Accumulable<G4int> ROIVetoPossibleEvtCount;



	G4String filename;
	G4String txtname;
};

#endif