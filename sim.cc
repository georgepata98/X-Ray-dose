#include <iostream>
#include <string>
#include <ctime>
#include <climits>

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4ScoringManager.hh"

#include "Randomize.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char** argv)
{
    G4ScoringManager::GetScoringManager();
    
    //Declararea variabilelor

    unsigned long long int totalEvents = 1;  //Aici se modifica nr. total de evenimente dorit!!!
    
    G4int eventsPerRun = 2000000000;  //2 miliarde evenimente per run
    G4int nOfRuns = totalEvents / eventsPerRun;  //nr. total de Run-uri cu eventsPerRun evenimente
    G4int eventsInFinalRun = totalEvents - eventsPerRun * nOfRuns;  //evenimentele ramase in ultimul Run



    //Initializarea simularii

    G4RunManager *runManager = new G4RunManager();
    G4VisManager *visManager = new G4VisExecutive();


    for(G4int i=0; i<nOfRuns; i++)
    {
        runManager->SetUserInitialization(new MyDetectorConstruction());
        runManager->SetUserInitialization(new MyPhysicsList());
        runManager->SetUserInitialization(new MyActionInitialization());  //contine PrimaryGenerator, RunAction si EventAction

        G4Random::setTheSeed(time(0) + i);
        runManager->Initialize();

        if(i == 0)  //visManager se initializeaza o singura data
        {
            G4UIExecutive *ui = 0;

            if(argc == 1)
            {
                ui = new G4UIExecutive(argc, argv);
            }

            visManager->Initialize();

            G4UImanager *UImanager = G4UImanager::GetUIpointer();

            if(ui)
            {
                UImanager->ApplyCommand("/control/execute vis.mac");
                runManager->BeamOn(eventsPerRun);

                ui->SessionStart();  //activare gui
                delete ui;
            }

            else
            {
                G4String command = "/control/execute ";
                G4String fileName = argv[1];
                UImanager->ApplyCommand(command + fileName);
            }
        }

        if(i > 0) { runManager->BeamOn(eventsPerRun); }
    }

    if(eventsInFinalRun > 0)
    {
        runManager->SetUserInitialization(new MyDetectorConstruction());
        runManager->SetUserInitialization(new MyPhysicsList());
        runManager->SetUserInitialization(new MyActionInitialization());

        G4Random::setTheSeed(time(0) + nOfRuns);
        runManager->Initialize();

        if(nOfRuns == 0)
        {
            G4UIExecutive *ui = 0;

            if(argc == 1)
            {
                ui = new G4UIExecutive(argc, argv);
            }

            visManager->Initialize();

            G4UImanager *UImanager = G4UImanager::GetUIpointer();

            if(ui)
            {
                UImanager->ApplyCommand("/control/execute vis.mac");
                runManager->BeamOn(eventsInFinalRun);

                ui->SessionStart();  //activare gui
                delete ui;
            }

            else
            {
                G4String command = "/control/execute ";
                G4String fileName = argv[1];
                UImanager->ApplyCommand(command + fileName);
            }
        }

        if(nOfRuns > 0) { runManager->BeamOn(eventsInFinalRun); }
    }

    
    delete visManager;
    delete runManager;

    return 0;
}