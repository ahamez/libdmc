// $Author: ahamez $
// $Revision: 1023 $
// $Date: 2008-03-29 00:52:26 +0100 (sam, 29 mar 2008) $

#include <getopt.h>
#include <sstream>
#include <map>
#include <iostream>
#include <cstdlib>

#include "options.hh"

#define DMC_NO_SHORT_NAME '@'

namespace dmc { namespace conf {

//////////////////////////////////////////////////////////////////////

options::options(int argc, char** argv)
	:
	args_(),
	argc_(argc),
	argv_(argv),
	help_(false),
	nb_threads_(),
	force_local_(),
	hosts_file_()
{
	add_option( "help", DMC_NO_SHORT_NAME, &help_, false);
	add_option( "threads", DMC_NO_SHORT_NAME, &nb_threads_, 1);
	add_option( "force-local", DMC_NO_SHORT_NAME, &force_local_, false);
	add_option( "hosts-file", DMC_NO_SHORT_NAME, &hosts_file_, "hosts");
}

//////////////////////////////////////////////////////////////////////

void
options::add_option( std::string long_name,
					 char short_name,
					 bool* value,
					 bool default_value,
					 std::string help)
{
	arg_t arg;
	arg.long_name = long_name;
	arg.short_name = short_name;
	arg.arg_type = DMC_OPT_BOOL;
	arg.ptr_value_bool = value;
	*(arg.ptr_value_bool) = default_value;
	arg.help = help;

	args_.push_back(arg);
}

//////////////////////////////////////////////////////////////////////

void
options::add_option( std::string long_name,
					 char short_name,
					 long* value,
					 long default_value,
					 std::string help)
{
	arg_t arg;
	arg.long_name = long_name;
	arg.short_name = short_name;
	arg.arg_type = DMC_OPT_LONG;
	arg.ptr_value_long = value;
	*(arg.ptr_value_long) = default_value;
	arg.help = help;

	args_.push_back(arg);
}

//////////////////////////////////////////////////////////////////////

void
options::add_option( std::string long_name,
					 char short_name,
					 std::string* value,
					 std::string default_value,
					 std::string help)
{
	arg_t arg;
	arg.long_name = long_name;
	arg.short_name = short_name;
	arg.arg_type = DMC_OPT_STRING;
	arg.ptr_value_string = value;
	*(arg.ptr_value_string) = default_value;
	arg.help = help;

	args_.push_back(arg);
}

//////////////////////////////////////////////////////////////////////

void
options::parse()
{
	bool not_found = false;

	auto   longopts = new option[args_.size() + 1];
	arguments::iterator it;

	std::stringstream s;
	int i;

	std::map < std::string, int* > opt_flags;
	int val = 0;

	for( it = args_.begin() ,  i = 0;
		 it != args_.end() ;
		 ++it , ++i )
	{
		option tmp;
		tmp.name = it->long_name.c_str();

		if( it->arg_type != DMC_OPT_BOOL )
		{
			tmp.has_arg = required_argument;
		}
		else
		{
			tmp.has_arg = no_argument;
		}

		if( it->short_name == DMC_NO_SHORT_NAME )
		{
			tmp.val = val;
			tmp.flag = new int;
			opt_flags[it->long_name] = tmp.flag;
			*(tmp.flag) = -1;
			++val;
		}
		else
		{
			tmp.val = it->short_name;
			s << it->short_name;
			if( tmp.has_arg == required_argument )
			{
				 s << ":";
			}
			tmp.flag = nullptr;
		}

		longopts[i] = tmp;
	}

	// marks the end of the options
	option last_opt;
	last_opt.name = nullptr;
	last_opt.has_arg = 0;
	last_opt.flag = nullptr;
	last_opt.val = 0;
	longopts[i] = last_opt;

	bool found = false;

	int ch;
	while( (ch = getopt_long(argc_,argv_,s.str().c_str(),longopts,nullptr)) != -1 )
	{
		found = false;

		for( it = args_.begin() ; it != args_.end() ; ++ it )
		{
			if( it->short_name == ch )
			{
				switch( it->arg_type )
				{
					case DMC_OPT_BOOL:
						*(it->ptr_value_bool) = true;
						break;

					case DMC_OPT_LONG:
						*(it->ptr_value_long) = strtol(optarg,nullptr,10);
						break;

					case DMC_OPT_STRING:
						*(it->ptr_value_string) = optarg;
						break;

					default:
						// can't happen
						exit(EXIT_FAILURE);
						break;
				}
				found = true;
				break;
			}
		}

		if( found )
		{
			continue;
		}

		// if not found, search in long names
		std::map < std::string, int* >::iterator map_it;
		for( map_it = opt_flags.begin() ; map_it != opt_flags.end() ; ++map_it )
		{
			if( *(map_it->second) != -1 )
			{
				for( it = args_.begin() ; it != args_.end() ; ++ it )
				{
					if( it->long_name == (*map_it).first )
					{
						switch( (*it).arg_type )
						{
							case DMC_OPT_BOOL:
								*(it->ptr_value_bool) = true;
								break;

							case DMC_OPT_LONG:
								*(it->ptr_value_long) = strtol(optarg,nullptr,10);
								break;

							case DMC_OPT_STRING:
								*(it->ptr_value_string) = optarg;
								break;

							default:
								// can't happen
								exit(EXIT_FAILURE);
								break;
						}
						*(map_it->second) = -1;
						break;
					}
				}
				found = true;
				break;
			}
		}

		if( found )
		{
			continue;
		}

		// if not found, exit
		not_found = true;
		break;
	}


	for( unsigned int n = 0 ; n < args_.size() + 1 ; ++n )
	{
		if( longopts[n].flag != nullptr )
		{
			delete longopts[n].flag;
		}
	}
	delete[] longopts;

	if( not_found )
	{
		usage();
		exit(EXIT_FAILURE);
	}

	if( help_ )
	{
		usage();
		exit(0);
	}
}

//////////////////////////////////////////////////////////////////////

void
options::usage()
{
	std::cout << std::endl << "Help for " << argv_[0]
	<< std::endl << std::endl;

	arguments::iterator it;
	for( it = args_.begin() ; it != args_.end() ; ++it )
	{
		if( (*it).short_name != DMC_NO_SHORT_NAME )
		{
			std::cout << " -" << it->short_name << ", ";
		}
		else
		{
			std::cout <<  "     ";
		}
		std::cout << "--" << it->long_name;
		switch( (*it).arg_type )
		{
			case DMC_OPT_LONG:
				std::cout << " VALUE ";
				break;

			case DMC_OPT_STRING:
				std::cout << " STRING";
				break;

			default:
				std::cout << "       ";
				break;
		}
		int padding = 20 - it->long_name.size();
		for( int i = 0 ; i < padding ; ++ i)
		{
			std::cout << " ";
		}
		std::cout << it->help << std::endl;
	}

	std::cout << std::endl;
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
