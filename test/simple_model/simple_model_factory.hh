// $Author: ahamez $
// $Revision: 653 $
// $Date: 2006-08-10 15:03:39 +0200 (Thu, 10 Aug 2006) $

#ifndef SIMPLE_MODEL_FACTORY_H_
#define SIMPLE_MODEL_FACTORY_H_

//#include "rg/abstract_model_factory.hh"
//#include "rg/abstract_model.hh"
#include "dmc.hh"

namespace simple_model
{

//////////////////////////////////////////////////////////////////////

class simple_model_factory : public dmc::rg::abstract_model_factory
{
// attributes
private:
	
	int cost_;
	bool verbose_;
	
	
// methods
public:

	simple_model_factory(int cost,
						 bool verbose);
	virtual dmc::rg::abstract_model* create_model() ;
	
};

//////////////////////////////////////////////////////////////////////

}

#endif
