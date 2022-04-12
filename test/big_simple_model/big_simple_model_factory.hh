// $Author: ahamez $
// $Revision: 653 $
// $Date: 2006-08-10 15:03:39 +0200 (Thu, 10 Aug 2006) $

#ifndef BIG_SIMPLE_MODEL_FACTORY_H_
#define BIG_SIMPLE_MODEL_FACTORY_H_

#include "dmc.hh"

namespace big_simple_model
{
	
//////////////////////////////////////////////////////////////////////
	
class big_simple_model_factory : public dmc::rg::abstract_model_factory
{
// attributes
private:
	
	int cost_;
	int nb_states_;
	
	
// methods
public:
		
	big_simple_model_factory(int nb_threads,
							 int cost);
	virtual dmc::rg::abstract_model* create_model() ;
	
};

//////////////////////////////////////////////////////////////////////

}

#endif
