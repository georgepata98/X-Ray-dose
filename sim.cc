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
    //Declararea variabilelor

    unsigned long long int totalEvents = 30;  //Aici se modifica nr. total de evenimente dorit a fi simulat!!!
    G4bool visualization = true;  //Se seteaza: true = cu vizualizare, false = fara vizualizare
    G4int visEvents = 1;  //Aici se modifica nr. de evenimente care se doreste a fi vazut cu GUI!!!
    
    G4int eventsPerRun = 2000000000;  //2 miliarde evenimente per run
    G4int nOfRuns = totalEvents / eventsPerRun;  //nr. total de Run-uri cu eventsPerRun evenimente
    G4int eventsInFinalRun = totalEvents - eventsPerRun * nOfRuns;  //evenimentele ramase in ultimul Run



    //Initializarea simularii

    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    G4ScoringManager::GetScoringManager();

    G4UIExecutive *ui = nullptr;

    if(argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }
    
    //RunManager

    G4RunManager *runManager = new G4RunManager();

    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());
    runManager->Initialize();

    for(G4int i=0; i<nOfRuns; ++i)
    {
        G4Random::setTheSeed(time(0) + i);
        runManager->BeamOn(eventsPerRun);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));  //time-delay intre Run-uri pt. a ii da sistemului timp sa elibereze memoria
    }

    if(eventsInFinalRun > 0)
    {
        G4Random::setTheSeed(time(0) + nOfRuns);
        runManager->BeamOn(eventsInFinalRun);
    }

    //Visualization UI

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    if(ui)
    {
        if(visualization)
        {
            UImanager->ApplyCommand("/control/execute vis.mac");
            runManager->BeamOn(visEvents);

            ui->SessionStart();  //activare gui
        }
        delete ui;
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }


    delete visManager;
    delete runManager;

    return 0;
}
