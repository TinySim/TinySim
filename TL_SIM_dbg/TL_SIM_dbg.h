#ifndef TL_SIM_DBG_H_INCLUDED
#define TL_SIM_DBG_H_INCLUDED 


#include <vector>
#include <string>

using namespace std;

class TL_SIM_dbg
{
public:
	TL_SIM_dbg(){}
	~TL_SIM_dbg(){}


	// static vector<string> dbg_filter(const string& path, const string& regStr);
	static void dbg_filter(const string& path, const string& regStr, vector<string>& outp) ;
	static void dbg(string channel, const char* format, ...);
	static void dbg_add_channel(string channel);
	static void dbg_remove_channel(string channel);
	static string getTinySimTime();
} ;

#endif
