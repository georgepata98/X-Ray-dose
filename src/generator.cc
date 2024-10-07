#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
:G4VUserPrimaryGeneratorAction()
{
    fParticleGun = new G4ParticleGun(1);  //1 primary particle per Event
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();  //accesare date cu particule si proprietatile lor
    G4ParticleDefinition *particle = particleTable->FindParticle("e-");

    G4ThreeVector pos(0, 0, 9.3*cm);  //pozitia de generare a particulelor
    G4ThreeVector mom(0, 0, 1);  //vectorul de propagare, in lungul axei Z

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(150*keV);  //energia cinetica a particulelor primare
    fParticleGun->SetParticleDefinition(particle);

    fParticleGun->GeneratePrimaryVertex(anEvent);
}