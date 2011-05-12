#pragma once
#include "../directXClass.h"
//#include "SocketExtras.h"
#include "time.h"
//#include <string>
#include <stdio.h>
//#include <list>
using namespace std;

class Client
{
private:
   int nPort;
   char *szServer;
   SOCKET Socket;
   //char szIncoming[1024];
   int szSize;
   char* _id;
   int _c;
   list<char*> _msg;
   string _tmppop;
   char _tmpmsg[128];
   char* _name;
   //char szHistory[10000];
public:
   char* GetName();
   void PushRcvdMsg(char *);
   int getLastMsgSize();
   char* PopRcvdMsg();
   ESocketEvent OnSocketEvent(LPARAM lParam);
   void SetHost(char *hostip);
   void Shutdown();
   void SendMsg(char msg[]);
   ESocketError ConnectToHost(string* msg);
   ESocketError AsyncSelect(HWND hWnd, UINT wm, string* msg);
   ESocketError IniClientSocket(string* msg);
   void InitializeObject();
   Client(char *id);
   Client(void);
   ~Client(void);
};