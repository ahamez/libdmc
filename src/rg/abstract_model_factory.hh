// $Author: ahamez $
// $Revision: 682 $
// $Date: 2006-08-22 12:14:18 +0200 (Tue, 22 Aug 2006) $

#ifndef DMC_ABSTRACT_MODEL_FACTORY_H_
#define DMC_ABSTRACT_MODEL_FACTORY_H_

#include "rg/abstract_model.hh"

namespace dmc { namespace rg {
		
//////////////////////////////////////////////////////////////////////

class abstract_model_factory
{
	
// methods
public:
		
	virtual ~abstract_model_factory(){};
	virtual abstract_model* create_model() = 0;
	
};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
