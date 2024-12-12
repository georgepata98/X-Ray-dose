#ifndef RUN_HH
#define RUN_HH

#include "G4SystemOfUnits.hh"
#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4Accumulable.hh"
#include "G4AccumulableManager.hh"

#include <iostream>
#include <fstream>
#include <cmath>

class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction();
    ~MyRunAction() override;

    virtual void BeginOfRunAction(const G4Run *run) override;
    virtual void EndOfRunAction(const G4Run *run) override;
    
    void AddDdep(G4double dose) { totalDose += dose; }
    void AddDdep2(G4double dose) { totalDose2 += dose * dose; }

    std::ofstream* GetOutputFile();

private:
    G4Accumulable<G4double> totalDose = 0;
    G4Accumulable<G4double> totalDose2 = 0;

    std::ofstream fOutputFile;

    // Variabile pentru acumularea pe toate Run-urile simularii

    G4double totalDoseAllRuns, totalDoseAllRuns2;
    unsigned long long int totalEvents;
};

#endif