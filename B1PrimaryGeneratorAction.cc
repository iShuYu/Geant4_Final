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
/// \file B1PrimaryGeneratorAction.cc
/// \brief Implementation of the B1PrimaryGeneratorAction class

#include "B1PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), 
  fEnvelopeBox(0)
{
	G4int n_particle = 1;
	fParticleGun = new G4ParticleGun(n_particle);	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of ecah event
  //

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.
  
  G4double envSizeXY = 0;
  G4double envSizeZ = 0;
//这里大概意思就是说，若fEnvelope==0，也就是说找得到envelope，就显示它的形状体积等等，这里和DetectorConstruction.cc里的定义要联系得上，在那里定义的envelope是方体（G4Box），
  if (!fEnvelopeBox)
  {
    G4LogicalVolume* envLV
      = G4LogicalVolumeStore::GetInstance()->GetVolume("envelope");
    if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
  }

  if ( fEnvelopeBox ) {
    envSizeXY = fEnvelopeBox->GetXHalfLength()*2.;
    envSizeZ = fEnvelopeBox->GetZHalfLength()*2.;
  }  
  else  {
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("B1PrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }
//定义粒子的位置，动量和能量，因为是静止，所以全部选择为0就行了
	G4ParticleDefinition *particleDefinition = G4ParticleTable::GetParticleTable() -> FindParticle("mu-");
	fParticleGun -> SetParticleDefinition(particleDefinition);
	fParticleGun -> SetParticlePosition(G4ThreeVector(0.,0.,0.));
	fParticleGun -> SetParticleEnergy(0.*GeV);
//	fParticleGun -> SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
	fParticleGun -> SetParticleTime(0.*ns);


/*这里是原来的定义，大概意思是说在envelope中随机位置打出一个粒子，但是我们在这里简单地放在中间就行
  G4double size = 0.8; 
  G4double x0 = size * env_size_XYZ * (G4UniformRand()-0.5);
  G4double y0 = size * env_size_XYZ * (G4UniformRand()-0.5);
  G4double z0 = -0.5 * env_size_XYZ;
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));*/

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

