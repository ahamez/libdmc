// $Author: ahamez $
// $Revision: 736 $
// $Date: 2006-09-01 15:13:32 +0200 (Fri, 01 Sep 2006) $

#include "big_simple_model_factory.hh"
#include "big_simple_model.hh"

namespace big_simple_model
{
	
//////////////////////////////////////////////////////////////////////
	
big_simple_model_factory::big_simple_model_factory(int nb_states,
												   int cost)
{
	cost_ = cost;
	nb_states_ = nb_states;
}

//////////////////////////////////////////////////////////////////////

dmc::rg::abstract_model* 
big_simple_model_factory::create_model()
{
	return new big_simple_model(nb_states_,cost_);
}

//////////////////////////////////////////////////////////////////////

} // namespace
