// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#include "dmc_manager.hh"
#include "base/cluster_manager.hh"
#include "base/cluster_slave.hh"
#include "base/node_manager.hh"

namespace dmc {

//////////////////////////////////////////////////////////////////////

/// Links components together
dmc_manager::dmc_manager( rg::abstract_model_factory* amf,
                          conf::options* opt)
	:
	conf_(opt), // configuration is done with this constructor
	fm_(),
	amf_(amf),
	stats_(&conf_)
{
	if( conf_.local_node_is_master() )
	{
		fm_ = new base::cluster_manager( &conf_, stats_, amf);
	}
	else
	{
		fm_ = new base::cluster_slave( &conf_, stats_, amf);
	}
}

//////////////////////////////////////////////////////////////////////

dmc_manager::~dmc_manager()
{
	delete fm_;
}

//////////////////////////////////////////////////////////////////////

void
dmc_manager::start_generation()
{
	fm_->process_initial_state();
	fm_->start_generation();
}

//////////////////////////////////////////////////////////////////////

void
dmc_manager::stop_generation()
{
	fm_->stop_generation();
}

//////////////////////////////////////////////////////////////////////

} // namespace
