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
    G4int runID = run->GetRunID();
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();

    accumulableManager->Reset();

    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    fOutputFile.open(("outp_R" + std::to_string(runID) + ".txt").c_str());  //numele fisierului de output incarcat dupa fiecare event de succes

    if(runID == 0)
    {
        totalDoseAllRuns = 0;  //variabila este egala cu 0 cand incepe simularea (la primul Run)
        totalDoseAllRuns2 = 0;
    }
}

void MyRunAction::EndOfRunAction(const G4Run* run)
{
    G4int runID = run->GetRunID();  //ID-ul Run-ului curent

    G4int nOfEvents = run->GetNumberOfEvent();
    if(nOfEvents == 0) return;

    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();

    if(fOutputFile.is_open()) fOutputFile.close();

    G4double meanDose = totalDose.GetValue() / nOfEvents;
    G4double meanDose2 = totalDose2.GetValue() / nOfEvents;
    G4double variance = meanDose2 - meanDose * meanDose;
    G4double err = std::sqrt(variance / nOfEvents);

    G4cout << G4endl << "--------------------------------------------------" << G4endl << "Number of events in Run" << runID << ": " << nOfEvents << G4endl << "Mean absorbed dose (dose / number of events): " << G4BestUnit(meanDose, "Dose") << G4endl << "Uncertainty: +/- " << G4BestUnit(err, "Dose") << G4endl << "--------------------------------------------------" << G4endl << G4endl;



    //Crearea si umplerea fisierelor de output dupa fiecare Run

    std::ostringstream runFileName;
    runFileName << "output_Run" << runID << ".txt";  //numele fisierelor de output incarcat dupa fiecare Run

    std::ofstream runOutputFile(runFileName.str());
    runOutputFile << "----- Rezultate Run " << runID << " -----" << "\n";
    runOutputFile << G4BestUnit(meanDose, "Dose") << " +/- " << G4BestUnit(err, "Dose") << "\n";  //coloane cu doza medie per event si eroarea


    //Acumularea dozei totale dupa fiecare Run si a numarului total de evenimente

    if(runID == 0) { totalEvents = run->GetNumberOfEvent(); }
    if(runID > 0) { totalEvents += run->GetNumberOfEvent(); }

    totalDoseAllRuns += totalDose.GetValue();
    totalDoseAllRuns2 += totalDose2.GetValue();

    runOutputFile << "totalDoseAllRuns = " << G4BestUnit(totalDoseAllRuns, "Dose") << "  totalDoseAllRuns2 = " << G4BestUnit(totalDoseAllRuns2, "Dose") << "  totalEvents = " << totalEvents << "\n";
    
    runOutputFile.close();
}

std::ofstream* MyRunAction::GetOutputFile()
{
    return &fOutputFile;  //fisierul de output incarcat dupa fiecare event
}