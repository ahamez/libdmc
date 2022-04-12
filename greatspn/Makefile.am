#GSPN_DIR = /data/thierry/greatspn
#GSPN_VER = `$(GSPN_DIR)/SOURCES/contrib/version.sh`
#GSPN_LIB = $(GSPN_DIR)/x86_64_R2.6.17-5mdv/2bin/LIBDMC/lib
##################################################################

bin_PROGRAMS		= 	greatspn

GSPN_INC = $(GSPN_DIR)/SOURCES/WN/INCLUDE
GSPN_LIB = $(GSPN_DIR)/$(GSPN_VER)/2bin/LIBDMC/lib

all-local : libgspndmcSRG.so

libgspndmcSRG.so : $(GSPN_LIB)/libgspndmcSRG.so
	cp $(GSPN_LIB)/libgspndmcSRG.so ./libgspndmcSRG.so

greatspn_SOURCES =	main.cc \
					gspn.cc \
					gspn.hh \
					gspn_succiter.cc \
					gspn_succiter.hh \
					gspn_fact.cc \
					gspn_fact.hh

greatspn_CPPFLAGS	=	-I$(GSPN_INC) \
						-I$(top_srcdir)/ \
						-I$(top_srcdir)/src 

greatspn_LDFLAGS		=	-lpthread \
							-lcrypto \
							-ltbb

LDADD			=	$(top_builddir)/src/libdmc.a \
					-lfl
