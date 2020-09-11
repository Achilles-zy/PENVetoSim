//PENRunAction.hh

#ifndef PENRunAction_h
#define PENRunAction_h

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4AccumulableManager.hh"
#include "G4Accumulable.hh"

class G4Run;

class PENRunAction : public G4UserRunAction
{
    public:
    PENRunAction();
    ~PENRunAction();

    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);

	void CountBulkEvent() {
		BulkEventCount += 1;
	};
	void CountSiPMEvent() { 
		SiPMEventCount += 1;
	};
	void CountVetoEvent() {
		VetoEventCount += 1;
	}

private:
	//number of events that generate signals in bulk
	//G4int BulkEventCount;
	//number of events that generate signals in SiPMs
	//G4int SiPMEventCount;
	//G4int VetoEventCount;
	G4Accumulable<G4int> SiPMEventCount;
	G4Accumulable<G4int> VetoEventCount;
	G4Accumulable<G4int> BulkEventCount;
};

#endif