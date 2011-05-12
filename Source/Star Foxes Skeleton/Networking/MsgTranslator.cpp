#include "MsgTranslator.h"

MsgTranslator::MsgTranslator(void)
{
   _msg = new Msg(_szType, _szCmd, _szMsg);
}

void MsgTranslator::SetMsg(char* msg)
{
   _msg->SetMsg(msg);
}

Msg* MsgTranslator::GetMsg()
{
   return _msg;
}
MsgTranslator::~MsgTranslator(void)
{
}

int MsgTranslator::CreateMsg(char* msg, EMSG_TYPE type, ECOMMAND cmd, char *body)
{              
   msg[0] = type;
   msg[1] = cmd;
   msg[2] = body[0];
   return 3;
}

int MsgTranslator::CreateMsg(char* msg, EMSG_TYPE type, char* id, char *body)
{              
   msg[0] = type;
   msg[1] = id[0];
   msg[2] = body[0];
   return 3;
}

EMSG_TYPE MsgTranslator::TranslateMsg(char* msg)
{
   switch(msg[0])
   {
   case MSG_TXT:
      break;
   }
   return MSG_CMD;
}
//// MSG

/*char* Msg::GetBody()
{
   return _body;
}*/

Msg::Msg(char *msg, int sztype, int szid, int szbody)
{
   Initialize(msg, sztype, szid, szbody);
}

Msg::Msg(int sztype, int szid, int szbody)
{
   Initialize(NULL, sztype, szid, szbody);
}

Msg::Msg(void)
{
}

Msg::~Msg(void)
{
}

char* Msg::GetMsg()
{
   return _msg;
}

void Msg::SetMsg(char* msg)
{
   _msg = msg;
}

int Msg::CmpType(char* type)
{
   char t = _msg[_ixType];
   return strncmp(&t, type, _szType);
}

int Msg::CmpID(char* cmd)
{
   char* t = &_msg[_ixCmd];
   return strncmp(t, cmd, _szCmd);
}

int Msg::CmpBody(char* body)
{
   char* t = &_msg[_ixBody];
   return strncmp(t, body, _szBody);
}

char Msg::GetType()
{
   return _msg[0];
}

char Msg::GetCmd()
{
   return _msg[1];
}

char Msg::GetBody()
{
   return _msg[2];
}

void Msg::Initialize(char* msg, int sztype, int szcmd, int szbody)
{
   _szType = sztype;
   _szCmd = szcmd;
   _szBody = szbody;
   _ixType = 0;
   _ixCmd = _ixType + sztype;
   _ixBody = _ixCmd + szcmd;
   _msg = msg;
}