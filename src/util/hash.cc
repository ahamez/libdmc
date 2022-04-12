// $Author: ahamez $
// $Revision: 902 $
// $Date: 2006-12-21 14:43:49 +0100 (Thu, 21 Dec 2006) $

#include "hash.hh"

#include <cstdlib>
#include <ctime>

namespace dmc { namespace util {
		
	size_t hash::rand_tab_zobrist[MAXBYTES][256];
		
//////////////////////////////////////////////////////////////////////	
		
void 
hash::init()
{
	for(auto & elem : hash::rand_tab_zobrist)
	{
		srand(time(nullptr));
		for(auto & elem_j : elem)
		{
			elem_j = rand() ;
		}
	}	
}
				
//////////////////////////////////////////////////////////////////////	
		
}} // namespaces
