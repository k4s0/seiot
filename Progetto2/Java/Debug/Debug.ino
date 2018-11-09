#include "MsgService.h"
int c = 0;
void setup() {
  MsgService.init();
  
}

void loop() {
 if(c == 0){
    delay(500);
         MsgService.sendMsg("n");
         c = 1;
 }
 if (MsgService.isMsgAvailable()) {
    Msg* msg = MsgService.receiveMsg();    
    if (msg->getContent() == "j"){
       delay(500);
       c = 2;
    }
    delete msg;
  }

   if(c == 2){
    delay(3000);
         MsgService.sendMsg("w");
         delay(500);
         MsgService.sendMsg("m");
         delay(500);
         MsgService.sendMsg("r");
         delay(500);
         c = 0;
 }

 delay(5000);
}
