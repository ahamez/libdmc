// $Author: ahamez $
// $Revision: 689 $
// $Date: 2006-08-25 10:48:52 +0200 (Fri, 25 Aug 2006) $

#ifndef GSPN_MODEL_FACTORY_H_
#define GSPN_MODEL_FACTORY_H_

#include <string>
#include "dmc.hh"

namespace greatspn
{

//////////////////////////////////////////////////////////////////////

class gspn_model_factory : public dmc::rg::abstract_model_factory
{
// attributes
private:
  int nb_model;
  std::string path;
// methods
public:

  void set_options(const std::string & ppath) ;
  virtual dmc::rg::abstract_model* create_model();
	
};

//////////////////////////////////////////////////////////////////////

}

#endif
