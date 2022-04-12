// $Author: ahamez $
// $Revision: 316 $
// $Date: 2006-07-10 15:35:05 +0200 (Mon, 10 Jul 2006) $

#include "simple_model_factory.hh"
#include "simple_model.hh"

namespace simple_model
{

//////////////////////////////////////////////////////////////////////

simple_model_factory::simple_model_factory(int cost,
										   bool verbose)
{
	cost_ = cost;
	verbose_ = verbose;
}

//////////////////////////////////////////////////////////////////////

dmc::rg::abstract_model* 
simple_model_factory::create_model()
{
	return new simple_model(cost_,verbose_);
}

//////////////////////////////////////////////////////////////////////

}
