// $Author: ahamez $
// $Revision: 947 $
// $Date: 2007-03-20 21:17:05 +0100 (Tue, 20 Mar 2007) $

#include "rg/dmc_state.hh"
#include "util/hash.hh"

#include <openssl/md5.h>

namespace dmc { namespace rg {
		
//////////////////////////////////////////////////////////////////////

size_t
dmc_state::hash() const
{
	// built-in hash function
	return util::hash::zobrist(*this);
}

//////////////////////////////////////////////////////////////////////

int
dmc_state::localization() const
{
	unsigned char tab[16];
	
	MD5( (unsigned char*) this->data,
		 this->size,
		 tab);
	
	return (int)(*tab) ;
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
