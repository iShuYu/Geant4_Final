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
/// \file B2RunAction.cc
/// \brief Implementation of the B2RunAction class

#include "B2RunAction.hh"
//需要添加头文件
#include "B2PrimaryGeneratorAction.hh"
#include "G4AccumulableManager.hh"  //我的理解里有点像<vector>
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"	//利用root
//
#include "G4Run.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2RunAction::B2RunAction()
 : G4UserRunAction(),
	fGoodEvents(0.),
	fSumDose(0.)	//设置初始值为0.
{ 
  // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(1000);     

//创建累积变量，用于储存每个run的数据
	G4AccumulableManager *accumulableManager = G4AccumulableManager::Instance();
	accumulableManager -> RegisterAccumulable(fGoodEvents);
	accumulableManager -> RegisterAccumulable(fSumDose);
//创建tree储存变量，这里的tree用到了g4root.hh
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager -> SetVerboseLevel(1);
	analysisManager -> SetNtupleMerging(1); //有点像root里的hadd
	analysisManager -> CreateNtuple("tree", "tree");
	analysisManager -> CreateNtupleDColumn("E_Neutrino");
	analysisManager -> CreateNtupleDColumn("Theta_Neutrino");
	analysisManager -> CreateNtupleDColumn("Phi_Neutrino");
	//因为这里我们只关心反应后的反中微子能量
	analysisManager -> FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2RunAction::~B2RunAction()
{
//添加
	delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2RunAction::BeginOfRunAction(const G4Run*)
{ 
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
	
//重置累积变量的初始值
	G4AccumulableManager *accumulableManager = G4AccumulableManager::Instance();
	accumulableManager -> Reset();
//runManager储存随机变量
	G4RunManager::GetRunManager() -> SetRandomNumberStore(false);
	auto analysisManager = G4AnalysisManager::Instance();
	G4String filename = "Signal";
	analysisManager -> OpenFile(filename);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2RunAction::EndOfRunAction(const G4Run* run)
{
	G4int  N_Events = run -> GetNumberOfEvent();
	if(N_Events == 0) return;
//把累积变量加起来（不是很懂，不是应该把文件加起来吗，这个accumulable加起来干嘛）
	G4AccumulableManager *accumulableManager = G4AccumulableManager::Instance();
	accumulableManager -> Merge();
	
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager -> Write();
	analysisManager -> CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
