// $Author: ahamez $
// $Revision: 689 $
// $Date: 2006-08-25 10:48:52 +0200 (Fri, 25 Aug 2006) $

#ifndef gspn_STATE_HH
#define gspn_STATE_HH

#include "dmc.hh"
#include "gspndmclib.h"

namespace greatspn{

//////////////////////////////////////////////////////////////////////
	
class gspn_state // : public dmc::rg::dmc_state 
{

// attributes	
private:

  pdmcState s;
  mutable char * buff;
	
// methods
public:
  gspn_state(void* data,size_t size) ;

  gspn_state (pdmcState ss) : s(ss),buff(NULL){ }

  ~gspn_state();
  
  pdmcState getState() { return s;}
  // overrides the ones from libdmc
  // size_t hash() const;
  //int localization() const;
  
  // required from libdmc
  bool operator==(const dmc::rg::dmc_state& as) const;	
  void get_content(const void** data , size_t* size) const;
  dmc::rg::abstract_state_succ_iterator* get_succ_iterator();
  
};

//////////////////////////////////////////////////////////////////////

} // namespace

#endif
