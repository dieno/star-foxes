#include "Client.h"

char* Client::GetName()
{
   return _name;
}

// Returns the last msg sent by this client.
char* Client::PopRcvdMsg()
{
   //return szIncoming;
   char* msg = _msg.front();
   _msg.pop_front();
   return msg;
}

// Returns the size of the last msg sent by this client.
int Client::getLastMsgSize()
{
   return szSize;
}

void Client::PushRcvdMsg(char * msg)
{
   _msg.push_back(msg);
}

void Client::SetHost(char *hostip)
{
      szServer = hostip;
}

// Processes the socket events for this client.
ESocketEvent Client::OnSocketEvent(LPARAM lParam)
{
	if(WSAGETSELECTERROR(lParam))
		return EV_SERVER_ERR;

	switch(WSAGETSELECTEVENT(lParam))
	{
		case FD_READ:
		{
			int inDataLength=recv(Socket,
				(char*)_tmpmsg,
				3,
				0);

         //_tmpmsg[inDataLength] = NULL;
         //_tmpmsg[inDataLength+1] = NULL;
         szSize = inDataLength;
         PushRcvdMsg(_tmpmsg);
         return EV_READ;
		}

		case FD_CLOSE:		
         return EV_CLOSE;
	}
   return EV_NONE;
}

// Sends a message to the server.
void Client::SendMsg(char msg[])
{
   //if(strlen(msg) > 0)
   {   /*
      string str = _id;
      str.append(": ");
      str.append(msg);
      const char * m = str.c_str();
      send(Socket,m,strlen(m),0);
      */

      //int len = strlen(a);*/
      //a[len] = NULL;
      
      //send(Socket,msg, strlen(msg), 0);
      send(Socket,msg, 3, 0);
      //Sleep(1);
      //send(Socket,"hi", 1, 0);
   }
}

// Shuts down client and cleans resources.
void Client::Shutdown()
{
   shutdown(Socket,SD_BOTH);
	closesocket(Socket);
	WSACleanup();
}

// Initializes the socket for the client.
ESocketError Client::IniClientSocket(string* msg)
{
   WSADATA WsaDat;
   int nResult=WSAStartup(MAKEWORD(2,2),&WsaDat);
   if(nResult!=0)
   {
      *msg = "Winsock initialization failed. Critical Error";
	   return ERR_SOCK_INI;
   }

   Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
   if(Socket==INVALID_SOCKET)
   {
      *msg = "Socket creation failed. Critical Error";
	   return ERR_SOCK_CREATE;
   }

   return ERR_NONE;
}

// Attaches the Socket events of this client to the respective window HWND.
ESocketError Client::AsyncSelect(HWND hWnd, UINT wm, string* msg)
{
   int nResult=WSAAsyncSelect(Socket, hWnd, wm, (FD_CLOSE|FD_READ));
   if(nResult)
   {
      *msg = "WSAAsyncSelect failed";
	   return ERR_ASYNC;
   }

   return ERR_NONE;
}

// Connects the client to the host.
ESocketError Client::ConnectToHost(string *msg)
{
	// Resolve IP address for hostname
	struct hostent *host;
	if((host=gethostbyname(szServer))==NULL)
	{
      *msg = "Unable to resolve host name";
		return ERR_HOST_NAME;
	}

	// Set up our socket address structure
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port=htons(nPort);
	SockAddr.sin_family=AF_INET;
	SockAddr.sin_addr.s_addr=*((unsigned long*)host->h_addr);

	connect(Socket,(LPSOCKADDR)(&SockAddr),sizeof(SockAddr));

   return ERR_NONE;
}

// Initializes the server object with default values.
void Client::InitializeObject()
{
   _name = "default";
   _c = 100;
   nPort = 5556;
   szServer = "localhost";
   Socket = NULL;
   szSize = 0;
   ZeroMemory(_tmpmsg,sizeof(_tmpmsg));
}

// Constructor: initializes with default values.
Client::Client(char* id)
{
   _id = id;
   InitializeObject();
}

Client::Client(void)
{  
   //char* name = (char*)malloc(1024);
   //char* name2 = (char*)malloc(1024);
   _id = (char*) malloc(128);
   itoa(rand() % 100 + 1, _id, time(NULL));
   //_id = "me";
   InitializeObject();
}

// Destructor
Client::~Client(void)
{
}
