#include"DetectorConstruction.hh"
#include"SensitiveDetector_new.hh" 
#include<G4NistManager.hh>
#include<G4Box.hh>
#include<G4Tubs.hh>
#include<G4Polyhedra.hh>
#include<G4LogicalVolume.hh>
#include<G4PVPlacement.hh>
#include<G4SDManager.hh>
#include<G4VisAttributes.hh>
#include "G4SystemOfUnits.hh"

//#include "ExN02TrackerSD.hh"

using namespace std;

#define Mat(x) (G4NistManager::Instance()->FindOrBuildMaterial(x))


World::World(double size_x, double size_y, double size_z,  G4Material *mater_): mater(mater_), sizex(size_x), sizey(size_y), sizez(size_z)
{
	//double size05 = size/2;
	solid = new G4Box("world", sizex/2, sizey/2, sizez/2);
	logic = new G4LogicalVolume( solid, mater, "World", 0, 0, 0);
	physic = new G4PVPlacement(0, G4ThreeVector(), logic, "World",
				   0, false, 0);
}

DetectorConstruction::DetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
/*materials*/
	G4Material		*materDet,
					*materlay1,
					*materlay2,
					*materEndLay;

	G4Material		*materBC400,
					*materPbGL,
					*materPb,
					*materPbO,
					*materW,
					*materCu,
					*materFGL,
					*materSiO2,
					*materAl2O3,
					*materMgO,
					*materCaO,
					*materNa2O;
	G4NistManager	*man = G4NistManager::Instance();
	man->SetVerbose(1);

	G4Element* elemH   = new G4Element( "Hydrogen"  , "H"    ,  1 ,   1.0074  * g/mole );
	G4Element* elemC   = new G4Element( "Carbon"    , "C"    ,  6 ,  12.01    * g/mole );
	G4Element* elemO   = new G4Element( "Oxygen"    , "O"    ,  8 ,  16.00    * g/mole );
	G4Element* elemNa   = new G4Element( "Natrium"  , "Na"   , 11 ,  22.989   * g/mole );
	G4Element* elemMg  = new G4Element( "Magnesium" , "Mg"   , 12 ,  24.304   * g/mole );
	G4Element* elemAl  = new G4Element( "Aluminum"  , "Al"   , 13 ,  26.9815  * g/mole );
	G4Element* elemSi  = new G4Element( "Silicon"   , "Si"   , 14 ,  28.0855  * g/mole );
	G4Element* elemCa  = new G4Element( "Calcium"   , "Ca"   , 20 ,  40.078   * g/mole );
	G4Element* elemCu  = new G4Element( "Copper"    , "Cu"   , 29 ,  63.546   * g/mole );
	//~ G4Element* elemGe  = new G4Element( "Germanium" , "Ge"   , 32 ,  72.64    * g/mole );
	//~ G4Element* elemI   = new G4Element( "Iodine"    , "I"    , 53 , 126.904   * g/mole );
	//~ G4Element* elemCs  = new G4Element( "Cesium"    , "Cs"   , 53 , 126.904   * g/mole );
	G4Element* elemW   = new G4Element( "Tungsten"  , "W"    , 74 , 183.84    * g/mole );
	//~ G4Element* elemTl  = new G4Element( "Thallium"  , "Tl"   , 81 , 204.3833  * g/mole );
	G4Element* elemPb  = new G4Element( "Plumbum"   , "Pb"   , 82 ,  207.211  * g/mole );
	//~ G4Element* elemBi  = new G4Element( "Bismuth"   , "Bi"   , 83 , 208.980   * g/mole );

	G4Element* Zn = man->FindOrBuildElement("Zn");
	G4Element* S = man->FindOrBuildElement("S");
	//~ G4Element* Pb = man->FindOrBuildElement("Pb");

	//~ Carbon-fiber-reinforced polymer
/*
	materCFRP   = new G4Material( "CFRP"         , 1.46     * g/cm3 , 3 ); 
	materCsI    = new G4Material( "CsI(Tl)"      , 4.53     * g/cm3 , 3 );
	materCSota  = new G4Material( "CSota"        , 0.004    * g/cm3 , 1 );
	materAlSota = new G4Material( "AlSota"       , 0.0019   * g/cm3 , 1 );
	materSi     = new G4Material( "Si"           , 0.003    * g/cm3 , 1 );
	materPSty   = new G4Material( "Polystyrene"  , 1.060    * g/cm3 , 2 );
	materCuSi   = new G4Material( "CuSi"         , 2*0.0019 * g/cm3 , 2 );
*/
	materBC400  = new G4Material( "BC400"  , 1.032   * g/cm3 , 4 );  //4
	materPb     = new G4Material( "matPb"  , 11.3415 * g/cm3 , 1 );
	materCu     = new G4Material( "matCu"  , 8.92    * g/cm3 , 1 );
	materW      = new G4Material( "matW"   , 19.25   * g/cm3 , 1 );
/*
	materCFRP	-> AddElement( elemC  ,.7195);
	materCFRP	-> AddElement( elemH  ,.2439);
	materCFRP	-> AddElement( elemO  ,.0366);
	materCsI	-> AddElement( elemCs ,  5  );
	materCsI	-> AddElement( elemI  ,  5  );
	materCsI	-> AddElement( elemTl ,  0  );
	materCSota	-> AddElement( elemC  , 1.0);
	materAlSota	-> AddElement( elemAl , 1.0);
	materSi		-> AddElement( elemSi , 1.0);
	materPSty	-> AddElement( elemH  ,.0774);
	materPSty	-> AddElement( elemC  ,.9226);
	materCuSi	-> AddElement( elemCu  ,.5);
	materCuSi	-> AddElement( elemSi  ,.5);
*/
	materBC400	-> AddElement( elemH , .523);
	materBC400	-> AddElement( elemC , .474);
	materBC400	-> AddElement( Zn    , .0015);
	materBC400	-> AddElement( S     , .0015);

// FGL - fiberglass laminate
	materFGL   = new G4Material( "FGL"   , 1.8    * g/cm3 , 5 );
	materSiO2  = new G4Material( "SiO2"  , 2.65   * g/cm3 , 2 );
	materAl2O3 = new G4Material( "Al2O3" , 3.95   * g/cm3 , 2 );
	materMgO   = new G4Material( "MgO"   , 3.58   * g/cm3 , 2 );
	materCaO   = new G4Material( "CaO"   , 3.34   * g/cm3 , 2 );
	materNa2O  = new G4Material( "Na2O"  , 2.27   * g/cm3 , 2 );

	materSiO2	-> AddElement( elemSi , .333);
	materSiO2	-> AddElement( elemO  , .667);

	materAl2O3	-> AddElement( elemAl , .4);
	materAl2O3	-> AddElement( elemO  , .6);

	materMgO	-> AddElement( elemMg , .5);
	materMgO	-> AddElement( elemO  , .5);

	materCaO	-> AddElement( elemCa , .5);
	materCaO	-> AddElement( elemO  , .5);

	materNa2O	-> AddElement( elemNa , .667);
	materNa2O	-> AddElement( elemO  , .333);

	materFGL	-> AddMaterial( materSiO2  , .71);
	materFGL	-> AddMaterial( materAl2O3 , .03);
	materFGL	-> AddMaterial( materMgO   , .03);
	materFGL	-> AddMaterial( materCaO   , .08);
	materFGL	-> AddMaterial( materNa2O  , .15);


	materPb -> AddElement( elemPb , 1.);
	materCu -> AddElement( elemCu , 1.);
	materW  -> AddElement( elemW  , 1.);

// Pb glass
	materPbO   = new G4Material( "PbO"   , 9.63   * g/cm3 , 2 );
	materPbO	-> AddElement( elemPb , .5);
	materPbO	-> AddElement( elemO  , .5);
	
	materPbGL  = new G4Material( "PbGL"   , 3.0   * g/cm3 , 2 );
	materPbGL	-> AddMaterial( materPbO  , .5);
	materPbGL	-> AddMaterial( materSiO2  , .5);
	

	materDet = materBC400;
	//~ materlay1 = materFGL;
	materlay1 = Mat("G4_Galactic");
	//~ materlay2 = materCu;
	//~ materlay2 = materW;
	materlay2 = materPbGL;
	//materlay2 = materPb;
	materEndLay = Mat("G4_Galactic");

	/*end-of-materials*/

	world = new World(3 *m, 3 *m, 24 *m, Mat("G4_Galactic"));


// row:col 3:3
	const int		sheet = 15,
					row = 8,
					col = 8;

	double			boxH = 30.0, //plactic box height
					boxW = 30.0, //plactic box width
					boxD = 20.0, //plactic box depth

					pbH = row * boxH,
					pbW = row * boxH,
					pbD = 20.0,

					//~ fglH = row * boxH,
					//~ fglW = row * boxH,
					fglD = 0.0;

	// -120___<x<___ 120
	//     |xyz -> |  <
	//     |    -> |  y
	//     |       |  <
	//     |_______|-120

	double			stX = -col * boxW / 2, //start position calorimetr
					stY = row * boxH / 2,
					stZ = 0.0,
					coordZ = stZ;

	//stX = -120.0, //start position calorimetr
	//stY = 120.0,
	//stZ = 0.0;
	char detname[50];

	G4SDManager	*SDman = G4SDManager::GetSDMpointer();

	G4Box				*plasticBox[sheet][row][col];
	G4LogicalVolume		*plasticLogic[sheet][row][col];
	G4VPhysicalVolume	*plasticPhys[sheet][row][col];
	SensitiveDetector	*spdCal[sheet][row][col];

	double				pbStX = 0.0, 
						pbStY = 0.0,
						pbStZ = 0.0;

	G4Box				*pbBox[sheet-1];
	G4LogicalVolume		*pbLogic[sheet-1];
	G4VPhysicalVolume	*pbPhys[sheet-1];
	SensitiveDetector	*pbCal[sheet-1];


	for(int st = 0; st < sheet; st++)
	{
		coordZ += boxD/2;
		for(int r = 0; r < row; r++)
		{
			for(int c = 0; c < col; c++)
			{
				plasticBox[st][r][c] = new G4Box("plasticBox", (boxW/2)*mm, (boxH/2)*mm, (boxD/2)*mm);
				plasticLogic[st][r][c] = new G4LogicalVolume(plasticBox[st][r][c], materDet, "plasticLogic"); //materBC400
				plasticPhys[st][r][c]  = new G4PVPlacement(0, 
					G4ThreeVector((stX + boxW/2 + boxW*c)*mm, (stY - boxH/2 - boxH*r)*mm, coordZ*mm), 
					plasticLogic[st][r][c], "plasticPhys", world->getLogic(), false, 0);

				sprintf(detname, "spdCal_%d_%d_%d", st, r, c);
				spdCal[st][r][c] = new SensitiveDetector(detname);

				SDman->AddNewDetector(spdCal[st][r][c]);
				plasticLogic[st][r][c]->SetSensitiveDetector(spdCal[st][r][c]);
			}
		}
		coordZ += boxD/2;
		if (st < sheet - 1)
		{
			coordZ += pbD/2;

			pbBox[st] = new G4Box("pbBox", (pbW/2)*mm, (pbH/2)*mm, (pbD/2)*mm);
			pbLogic[st] = new G4LogicalVolume(pbBox[st], materlay2, "pbLogic");       //Mat("G4_Pb")  materPb
			pbPhys[st]  = new G4PVPlacement(0,
			G4ThreeVector(pbStX*mm, pbStY*mm, coordZ*mm), pbLogic[st], "pbPhys", world->getLogic(), false, 0);

			sprintf(detname, "Pb_%d", st);
			pbCal[st] = new SensitiveDetector(detname);

			SDman->AddNewDetector(pbCal[st]);
			pbLogic[st]->SetSensitiveDetector(pbCal[st]);
			coordZ += pbD/2;
		}
	}

		// endLay
	SensitiveDetector	*endLayCal;
	G4Box				*endLayBox;
	G4LogicalVolume		*endLayLogic;
	G4VPhysicalVolume	*endLayPhys;

	double	endLayStX = 0.0, 
			endLayStY = 0.0;
			//pbStZ = 0.0;

	coordZ += boxD; // толщина как у pb

	endLayBox = new G4Box("endLayBox", ((col * boxW) / 2)*mm, ((row * boxH) / 2)*mm, (boxD)*mm);
	endLayLogic = new G4LogicalVolume(endLayBox, materEndLay, "endLayLogic");       //Mat("G4_Galactic") 
	endLayPhys  = new G4PVPlacement(0,
		G4ThreeVector(endLayStX*mm, endLayStY*mm, coordZ*mm),
		endLayLogic, "endLayPhys", world->getLogic(), false, 0);

	sprintf(detname, "endLay");
	endLayCal = new SensitiveDetector(detname);
	SDman->AddNewDetector(endLayCal);
	endLayLogic->SetSensitiveDetector(endLayCal);

	world->getLogic()->SetVisAttributes (G4VisAttributes::Invisible);

	return world->getPhysic();
}
