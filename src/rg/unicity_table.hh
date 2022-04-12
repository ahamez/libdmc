// $Author: ahamez $
// $Revision: 1024 $
// $Date: 2008-03-29 00:55:17 +0100 (sam, 29 mar 2008) $

#ifndef DMC_UNICITY_TABLE_H_
#define DMC_UNICITY_TABLE_H_

#include <pthread.h>
#include <ext/hash_set>

#include <tbb/concurrent_hash_map.h>

#include "rg/dmc_state.hh"

namespace dmc { namespace rg {

	enum state_state
	{
		NEW_S,
		NOT_NEW_S,
		RECOMPUTED_S
	};

//////////////////////////////////////////////////////////////////////

class unicity_table
{

// attributes
private:

	//////////////////////////////////////////////////////////////////////

    struct hash_compare
    {
        static
        size_t hash(const rg::dmc_state* state)
        {
            return state->hash();
        }

        static
        bool equal(const rg::dmc_state* state1 , const rg::dmc_state* state2 )
        {
			return  (state1 == state2) || (*state1 == *state2);
        }

    };

	//////////////////////////////////////////////////////////////////////

    struct Dummy{};

    typedef
        tbb::concurrent_hash_map<rg::dmc_state*, Dummy, hash_compare>
        concurrent_set;

    concurrent_set concur_set_;



public:

	unsigned long depth_;


// methods
public:

	unicity_table();
	~unicity_table();

	state_state
	insert(rg::dmc_state* as);

	unsigned long
	get_size() const;

private:

	// cannot copy unicity_table
	unicity_table(const unicity_table& ut);
	unicity_table& operator=(const unicity_table& ut);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
