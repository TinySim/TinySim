#include "TL_SIM_time_util.h"

#include <string>

using namespace std;

const int TL_SIM_time_util::S_STATE_NUM = 5;
const int TL_SIM_time_util::S_PKT_SEND = 0;
const int TL_SIM_time_util::S_PKT_RECEIVED = 1;
const int TL_SIM_time_util::S_PKT_PEND = 2;

const int TL_SIM_time_util::Mobile_Send_Request = 3;
const int TL_SIM_time_util::LOCK_OPEN = 4;

string TL_SIM_time_util::state_name[] = {
						   "pktsend", "pktreceive", "pktpend",
                           "Mobile_Send_Request","OPEN_LOCK"
                       };

string TL_SIM_time_util::TIME_CHANNEL = "TIME_CHANNEL";
string TL_SIM_time_util::MY_TIME_CHANNEL = "MY_TIME_CHANNEL";

string TL_SIM_time_util::my_time_regex = R"(TIME:(.+), NODE:(.+), STATE:(.+)\.)";
string TL_SIM_time_util::time_regex = R"(TIME:(.+), NODE:(.+), STATE:(.+), TXNUM:(.+)\.)";

string TL_SIM_time_util::time_regex_muti = R"(TIME:(.+), SRCNODE:(.+), DSTNODE:(.+), STATE:(.+), TXNUM:(.+)\.)";











