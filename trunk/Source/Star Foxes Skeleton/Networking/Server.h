//#pragma once
//#ifndef SOCKETEXTRAS
//#define SOCKETEXTRAS
//#include "SocketExtras.h"
#include "../directXClass.h"
//#endif

#ifndef GAMESERVER
#define GAMESERVER

class Server
{
private:
   int nPort;
   int nClient;
   int nMaxClients;
   std::list<SOCKET> ClientSocket;
   SOCKET ServerSocket;
   //char szHistory[10000];
   sockaddr sockAddrClient;
   //char _msg[1024];
   std::list<std::string> _msg;
   char _clientIDs[4][128];
   char _tmpmsg[1024];
   std::string _tmppop;
   char _nullmsg[1];
   bool _binded;
public:
   //void SendMsg(char msg[]);
   void SendMsg(SOCKET* s, const char msg[], int size);
   SOCKET* GetBacklist();
   bool IsBinded();
   int GenID();
   ESocketError AsyncSelect(HWND hWnd, UINT wn, std::string* msg);
   ESocketError IniServer(std::string* msg);
   void BroadcastMsg(const char msg[], int size);
   void Destroy();
   ESocketEvent OnSocketEvent(LPARAM lParam, WPARAM wParam , int* msgsize, HWND hWnd);
   void Initialize(int maxclients);
   const char* PopFrontMsg(int* len);
   const char* PeekFrontMsg();
   void AddMsg(char* msg);
   Server(int maxclients);
   Server();
   ~Server(void);
};

#endif