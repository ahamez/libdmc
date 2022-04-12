bin_PROGRAMS		= 	big_simple

big_simple_SOURCES	=	main.cc \
						state.cc \
						state.hh \
						state_succ_iterator.cc \
						state_succ_iterator.hh \
						big_simple_model.cc \
						big_simple_model.hh \
						big_simple_model_factory.cc \
						big_simple_model_factory.hh 

big_simple_CPPFLAGS	=	-I$(top_srcdir)/src \
						-I$(top_srcdir)/test

big_simple_LDFLAGS	=	-lpthread \
						-lcrypto \
						-ltbb

LDADD				=	$(top_builddir)/src/libdmc.a
