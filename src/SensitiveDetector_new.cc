#include <fstream>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <string>
//создание папки
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <G4Step.hh>
#include <G4RunManager.hh>
#include <G4ParticleDefinition.hh>
#include "G4SteppingManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <G4UserRunAction.hh>
#include <G4Run.hh>

#include "SensitiveDetector_new.hh"
#include "PrimaryGeneratorAction.hh"

//#include "Hits.hh"

#define EPS 0.000001

//~ #define LOG_ON

using namespace std;

G4int			eID,       // номер события
				RunNum;

G4double		energy,
				posX1,
				posY1,
				posZ1,
				posX2,
				posY2,
				posZ2; 
G4StepPoint*	point1; 
G4StepPoint*	point2;

ifstream		FN;
ofstream		hitLog,
				eDepDat,
				EnergyParticle;

char			SN[40];
int				folderNum;

G4Track			*currentTrack;
G4int			fParentID;

////// ParticleEnergy
G4double	Epoint1,
			Epoint2;       

G4double	PDGCharge;

G4double	length = 0.0,
			ParticleLength = 0.0;

G4double	Gtime1 = 0.0,
			Gtime2 = 0.0;

G4double	ParticleEnergy = 0.0;

G4String 	aParticle;
  
///////////////////////////////////////

SensitiveDetector::SensitiveDetector(G4String name): G4VSensitiveDetector(name)
{
	energy_event = 0.0;               //полная энергия в детекторе от одной частицы
	stepnumber = 0;                  //?количество хитов
	Gtime_sum = 0.0;
	N_Hit = 0;

	const char	*ch;

	ch=name.data();
	sprintf(detname, "%s", ch);

////////////////////////////////////////////////////////////////
	FN.open("FolderN");
	FN >> folderNum;
	FN.close();

/////////////////////////////////////////////////////////////////	

} 

G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *hist)
{
 
	energy = aStep->GetTotalEnergyDeposit();
  
	//G4Run* aRun;
	RunNum = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
	currentTrack = aStep->GetTrack();
	fParentID = currentTrack->GetParentID();
	aParticle = aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
	point1 = aStep->GetPreStepPoint();
	point2 = aStep->GetPostStepPoint();
	Epoint1 = point1-> GetKineticEnergy();
	Epoint2 = point2-> GetKineticEnergy();
  
	//pos1 = point1->GetPosition();
	//pos2 = point2->GetPosition();
	posX1 = point1->GetPosition().x();
	posX2 = point2->GetPosition().x();
	posY1 = point1->GetPosition().y();
	posY2 = point2->GetPosition().y();
	posZ1 = point1->GetPosition().z();
	posZ2 = point2->GetPosition().z();
  
	length = sqrt((posX2-posX1)*(posX2-posX1)+(posY2-posY1)*(posY2-posY1)+(posZ2-posZ1)*(posZ2-posZ1));
	const G4Event* evt = G4RunManager::GetRunManager()->GetCurrentEvent();
	eID = evt->GetEventID();
  
	Gtime1 = point1-> GetGlobalTime();
	Gtime2 = point2-> GetGlobalTime();
  
	G4ParticleDefinition	*fpParticleDefinition;
	fpParticleDefinition = currentTrack->GetDefinition();
	PDGCharge = fpParticleDefinition->GetPDGCharge();


#ifdef LOG_ON
	sprintf(SN,"Runs/%d/Hit.log",folderNum);
	hitLog.open(SN, ios::app); 

	hitLog	<< setw(3)	<< RunNum
			<< setw(6)	<< eID
			//<< setw(15) << detname
			<< setw(10) << aParticle
		/*
			<< setw(15) << aStep->GetPreStepPoint()->GetKineticEnergy()
			<< setw(15) << aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy() 
		*/ 
			<< setw(15) << Epoint1
			<< setw(15) << Epoint2
			<< setw(15) << energy
			<< setw(15) << detname;
			// << setw(15) << aStep->GetTrack()->GetDynamicParticle()->GetTotalEnergy();

	if(aStep->GetPostStepPoint()->GetProcessDefinedStep() != NULL)
	{
		hitLog << setw(25)<< aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
	}
	else
	{
		hitLog << setw(25) << "UserLimit";
	}
	//hitLog << setw(15) << aParticle //aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName()
			//<< setw(15) << fParentID 
	hitLog	<< setw(15) << posX1
			<< setw(15) << posY1
			<< setw(15) << posZ1
			<< setw(15) << posX2
			<< setw(15) << posY2
			<< setw(15) << posZ2
			<< setw(15) << length
			<< setw(15) << Gtime1 //point1-> GetGlobalTime()
			<< setw(15) << Gtime2 //point2-> GetGlobalTime()
			<< setw(4) << fParentID
			<< setw(4) << PDGCharge << endl;
			//<< setw(15) << stepnumber << endl;
			//<< setw(15) << currentTrack->GetPosition().z() << endl;

	hitLog.close();
#endif


//##################################################################

	if (strcmp(detname, "endLay") == 0)
	{
		energy_event = Epoint1;
		Gtime_sum = Gtime1;
		N_Hit = 1;
		aStep->GetTrack()->SetTrackStatus(fStopAndKill);
		return true; 
	}
	
	stepnumber++;

	if (energy > EPS)
	{
		energy_event += energy;
		Gtime_sum += Gtime2;
		N_Hit++;
	}
	//~ cout << setw(15) << detname
		//~ << setw(15) << energy
		//~ << setw(15) << Gtime_sum
		//~ << setw(15) << N_Hit << endl;

	return true; 
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
	
#ifdef LOG_ON
	hitLog << endl;
#endif

	G4double	ev_time;

	ev_time = Gtime_sum / N_Hit;
	sprintf(SN,"Runs/%d/EnergyDeposit.dat",folderNum);
	eDepDat.open(SN, ios::app);
	if(energy_event > EPS)
	{
		eDepDat << setw(15) << RunNum
				<< setw(15) << eID
				<< setw(15) << detname
				<< setw(19) << energy_event
				<< setw(15) << ev_time << endl;
	}
	eDepDat.close();
	energy_event=0.0;
	stepnumber=0;
	Gtime_sum = 0.0;
	N_Hit = 0;
}

SensitiveDetector::~SensitiveDetector()
{
}
