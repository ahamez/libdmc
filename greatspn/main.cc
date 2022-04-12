// $Author: ahamez $
// $Revision: 1022 $
// $Date: 2008-03-28 22:08:39 +0100 (Fri, 28 Mar 2008) $

#include <iostream>
#include <sstream>
#include <fstream>

#include "dmc.hh"

#include "greatspn/gspn.hh"
#include "greatspn/gspn_fact.hh"

using namespace std;

int 
main( int argc , char* argv[] )
{

	using namespace dmc;
	
	std::string file_name;
	bool compute_concretes;

	// parsing options of command line
	// HAS to change (considering boost::options instead)
	// but now mandatory in order to let the libdmc knwoning its parameters...	
	util::options opt(argc,argv);
	opt.add_option("file",'f',&file_name,"simple.net");
	opt.add_option("concretes",'c',&compute_concretes,false);
	opt.parse();

	// a factory to build the model of gspn
	greatspn::gspn_model_factory* gmf = new greatspn::gspn_model_factory();

	// the model to be read
	gmf->set_options(file_name);

	greatspn::greatspn* model = (greatspn::greatspn*) gmf->create_model();

	// the object to manage the generation
	dmc::dmc_manager gen(gmf,&opt);

	// start gneration of the state space
	gen.start_generation();

	return EXIT_SUCCESS;
}
