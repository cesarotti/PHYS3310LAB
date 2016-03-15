/*
 * Dark Photon Primary Generator Action
 * !!!History
 *    CJC 6.14.15 created
 *    CJC 1.4.16 MODIFIED
 *
 * file: Primary Generator Action
 *
 * Description
 * This class sets up the experiment, including the world volume and the 
 * particle beam.
 * Methods to update:
 * PrimaryGeneratorAction
 * {adjust energy settings of the gun and particles}
 */

#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction()
{
  
  G4int nofParticles = 1;

  //Basic option, only fires number of particles with given kinetic info
  fParticleGun1 = new G4ParticleGun(nofParticles);

  // default particle kinematic

  // Choose particle
  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle("gamma"); 

  fParticleGun1->SetParticleDefinition(particleDefinition);

  //Direction of beam
  G4double pi = 3.14159264358979;
  G4double theta = G4RandFlat::shoot(0., pi);
  G4double phi = G4RandFlat::shoot(0., 2*pi);

  fParticleGun1->
    SetParticleMomentumDirection(G4ThreeVector(std::cos(phi)*std::sin(theta),
					       std::sin(phi)*std::sin(theta),
					       std::cos(theta))); 
  // fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

 
  //Note, this sets the KINETIC energy of the particle, not the TOTAL
  // for HEP, this is probably a negligible distinction
  
  fParticleGun1->SetParticleEnergy(1.33*MeV);

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun1;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore.

  G4double worldZHalfLength = 0;
  G4LogicalVolume* worldLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World"); //Defined in DetectorConstruction.cc

  G4Box* worldBox = NULL;
  if ( worldLV ) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  if ( worldBox ) worldZHalfLength = worldBox->GetZHalfLength();
  else  {
    G4cerr << "World volume of box not found." << G4endl;
    G4cerr << "Perhaps you have changed geometry." << G4endl;
    G4cerr << "The gun will be place in the center." << G4endl;
  }

  fParticleGun1->SetParticlePosition(G4ThreeVector(0., 0.*mm, 0.));

  fParticleGun1->GeneratePrimaryVertex(anEvent); //Creates event to run

}
