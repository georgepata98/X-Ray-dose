#include "physics.hh"

MyPhysicsList::MyPhysicsList()
:G4VModularPhysicsList(),
fEm(nullptr)
{
    SetVerboseLevel(0);
    G4EmParameters::Instance();

    fEm = new G4EmPenelopePhysics(0);
    //fEm = new G4EmStandardPhysics_option4(0);

    RegisterPhysics(fEm);
}

MyPhysicsList::~MyPhysicsList()
{}

void MyPhysicsList::SetCuts()
{
    G4VUserPhysicsList::SetCuts();
    defaultCutValue = 0.3*mm;  //default = 1 mm in Geant4
    SetCutsWithDefault();

    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100*eV, 100*GeV);
}
