#pragma once
#ifndef SOCKETEXTRAS
#define SOCKETEXTRAS
#define WM_SERVER		104
#define WM_CLIENT		105
#include "../directXClass.h"
/*#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")*/

enum ESocketEvent
{
   EV_READ,
   EV_ACCEPT,
   EV_CLOSE,
   EV_SERVER_ERR,
   EV_NONE,
   EV_0
};

enum ESocketError
{
   ERR_NONE,
   ERR_SOCK_INI,
   ERR_SOCK_CREATE,
   ERR_SOCK_BIND,
   ERR_ASYNC,
   ERR_LISTEN,
   ERR_HOST_NAME
};

#endif