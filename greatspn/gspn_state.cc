// $Author: ahamez $
// $Revision: 629 $
// $Date: 2006-08-07 15:23:09 +0200 (Mon, 07 Aug 2006) $

#include "greatspn/gspn_state.hh"
#include "greatspn/gspn_succiter.hh"

#include <cassert>
#include <cstring>

using namespace dmc;

namespace greatspn{

//////////////////////////////////////////////////////////////////////
	


//////////////////////////////////////////////////////////////////////
	
gspn_state::~gspn_state()
{}
	
//////////////////////////////////////////////////////////////////////
	
bool 
gspn_state::operator==(const dmc::rg::dmc_state& as) const
{
  //  return dmcStateEq(s, reinterpret_cast<const gspn_state&>(as).s); 
  
  return (s->length == reinterpret_cast<const gspn_state&>(as).s->length)
    && (s->d_ptr == reinterpret_cast<const gspn_state&>(as).s->d_ptr) 
    && (! memcmp(s->data,reinterpret_cast<const gspn_state&>(as).s->data,s->length));
}

//////////////////////////////////////////////////////////////////////

void
gspn_state::get_content(const void** data , size_t* size) const
{
  *size = s->length + sizeof(int);
  if (!buff) {
    buff = new char [*size];
    * ((size_t*)buff) = s->d_ptr;
    memcpy(buff+sizeof(int),s->data,s->length);
  }
  *data = buff;
}

//////////////////////////////////////////////////////////////////////

// rg::abstract_state_succ_iterator* 
// gspn_state::get_succ_iterator()
// {
//   return new gspn_succ_iterator(s);
// }

// //////////////////////////////////////////////////////////////////////

} // namespace
