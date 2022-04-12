// $Author: ahamez $
// $Revision: 702 $
// $Date: 2006-08-28 11:03:34 +0200 (Mon, 28 Aug 2006) $

#ifndef DMC_ABSTRACT_ACTIVITY_MONITOR_HH_
#define DMC_ABSTRACT_ACTIVITY_MONITOR_HH_

namespace dmc { namespace mon {

/////////////////////////////////////////////////////////////////////
	
class abstract_activity_monitor
{
	
// methods
public:
	
	virtual ~abstract_activity_monitor(){};
	
	/// Notify activity to the monitor
	virtual void notify_active() = 0;
	/// Notify inactivity to the monitor
	virtual void notify_inactive() = 0;
	
};
	
//////////////////////////////////////////////////////////////////////
	
}} // namespaces

#endif
