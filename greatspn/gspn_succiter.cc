// $Author: ahamez $
// $Revision: 629 $
// $Date: 2006-08-07 15:23:09 +0200 (Mon, 07 Aug 2006) $

#include <cassert>

#include "greatspn/gspn_succiter.hh"
#include "greatspn/gspn_state.hh"

using dmc::rg::dmc_state;

namespace greatspn{

//////////////////////////////////////////////////////////////////////

// gspn_succ_iterator::gspn_succ_iterator(const pdmcState  s)
// {
//   dmc_succ (s,&tab_succ,&tab_succ_size);
//   pos = 0;
// }

//////////////////////////////////////////////////////////////////////

gspn_succ_iterator::~gspn_succ_iterator() 
{
  free (tab_succ);
}

//////////////////////////////////////////////////////////////////////

bool
gspn_succ_iterator::has_successors()
{
  return pos < tab_succ_size;
}

//////////////////////////////////////////////////////////////////////

dmc::rg::dmc_state* 
gspn_succ_iterator::get_next_successor()
{
  // TODO LENGTH !!!
  pdmcState M = tab_succ + pos++ ;

  dmc_state * ret = new  dmc_state();
  ret->size = M->length + sizeof(size_t);
  ret->data = new char [ret->size];
  * ( (size_t*) ret->data ) = M->d_ptr;
  memcpy ( (char*) ret->data + sizeof(size_t), M->data, M->length );
  
  free (M->data);
    
  return ret;
}

//////////////////////////////////////////////////////////////////////

}
