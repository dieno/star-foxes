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
   _mv->top = 20;
   _mv->bottom = 0;
   _mv->left = -20;
   _mv->right = 20;
}

// Heart of the AI. Updates AI behaviour.
// TODO: Behaviour evaluator to evaluate what behaviour to activate.
void AIPlayer::Update(HWND hWnd, D3DXVECTOR3 pos)
{
   Move(hWnd, FWRD, NULL); // AI has constant forward movement
   //static int height;
   /*switch(_behave)
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
   }*/
   if(getPositionVector().y < 10) 
   { //Straightens ship if it gets to close to the ground.
      Straighten();
         //down(true);
   }
   else
   {
      //down(false);
      //Flee(hWnd, pos);
      //Wander(hWnd);
      Seek(pos);
   }
   //Wander(hWnd);
}
/*
bool AIPlayer::GainHeight(int* count)
{
   if(Straighten())
   {
      if(count > 0)
      {
         up(true);
         (*count)--;
      }
      else
         return true;
   }
   return false;
}
*/
// AI Behaviour: makes AI wander around randomly.
void AIPlayer::Wander(HWND hWnd)
{   
   bool thing = false;

   if(_mv->count <= 0 )
   {
      this->down(false);
      this->up(false);
      this->right(false);
      this->left(false);
      _mv->count = std::rand() % 20;
      _mv->dir =  std::rand() % 12;
      if(_mv->dir > 8)
         _mv->dir = 8;
      if(_mv->dir == FWRD )
         _mv->count *= 5;
   }   
   /*if(getPositionVector().y < 10) 
   { //Straightens ship if it gets to close to the ground.
      Straighten();
   }*/
   Move(hWnd, _mv->dir, NULL);
   _mv->count--;
}

// AI Behaviour: makes the AI flee from a point in the screen.
// param pos: the location in screen to flee from.
void AIPlayer::Flee(HWND hWnd, D3DXVECTOR3 pos)
{
   // getting vector from this ship position to enemy position.
   D3DXVECTOR3 vpos = D3DXVECTOR3(pos.x - getPositionVector().x,
      pos.y - getPositionVector().y,
      pos.z - getPositionVector().z);
   float dist = sqrt(vpos.x*vpos.x + vpos.y*vpos.y + vpos.z*vpos.z);

   //creating univector for vpos.
   vpos.x /= -dist;
   vpos.y /= dist;
   vpos.z /= dist;

   D3DXVECTOR3 vdir = getDirectionVector();
   float halfPi = 2.5f; // I know is not half pi. Radius to flee from.
   float angle = atan2(vdir.z, vdir.x);
   Rotate2DvectorXZ(&vpos, -angle);
   float angle2 = atan2(vpos.z, vpos.x) - atan2(0, 1.0f);

   //static bool fleeing = false;

   if(abs(angle2) < halfPi)
   {
      if(angle2 > 0)
         left(true);
      else
         right(true);
   }
   else
   {
      right(false);
      left(false);
      if(_mv->count <= 0)
      {
         _mv->dir = std::rand() % 6;
         _mv->count = std::rand() % 5 + 5;
         if(_mv->dir > 2)
         {
            _mv->dir = DIR_NONE;
            _mv->count *= 10;
         }
      }
      else
      {
         Move(hWnd, _mv->dir, NULL);
         _mv->count--;
      }
   }
}

// Moves AI to 8 determined directions: up, down, upleft, downright, etc.
// The movement happens within bounds set with SetBounds() function.
// If the AI stops whenever it tries to go outside the bounds.
EDir AIPlayer::Move(HWND hWnd, int dir, bool *outbound)
{
   switch(dir)
   {
   case UP: //0
      if(getPositionVector().y < _mv->top)
         this->down(true);//this->up(0.04f);
      else
      {
         this->down(false);
         this->up(true);
         _mv->dir = DWN;
      }
      return UP;
   case DWN: //1
      if(getPositionVector().y > _mv->bottom)
      //if(getPosition())
         this->up(true);//this->down(0.04f);
      else
      {
         this->down(true);
         this->up(false);
         _mv->dir = UP;
      }
      return DWN;
   case LFT: //2
      this->left(true);//this->left(0.05f);
      if(getPositionVector().x >= _mv->left)
         this->right(true);//this->right(0.05f);
      return LFT;
   case RGHT: //3
      this->right(true);//this->right(0.05f);
      if(getPositionVector().x <= _mv->right)
      {
         //this->left(0.05f);
         this->left(true);
      }
      return RGHT;
   case UPRGHT: //4
      Move(hWnd, UP, NULL);
      Move(hWnd, RGHT, NULL);
      return UPRGHT;
   case UPLEFT: //5
      Move(hWnd, UP, NULL);
      Move(hWnd, LFT, NULL);
      return UPLEFT;
   case DWNRGHT: //6
      Move(hWnd, DWN, NULL);
      Move(hWnd, RGHT, NULL);
      return DWNRGHT;
   case DWNLFT: //7
      Move(hWnd, DWN, NULL);
      Move(hWnd, LFT, NULL);
      return DWNLFT;
   case FWRD: //8
      this->boost(true);
      return FWRD;
   default:
      left(false);
      right(false);
      up(false);
      down(false);
      return DIR_NONE;
   }
}

void AIPlayer::Rotate2DvectorXZ(D3DXVECTOR3* pV2, float angle)
{
	// use local variables to find transformed components
	float Vx1 = cosf(angle)*pV2->x - sinf(angle)*pV2->z;
	float Vy1 = sinf(angle)*pV2->x + cosf(angle)*pV2->z;
	// store results thru the pointer
	pV2->x = Vx1;
	pV2->z = Vy1;

	return;
}

void AIPlayer::Rotate2DvectorYZ(D3DXVECTOR3* pV2, float angle)
{
	// use local variables to find transformed components
	float Vx1 = cosf(angle)*pV2->z - sinf(angle)*pV2->y;
	float Vy1 = sinf(angle)*pV2->z + cosf(angle)*pV2->y;
	// store results thru the pointer
	pV2->z = Vx1;
	pV2->y = Vy1;

	return;
}

void AIPlayer::Seek(D3DXVECTOR3 enemyPos) {
   float x = getPositionVector().x;//getPositionX();
	float z = getPositionVector().z;
	float y = getPositionVector().y;
	float xDif = enemyPos.x - x;
   float yDif = enemyPos.y - y;
   float zDif = enemyPos.z - z;
  
	float dist = sqrt(xDif*xDif + yDif*yDif + zDif*zDif);
   D3DXVECTOR3 d = enemyPos - getPositionVector();
   d.x /= -dist; // for some reason.. need to do this reflection first
   d.y /= dist; // Setting univector from this ship to enemy position.
   d.z /= dist;
   D3DXVECTOR3 d2 = d;
   float off = 0.2f;
   float angle, angle2;
   // Calculating horizontal rotation
   angle = atan2(getDirectionVector().z, getDirectionVector().x);
   Rotate2DvectorXZ(&d, -angle);
      
   angle2 = atan2(0, 1.0f) - atan2(d.z, d.x);
   if(angle2 > off)
      if(getUpVector().y > 0)
         left(true);
      else
         right(true);
   else if(angle2 < -off)
      if(getUpVector().y > 0)
         right(true);
      else 
         left(true);
   else
   {
      left(false);
      right(false);
   }
   
   //calculating vertical rotation
   d2.z = d2.z;
   d2.y = -d2.y;
   angle = atan2(getDirectionVector().y, getDirectionVector().z);
   Rotate2DvectorYZ(&d2, angle);
   
   angle2 = atan2(0, 1.0f) - atan2(d2.y, d2.z);
   if(angle2 > off)
      up(true);
   else if(angle2 < -off)
      down(true);
   else
   {
      up(false);
      down(false);
   }
}


void AIPlayer::SeekIra(D3DXVECTOR3 enemyPos) {

   float x = getPositionVector().x;//getPositionX();
	float z = getPositionVector().z;
	float y = getPositionVector().y;
	float xDif = enemyPos.x - x;
   float yDif = enemyPos.y - y;
   float zDif = enemyPos.z - z;

	float dist = sqrt(xDif*xDif + yDif*yDif + zDif*zDif);
	bool offset = true; //dist >= 1;
   //this->up(true);
   
	if((enemyPos.x < x) && (offset)) {
		this->left(true);//this->bankLeft(0.03f);
      //this->right(false);
		//directXClass::SetError(TEXT("SEEK: Moving [left] | from %f | to %f"), x, enemyPos.x);  
	} else if((enemyPos.x > x) && (offset)){
		this->right(true); //this->bankRight(0.03f);
      //this->left(false);
		//directXClass::SetError(TEXT("SEEK: Moving [right] | from %f | to %f"), x, enemyPos.x);  
	}
   
	if((enemyPos.y > y) && (offset)) {
		this->down(true);//this->bankDown(0.02f);
      //this->up(false);
		//directXClass::SetError(TEXT("SEEK: Moving [down] | from %f | to %f"), z, enemyPos.z);  
	} else if((enemyPos.y < y) && (offset)){
		this->up(true);	//this->bankUp(0.02f);	
		//this->down(false);
      //directXClass::SetError(TEXT("SEEK: Moving [up] | from %f | to %f"), z, enemyPos.z);  
	}

}

bool AIPlayer::Straighten()
{
   float off = 0.2f;
   if(getDirectionVector().y > 0.4f || getDirectionVector().y < off)
   {
      if(getUpVector().y > 0)
         Move(NULL, UP, NULL);
      else
         Move(NULL, DWN, NULL);
      return false;
   }
   else
   {
      up(false);
      down(false);
      left(false);
      right(false);
      return true;
   }
}

// Initializes AI. Function usually called in constructor.
void AIPlayer::IniAI()
{
   //_mv = (PMovement) malloc (sizeof(PMovement));
   srand(timeGetTime());
   _mv = (Movement*) malloc (sizeof(Movement));
   _mv->dir = -1;
}

AIPlayer::~AIPlayer(void)
{
}
