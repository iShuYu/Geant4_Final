#ifndef B2PhysicsList_h
#define B2PhysicsList_h 1
#include "G4VModularPhysicsList.hh"

//类继承于G4VModularPhysicsList
class B2PhysicsList: public G4VModularPhysicsList
{
public:
  B2PhysicsList();
  virtual ~B2PhysicsList();

  virtual void SetCuts();
};
#endif
