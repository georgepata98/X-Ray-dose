#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4SystemOfUnits.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    MyPrimaryGenerator();
    ~MyPrimaryGenerator() override;

public:
    virtual void GeneratePrimaries(G4Event* anEvent) override;

private:
    G4ParticleGun *fParticleGun = nullptr;  //clasa G4ParticleGun genereaza particule cu un impuls si o pozitie bine-definite (nealeatoare)
};

#endif