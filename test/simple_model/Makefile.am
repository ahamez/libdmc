bin_PROGRAMS		= 	simple

simple_SOURCES		=	main.cc \
						state.cc \
						state.hh \
						state_succ_iterator.cc \
						state_succ_iterator.hh \
						simple_model.cc \
						simple_model.hh \
						simple_model_factory.cc \
						simple_model_factory.hh

simple_CPPFLAGS		=	-I$(top_srcdir)/src \
						-I$(top_srcdir)/test

simple_LDFLAGS		=	-lpthread \
						-lcrypto \
						-ltbb

LDADD				=	$(top_builddir)/src/libdmc.a
