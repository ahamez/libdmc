// $Author: ahamez $
// $Revision: 1041 $
// $Date: 2008-05-14 11:56:37 +0200 (Wed, 14 May 2008) $

#include <iostream>

#include "dmc.hh"

#include "simple_model/simple_model.hh"
#include "simple_model/simple_model_factory.hh"

///////////////////////////////////////////////////////////////////

int
main( int argc , char* argv[] )
{

	using namespace dmc;
	using namespace simple_model;

	long state_cost;
    bool verbose;

	// parsing options of command line
	// HAS to change (considering boost::options instead)
	// but now mandatory in order to let the libdmc knwoning its parameters...
	conf::options opt(argc, argv);
	opt.add_option("cost",'c', &state_cost, 1);
	opt.add_option("verbose",'v', &verbose, false);
	opt.parse();

    // a factory needed to instanciate a model
	simple_model_factory* smf = new simple_model_factory(state_cost,verbose);

    // finally, the libdmc itself
	dmc::dmc_manager dmc(smf,&opt);

    // generation
	dmc.start_generation();

	return EXIT_SUCCESS;
}
