// $Author: ahamez $
// $Revision: 689 $
// $Date: 2006-08-25 10:48:52 +0200 (Fri, 25 Aug 2006) $

#ifndef greatspn_PN_SUCC_ITERATOR_H_
#define greatspn_PN_SUCC_ITERATOR_H_

#include <string>
#include <list>

#include "dmc.hh"
#include "gspndmclib.h"

namespace greatspn{

//////////////////////////////////////////////////////////////////////
	
class gspn_succ_iterator : public dmc::rg::abstract_state_succ_iterator
{

// attributes
private:
  pdmcState  tab_succ;
  size_t tab_succ_size;
  size_t pos;
// methods
public:
	
  gspn_succ_iterator (pdmcState ttab_succ, size_t ttab_succ_size) : tab_succ(ttab_succ), tab_succ_size(ttab_succ_size), pos(0) {};

  virtual ~gspn_succ_iterator();

  // required from libdmc
  bool has_successors();
  dmc::rg::dmc_state* get_next_successor();

private:

  gspn_succ_iterator(const gspn_succ_iterator& s);
  gspn_succ_iterator& operator=(const gspn_succ_iterator& s);
  	
};

//////////////////////////////////////////////////////////////////////

} // namespace

#endif
