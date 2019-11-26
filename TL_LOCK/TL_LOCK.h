/***************************/
/* author: qiufujian       */
/* date : 2018-3-16 15:37  */
/***************************/

#include <iostream>
// #include "UE.h"

using namespace std;

class TL_LOCK{

public:
  TL_LOCK();
  ~TL_LOCK(){}
  // void lock(TL_NB_UE ue, TL_NB_UE bs, TL_COAP coap);
  // void unlock(TL_NB_UE ue, TL_NB_UE bs, TL_COAP coap);
  string state();
  void lock();
  void unlock();
  void setNodeId(int id);
  int getNodeId();
  
private:
  int nodeId;
 	string lock_state;
};
