// $Author: ahamez $
// $Revision: 971 $
// $Date: 2007-06-14 11:30:18 +0200 (jeu, 14 jun 2007) $

#ifndef DMC_OPTIONS_HH_
#define DMC_OPTIONS_HH_

#include <string>
#include <vector>

namespace dmc { namespace conf {

	typedef enum
	{
		DMC_OPT_BOOL,
		DMC_OPT_LONG,
		DMC_OPT_STRING
	} arg_type_t ;

	typedef struct
	{
		std::string long_name;
		char short_name;
		arg_type_t arg_type;
		bool* ptr_value_bool;
		long* ptr_value_long;
		std::string* ptr_value_string;
		std::string help;
	} arg_t;

	typedef std::vector<arg_t> arguments;

//////////////////////////////////////////////////////////////////////

class options
{

// attributes
private:

	/// Holds the default args
	arguments args_;

	/// The information from the command line
	int argc_; char** argv_;

	bool help_;

	long nb_threads_;

	bool force_local_;

	std::string hosts_file_;


// methods
public:

	options( int argc, char** argv);

	void add_option( std::string long_name,
					 char short_name,
					 bool* value,
					 bool default_value,
					 std::string help = "No help available.");
	void add_option( std::string long_name,
					 char short_name,
					 long* value,
					 long default_value,
					 std::string help = "No help available.");
	void add_option( std::string long_name,
					 char short_name,
					 std::string* value,
					 std::string default_value,
					 std::string help = "No help available.");
	void parse();

	void usage();

private:

	// cannot copy options
	options(const options&);

	const options&
	operator=(const options&);

	friend class configuration;
};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
