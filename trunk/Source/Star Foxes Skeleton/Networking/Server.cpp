#include "Server.h"

using namespace std;

SOCKET* Server::GetBacklist()
{
   return &ClientSocket.back();
}

bool Server::IsBinded()
{
   return _binded;
}

int Server::GenID()
{
   return nClient;
}
// Destroys/Shutdowns the server.
void Server::Destroy()
{
   shutdown(ServerSocket,SD_BOTH);
	closesocket(ServerSocket);
	WSACleanup();
}

// Returns the last client message read by the server.
const char* Server::PopFrontMsg(int* len)
{
   _tmppop = _msg.front();   
   //_tmppop.append(_nullmsg);
   *len = _msg.front().length()+1;
   _msg.pop_front();
   return _tmppop.c_str();
}

const char* Server::PeekFrontMsg()
{
   //return _msg;
   const char *msg = _msg.front().c_str();
   return msg;
}

void Server::AddMsg(char* msg)
{
   _msg.push_back(msg);
}

// Processess socket events.
ESocketEvent Server::OnSocketEvent(LPARAM lParam, WPARAM wParam, int* msgsize, HWND hWnd)
{
   static int c = 0;
	switch(WSAGETSELECTEVENT(lParam))
	{
      
		case FD_READ:
		
			//for(int n=0;n<=nMaxClients;n++)
         for(list<SOCKET>::const_iterator ci = ClientSocket.begin(); ci != ClientSocket.end(); ++ci)
			{
            //ZeroMemory(msg, 128);
				//int inDataLength=recv(ClientSocket[n], 
            int inDataLength=recv(*ci, 
					(char*)_tmpmsg,
					3,
					0);

            _tmpmsg[inDataLength] = NULL;
           /* if(c++ == 100)            
                  MessageBox(hWnd,
         _tmpmsg,
			"Error!",
			MB_ICONINFORMATION|MB_OK);
           */
				if(inDataLength != -1)
            {
               //_tmpmsg[inDataLength] = NULL;
               
               //BroadcastMsg(_tmpmsg, inDataLength);

               AddMsg(_tmpmsg);
               /*if(_tmpmsg[inDataLength] == NULL)
                  AddMsg(&_tmpmsg[inDataLength]);*/
            }
			}
         return EV_READ;
		

		case FD_CLOSE:
         nClient--;
		   return EV_CLOSE; //break;

		case FD_ACCEPT:		
			if(nClient<nMaxClients)
			{
            int size=sizeof(sockaddr);
            ClientSocket.push_back(accept(wParam,&sockAddrClient,&size));
            
				//ClientSocket[nClient]=accept(wParam,&sockAddrClient,&size);
				//if (ClientSocket[nClient]==INVALID_SOCKET)
            if (ClientSocket.back() == INVALID_SOCKET)
				{
					//int nret = WSAGetLastError();
               ClientSocket.pop_back();
					WSACleanup();
				}
            
				nClient++;
			}
		   return EV_ACCEPT;
   }
   return EV_NONE;
}

// Initializes the server connection.
ESocketError Server::IniServer(string* msg)
{
   WSADATA WsaDat;
   int nResult=WSAStartup(MAKEWORD(2,2),&WsaDat);
   if(nResult!=0)
   {
	   *msg = "Winsock initialization failed. Critical Error";
	   return ERR_SOCK_INI;
   }

   ServerSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
   if(ServerSocket==INVALID_SOCKET)
   {
	   *msg = "Socket creation failed. Critical Error";
	   return ERR_SOCK_CREATE;
   }

   SOCKADDR_IN SockAddr;
   SockAddr.sin_port=htons(nPort);
   SockAddr.sin_family=AF_INET;
   SockAddr.sin_addr.s_addr=htonl(INADDR_ANY);

   if(bind(ServerSocket,(LPSOCKADDR)&SockAddr,sizeof(SockAddr))==SOCKET_ERROR)
   {
	   *msg = "Unable to bind socket. Error";				
	   return ERR_SOCK_BIND;
   }
 /*BOOL bOptVal = TRUE;
  int bOptLen = sizeof(BOOL);
  int iOptVal;
  int iOptLen = sizeof(int);


  /*if (setsockopt(ServerSocket, SOL_SOCKET, TCP_NODELAY, (char*)&bOptVal, bOptLen) == SOCKET_ERROR) {
      /*MessageBox(hWnd,
         _tmpmsg,
			"Error!",
			MB_ICONINFORMATION|MB_OK);
     *msg = "Can't set to NO_DELAY";
     return ERR_SOCK_BIND;
  }*/

   return ERR_NONE;
}

// Synchronizes socket events to a given HWND.
// Always call this a
ESocketError Server::AsyncSelect(HWND hWnd, UINT wn, string* msg)
{
	int nResult = WSAAsyncSelect(ServerSocket,
		hWnd,
		wn,
		(FD_CLOSE|FD_ACCEPT|FD_READ));
	
   if(nResult)
	{
		*msg = "WSAAsyncSelect failed. Critical Error";
		return ERR_ASYNC;
	}

	if(listen(ServerSocket,SOMAXCONN)==SOCKET_ERROR)
	{
		*msg = "Unable to listen!. Error";
		return ERR_LISTEN;
	}
   _binded = true;
   return ERR_NONE;
}

// Constructor: initializes server with the number of max clients given.
Server::Server(int maxclients)
{
   Initialize(maxclients);
}

// Constructor: initializes server with max number of clients of 2.
Server::Server()
{   
   Initialize(2);
}

// Sends a string msg to all the clients.
void Server::BroadcastMsg(const char msg[], int size)
{
   //for(int n=0;n<=nClient;n++)
   for(list<SOCKET>::const_iterator ci = ClientSocket.begin(); ci != ClientSocket.end(); ++ci)
   {
	   //send(ClientSocket[n],msg,size,0);
      //send(ClientSocket[n],msg,strlen(msg),0);
      send(*ci, msg, 3, 0);
      //send(ClientSocket[n],_nullmsg,1,0);
   }
}

void Server::SendMsg(SOCKET* s, const char msg[], int size)
{
   send(*s, msg, size, 0);
}

// Initializes server with default values and the given max number of clients.
void Server::Initialize(int maxclients)
{
   _nullmsg[0] = NULL;
   nPort=5556;
   nClient = 0;
   ServerSocket = NULL;
   nMaxClients = maxclients;
   _binded = false;
   //ClientSocket = new SOCKET[maxclients];
}

// Destructor
Server::~Server(void)
{
}
