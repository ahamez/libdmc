lib_LIBRARIES 	= 	libdmc.a


dmcdir		=	$(pkgincludedir)
dmc_HEADERS 	=	dmc.hh

basedir		=	$(pkgincludedir)/base
base_HEADERS	=	base/abstract_node.hh \
					base/cluster_manager.hh \
					base/cluster_slave.hh \
					base/dmc_manager.hh \
					base/node_manager.hh

confdir		=	$(pkgincludedir)/conf
conf_HEADERS 	=	conf/configuration.hh \
				conf/options.hh

fmdir		=	$(pkgincludedir)/fm
fm_HEADERS	=	fm/abstract_firing_manager.hh \
				fm/firing_manager_multiple.hh \
				fm/firing_manager_thread.hh \
				fm/firing_manager_protocol.hh \
				fm/firing_manager_proxy.hh \
				fm/firing_manager_service.hh

mondir		=	$(pkgincludedir)/mon
mon_HEADERS	=	mon/activity_monitor_proxy.hh \
				mon/activity_monitor_service.hh

rgdir		=	$(pkgincludedir)/rg
rg_HEADERS 	=	rg/dmc_state.hh \
				rg/abstract_model.hh \
				rg/abstract_model_factory.hh \
				rg/abstract_state_succ_iterator.hh \
				rg/new_states_set.hh \
				rg/unicity_table.hh

smdir		=	$(pkgincludedir)/sm
sm_HEADERS	=	sm/abstract_state_manager.hh \
				sm/distributed_state_manager.hh \
				sm/state_manager.hh \
				sm/state_manager_proxy.hh \
				sm/state_manager_service.hh

utildir		=	$(pkgincludedir)/util
util_HEADERS	=	util/hash.hh

saldir		=	$(pkgincludedir)/sal
sal_HEADERS	=	sal/tcp_client.hh \
					sal/tcp_server.hh \
					sal/thread.hh

libdmc_a_SOURCES= 	\
			base/abstract_node.hh \
			base/dmc_manager.hh \
			base/dmc_manager.cc \
			base/node_manager.cc \
			base/node_manager.hh \
			base/cluster_manager.hh \
			base/cluster_manager.cc \
			base/cluster_slave.hh \
			base/cluster_slave.cc \
			fm/abstract_firing_manager.hh \
			fm/firing_manager_itf.hh \
			fm/firing_manager_multiple.cc \
			fm/firing_manager_multiple.hh \
			fm/firing_manager_thread.cc \
			fm/firing_manager_thread.hh \
			rg/new_states_set.hh \
			mon/abstract_activity_monitor.hh \
			mon/barrier_activity_monitor.hh \
			mon/barrier_condition.hh \
			mon/barrier_integer_activity_monitor \
			mon/barrier_integer_condition.hh \
			sm/state_manager_service.hh \
			sm/state_manager_service.cc \
			sm/state_manager_proxy.hh \
			sm/state_manager_proxy.cc \
			mon/activity_monitor_proxy.hh \
			mon/activity_monitor_service.hh \
			mon/activity_monitor_proxy.cc \
			mon/activity_monitor_service.cc \
			fm/firing_manager_proxy.hh \
			fm/firing_manager_service.hh \
			fm/firing_manager_proxy.cc \
			fm/firing_manager_service.cc \
			rg/dmc_state.cc \
			rg/dmc_state.hh \
			rg/abstract_model.hh \
			rg/abstract_model_factory.hh \
			rg/abstract_state_succ_iterator.hh \
			sm/state_manager_itf.hh \
			sm/state_manager.cc \
			sm/state_manager.hh \
			sm/distributed_state_manager.cc \
			sm/distributed_state_manager.hh \
			rg/unicity_table.cc \
			rg/unicity_table.hh \
			conf/configuration.hh \
			conf/configuration.cc \
			util/hash.hh \
			util/hash.cc \
			sal/tcp_server.hh \
			sal/tcp_server.cc \
			sal/tcp_client.hh \
			sal/tcp_client.cc \
			sal/thread.hh \
			conf/options.hh \
			conf/options.cc \
			stats/stats.hh \
			stats/stats.cc

libdmc_a_CPPFLAGS = \
			-I$(top_srcdir) \
			-I$(top_srcdir)/base \
			-I$(top_srcdir)/fm \
			-I$(top_srcdir)/rg \
			-I$(top_srcdir)/sm \
			-I$(top_srcdir)/util
