#include "B2SteppingAction.hh"
#include "B2EventAction.hh"
#include "B2aDetectorConstruction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4AntiNeutrinoE.hh"

B2SteppingAction::B2SteppingAction(B2EventAction* eventAction):
	G4UserSteppingAction(),
	fEventAction(eventAction)
  {}

B2SteppingAction::~B2SteppingAction()
 {}

void B2SteppingAction::UserSteppingAction(const G4Step *step)
{
	G4Track *track = step -> GetTrack();
	G4ThreeVector momentum = track -> GetMomentumDirection();
	if(track->GetDefinition() == G4AntiNeutrinoE::AntiNeutrinoE())
	{
		fEventAction->nuEnergy = track->GetKineticEnergy();
 	        fEventAction->nuTheta = momentum.theta();
        	fEventAction->nuPhi = momentum.phi();
	}
	//如果径迹判断为反中微子，那么将动量等信息复制给在EventAction中的设置的变量中，并在EventAction中fill到tree里
}
