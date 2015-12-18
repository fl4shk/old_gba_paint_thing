#ifndef misc_funcs_hpp
#define misc_funcs_hpp

#include "misc_includes.hpp"

inline void show_quit_message()
{
	cout << "Exiting....\n";
}

inline void quit_non_slot( int return_val = 0 )
{
	show_quit_message();
	qApp->quit();
	exit(return_val);
}

#endif		// misc_funcs_hpp
