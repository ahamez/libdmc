// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#include <iostream>
#include <fstream>

#include "dmc.hh"

#include "big_simple_model/big_simple_model.hh"
#include "big_simple_model/big_simple_model_factory.hh"

int
main( int argc , char* argv[] )
{
	using namespace dmc;
	using namespace big_simple_model;

	std::cout << "Launching big simple model..." << std::endl;

	long nb_states;
	long state_cost;

	conf::options opt(argc,argv);
	opt.add_option("cost",'c',&state_cost,1000);
	opt.add_option("states",'s',&nb_states,1000);
	opt.parse();

	//////////////////////////////////////////////////////////////////////////
	// the real work begins here...

	big_simple_model_factory* bsmf = new big_simple_model_factory(nb_states,state_cost);
	dmc::dmc_manager dmc(bsmf,&opt);
	dmc.start_generation();

	// ... and ends here
	//////////////////////////////////////////////////////////////////////////


	return EXIT_SUCCESS;
}
