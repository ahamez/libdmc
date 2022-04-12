// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#ifndef DMC_MANAGER_H_
#define DMC_MANAGER_H_

#include <vector>

#include "fm/abstract_firing_manager.hh"
#include "rg/abstract_model_factory.hh"
#include "rg/dmc_state.hh"
#include "conf/configuration.hh"
#include "conf/options.hh"
#include "stats/stats.hh"

namespace dmc {

//////////////////////////////////////////////////////////////////////

/// The entry point of libdmc
class dmc_manager
	:
	public fm::abstract_firing_manager
{

// attributes
private:

	/// The objet giving informations on the configuration of libdmc
	conf::configuration conf_;

	///
	fm::abstract_firing_manager* fm_;

	///
    rg::abstract_model_factory* amf_;

	/// holds statistics
	stats::stats stats_;


// methods
public:

	dmc_manager( rg::abstract_model_factory* amf,
				 conf::options* opt);

	~dmc_manager();

	/// Invoked by the user to start the whole generation
	void
    start_generation() override;

    /// true if master node
    bool
    is_master_node();


private:

	void
	process_initial_state() override
	{
	};

	void
	stop_generation() override;

	// cannot copy dmc_manager
	dmc_manager(const dmc_manager&);
	const dmc_manager& operator=(const dmc_manager&);

};

//////////////////////////////////////////////////////////////////////

} // namespaces

#endif
