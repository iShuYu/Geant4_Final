# Geant4_Final
基于Geant4自带的B2例子修改而来（所有tab为8格的地方都为自己修改，为2格的地方为原文件）;    
Step1: 在DetectorConstruction.cc中修改探测器的材料，尺寸，结构（注意#include G4Orb.hh）;  
        1.在DetectorConstruction::DefineMaterials()中定义材料;  
        2.在DetectorConstruction::DefineVolumes()中定义探测器的尺寸，形状，摆放;  
        3.在B2aDetectorConstruction::ConstructSDandField()中除去灵敏探测器的放置;  
        4.删除B2aDetectorConstruction::SetTargetMaterial(G4String materialName)，因为不是打靶模拟，不需要设置靶的材料;  
        5.删除B2aDetectorConstruction::changeGeo()，因为我们不需要改变几何;  
        6.在void B2aDetectorConstruction::SetChamberMaterial(G4String materialName)中删除复制过程;    
        7.注释头文件DetectorConstruction.hh中不必要的东西，并将fLogicChamber改为一重指针（其实为了代码的好看应该改成logicDetector但是可能会         涉及到别的改动，所以作罢）;  
        8.删除DetectorMessenger.cc和对应头文件中有关于Target的部分，不需要;  
        
 Step2: 在RunAction.cc中做修改，其中包括BeginOfRun，EndOfRun;  
        1.设置RunAction的初始值（注意在RunAction.hh中添加相应的计数函数）;  
        2.在RunAction中创建tree储存需要的变量（注意，这里储存的变量需要refer to你的PhysicsList），这个例子只关心反中微子;  
        3.在BeginOfRun中创建文件，用于储存结果;  
        4.在EndOfRun中将结果写入在前一步创建的文件中;  
   
 Step3: 在EventAction.cc中做修改，其中包括BeginOfEvent，EndOfEvent;  
        1.添加需要的头文件，注释不必要的头文件，比如我们不需要记录径迹，那我们就注释G4Trajectory*.hh;  
        2.那么在EndOfEvent中就可以注释掉和Trajectory（轨迹，弹道）有关的部分;  
        3.在BeginOfEvent中设置每个变量的初值（注意同时在EventAction.hh中声明这些变量），这些变量的赋值在SteppingAction.cc中执行，下一步会说到;  
        4.在EndOfEvent中Fill每个Branch的值;  
         
Step4: 自己写一个SteppingAction（因为修改的是B2a，这个例子没有SteppingAction，可以借鉴B4中的SteppingAction）;  
        1.添加需要的头文件（其中SteppingAction.hh需要自己编写），注意到加入了AntiNeutrinoE.hh。主要功能是判断粒子径迹为反中微子，那么就记录信息到EventAction中创建的变量里;  
        2.编写相应的头文件（其实按照顺序说是先编写，但是谁让我菜只能借鉴别人的代码）;  
        3.在ActionInitialization中添加相应的初始化条件（因为之前没有SteppingAction）
Step5: 编写一个PhysicsList.hh和PhysicsList.cc;  
 











