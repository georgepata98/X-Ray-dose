#include "run.hh"

MyRunAction::MyRunAction()
{
    G4RunManager::GetRunManager()->SetPrintProgress(0);

    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();

    accumulableManager->RegisterAccumulable(totalDose);
    accumulableManager->RegisterAccumulable(totalDose2);
}

MyRunAction::~MyRunAction()
{
    if(fOutputFile.is_open()) fOutputFile.close();
}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();

    accumulableManager->Reset();

    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    fOutputFile.open("outp.txt");
}

void MyRunAction::EndOfRunAction(const G4Run* run)
{
    G4int nOfEvents = run->GetNumberOfEvent();
    if(nOfEvents == 0) return;

    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();

    if(fOutputFile.is_open()) fOutputFile.close();

    G4double meanDose = totalDose.GetValue() / nOfEvents;
    G4double meanDose2 = totalDose2.GetValue() / nOfEvents;
    G4double variance = meanDose2 - meanDose * meanDose;
    G4double stdev = std::sqrt(variance / nOfEvents);


    G4cout << G4endl << "--------------------------------------------------" << G4endl << "Number of primary events: " << nOfEvents << G4endl << "Mean absorbed dose (dose / number of events): " << G4BestUnit(meanDose, "Dose") << G4endl << "Uncertainty: +/- " << G4BestUnit(stdev, "Dose") << G4endl << "--------------------------------------------------" << G4endl << G4endl;
}

std::ofstream* MyRunAction::GetOutputFile()
{
    return &fOutputFile;
}