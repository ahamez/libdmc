// $Author: ahamez $
// $Revision: 1024 $
// $Date: 2008-03-29 00:55:17 +0100 (sam, 29 mar 2008) $

#include <iostream>

#include "unicity_table.hh"

namespace dmc { namespace rg {

//////////////////////////////////////////////////////////////////////

unicity_table::unicity_table()
	:
    concur_set_(),
	depth_(0)
{
	depth_ = 0;
}

//////////////////////////////////////////////////////////////////////

state_state
unicity_table::insert(rg::dmc_state* as)
{
    state_state ss;

    concurrent_set::accessor access;
    if( concur_set_.insert( access, as) )
    {   // state not found
        ss = NEW_S;
    }
    else
    {   // state found
        rg::dmc_state* state_in_set = access->first;
        if( state_in_set->get_distance() > as->get_distance() )
        {
            state_in_set->set_distance( as->get_distance() );
            ss = RECOMPUTED_S;
        }
        else
        {
            ss = NOT_NEW_S;
        }

        if( depth_ < as->get_distance() )
        {
            depth_ = as->get_distance();
        }

    }
    return ss;

}

//////////////////////////////////////////////////////////////////////

unicity_table::~unicity_table()
{
    std::cout
        << "Number of unique states in local hash table: "
        << concur_set_.size()
        << std::endl;

    concurrent_set::iterator it;
	for(it = concur_set_.begin() ;
		it != concur_set_.end();)
	{
        concurrent_set::iterator cit = it;
		++it;

		rg::dmc_state* tmp = (*cit).first ;
		concur_set_.erase((*cit).first);

		delete tmp;
	}
}

//////////////////////////////////////////////////////////////////////

unsigned long
unicity_table::get_size() const
{
	return concur_set_.size();
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
