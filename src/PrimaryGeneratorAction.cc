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
/// \file hadronic/Hadr01/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
// $Id: PrimaryGeneratorAction.cc 70761 2013-06-05 12:30:51Z gcosmo $
//
/////////////////////////////////////////////////////////////////////////
//
// PrimaryGeneratorAction
//
// Created: 31.01.03 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of Hadr01 (V.Ivanchenko)
// 16.11.2006 Add option allowing to have user defined beam position (VI)
//
////////////////////////////////////////////////////////////////////////
//
#include "PrimaryGeneratorAction.hh"
   
#include<G4Event.hh>
#include<G4ParticleGun.hh>
#include<G4ParticleTable.hh>
#include<G4ParticleDefinition.hh>
#include<globals.hh>
#include "G4SystemOfUnits.hh"
//#include "HistoManager.hh"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <string>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
   {
	   srand(time(NULL));
	   
       G4int n_particle = 1;
       particleGun = new G4ParticleGun(n_particle);
   
       G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
       G4ParticleDefinition* particle = particleTable->FindParticle("proton");
     
       particleGun->SetParticleDefinition(particle);
       particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
       
       //~ srand(111);
       
       //~ double x = (double)((rand() - RAND_MAX / 2) * 2) / RAND_MAX * 42.5;
       //~ double y = (double)((rand() - RAND_MAX / 2) * 2) / RAND_MAX * 42.5;
       
       //~ ifstream		FN;
	   //~ ofstream		eDepDat;
	   //~ char			SN[40];
	   //~ int			folderNum;
	   
       //~ FN.open("FolderN");
	   //~ FN >> folderNum;
	   //~ FN.close();
	   //~ sprintf(SN,"Runs/%d/EnergyDeposit.dat",folderNum);
	   //~ eDepDat.open(SN, ios::app);
	   //~ eDepDat << setw(15) << "coordinate"
			   //~ << setw(15) << x << endl;
	   //~ eDepDat.close();
	   
	   //~ cout << setw(15) << folderNum
			//~ << setw(15) << x << endl;
       //~ particleGun->SetParticlePosition(G4ThreeVector(x*mm, y*mm, 0.0*mm));
       particleGun->SetParticleEnergy(1*GeV);
   }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
       delete particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
       
       double x = -120.0 + (double)(rand())/RAND_MAX * 240.0;
       double y = -120.0 + (double)(rand())/RAND_MAX * 240.0;
       
       ifstream		FN;
	   ofstream		eDepDat;
	   char			SN[40];
	   int			folderNum;
	   
       FN.open("FolderN");
	   FN >> folderNum;
	   FN.close();
	   sprintf(SN,"Runs/%d/EnergyDeposit.dat",folderNum);
	   eDepDat.open(SN, ios::app);
	   eDepDat << setw(15) << "coordinate"
			   << setw(15) << x << endl;
	   eDepDat.close();
	   
	   //~ cout << setw(15) << folderNum
			//~ << setw(15) << x << endl;
       particleGun->SetParticlePosition(G4ThreeVector(x*mm, y*mm, -100.0*mm));
       
  particleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
