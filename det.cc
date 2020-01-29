//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: hadr01.cc,v 1.9 2006/11/24 16:48:57 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02-patch-04 $
//
// -------------------------------------------------------------
//      GEANT4 hadr01
//
//  Application demonstrating Geant4 hadronic physics:
//  beam interaction with a target
//
//  Authors: A.Bagulya, I.Gudowska, V.Ivanchenko, N.Starkov
//
//  Modified: 
//
// -------------------------------------------------------------
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingVerbose.hh"
#include "PhysicsListMessenger.hh"
#include <G4Material.hh>
#include <G4UserRunAction.hh>
#include <G4Run.hh>
#include <CLHEP/Random/Random.h>
#include <unistd.h>
#include <time.h>


/////////////////////////////////////////////////////

#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "G4EmUserPhysics.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
//#include "G4UIExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
//#include "G4VisExecutive.hh"
#endif

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

///////////////////////////////////////////////////////

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


int main(int argc,char** argv) {

int Num=0;
char SNum[40];

//~ ifstream FNum("FolderN");
//~ if(!FNum.is_open()) 
//~ {
	//~ cout << "\n Cant find this file : FolderN" << endl;
	//~ FNum.close();
	//~ ofstream FNum("FolderN");
	//~ FNum << "0";
	//~ FNum.close();
	//~ ifstream FNum("FolderN");
//~ }

//~ FNum >> Num;
//~ FNum.close();

FILE *FNum;
if ((FNum=fopen("FolderN", "r+")) == NULL) 
{
    	printf("================================================\n");
    	printf("Не могу открыть файл FolderN:  %s\n", strerror(errno));
    	FNum=fopen("FolderN", "w+");
    	printf("Создан новый файл FolderN:  %s\n", strerror(errno));
    	fprintf(FNum, "%d", 1);  
    	printf("================================================\n");  	
}

fscanf(FNum, "%d", &Num);
fclose(FNum);

int status;	
status=mkdir("Runs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
if (status==(-1))
{
	printf("================================================\n");
	printf("Cоздание папки  Runs:  %s\n", strerror(errno));
	printf("================================================\n");
}
else 
{
	cout << "================================================\n";
	cout << "Директория Runs создана\n"; 
	cout << "================================================\n";
}

sprintf(SNum, "Runs/%d", Num);
status=mkdir(SNum, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
if (status==(-1))
{
	printf("================================================\n");
	printf("Cоздание папки  Runs/%d:  %s\n", Num, strerror(errno));
	printf("================================================\n");
}
else 
{
	cout << "================================================\n";
	cout << "Директория Runs/" << Num << " создана\n"; 
	cout << "================================================\n";
}


//~ sprintf(SNum, "Runs/%d/det", Num);
//~ status=mkdir(SNum, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
//~ if (status==(-1))
//~ {
	//~ printf("================================================\n");
	//~ printf("Cоздание папки  Runs/%d/det:  %s\n", Num, strerror(errno));
	//~ printf("================================================\n");
//~ }
//~ else 
//~ {
	//~ cout << "================================================\n";
	//~ cout << "Директория Runs/" << Num << "/det создана\n"; 
	//~ cout << "================================================\n";
//~ }
	
 
  //choose the Random engine
  //CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  CLHEP::HepRandom::setTheSeed(time(0)+getpid());

  //Construct the default run manager
  G4RunManager * runManager = new G4RunManager();

  //set mandatory initialization classes
  //runManager->SetUserInitialization(new DetectorConstruction());
  DetectorConstruction* detector_c = new DetectorConstruction;
  runManager->SetUserInitialization(detector_c);

  G4PhysListFactory factory;
  G4VModularPhysicsList* phys = 0;
  PhysicsListMessenger* mess = 0;
  G4String physName = "";

 // Physics List name defined via 3nd argument
  if (argc==3) { physName = argv[2]; }

  // Physics List name defined via environment variable
  if("" == physName) {
    char* path = getenv("PHYSLIST");
    if (path) { physName = G4String(path); }
  }

  // reference PhysicsList via its name
  if("" != physName && factory.IsReferencePhysList(physName)) {
    phys = factory.GetReferencePhysList(physName);

    // added extra EM options
    phys->RegisterPhysics(new G4EmUserPhysics(1));

    // instantiated messenger
    mess = new PhysicsListMessenger();
  }

 // local Physics List
  if(!phys) { phys = new PhysicsList(); }

  // define physics
  runManager->SetUserInitialization(phys);
  runManager->SetUserAction(new PrimaryGeneratorAction());


//  runManager->SetUserInitialization(new PhysicsList);

  //runManager->SetUserInitialization(new QBBC(1,"QBEC_HP"));
  //runManager->SetUserInitialization(new QGSP);

//  runManager->SetUserAction(new PrimaryGeneratorAction(detector_c));

  //set user action classes
  //runManager->SetUserAction(new RunAction());
  //runManager->SetUserAction(new EventAction());
  //runManager->SetUserAction(new StackingAction());

  //get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

#ifdef G4VIS_USE
   G4VisManager* visManager = 0;
#endif
if (argc==1)   // Define UI terminal for interactive mode
    {
#ifdef G4VIS_USE
      //visualization manager
      visManager = new G4VisExecutive;
      visManager->Initialize();
#endif
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
      ui->SessionStart();
      delete ui;
#endif
    }
  else           // Batch mode
    {
      //G4String command = "/control/execute vis1.mac";
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }

  //job termination
#ifdef G4VIS_USE
  delete visManager;
#endif     
  delete runManager;
  delete mess;

FNum=fopen("FolderN", "w");
fprintf(FNum, "%d", Num+1);
fclose(FNum);

//~ ofstream FNum("FolderN");
//~ FNum << Num + 1;
//~ FNum.close();

return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
