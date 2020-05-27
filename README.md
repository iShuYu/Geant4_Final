# Geant4_Final
基于Geant4自带的B2例子修改而来
Step1: 在DetectorConstruction.cc中修改探测器的材料，尺寸，结构（注意#include G4Orb.hh）
        1.在DetectorConstruction::DefineMaterials()中定义材料
        2.在DetectorConstruction::DefineVolumes()中定义探测器的尺寸，形状，摆放
        3.在B2aDetectorConstruction::ConstructSDandField()中除去灵敏探测器的放置
        4.删除B2aDetectorConstruction::SetTargetMaterial(G4String materialName)，因为不是打靶模拟，不需要设置靶的材料
        5.删除B2aDetectorConstruction::changeGeo()，因为我们不需要改变几何
        6.在void B2aDetectorConstruction::SetChamberMaterial(G4String materialName)中删除复制过程
        7.注释头文件DetectorConstruction.hh中不必要的东西，并将fLogicChamber改为一重指针（其实为了代码的好看应该改成logicDetector但是可能会         涉及到别的改动，所以作罢）
        8.删除DetectorMessenger.cc和对应头文件中有关于Target的部分，不需要
