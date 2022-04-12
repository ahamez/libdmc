// $Author: ahamez $
// $Revision: 994 $
// $Date: 2007-06-26 20:47:57 +0200 (Tue, 26 Jun 2007) $

#ifndef greatspn_H_
#define greatspn_H_

#include <string>
#include "gspndmclib.h"
#include "dmc.hh"




namespace greatspn {
		
class greatspn : public dmc::rg::abstract_model
{

// attributes:
private:
  int (* my_dmcInitialize) (int,char**) ;
  int (* my_dmcFinalize) ();
  int (* my_dmc_initial_state) (pdmcState *);
  int (* my_dmc_succ) (const pdmcState , pdmcState * , size_t * ) ;
public:
  int (* my_dmc_print_state)(const pdmcState, char**);
  long (* my_dmc_concrete_count) (dmcState *);
// methods
public:
		
  greatspn(const std::string &path, void *libHandle);
  
  dmc::rg::dmc_state* get_initial_state();
  dmc::rg::abstract_state_succ_iterator* get_succ_iterator(dmc::rg::dmc_state* s);
  virtual ~greatspn ();
};
		
} // namespace model

#endif
