#include "PENDetectorConstruction.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4String.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4NistManager.hh"
#include "G4Cerenkov.hh"

#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"

#include "PENMaterials.hh"
#include <G4VisAttributes.hh>
#include <iostream>
#include <fstream>
#include <iterator>

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

//#include "TMath.h"

using namespace std;
PENDetectorConstruction::PENDetectorConstruction():
	G4VUserDetectorConstruction(),
    PENShell(nullptr),
    SiPM_0(nullptr),
    SiPM_1(nullptr),
    SiPM_2(nullptr),
    SiPM_3(nullptr),
    SiPM_4(nullptr),
    SiPM_5(nullptr),
    SiPM_6(nullptr),
    SiPM_7(nullptr),
    SiPM_8(nullptr),
    SiPM_9(nullptr),
	SiPM_10(nullptr),
	SiPM_11(nullptr)
{
	matconstructor = new PENMaterials;
	MPT_PEN = new G4MaterialPropertiesTable();
	AbsorptionLength = 1.5;//value at 400 nm
	fRES = 1.0;
	fLY = 3000. / MeV;
	fABSFile = "PEN_ABS";
	pmtReflectivity = 0.50;
	G4cout << "Start Construction" << G4endl;
	DefineMat();
	fTargetMaterial = G4Material::GetMaterial("PVT_structure");
	fGlassMaterialPMT = G4Material::GetMaterial("BorosilicateGlass");
	SetABS(AbsorptionLength);
}

PENDetectorConstruction::~PENDetectorConstruction()
{
}

void PENDetectorConstruction::DefineMat() 
{
	matconstructor->Construct();
	// ============================================================= Materials =============================================================
  //materialConstruction = new PenMaterials;
	matAir = G4Material::GetMaterial("Air");
	matBialkali = G4Material::GetMaterial("Bialkali");
	fGlass = G4Material::GetMaterial("BorosilicateGlass");
	fPOM = G4Material::GetMaterial("POM");
	fABS = G4Material::GetMaterial("ABS");
	matPEN = G4Material::GetMaterial("PEN");
	matSi = G4Material::GetMaterial("G4_Si");
	matTriggerFoilEJ212 = G4Material::GetMaterial("EJ212");
	Pstyrene = G4Material::GetMaterial("Polystyrene");
	matPMMA = G4Material::GetMaterial("PMMA");
	fVacuum = G4Material::GetMaterial("Vacuum");
	matGreaseEJ550 = G4Material::GetMaterial("Grease");
	matTeflon = G4Material::GetMaterial("G4_TEFLON");
	matVikuiti = G4Material::GetMaterial("Vikuiti");
	matTitanium = G4Material::GetMaterial("titanium");
	matPolyethylene = G4Material::GetMaterial("G4_POLYETHYLENE");
	matEnGe = G4Material::GetMaterial("EnGe");
	matNaGe = G4Material::GetMaterial("NaGe");
	matLN2 = G4Material::GetMaterial("G4_lN2");
	matLAr = G4Material::GetMaterial("G4_lAr");
	matGAGG = G4Material::GetMaterial("GAGG");

	G4cout << " materials ok " << G4endl;

	G4double wavelength;
	char filler;
	G4double varAbsorLength;
	G4double emission;
	G4double rindex;

	G4double wlPhotonEnergy[102] = { 0 };
	G4double ABSORPTION_PEN[102] = { 0 };
	G4double RINDEX_PEN[102] = { 0 };

	G4int absEntries = 0;
	ifstream ReadAbs;

	G4String absFile = "../input_files/" + fABSFile + ".csv";
	ReadAbs.open(absFile);
	if (ReadAbs.is_open())
	{
		while (!ReadAbs.eof())
		{
			ReadAbs >> wavelength >> filler >> varAbsorLength >> filler >> emission >> filler >> rindex;
			if (ReadAbs.eof()) {
				break;
			}
			wlPhotonEnergy[absEntries] = (1240. / wavelength) * eV;
			ABSORPTION_PEN[absEntries] = (varAbsorLength)*mm;
			RINDEX_PEN[absEntries] = rindex;
			absEntries++;
		}
	}

	else G4cout << "Error opening file: " << absFile << G4endl;
	ReadAbs.close();
	absEntries--;

	const G4int nEntries1 = sizeof(wlPhotonEnergy) / sizeof(G4double);
	assert(sizeof(RINDEX_PEN) == sizeof(wlPhotonEnergy));
	assert(sizeof(ABSORPTION_PEN) == sizeof(wlPhotonEnergy));
	//assert(sizeof(EMISSION_PEN) == sizeof(wlPhotonEnergy));

	MPT_PEN = new G4MaterialPropertiesTable();

	MPT_PEN->AddProperty("RINDEX", wlPhotonEnergy, RINDEX_PEN, nEntries1)->SetSpline(true);
	MPT_PEN->AddProperty("ABSLENGTH", wlPhotonEnergy, ABSORPTION_PEN, nEntries1)->SetSpline(true); // *

	// Read primary emission spectrum from PEN
	// Measurements from MPP Munich
	G4double pWavelength;
	G4String  Scint_file = "../properties/PEN_EM_SPECTRUM.dat";
	std::ifstream ReadScint2(Scint_file), ReadScintPEN;
	//count number of entries
	ReadScint2.unsetf(std::ios_base::skipws);
	//unsigned line_count = std::count(
	int line_count = std::count(
		std::istream_iterator<char>(ReadScint2),
		std::istream_iterator<char>(),
		'\n');
	std::cout << "Lines: " << line_count << "\n";
	ReadScint2.close();
	G4double PEN_EMISSION[500];
	G4double PEN_WL_ENERGY[500];
	G4int nEntriesPEN = 0;
	ReadScintPEN.open(Scint_file);
	if (ReadScintPEN.is_open()) {
		while (!ReadScintPEN.eof()) {

			ReadScintPEN >> pWavelength >> PEN_EMISSION[nEntriesPEN];
			if (ReadScintPEN.eof()) {
				break;
			}
			PEN_WL_ENERGY[nEntriesPEN] = (1240. / pWavelength) * eV;//convert wavelength to eV
		//G4cout<<nEntriesPEN<<" wl "<<PEN_WL_ENERGY[nEntriesPEN]<<" "<<PEN_EMISSION[nEntriesPEN]<<G4endl;
			nEntriesPEN++;
			if (nEntriesPEN > (line_count - 1)) { G4cout << " entries completed " << G4endl; break; }
		}
	}
	else
		G4cout << "Error opening file: " << Scint_file << G4endl;
	ReadScintPEN.close();
	G4cout << " nEntriesPEN " << nEntriesPEN << G4endl;

	MPT_PEN->AddProperty("FASTCOMPONENT", PEN_WL_ENERGY, PEN_EMISSION, line_count)->SetSpline(true);
	MPT_PEN->AddProperty("SLOWCOMPONENT", PEN_WL_ENERGY, PEN_EMISSION, line_count)->SetSpline(true);

	MPT_PEN->AddConstProperty("SCINTILLATIONYIELD", fLY / MeV); // * 2.5 * PEN = PS, 10*PEN=PS
	MPT_PEN->AddConstProperty("RESOLUTIONSCALE", fRES); // * 1, 4, 8
	MPT_PEN->AddConstProperty("FASTTIMECONSTANT", 5.198 * ns);
	MPT_PEN->AddConstProperty("SLOWTIMECONSTANT", 24.336 * ns);
	MPT_PEN->AddConstProperty("YIELDRATIO", 1.);

	matPEN->SetMaterialPropertiesTable(MPT_PEN);
	//pvt_structure->SetMaterialPropertiesTable(MPT_PEN);


	G4cout << " pen ok " << G4endl;


	G4double rindexEnergy[500] = { 0 };
	G4double scintIndex[500] = { 0 };

	G4int rindexEntries = 0;
	ifstream ReadRindex;

	G4String rindex_file = "../input_files/rindexScint.txt";
	ReadRindex.open(rindex_file);

	if (ReadRindex.is_open())
	{
		while (!ReadRindex.eof())
		{

			ReadRindex >> wavelength >> filler >> scintIndex[rindexEntries];
			if (ReadRindex.eof()) {
				break;
			}
			rindexEnergy[rindexEntries] = (1240. / wavelength) * eV;
			rindexEntries++;
		}
	}
	else G4cout << "Error opening file: " << rindex_file << G4endl;
	ReadRindex.close();
	rindexEntries--;

	G4double scintEnergy[501] = { 0 };
	G4double scintEmit[501] = { 0 };
	G4double scintEmitSlow[501] = { 0 };

	G4int scintEntries = 0;
	ifstream ReadScint;

	Scint_file = "../input_files/pTP_emission.txt";
	ReadScint.open(Scint_file);

	if (ReadScint.is_open())
	{
		while (!ReadScint.eof())
		{

			ReadScint >> wavelength >> filler >> scintEmit[scintEntries];
			if (ReadScint.eof()) {
				break;
			}
			//convert wavelength to eV:
			scintEnergy[scintEntries] = (1240. / wavelength) * eV;
			scintEmitSlow[scintEntries] = scintEmit[scintEntries];
			scintEntries++;
		}
	}
	else G4cout << "Error opening file: " << Scint_file << G4endl;
	ReadScint.close();
	scintEntries--;

	G4int absorbEntries = 0;
	G4double varAbsorbLength;
	G4double absorbEnergy[501] = { 0 };
	G4double Absorb[501] = { 0 };

	ifstream ReadAbsorb;
	G4String ReadAbsorbLength = "../input_files/PlasticBulkAbsorb2.cfg";

	ReadAbsorb.open(ReadAbsorbLength);
	if (ReadAbsorb.is_open())
	{
		while (!ReadAbsorb.eof())
		{

			ReadAbsorb >> wavelength >> filler >> varAbsorbLength;
			if (ReadAbsorb.eof()) {
				break;
			}
			absorbEnergy[absorbEntries] = (1240 / wavelength) * eV;
			Absorb[absorbEntries] = (varAbsorbLength)*m;
			absorbEntries++;
		}
	}
	else G4cout << "Error opening file: " << ReadAbsorbLength << G4endl;
	ReadAbsorb.close();
	absorbEntries--;

	G4double wlsEnergy[501] = { 0 };
	G4double wlsEmit[501] = { 0 };

	G4int wlsScintEntries = 0;
	ifstream ReadWLSScint;

	G4String wls_Scint_file = "../input_files/full_popop_emission.cfg";
	ReadWLSScint.open(wls_Scint_file);

	if (ReadWLSScint.is_open())
	{
		while (!ReadWLSScint.eof())
		{

			ReadWLSScint >> wavelength >> filler >> wlsEmit[500 - wlsScintEntries];
			if (ReadWLSScint.eof()) {
				break;
			}
			//convert wavelength to eV:
			wlsEnergy[500 - wlsScintEntries] = (1240 / wavelength) * eV;
			wlsScintEntries++;
		}
	}
	else G4cout << "Error opening file: " << wls_Scint_file << G4endl;
	ReadWLSScint.close();
	wlsScintEntries--;

	G4int wlsAbsorbEntries = 0;
	G4double wlsAbsorbEnergy[501] = { 0 };
	G4double wlsAbsorb[501] = { 0 };

	ifstream ReadWLSAbsorb;
	G4String ReadWLSAbsorbLength = "../input_files/scintAbsLen.txt";

	ReadWLSAbsorb.open(ReadWLSAbsorbLength);
	if (ReadWLSAbsorb.is_open())
	{
		while (!ReadWLSAbsorb.eof())
		{
			ReadWLSAbsorb >> wavelength >> filler >> varAbsorbLength;
			if (ReadWLSAbsorb.eof()) {
				break;
			}
			wlsAbsorbEnergy[wlsAbsorbEntries] = (1240. / wavelength) * eV;
			wlsAbsorb[wlsAbsorbEntries] = varAbsorbLength * mm;
			wlsAbsorbEntries++;
		}
	}
	else G4cout << "Error opening file: " << ReadWLSAbsorbLength << G4endl;
	ReadWLSAbsorb.close();
	wlsAbsorbEntries--;

	G4MaterialPropertiesTable* MPT_FoilEJ212 = new G4MaterialPropertiesTable();

	MPT_FoilEJ212->AddProperty("WLSABSLENGTH", wlsAbsorbEnergy, wlsAbsorb, wlsAbsorbEntries);
	MPT_FoilEJ212->AddProperty("WLSCOMPONENT", wlsEnergy, wlsEmit, wlsScintEntries);
	MPT_FoilEJ212->AddConstProperty("WLSTIMECONSTANT", 12 * ns);

	MPT_FoilEJ212->AddProperty("RINDEX", rindexEnergy, scintIndex, rindexEntries);
	MPT_FoilEJ212->AddProperty("ABSLENGTH", absorbEnergy, Absorb, absorbEntries);
	MPT_FoilEJ212->AddProperty("FASTCOMPONENT", scintEnergy, scintEmit, scintEntries);
	MPT_FoilEJ212->AddProperty("SLOWCOMPONENT", scintEnergy, scintEmitSlow, scintEntries);

	//MPT_FoilEJ212->AddConstProperty("SCINTILLATIONYIELD",11520./MeV);
	MPT_FoilEJ212->AddConstProperty("SCINTILLATIONYIELD", 10. / MeV);//set low LY to make it faster, intead use Edep for coincidences
	MPT_FoilEJ212->AddConstProperty("RESOLUTIONSCALE", 4.0);
	MPT_FoilEJ212->AddConstProperty("FASTTIMECONSTANT", 2.1 * ns);
	MPT_FoilEJ212->AddConstProperty("SLOWTIMECONSTANT", 14.2 * ns);
	MPT_FoilEJ212->AddConstProperty("YIELDRATIO", 1.0);

	matTriggerFoilEJ212->SetMaterialPropertiesTable(MPT_FoilEJ212);

	G4cout << " EJ212 ok " << G4endl;

	G4double refractive_index[] = { 1.49, 1.49, 1.49, 1.49, 1.49, 1.49 };
	G4double absPMMA[] = { 5 * m, 5 * m, 5 * m, 5 * m, 5 * m, 5 * m };
	G4double reflPMMA[] = { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 };
	G4double energyPMMA[] = { 2.18 * eV, 2.48 * eV, 2.58 * eV, 2.68 * eV, 2.78 * eV, 4.1 * eV };
	const G4int nEntries3 = sizeof(energyPMMA) / sizeof(G4double);

	G4MaterialPropertiesTable* MPT_PMMA = new G4MaterialPropertiesTable();
	MPT_PMMA->AddProperty("RINDEX", energyPMMA, refractive_index, nEntries3);
	MPT_PMMA->AddProperty("ABSLENGTH", energyPMMA, absPMMA, nEntries3)->SetSpline(true);
	MPT_PMMA->AddProperty("REFLECTIVITY", energyPMMA, reflPMMA, nEntries3)->SetSpline(true);
	matPMMA->SetMaterialPropertiesTable(MPT_PMMA);

}

void PENDetectorConstruction::SetABS(G4double value) {
	AbsorptionLength = value;
	//read file and add the value given by the user
	G4double wavelength;
	char filler;
	G4double varAbsorLength;
	G4double emission;
	G4double rindex;

	G4double wlPhotonEnergy[102] = { 0 };
	G4double ABSORPTION_PEN[102] = { 0 };

	G4int absEntries = 0;
	ifstream ReadAbs;

	G4String absFile = "../input_files/" + fABSFile + ".csv";
	ReadAbs.open(absFile);
	if (ReadAbs.is_open())
	{
		while (!ReadAbs.eof())
		{
			ReadAbs >> wavelength >> filler >> varAbsorLength >> filler >> emission >> filler >> rindex;
			if (ReadAbs.eof()) {
				break;
			}
			wlPhotonEnergy[absEntries] = (1240. / wavelength) * eV;
			ABSORPTION_PEN[absEntries] = (varAbsorLength * AbsorptionLength) * mm; //use measured value of attenuation to constrain curve and then change values multiplying the curve for a given factor
			absEntries++;
		}
	}

	else G4cout << "Error opening file: " << absFile << G4endl;
	ReadAbs.close();
	absEntries--;

	const G4int nEntries1 = sizeof(wlPhotonEnergy) / sizeof(G4double);
	assert(sizeof(ABSORPTION_PEN) == sizeof(wlPhotonEnergy));
	MPT_PEN->AddProperty("ABSLENGTH", wlPhotonEnergy, ABSORPTION_PEN, nEntries1)->SetSpline(true); // *
	//G4RunManager::GetRunManager()->PhysicsHasBeenModified();
#ifdef G4MULTITHREADED
	G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
#else
	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
#endif
}



G4VPhysicalVolume* PENDetectorConstruction::Construct()
{
	G4NistManager* nist = G4NistManager::Instance();

	/*
	//Material Properties
	//PEN
	auto matPEN = new G4Material("PEN", density = 1.33 * g / cm3, nComponents = 3);
	matPEN->AddElement(elC, nAtoms = 14);
	matPEN->AddElement(elH, nAtoms = 10);
	matPEN->AddElement(elH, nAtoms = 4);

	const G4int NUMENTRIES_PEN = 11;
	G4double Scnt_PP_PEN[NUMENTRIES_PEN] = { hc / 545. * eV, hc / 520. * eV, hc / 495. * eV, hc / 480. * eV, hc / 467. * eV, hc / 450. * eV, hc / 431. * eV, hc / 425. * eV, hc / 419. * eV,hc / 412. * eV,hc / 407. * eV };
	G4double Scnt_FAST_PEN[NUMENTRIES_PEN] = { 0.1, 0.2, 0.4, 0.6, 0.8, 1, 0.8, 0.6, 0.4, 0.2, 0.1 };
	G4double Scnt_SLOW_PEN[NUMENTRIES_PEN] = { 0.1, 0.2, 0.4, 0.6, 0.8, 1, 0.8, 0.6, 0.4, 0.2, 0.1 };
	G4double Scnt_RefractiveIndex_PEN[NUMENTRIES_PEN] = { 1.63, 1.63, 1.63, 1.63, 1.63, 1.63, 1.63, 1.63, 1.63, 1.63, 1.63 };
	G4double Scnt_AbsorptionLength_PEN[NUMENTRIES_PEN] = { 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm };
	G4double Scnt_Electron_Energy_PEN[NUMENTRIES_PEN] = { 0.0001 * keV, 0.3 * keV, 15. * keV, 22. * keV, 40. * keV, 60. * keV, 100. * keV,300. * keV, 662. * keV, 1. * MeV, 10. * GeV };
	G4double Scnt_Proton_Energy_PEN[NUMENTRIES_PEN] = { 0.0001 * keV, 10. * keV, 100. * keV, 1. * MeV, 2. * MeV,3.3 * MeV, 5.5 * MeV,8.8 * MeV, 10. * MeV, 2700. * MeV, 10. * GeV };
	G4double Scnt_Electron_Yield_PEN[NUMENTRIES_PEN] = { 0,6.254,927.64,1391.,2640.,3919.,6671.,20637.5,46000.,69486.,6.9486E+08 };
	G4double Scnt_Proton_Yield_PEN[NUMENTRIES_PEN] = { 0,137.58,1500.,15634.,31964,55033,95544,183444,222356,1.8761E+08,6.9486E+08 };
	G4double Scnt_Alpha_Yield_PEN[NUMENTRIES_PEN] = { 0,137.58,1500.,15634.,31964,55033,95544,183444,222356,1.8761E+08,6.9486E+08 };
	G4double Scnt_Ion_Yield_PEN[NUMENTRIES_PEN] = { 0,137.58,1500.,15634.,31964,55033,95544,183444,222356,1.8761E+08,6.9486E+08 };
	G4MaterialPropertiesTable* Scnt_MPT_PEN = new G4MaterialPropertiesTable();
	Scnt_MPT_PEN->AddProperty("FASTCOMPONENT", Scnt_PP_PEN, Scnt_FAST_PEN, NUMENTRIES_PEN);
	//Scnt_MPT_PEN->AddProperty("SLOWCOMPONENT", Scnt_PP_PEN, Scnt_SLOW_PEN, NUMENTRIES_PEN);
	Scnt_MPT_PEN->AddProperty("RINDEX", Scnt_PP_PEN, Scnt_RefractiveIndex_PEN, NUMENTRIES_PEN);
	Scnt_MPT_PEN->AddProperty("ABSLENGTH", Scnt_PP_PEN, Scnt_AbsorptionLength_PEN, NUMENTRIES_PEN);
	//Scnt_MPT_PEN->AddProperty("ELECTRONSCINTILLATIONYIELD", Scnt_Electron_Energy_PEN, Scnt_Electron_Yield_PEN, NUMENTRIES_PEN);
	Scnt_MPT_PEN->AddConstProperty("SCINTILLATIONYIELD", 4000. / MeV);
	Scnt_MPT_PEN->AddConstProperty("RESOLUTIONSCALE", 1.0);
	Scnt_MPT_PEN->AddConstProperty("FASTTIMECONSTANT", 27. * ns);
	//Scnt_MPT_PEN->AddConstProperty("SLOWTIMECONSTANT", 100. * ns);
	//Scnt_MPT_PEN->AddConstProperty("YIELDRATIO", 1.0);
	matPEN->SetMaterialPropertiesTable(Scnt_MPT_PEN);
	*/
	
	// Option to switch on/off checking of volumes overlaps
    //
	G4bool checkOverlaps = true;

  //Vaccum for world
  //G4Material* vacuum=new G4Material("Galactic",z=1.,a=1.01*g/mole,density=universe_mean_density,kStateGas,2.73*kelvin,3.e-18*pascal);

//------------------------------------------------------ volumes
//
  G4Material* world_mat = fVacuum;
  G4Material* env_mat = matLN2;
  G4Material* det_mat = matEnGe;

  //     
  // World&Envelope
  //
  G4double world_size = 60 * cm;
  G4double env_size = 50 * cm;
  G4Box* solidWorld = new G4Box("World", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  G4VPhysicalVolume* physWorld =
	  new G4PVPlacement(0,                     //no rotation
		  G4ThreeVector(),       //at (0,0,0)
		  logicWorld,            //its logical volume
		  "World",               //its name
		  0,                     //its mother  volume
		  false,                 //no boolean operation
		  0,                     //copy number
		  checkOverlaps);        //overlaps checking

  G4Box* solidEnv = new G4Box("Envelope",  0.5 * env_size, 0.5 * env_size, 0.5 * env_size);
  G4LogicalVolume* logicEnv = new G4LogicalVolume(solidEnv, env_mat, "Envelope");
  auto physEnv = new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

  //flatBEGe
  double GeChamfer = 2. * mm;
  double outerGeRadius = 31.35 * mm;
  double innerGeRadius = 1.50 * mm;
  double GeHeight1 = 60.80 * mm;
  double SrcThickness = 0.01 * mm;
  double lSmallValue = 0.01 * mm;
  double groovedepth = 1.5 * mm;
  double grooveradius = 9 * mm;
  double groovethickness = 0.001 * mm;
  double outerplayerthickness = 1 * um;
  G4double orbradius = 1.89 * mm;
  G4double deadlayerthickness = 1 * mm;

  // G4Height2 is the depth of the hole for pin contact
  double GeHeight2 = 4.0 * mm;
  double GeHeight3 = GeHeight2 + deadlayerthickness;

  auto GeP1 = new G4Tubs("GeP1", 0., outerGeRadius, GeHeight1 / 2, 0., twopi);
  auto GeP2 = new G4Tubs("GeP2", 0., outerGeRadius - GeChamfer, GeChamfer, 0., twopi);
  auto GeP3 = new G4Torus("GeP3", 0., GeChamfer, outerGeRadius - GeChamfer, 0., twopi);
  auto GeGroove = new G4Torus("solidgroove", 0., groovedepth, grooveradius, 0., twopi);

  //auto GeCut = new G4Tubs("solidGeCut", 0., 3 * cm, GeHeight2 / 2, 0., twopi);
  //auto GeM2 = new G4Tubs("solidGeM2", 0., innerGeRadius, GeHeight3 / 2., 0., twopi);

  G4ThreeVector zTransGe0(0., 0., -GeHeight1 / 2);
  G4ThreeVector zTransGe1(0., 0., -GeHeight1 / 2 + deadlayerthickness);
  G4ThreeVector zTransGe2(0., 0., GeHeight1 / 2 + lSmallValue);
  G4ThreeVector zTransGroove(0., 0., -GeHeight1 / 2);

  // total germanium crystal
  auto GeTemp1 = new G4UnionSolid("GeTemp1", GeP2, GeP3);
  auto GeTemp2 = new G4UnionSolid("GeTemp2", GeP1, GeTemp1, 0, zTransGe2);
  //?
  //auto solidtempTotalCrystal = new G4SubtractionSolid("totaltempCrystal", GeTemp2, GeGroove, 0, zTransGroove);
  auto solidTotalCrystal = new G4SubtractionSolid("totalCrystal", GeTemp2, GeGroove, 0, zTransGroove);
  auto logicTotalCrystal = new G4LogicalVolume(solidTotalCrystal, det_mat, "logicTotalCrystal");

  // bulk
  auto GeP1In = new G4Tubs("GeP1In", 0., outerGeRadius - deadlayerthickness, (GeHeight1 - deadlayerthickness * 2) / 2, 0., twopi);
  auto GeP2In = new G4Tubs("GeP2In", 0., outerGeRadius - deadlayerthickness - GeChamfer, GeChamfer, 0., twopi);
  auto GeP3In = new G4Torus("GeP3In", 0., GeChamfer, outerGeRadius - deadlayerthickness - GeChamfer, 0., twopi);
  auto GeP4In = new G4Tubs("GeP4In", 0., grooveradius, deadlayerthickness / 2, 0., twopi);

  auto GeLargerGroove = new G4Torus("solidlargergroove", 0., groovedepth + groovethickness, grooveradius, 0., twopi);
  auto GeOuterpLayer = new G4Tubs("solidouterplayer", 0., grooveradius - groovedepth, outerplayerthickness / 2, 0., twopi);
  //auto GeOuterpLayer = new G4Tubs("solidouterplayer", 0., grooveradius - groovedepth - groovethickness, outerplayerthickness, 0., twopi);

  auto GeInTemp1 = new G4UnionSolid("GeInTemp1", GeP2In, GeP3In);
  auto GeInTemp2 = new G4UnionSolid("GeInTemp2", GeP1In, GeInTemp1, 0, G4ThreeVector(0., 0., (GeHeight1 - deadlayerthickness * 2) / 2.0 + lSmallValue));
  auto GeInTemp3 = new G4UnionSolid("GeInTemp3", GeInTemp2, GeP4In, 0, G4ThreeVector(0., 0., -(GeHeight1 - deadlayerthickness) / 2));
  G4ThreeVector zbulkTrans(0., 0., -(GeHeight1 - deadlayerthickness * 2 - (GeHeight2 - deadlayerthickness)) / 2);
  G4ThreeVector zbulkTransOuterp(0., 0., -(GeHeight1 - outerplayerthickness) / 2);

  auto GeInTemp4 = new G4SubtractionSolid("GeInTemp4", GeInTemp3, GeLargerGroove, 0, zTransGroove);
  auto solidBulk = new G4SubtractionSolid("Bulk", GeInTemp4, GeOuterpLayer, 0, zbulkTransOuterp);
  auto logicBulk = new G4LogicalVolume(solidBulk, det_mat, "Bulk");
  //auto logicBulk = new G4LogicalVolume(GeInTemp4, det_mat, "Bulk");

  //deadlayer
  auto tempdeadlayer = new G4SubtractionSolid("tempdeadlayer", solidTotalCrystal, GeInTemp3, 0, G4ThreeVector(0., 0., 0.));
  auto solidOuterDeadlayer = new G4SubtractionSolid("OuterDeadlayer", tempdeadlayer, GeLargerGroove, 0, zTransGroove);
  auto logicOuterDeadlayer = new G4LogicalVolume(solidOuterDeadlayer, det_mat, "OuterDeadlayer");

  //groove Layer
  auto GrooveTorus = new G4Torus("solidpLayer", groovedepth, groovedepth + groovethickness, grooveradius, 0., twopi);
  auto GrooveCut1 = new G4Tubs("groovecut1", 0, grooveradius + groovedepth + groovethickness, groovedepth + groovethickness, 0, twopi);
  auto GrooveCut2 = new G4Tubs("groovecut2", 0, grooveradius - groovedepth, outerplayerthickness, 0, twopi);
  auto tempGroove1 = new G4SubtractionSolid("tempgroove1", GrooveTorus, GrooveCut1, 0, G4ThreeVector(0., 0., -(groovedepth + groovethickness)));
  auto GrooveLayer = new G4SubtractionSolid("GrooveLayer", tempGroove1, GrooveCut2, 0, G4ThreeVector(0., 0., 0.));
  auto logicGrooveLayer = new G4LogicalVolume(GrooveLayer, det_mat, "GrooveLayer");

  //outer pLayer
  auto OuterpLayer = new G4Tubs("solidOuterpLayer", 0, grooveradius - groovedepth, outerplayerthickness / 2, 0, twopi);
  auto logicOuterpLayer = new G4LogicalVolume(OuterpLayer, det_mat, "OuterpLayer");

  auto physDet = new G4PVPlacement(0, G4ThreeVector(), logicTotalCrystal, "PhysDet", logicEnv, false, 0, checkOverlaps);
  auto physBulk = new G4PVPlacement(0, G4ThreeVector(), logicBulk, "Bulk", logicTotalCrystal, false, 0, checkOverlaps);
  //new G4PVPlacement(0, G4ThreeVector(), logicOuterDeadlayer, "OuterDeadlayer", logicTotalCrystal, false, 0, checkOverlaps);
  //new G4PVPlacement(0, zbulkTransOuterp, logicOuterpLayer, "OuterpLayer", logicTotalCrystal, false, 0, checkOverlaps);
  //new G4PVPlacement(0, zTransGroove, logicGrooveLayer, "GrooveLayer", logicTotalCrystal, false, 0, checkOverlaps);


  //PEN shell scintilator
  G4double LN2Gap = 0.5 * mm;
  G4double ShellThickness = 1 * cm;
  auto solidPENShell = new G4Tubs("solidPENShell", outerGeRadius + LN2Gap, outerGeRadius + LN2Gap + ShellThickness, GeHeight1, 0., twopi);
  auto logicPENShell = new G4LogicalVolume(solidPENShell, matPEN, "logicPENShell");
  auto physPENShell = new G4PVPlacement(0, G4ThreeVector(), logicPENShell, "PENShell", logicEnv, false, 0, checkOverlaps);

  //SiPMs
  G4double SiPM_L = 10 * mm, SiPM_D = 0.5 * mm;
  G4Box* solidSiPM = new G4Box("SiPM_box", SiPM_D / 2, SiPM_L / 2, SiPM_L / 2);
  G4LogicalVolume* logicSiPM = new G4LogicalVolume(solidSiPM, matSi, "SiPM_log");
  G4double LN2Gap2 = 0.5 * mm;
  G4double SiPMtoCenDist = outerGeRadius + LN2Gap + ShellThickness + LN2Gap2 + SiPM_D / 2 + 5 * cm;

  G4double xyAngle0 = 0 * degree;
  auto rotSiPM0 = new G4RotationMatrix();
  rotSiPM0->rotateZ(xyAngle0);
  auto physSiPM0 = new G4PVPlacement(rotSiPM0, G4ThreeVector(SiPMtoCenDist * cos(xyAngle0), SiPMtoCenDist * sin(-xyAngle0), 0), logicSiPM, "SiPM0", logicEnv, false, 0, checkOverlaps);

  G4double xyAngle1 = 60 * degree;
  auto rotSiPM1 = new G4RotationMatrix();
  rotSiPM1->rotateZ(xyAngle1);
  auto physSiPM1 = new G4PVPlacement(rotSiPM1, G4ThreeVector(SiPMtoCenDist * cos(xyAngle1), SiPMtoCenDist * sin(-xyAngle1), 0), logicSiPM, "SiPM1", logicEnv, false, 1, checkOverlaps);

  G4double xyAngle2 = 120 * degree;
  auto rotSiPM2 = new G4RotationMatrix();
  rotSiPM2->rotateZ(xyAngle2);
  auto physSiPM2 = new G4PVPlacement(rotSiPM2, G4ThreeVector(SiPMtoCenDist * cos(xyAngle2), SiPMtoCenDist * sin(-xyAngle2), 0), logicSiPM, "SiPM2", logicEnv, false, 2, checkOverlaps);

  G4double xyAngle3 = 180 * degree;
  auto rotSiPM3 = new G4RotationMatrix();
  rotSiPM3->rotateZ(xyAngle3);
  auto physSiPM3 = new G4PVPlacement(rotSiPM3, G4ThreeVector(SiPMtoCenDist * cos(xyAngle3), SiPMtoCenDist * sin(-xyAngle3), 0), logicSiPM, "SiPM3", logicEnv, false, 3, checkOverlaps);

  G4double xyAngle4 = 240 * degree;
  auto rotSiPM4 = new G4RotationMatrix();
  rotSiPM4->rotateZ(xyAngle4);
  auto physSiPM4 = new G4PVPlacement(rotSiPM4, G4ThreeVector(SiPMtoCenDist * cos(xyAngle4), SiPMtoCenDist * sin(-xyAngle4), 0), logicSiPM, "SiPM4", logicEnv, false, 4, checkOverlaps);

  G4double xyAngle5 = 300 * degree;
  auto rotSiPM5 = new G4RotationMatrix();
  rotSiPM5->rotateZ(xyAngle5);
  auto physSiPM5 = new G4PVPlacement(rotSiPM5, G4ThreeVector(SiPMtoCenDist * cos(xyAngle5), SiPMtoCenDist * sin(-xyAngle5), 0), logicSiPM, "SiPM5", logicEnv, false, 5, checkOverlaps);

 // G4Box* solidSiPM_P = new G4Box("SiPM_box", SiPM_L / 2, SiPM_L / 2, SiPM_L / 2);
 // G4LogicalVolume* logicSiPM_P = new G4LogicalVolume(solidSiPM_P, matSi, "SiPM_log");
 // auto physSiPMP = new G4PVPlacement(0, G4ThreeVector(0, 0, -40 * mm), logicSiPM_P, "SiPM_P", logicEnv, false, 0, checkOverlaps);

  G4OpticalSurface* PEN_LN2 = new G4OpticalSurface("PEN_LN2");
  G4OpticalSurface* Ge_LN2 = new G4OpticalSurface("Ge_LN2");
  G4OpticalSurface* SiPM_LN2_0 = new G4OpticalSurface("SiPM_LN2_0");
  G4OpticalSurface* SiPM_LN2_1 = new G4OpticalSurface("SiPM_LN2_1");
  G4OpticalSurface* SiPM_LN2_2 = new G4OpticalSurface("SiPM_LN2_2");
  G4OpticalSurface* SiPM_LN2_3 = new G4OpticalSurface("SiPM_LN2_3");
  G4OpticalSurface* SiPM_LN2_4 = new G4OpticalSurface("SiPM_LN2_4");
  G4OpticalSurface* SiPM_LN2_5 = new G4OpticalSurface("SiPM_LN2_5");
 // G4OpticalSurface* SiPM_LN2_P = new G4OpticalSurface("SiPM_LN2_P");

  G4LogicalBorderSurface* PEN_LN2_LBS = new G4LogicalBorderSurface("PEN_LN2_LBS", physEnv, physPENShell, PEN_LN2);
  G4LogicalBorderSurface* Ge_LN2_LBS = new G4LogicalBorderSurface("PEN_LN2_LBS", physEnv, physDet, PEN_LN2);
  G4LogicalBorderSurface* SiPM_LN2_LBS_0 = new G4LogicalBorderSurface("SiPM_LN2_LBS_0", physEnv, physSiPM0, SiPM_LN2_0);
  G4LogicalBorderSurface* SiPM_LN2_LBS_1 = new G4LogicalBorderSurface("SiPM_LN2_LBS_1", physEnv, physSiPM1, SiPM_LN2_1);
  G4LogicalBorderSurface* SiPM_LN2_LBS_2 = new G4LogicalBorderSurface("SiPM_LN2_LBS_2", physEnv, physSiPM2, SiPM_LN2_2);
  G4LogicalBorderSurface* SiPM_LN2_LBS_3 = new G4LogicalBorderSurface("SiPM_LN2_LBS_3", physEnv, physSiPM3, SiPM_LN2_3);
  G4LogicalBorderSurface* SiPM_LN2_LBS_4 = new G4LogicalBorderSurface("SiPM_LN2_LBS_4", physEnv, physSiPM4, SiPM_LN2_4);
  G4LogicalBorderSurface* SiPM_LN2_LBS_5 = new G4LogicalBorderSurface("SiPM_LN2_LBS_5", physEnv, physSiPM5, SiPM_LN2_5);
 // G4LogicalBorderSurface* SiPM_LN2_LBS_P = new G4LogicalBorderSurface("SiPM_LN2_LBS_P", physEnv, physSiPMP, SiPM_LN2_P);

  PEN_LN2 = dynamic_cast <G4OpticalSurface*>(PEN_LN2_LBS->GetSurface(physEnv, physPENShell)->GetSurfaceProperty());
  Ge_LN2 = dynamic_cast <G4OpticalSurface*>(PEN_LN2_LBS->GetSurface(physEnv, physDet)->GetSurfaceProperty());
  SiPM_LN2_0 = dynamic_cast <G4OpticalSurface*>(SiPM_LN2_LBS_0->GetSurface(physEnv, physSiPM0)->GetSurfaceProperty());
  SiPM_LN2_1 = dynamic_cast <G4OpticalSurface*>(SiPM_LN2_LBS_1->GetSurface(physEnv, physSiPM1)->GetSurfaceProperty());
  SiPM_LN2_2 = dynamic_cast <G4OpticalSurface*>(SiPM_LN2_LBS_2->GetSurface(physEnv, physSiPM2)->GetSurfaceProperty());
  SiPM_LN2_3 = dynamic_cast <G4OpticalSurface*>(SiPM_LN2_LBS_3->GetSurface(physEnv, physSiPM3)->GetSurfaceProperty());
  SiPM_LN2_4 = dynamic_cast <G4OpticalSurface*>(SiPM_LN2_LBS_4->GetSurface(physEnv, physSiPM4)->GetSurfaceProperty());
  SiPM_LN2_5 = dynamic_cast <G4OpticalSurface*>(SiPM_LN2_LBS_5->GetSurface(physEnv, physSiPM5)->GetSurfaceProperty());
 // SiPM_LN2_P = dynamic_cast <G4OpticalSurface*>(SiPM_LN2_LBS_P->GetSurface(physEnv, physSiPMP)->GetSurfaceProperty());

  PEN_LN2->SetType(dielectric_dielectric);
  Ge_LN2->SetType(dielectric_metal);
  SiPM_LN2_0->SetType(dielectric_dielectric);
  SiPM_LN2_1->SetType(dielectric_dielectric);
  SiPM_LN2_2->SetType(dielectric_dielectric);
  SiPM_LN2_3->SetType(dielectric_dielectric);
  SiPM_LN2_4->SetType(dielectric_dielectric);
  SiPM_LN2_5->SetType(dielectric_dielectric);
 // SiPM_LN2_P->SetType(dielectric_dielectric);

  PEN_LN2->SetModel(DAVIS);
  Ge_LN2->SetModel(glisur);
  SiPM_LN2_0->SetModel(DAVIS);
  SiPM_LN2_1->SetModel(DAVIS);
  SiPM_LN2_2->SetModel(DAVIS);
  SiPM_LN2_3->SetModel(DAVIS);
  SiPM_LN2_4->SetModel(DAVIS);
  SiPM_LN2_5->SetModel(DAVIS);
  //SiPM_LN2_P->SetModel(DAVIS);

  PEN_LN2->SetFinish(Polished_LUT);
  Ge_LN2->SetFinish(Detector_LUT);
  SiPM_LN2_0->SetFinish(Detector_LUT);
  SiPM_LN2_1->SetFinish(Detector_LUT);
  SiPM_LN2_2->SetFinish(Detector_LUT);
  SiPM_LN2_3->SetFinish(Detector_LUT);
  SiPM_LN2_4->SetFinish(Detector_LUT);
  SiPM_LN2_5->SetFinish(Detector_LUT);
  //SiPM_LN2_P->SetFinish(Detector_LUT);
  const G4int NUMENTRIES_CHIP = 11;
  const double hc = 6.62606876 * 2.99792458 * 100. / 1.602176462;
  G4double sipm_pp[NUMENTRIES_CHIP] = { hc / 600. * eV, hc / 590. * eV, hc / 580. * eV, hc / 570. * eV, hc / 560. * eV, hc / 550. * eV, hc / 540. * eV, hc / 530. * eV, hc / 520. * eV,hc / 510. * eV,hc / 500. * eV };
  G4double sipm_sl[NUMENTRIES_CHIP] = { 0,0,0,0,0,0,0,0,0,0,0 };
  G4double sipm_ss[NUMENTRIES_CHIP] = { 0,0,0,0,0,0,0,0,0,0,0 };
  G4double sipm_bs[NUMENTRIES_CHIP] = { 0,0,0,0,0,0,0,0,0,0,0 };
  G4double sipm_rindex[NUMENTRIES_CHIP] = { 1.406,1.406,1.406,1.406,1.406,1.406,1.406,1.406,1.406,1.406,1.406 };
  G4double sipm_reflectivity[NUMENTRIES_CHIP] = { 0,0,0,0,0,0,0,0,0,0,0 };
  // G4double sipm_efficiency[NUMENTRIES_CHIP] = {0.20,0.21,0.23,0.25,0.26,0.28,0.30,0.32,0.34,0.36,0.38};
  G4double sipm_efficiency[NUMENTRIES_CHIP] = { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 };

  G4MaterialPropertiesTable* SIPM_MPT_Surf = new G4MaterialPropertiesTable();
  // SIPM_MPT_Surf->AddProperty("SPECULARLOBECONSTANT",sipm_pp,sipm_sl,NUMENTRIES_CHIP);
  // SIPM_MPT_Surf->AddProperty("SPECULARSPIKECONSTANT",sipm_pp,sipm_ss,NUMENTRIES_CHIP);
  // SIPM_MPT_Surf->AddProperty("BACKSCATTERCONSTANT",sipm_pp,sipm_bs,NUMENTRIES_CHIP);
  SIPM_MPT_Surf->AddProperty("REFLECTIVITY", sipm_pp, sipm_reflectivity, NUMENTRIES_CHIP);
  SIPM_MPT_Surf->AddProperty("EFFICIENCY", sipm_pp, sipm_efficiency, NUMENTRIES_CHIP);
  // SIPM_MPT_Surf->AddProperty("RINDEX",sipm_pp,sipm_rindex,NUMENTRIES_CHIP);

  SiPM_LN2_0->SetMaterialPropertiesTable(SIPM_MPT_Surf);
  SiPM_LN2_1->SetMaterialPropertiesTable(SIPM_MPT_Surf);
  SiPM_LN2_2->SetMaterialPropertiesTable(SIPM_MPT_Surf);
  SiPM_LN2_3->SetMaterialPropertiesTable(SIPM_MPT_Surf);
  SiPM_LN2_4->SetMaterialPropertiesTable(SIPM_MPT_Surf);
  SiPM_LN2_5->SetMaterialPropertiesTable(SIPM_MPT_Surf);

  Bulk = physBulk;
  PENShell = physPENShell;
  SiPM_0 = physSiPM0;
  SiPM_1 = physSiPM1;
  SiPM_2 = physSiPM2;
  SiPM_3 = physSiPM3;
  SiPM_4 = physSiPM4;
  SiPM_5 = physSiPM5;
  //SiPM_6 = physSiPMP;

  return physWorld;
}

