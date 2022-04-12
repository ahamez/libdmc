// $Author: ahamez $
// $Revision: 1002 $
// $Date: 2007-10-08 06:30:47 +0200 (Mon, 08 Oct 2007) $

#ifndef DMC_HASH_H_
#define DMC_HASH_H_

#include "rg/dmc_state.hh"

#define MAXBYTES 512

namespace dmc { namespace util {
		
//////////////////////////////////////////////////////////////////////

/// Provides a more or less 'generic' hash function 
/// which should not care about its input
class hash
{

// attributes
private:
		
	// random tab for the zobrist hash function
	static size_t rand_tab_zobrist[MAXBYTES][256];
	

// methods
public:
		
	static void init();
		
	//////////////////////////////////////////////////////////////////////	
	
	inline static size_t zobrist(const rg::dmc_state& as)
	{
		const unsigned char* key;
		size_t len;
		as.get_content((const void**)&key,&len);
		
		register size_t r,i;
		// zobrist hash function, a kind of universal hash function?
		for( i = 0 , r=len ; i < len ; i++ )
		{
			r ^= hash::rand_tab_zobrist[i][(int)key[i]];
		}
		return r;
	};	
		
	//////////////////////////////////////////////////////////////////////	
		
};
	
}} // namespaces

#endif
