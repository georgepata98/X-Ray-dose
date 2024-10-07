#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);  //1 primary particle per Event

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();  //accesare date particule si proprietatile lor
    G4ParticleDefinition *particle = particleTable->FindParticle("e-");

    G4ThreeVector mom(0, 0, 1);  //vectorul de propagare, in lungul axei Z

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(150*keV);  //energia cinetica a particulelor primare
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ThreeVector pos(0, 0, 9.3*cm);  //pozitia de generare a particulelor    
    fParticleGun->SetParticlePosition(pos);

    fParticleGun->GeneratePrimaryVertex(anEvent);
}