#ifndef B2SteppingAction_h
#define B2SteppingAction_h 1
#include "G4UserSteppingAction.hh"

class B2aDetectorConstruction;
class B2EventAction;

class B2SteppingAction : public G4UserSteppingAction
{
	public:
	B2SteppingAction(B2EventAction *eventAction);
	virtual ~B2SteppingAction();
	virtual void UserSteppingAction(const G4Step *step);
	
	private:
	B2EventAction *fEventAction;
};

#endif
