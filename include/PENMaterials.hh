/**
 * @file PenMaterials.hh
 * @author: (modified by) Luis Manzanillas
 * @date 2020, Max Planck Institute for Physics
 */


#ifndef PENMaterials_H
#define PENMaterials_H

#include "globals.hh"


class PENMaterials {
public:
  PENMaterials();
  ~PENMaterials();
  
  void Construct();
  
private:
  G4double lightYieldAntracene;
    
};

#endif
