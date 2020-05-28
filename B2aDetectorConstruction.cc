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
/// \file B2aDetectorConstruction.cc
/// \brief Implementation of the B2aDetectorConstruction class
 
#include "B2aDetectorConstruction.hh"
#include "B2aDetectorMessenger.hh"
#include "B2TrackerSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"

#include "G4RotationMatrix.hh"
#include "G4PhysicalConstants.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4ThreadLocal 
G4GlobalMagFieldMessenger* B2aDetectorConstruction::fMagFieldMessenger = 0;

B2aDetectorConstruction::B2aDetectorConstruction()
:G4VUserDetectorConstruction(), 
 fLogicChamber(NULL),
 fChamberMaterial(NULL), 
 fStepLimit(NULL),
 fCheckOverlaps(true)
{
  fMessenger = new B2aDetectorMessenger(this);
/*不需要这一部分，因为我们的探测器就是简单的一个球
  fNbOfChambers = 5;
  fLogicChamber = new G4LogicalVolume*[fNbOfChambers];
  fLogicCrystal = NULL;*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
B2aDetectorConstruction::~B2aDetectorConstruction()
{
  delete fLogicChamber; 
  delete fStepLimit;
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* B2aDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::DefineMaterials()
{
//材料定义
	G4bool isotopes = false;
	G4NistManager *nistManager = G4NistManager::Instance();
	G4Element *C = nistManager -> FindOrBuildElement("C", isotopes);
	G4Element *H = nistManager -> FindOrBuildElement("H", isotopes);
	G4Material *LAB = new G4Material("LAB", 0.863 * g/cm3, 2);
	LAB -> AddElement(C, 18);
	LAB -> AddElement(H, 30);
	fChamberMaterial = LAB;
  
G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B2aDetectorConstruction::DefineVolumes()
{
	//自己定义一个真空，作为world填充材料
	G4double density = 1.e-25 * g/cm3;
	G4double pressure = 1.e-19 * pascal;
	G4double temperature = 0.1 * kelvin;
	G4Material *vacuum = new G4Material("universe", 1., 1.01 * g/mole, density, kStateGas, temperature, pressure);

	//定义形状
	G4double Radius = pow(10000./4*3/3.1415927/0.863, 1./3) * m;
	G4double World_L = 1.2 * Radius;
	G4GeometryManager::GetInstance() -> SetWorldMaximumExtent(2*World_L);
	G4Box *solidWorld = new G4Box("world", World_L, World_L, World_L);
	G4Orb *solidDetector = new G4Orb("detector", Radius);

	//填充材料
	G4LogicalVolume *logicWorld = new G4LogicalVolume(
		solidWorld,		//对象体
		vacuum,			//填充材料
		"world");		//名称
	fLogicChamber = new G4LogicalVolume(
		solidDetector,		//对象体
		fChamberMaterial,	//材料，在上面定义为LAB
		"detector",		//名称（头文件定义过探测器的LogicVolume为fLogicChamber）
		0,
		0,
		0);			//具体这三个0看头文件
	//放置
	G4VPhysicalVolume *physWorld = new G4PVPlacement(
		0,			//无旋转
		G4ThreeVector(),	//（0，0，0）
		logicWorld,		//放置对象
		"world",		//名称
		0,			//无母体（world最大）
		false,			//无布尔运算
		0,			//复制数量
		fCheckOverlaps);	//检查重叠
	new G4PVPlacement(
		0,
		G4ThreeVector(),
		fLogicChamber,
		"detector",
		logicWorld,		//放置在world中
		false,
		0,
		fCheckOverlaps);

	//设置显示属性
	G4VisAttributes *worldVis = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	G4VisAttributes *detectorVis = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
	logicWorld -> SetVisAttributes(worldVis);
	fLogicChamber -> SetVisAttributes(detectorVis);

	//设置径迹最大步长
	G4double maxStep = 1.0 * m;
	fStepLimit = new G4UserLimits(maxStep);
	logicWorld -> SetUserLimits(fStepLimit);
	
	//返回值总为world的安放
	return physWorld;		
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void B2aDetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors

  G4String trackerChamberSDname = "B2/TrackerChamberSD";
  B2TrackerSD* aTrackerSD = new B2TrackerSD(trackerChamberSDname,
                                            "TrackerHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
  // 在所有的logical Volume里面放置灵敏探测器，实际上并不需要
  // of "Chamber_LV".
	//修改为不要
	SetSensitiveDetector("detector", aTrackerSD, false);

  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*删除这个部分，因为不是打靶模拟，不需要设置靶的材料 
void B2aDetectorConstruction::SetTargetMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial = 
              nistManager->FindOrBuildMaterial(materialName);

  if (fTargetMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fTargetMaterial = pttoMaterial;
        // if (fLogicTarget) fLogicTarget->SetMaterial(fTargetMaterial);
        G4cout 
          << G4endl 
          << "----> The target is made of " << materialName << G4endl;
     } else {
        G4cout 
          << G4endl 
          << "-->  WARNING from SetTargetMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
}*/

/*删除这个部分，我们不需要改变探测器的几何（这里改了它的方向）
 void B2aDetectorConstruction::changeGeo()
 {
   //delete fLogicTarget;
   //delete fPhysicTarget;
   //delete targetS;
   //targetS=new G4Tubs("target",0.,10*targetRadius,10*targetLength,0.*deg,360.*deg);
   //fLogicTarget
   // = new G4LogicalVolume(targetS, fTargetMaterial,"Target",0,0,0);
   fPhysicTarget->SetTranslation(G4ThreeVector(0,0,80));
   //fPhysicTarget->SetPos(G4ThreeVector());
   G4RunManager* runManager = G4RunManager::GetRunManager();
   runManager->GeometryHasBeenModified();
   //runManager->ReinitializeGeometry();
 }*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::SetChamberMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial =
              nistManager->FindOrBuildMaterial(materialName);

  if (fChamberMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fChamberMaterial = pttoMaterial;

/*删除复制过程
        for (G4int copyNo=0; copyNo<fNbOfChambers; copyNo++) {
            if (fLogicChamber[copyNo]) fLogicChamber[copyNo]->
                                               SetMaterial(fChamberMaterial);
        }*/

        G4cout 
          << G4endl 
          << "----> The chambers are made of " << materialName << G4endl;
     } else {
        G4cout 
          << G4endl 
          << "-->  WARNING from SetChamberMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  fCheckOverlaps = checkOverlaps;
}  
