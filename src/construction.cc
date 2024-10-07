#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    return DefineVolumes();
}

G4VPhysicalVolume* MyDetectorConstruction::DefineVolumes()
{
    //Materials

    G4NistManager *nist = G4NistManager::Instance();

    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *AlMat = nist->FindOrBuildMaterial("G4_Al");
    G4Material *CuMat = nist->FindOrBuildMaterial("G4_Cu");  //pereti din Cu ai tubului de RX
    G4Material *PbMat = nist->FindOrBuildMaterial("G4_Pb");
    G4Material *BeMat = nist->FindOrBuildMaterial("G4_Be");
    G4Material *WMat = nist->FindOrBuildMaterial("G4_W");
    G4Material *SiMat = nist->FindOrBuildMaterial("G4_Si");
    
    G4Material *voidMat = new G4Material("void", 1, 1.008*g/mole, 1e-25*g/cm3, kStateGas, 2.73*kelvin, 3e-18*pascal);

//==================================================================================

    //Volumes - tub de raze X

    G4Box *solidWorld = new G4Box("solidWorld", 1.5*m, 1.5*m, 1.5*m);  //world, 3x3x3 metri
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicWorld, "physWorld", 0, false, 0, true);

    G4Tubs *solidTube1 = new G4Tubs("solidTube1", 3.55*cm, 5*cm, 10.45*cm, 0*deg, 360*deg);  //cell 2 jos (pereti tub)
    G4LogicalVolume *logicTube1 = new G4LogicalVolume(solidTube1, CuMat, "logicTube1");
    G4VPhysicalVolume *physTube1 = new G4PVPlacement(0, G4ThreeVector(0, 0, 10.45*cm), logicTube1, "physTube1", logicWorld, false, 0, true);

    G4Tubs *solidTube2 = new G4Tubs("solidTube2", 0*cm, 3.55*cm, 10.45*cm, 0*deg, 360*deg);  //cell 1 jos (vid tub)
    G4LogicalVolume *logicTube2 = new G4LogicalVolume(solidTube2, voidMat, "logicTube2");
    G4VPhysicalVolume *physTube2 = new G4PVPlacement(0, G4ThreeVector(0, 0, 10.45*cm), logicTube2, "physTube2", logicWorld, false, 0, true);

    G4Tubs *solidTube3 = new G4Tubs("solidTube3", 0*cm, 3.55*cm, 3.5*cm, 0*deg, 360*deg);  //cell 1 sus (vid tub)
    G4Box *solidBox1 = new G4Box("solidBox1", .5*cm, 3.521*cm, 3.5*cm);  //cell 2 sus
    G4RotationMatrix *rotBox1 = new G4RotationMatrix();

    G4SubtractionSolid *solidSubtraction1 = new G4SubtractionSolid("solidSubtraction1", solidTube3, solidBox1, rotBox1, G4ThreeVector(3.05*cm, 0, 0));  //solidTube3 - solidBox1
    G4LogicalVolume *logicSubtraction1 = new G4LogicalVolume(solidSubtraction1, voidMat, "logicSubtraction1");
    G4VPhysicalVolume *physSubtraction1 = new G4PVPlacement(0, G4ThreeVector(0, 0, 24.4*cm), logicSubtraction1, "physSubtraction1", logicWorld, false, 0, true);

    G4Box *solidBox2 = new G4Box("solidBox2", .04*cm, 2*cm, 1.75*cm);  //cell 3 filtru 0.8 mm Be
    G4LogicalVolume *logicBox2 = new G4LogicalVolume(solidBox2, BeMat, "logicBox2");
    G4VPhysicalVolume *physBox2 = new G4PVPlacement(0, G4ThreeVector(2.51*cm, 0, -1.25*cm), logicBox2, "physBox2", logicSubtraction1, false, 0, true);

    G4Tubs *solidTube4 = new G4Tubs("solidTube4", 0*cm, .96*cm, 1.5*cm, 0*deg, 360*deg);  //cell 4 anod din W
    G4LogicalVolume *logicTube4 = new G4LogicalVolume(solidTube4, WMat, "logicTube4");
    G4RotationMatrix *rotTube4 = new G4RotationMatrix();
    rotTube4->rotateY(-20*deg);
    G4VPhysicalVolume *physTube4 = new G4PVPlacement(rotTube4, G4ThreeVector(-.513*cm, 0, .25*cm), logicTube4, "physTube4", logicSubtraction1, false, 0, true);

    G4Tubs *solidTube5 = new G4Tubs("solidTube5", 0*cm, 1.55*cm, .5*cm, 0*deg, 360*deg);  //cell 98 gaura
    G4RotationMatrix *rotTube5 = new G4RotationMatrix();
    rotTube5->rotateY(90*deg);

    G4SubtractionSolid *solidSubtractionHole = new G4SubtractionSolid("solidSubtractionHole", solidBox1, solidTube5, rotTube5, G4ThreeVector(0, 0, -1.25*cm));  //cell 2 drept de sus, cu gaura cell 98
    G4LogicalVolume *logicSubtractionHole = new G4LogicalVolume(solidSubtractionHole, CuMat, "logicSubtractionHole");
    G4VPhysicalVolume *physSubtractionHole = new G4PVPlacement(0, G4ThreeVector(3.05*cm, 0, 24.4*cm), logicSubtractionHole, "physSubtractionHole", logicWorld, false, 0, true);

    G4Tubs *solidTube6 = new G4Tubs("solidTube6", 3.55*cm, 5*cm, 3.5*cm, 0*deg, 360*deg);  //cell 2 sus
    G4Box *solidBox3 = new G4Box("solidBox3", 1.225*cm, 3.521*cm, 3.5*cm);
    G4RotationMatrix *rotBox3 = new G4RotationMatrix();

    G4SubtractionSolid *solidSubtraction2 = new G4SubtractionSolid("solidSubtraction2", solidTube6, solidBox3, rotBox3, G4ThreeVector(3.775*cm, 0, 0));  //solidTube6 - solidBox3
    G4LogicalVolume *logicSubtraction2 = new G4LogicalVolume(solidSubtraction2, CuMat, "logicSubtraction2");
    G4RotationMatrix *rotSubtraction2 = new G4RotationMatrix();
    G4VPhysicalVolume *physSubtraction2 = new G4PVPlacement(rotSubtraction2, G4ThreeVector(0, 0, 24.4*cm), logicSubtraction2, "physSubtraction2", logicWorld, false, 0, true);


    //Volumes - colimator din plumb

    G4Tubs *solidCollimator = new G4Tubs("solidCollimator", 3.12*cm, 10*cm, .75*cm, 0*deg, 360*deg);  //cell 13 colimator
    G4LogicalVolume *logicCollimator = new G4LogicalVolume(solidCollimator, PbMat, "logicCollimator");
    G4RotationMatrix *rotCollimator = new G4RotationMatrix();
    rotCollimator->rotateY(90*deg);
    G4VPhysicalVolume *physCollimator = new G4PVPlacement(rotCollimator, G4ThreeVector(16.95*cm, 0, 23.15*cm), logicCollimator, "physCollimator", logicWorld, false, 0, true);


    //Volumes - detector NOMEX

    G4Tubs *solidDet1 = new G4Tubs("solidDet1", 0*cm, 2.5*cm, .45*cm, 0*deg, 180*deg);  //cell 6 NOMEX
    G4Box *solidDet2 = new G4Box("solidDet2", 2.5*cm, .6*cm, .45*cm);  //cell 6 NOMEX
    G4Tubs *solidDet3 = new G4Tubs("solidDet3", 0*cm, 2.5*cm, .45*cm, 0*deg, 180*deg);  //cell 6 NOMEX

    G4RotationMatrix *rotDet1 = new G4RotationMatrix();
    G4RotationMatrix *rotDet2 = new G4RotationMatrix();
    G4RotationMatrix *rotDet3 = new G4RotationMatrix();
    rotDet3->rotateZ(180*deg);

    G4Transform3D trDet1 = G4Transform3D(*rotDet1, G4ThreeVector(0, .6*cm, 0));
    G4Transform3D trDet2 = G4Transform3D(*rotDet2, G4ThreeVector(0, 0, 0));
    G4Transform3D trDet3 = G4Transform3D(*rotDet3, G4ThreeVector(0, -.6*cm, 0));

    G4MultiUnion *solidUnion = new G4MultiUnion("solidUnion");  //solidDet1 + solidDet2 + solidDet3, detectorul NOMEX din Si
    solidUnion->AddNode(*solidDet1, trDet1);
    solidUnion->AddNode(*solidDet2, trDet2);
    solidUnion->AddNode(*solidDet3, trDet3);
    solidUnion->Voxelize();
    G4LogicalVolume *logicUnion = new G4LogicalVolume(solidUnion, SiMat, "logicUnion");
    G4RotationMatrix *rotUnion = new G4RotationMatrix();
    rotUnion->rotateY(-90*deg);
    G4VPhysicalVolume *physUnion = new G4PVPlacement(rotUnion, G4ThreeVector(100*cm, 0, 23.15*cm), logicUnion, "physUnion", logicWorld, false, 0, true);

//==================================================================================

    //Visualization
    
    logicTube1->SetVisAttributes(G4Colour::Blue());
    logicTube2->SetVisAttributes(G4Colour::Yellow());
    logicSubtraction1->SetVisAttributes(G4Colour::Yellow());
    logicSubtractionHole->SetVisAttributes(G4Colour::Blue());
    logicBox2->SetVisAttributes(G4Colour::Green());
    logicTube4->SetVisAttributes(G4Colour::Brown());
    logicSubtraction2->SetVisAttributes(G4Colour::Blue());
    logicCollimator->SetVisAttributes(G4Colour::Brown());
    logicUnion->SetVisAttributes(G4Colour::Red());

    
    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

    //Variabile pt. PrimitiveScorer, MultiFunctionalDetector si ParticleFilter
    G4VPrimitiveScorer *primitive;
    G4MultiFunctionalDetector *doseDet;
    G4SDParticleFilter *gammaFilter;

    doseDet = new G4MultiFunctionalDetector("doseDet");
    G4SDManager::GetSDMpointer()->AddNewDetector(doseDet);

    primitive = new G4PSDoseDeposit("Ddep");  //Doza depozitata

    /*gammaFilter = new G4SDParticleFilter("gammaFilter", "gamma");  //se inregistreaza doar particulele numite "gamma" (fotonii)

    gammaFilter->add("gamma");
    primitive->SetFilter(gammaFilter); */

    doseDet->RegisterPrimitive(primitive);  //inregistrarea primitivei "primitive" in "doseDet"
    
    SetSensitiveDetector("logicUnion", doseDet);  //volumului logic "logicUnion" i s-a atribuit variabila "doseDet"
}