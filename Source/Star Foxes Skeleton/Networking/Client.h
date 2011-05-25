//#pragma once
#include "../directXClass.h"
//#include "SocketExtras.h"
#include "time.h"
//#include <std::string>
#include <stdio.h>
//#include <std::list>
//using namespace std;

#ifndef GAMECLIENT
#define GAMECLIENT

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
   std::list<char*> _msg;
   std::string _tmppop;
   char _tmpmsg[128];
   char* _name;
   //char szHistory[10000];
public:
   char* GetName();
   void PushRcvdMsg(char *);
   int getLastMsgSize();
   char* PopRcvdMsg();
   ESocketEvent OnSocketEvent(LPARAM lParam, HWND hWnd);
   void SetHost(char *hostip);
   void Shutdown();
   void SendMsg(char msg[]);
   ESocketError ConnectToHost(std::string* msg);
   ESocketError AsyncSelect(HWND hWnd, UINT wm, std::string* msg);
   ESocketError IniClientSocket(std::string* msg);
   void InitializeObject();
   Client(char *id);
   Client(void);
   ~Client(void);
};

#endif