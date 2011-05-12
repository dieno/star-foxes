#include "AIPlayer.h"

// Sets the behaviour of the AI: Flee, attack, wander, seek.
void AIPlayer::SetBehaviour(EBehaviour beh)
{
   _behave = beh;   
}

// Sets the x and y bounds in which the AI can move around.
// Always set bounds of movement of AI by calling this function, otherwise it won't move.
void AIPlayer::SetBounds(D3DXVECTOR3 pos)
{
   _mv->top = pos.y - 0.5f;
   _mv->bottom = pos.y + 0.5f;
   _mv->left = pos.x + 2.5f;
   _mv->right = pos.x - 2.5f;
}

// Heart of the AI. Updates AI behaviour.
// TODO: Behaviour evaluator to evaluate what behaviour to activate.
void AIPlayer::Update(HWND hWnd, D3DXVECTOR3 pos)
{
   //srand((unsigned)time(0));
   //Wander(hWnd);
   switch(_behave)
   {
   case FLEE:
      Flee(hWnd, pos);
      break;
   default:
      Wander(hWnd);
      break;
   }
}

// AI Behaviour: makes AI wander around randomly.
void AIPlayer::Wander(HWND hWnd)
{   
   bool thing = false;

   if(_mv->count <= 0 )
   {  
      _mv->count = std::rand() % 50;
      _mv->dir =  std::rand() % 9;
   }

   Move(hWnd, _mv->dir, &thing);
   _mv->count--;
}

// AI Behaviour: makes the AI flee from a point in the screen.
// param pos: the location in screen to flee from.
void AIPlayer::Flee(HWND hWnd, D3DXVECTOR3 pos)
{
   float x = pos.x - getPositionX();
   float y = pos.y - getPositionY();
   float z = pos.z - getPositionZ();
   float dist = std::sqrt(x*x + y*y + z*z);

   static bool fleeing = false;

   if(dist <= 1 && _mv->count <=0)
   {
      _mv->dir = std::rand() % 9;
      _mv->count = 10;
   }

   if(_mv->count > 0)
   {
      Move(hWnd, _mv->dir, &fleeing);      
      _mv->count--;
   }
}

// Moves AI to 8 determined directions: up, down, upleft, downright, etc.
// The movement happens within bounds set with SetBounds() function.
// If the AI stops whenever it tries to go outside the bounds.
EDir AIPlayer::Move(HWND hWnd, int dir, bool *outbound)
{
   switch(dir)
   {
   case UP:      
      if(getRotation().x < .2 && getPosition().z > -1.5)
         this->bankUp(0.04f);
      return UP;
   case DWN:      
      if(getRotation().x > -.2 && getPosition().z < 1.5)
         this->bankDown(0.04f);       
      return DWN;
   case LFT:      
      this->bankLeft(0.05f);
      if(getPositionX() >= _mv->left)      
         this->bankRight(0.05f);      
      return LFT;
   case RGHT:
      this->bankRight(0.05f);
      if(getPositionX() <= _mv->right)
      {
         this->bankLeft(0.05f);
      }
      return RGHT;
   case UPRGHT:
      Move(hWnd, UP, NULL);
      Move(hWnd, RGHT, NULL);
      return UPRGHT;
   case UPLEFT:
      Move(hWnd, UP, NULL);
      Move(hWnd, LFT, NULL);
      return UPLEFT;
   case DWNRGHT:
      Move(hWnd, DWN, NULL);
      Move(hWnd, RGHT, NULL);
      return DWNRGHT;
   case DWNLFT:
      Move(hWnd, DWN, NULL);
      Move(hWnd, LFT, NULL);
      return DWNLFT;
   default:
      return DIR_NONE;
   }
}

AIPlayer::~AIPlayer(void)
{
}
