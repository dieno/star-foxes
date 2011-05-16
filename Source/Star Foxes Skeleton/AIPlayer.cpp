#include "AIPlayer.h"

#include <math.h>

using namespace std;

// Sets the behaviour of the AI: Flee, attack, wander, seek.
void AIPlayer::SetBehaviour(EBehaviour beh)
{
   _behave = beh;   
}

// Sets the x and y bounds in which the AI can move around.
// Always set bounds of movement of AI by calling this function, otherwise it won't move.
void AIPlayer::SetBounds(D3DXVECTOR3 pos)
{
   _mv->top = pos.y - 4.0f;
   _mv->bottom = pos.y - 1.0f;
   _mv->left = pos.x + 3.5f;
   _mv->right = pos.x - 3.5f;
}

// Heart of the AI. Updates AI behaviour.
// TODO: Behaviour evaluator to evaluate what behaviour to activate.
void AIPlayer::Update(HWND hWnd, D3DXVECTOR3 pos)
{
   //srand((unsigned)time(0));
	//Seek(pos);
   Move(hWnd, FWRD, NULL);
   switch(_behave)
   {
   case FLEE:
      Flee(hWnd, pos);
      break;
   case SEEK:
	  Seek(pos);
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

   
   _mv->count--;
}

// AI Behaviour: makes the AI flee from a point in the screen.
// param pos: the location in screen to flee from.
void AIPlayer::Flee(HWND hWnd, D3DXVECTOR3 pos)
{
   float x = pos.x - getPositionX();
   float y = pos.y - getPositionY();
   float z = pos.z - getPositionZ();
   float dist = sqrt(x*x + y*y + z*z);

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
      if(getRotation().x < .2 && getPosition().z > _mv->top)
         this->bankUp(0.04f);
      return UP;
   case DWN:
      if(getRotation().x > -.2 && getPosition().z < _mv->bottom)
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

   case FWRD:
      this->boost(true);
      return FWRD;
   default:
      return DIR_NONE;
   }
}

void AIPlayer::Seek(D3DXVECTOR3 enemyPos) {

	float x = getPositionX();
	float z = getPositionZ();
	float y = getPositionY();
	float xDif = enemyPos.x - getPositionX();
    float yDif = enemyPos.y - getPositionY();
    float zDif = enemyPos.z - getPositionZ();

	float dist = sqrt(xDif*xDif + yDif*yDif + zDif*zDif);
	bool offset = dist >= 2;

	if((enemyPos.x > x) && (offset)) {
		this->bankLeft(0.03f);
		//directXClass::SetError(TEXT("SEEK: Moving [left] | from %f | to %f"), x, enemyPos.x);  
	} else if((enemyPos.x < x) && (offset)){
		this->bankRight(0.03f);
		//directXClass::SetError(TEXT("SEEK: Moving [right] | from %f | to %f"), x, enemyPos.x);  
	}

	if((enemyPos.z > z) && (offset)) {
		this->bankDown(0.02f);
		//directXClass::SetError(TEXT("SEEK: Moving [down] | from %f | to %f"), z, enemyPos.z);  
	} else if((enemyPos.z < z) && (offset)){
		this->bankUp(0.02f);	
		//directXClass::SetError(TEXT("SEEK: Moving [up] | from %f | to %f"), z, enemyPos.z);  
	}

}

// Initializes AI. Function usually called in constructor.
void AIPlayer::IniAI()
{
   //_mv = (PMovement) malloc (sizeof(PMovement));
   _mv = (Movement*) malloc (sizeof(Movement));
   _mv->dir = -1;
}

AIPlayer::~AIPlayer(void)
{
}
