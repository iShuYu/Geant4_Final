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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

G4double pi = 3.14159265;
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{ 

//材料管理：
	G4NistManager *nist = G4NistManager::Instance();


//定义world（中心为原点）和envelope和detector的几何：因为这个探测器的半径算出来大概是14mm，所以这里就设置30m长宽高的env，当然了，这里的envelope和world都可以填充空气，我想或许一个world中可以放置多个envelope
	G4double env_size_XYZ =  30.0 * m;
	G4double world_size_XYZ = 1.2 * env_size_XYZ;
	G4double radius = pow(10000000.0/4*3/pi/863.0,1.0/3) * m;
	G4Box *solidWorld = new G4Box("world",0.5*world_size_XYZ,0.5*world_size_XYZ,0.5*world_size_XYZ);
	G4Box *solidEnv = new G4Box("envelope",0.5*env_size_XYZ,0.5*env_size_XYZ,0.5*env_size_XYZ);
	G4Orb *solidDetector = new G4Orb("detector",radius);
	G4bool checkOverlaps = true;


//定义world和envelope和detector的logicVolume，也就是给个材料
	G4Material *env_mat = nist -> FindOrBuildMaterial("G4_AIR");
	G4Material *world_mat = nist -> FindOrBuildMaterial("G4_AIR");
	G4Element *C = nist -> FindOrBuildElement("C",false); //非同位素
	G4Element *H = nist -> FindOrBuildElement("H",false);
	G4double density = 0.863 * g/cm3;
	G4Material *detector_mat = new G4Material("LAB",density,2);
	detector_mat -> AddElement(C,18);
	detector_mat -> AddElement(H,30);
	
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,world_mat,"World");
	G4LogicalVolume *logicEnv = new G4LogicalVolume(solidEnv,env_mat,"envelope");
	G4LogicalVolume *logicDetector = new G4LogicalVolume(solidDetector,detector_mat,"detector");
	

//放置
	G4VPhysicalVolume *physWorld = new G4PVPlacement(
		0,			//无旋转
		G4ThreeVector(),	//在（0，0，0）
		logicWorld,		//放置目标
		"world",		//名称
		0,			//母体
		false,			//无布尔运算
		0,			//复制数字
		checkOverlaps);		//检查有无Overlaps
	G4VPhysicalVolume *physEnv = new G4PVPlacement(
		0,			//无旋转
		G4ThreeVector(),	//在（0，0，0）
		logicEnv,		//放置目标
		"envelope",		//名称
		logicWorld,		//母体（放在world里）
		false,			//无布尔运算
		0,			//复制数
		checkOverlaps);		//检查有无overlaps
	G4VPhysicalVolume *physDetector = new G4PVPlacement(
		0,			//无旋转
		G4ThreeVector(),	//在（0，0，0）
		logicDetector,		//放置目标
		"detector",		//名称
		logicEnv,		//母体（放在environment中）
		false,			//无布尔运算
		0,			//复制数
		checkOverlaps);		//检查有无overlaps

//



return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
