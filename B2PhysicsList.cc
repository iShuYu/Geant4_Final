#include "B2PhysicsList.hh" 
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
B2PhysicsList::B2PhysicsList()
: G4VModularPhysicsList(){
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

}

B2PhysicsList::~B2PhysicsList() {}

void B2PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}
