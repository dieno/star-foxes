#include "AIPlayer.h"

#include <math.h>

using namespace std;

void FSM::setCurrentState(EState _state)
{
   currentState = _state;   
}

int FSM::getMasterIdx() 
{
	return masterIdx;
}

string FSM::getCurrentStateName()
{
	switch(currentState){
		case FLEE:
			return "FSM // Current State: Fleeing";
			break;
		case WAND:
			return "FSM // Current State: Wandering";
			break;
		case ATCK:
			return "FSM // Current State: Attacking";
			break;
		case SEEK:
			return "FSM // Current State: Seeking";
			break;
		default:
			return "FSM // ERROR - Unable to evaluate current state";
	}
}

EState FSM::getCurrentState() 
{
	return currentState;
}

float FSM::evalDistToTarget(D3DXVECTOR3 target, D3DXVECTOR3 origin)
{
	float dist;
	D3DXVECTOR3 res;
	res = target - origin;
	dist = sqrt(res.x*res.x + res.y*res.y + res.z*res.z);
   
   res.x /= dist; // need to do this reflection first
   res.y /= dist; // Setting univector from this ship to enemy position.
   res.z /= dist;

   return dist;
}


/*
	Call appropriate evaluation function depending on current state.
*/
D3DXVECTOR3 FSM::eval()
{
	std::list<MainPlayerClass*> players;
	float shortestDist = 100; //the radar / 'sight' distance the AI can detect
	float currDist;
	int targetIdx = 0;
	D3DXVECTOR3 target;

	for (std::list<MainPlayerClass*>::const_iterator pi = players.begin(); pi != players.end(); ++pi)
	{
		//Search through enemies until one is found within range. Begin seeking target.
		if((*pi) != NULL && ((*pi)->getID() != getMasterIdx())) {
			currDist = evalDistToTarget((*pi)->getPosition(), gamestate->getPlayer(getMasterIdx()).getPosition());
			if(currDist < shortestDist) {
				shortestDist = currDist;
				targetIdx = (*pi)->getID();
				setCurrentState(SEEK);
			}
		}  		
	}

	target = gamestate->getPlayer(targetIdx).getPosition();

	/*
	switch(currentState){
		case FLEE:
			evalFlee();
			break;
		case WAND:
			evalWander();
			break;
		case ATCK:
			evalAttack();
			break;
		case SEEK:
			evalSeek();
			break;
	}
	*/
	
	return target;
}

/*
	Evaluate gamestate and determine transition (if any) when current state is FLEE. 
*/
D3DXVECTOR3 FSM::evalFlee()
{
	D3DXVECTOR3 tmp;
	return tmp;
}

/*
	Evaluate gamestate and determine transition (if any) when current state is WAND. 
*/
D3DXVECTOR3 FSM::evalWander()
{
	D3DXVECTOR3 tmp;
	return tmp;
}

/*
	Evaluate gamestate and determine transition (if any) when current state is ATCK. 
*/
D3DXVECTOR3 FSM::evalAttack()
{
	D3DXVECTOR3 tmp;
	return tmp;
}

/*
	Evaluate gamestate and determine transition (if any) when current state is SEEK. 
*/
D3DXVECTOR3 FSM::evalSeek()
{
	D3DXVECTOR3 tmp;
	return tmp;
}


//------------------------------------------------------------


// Sets the x and y bounds in which the AI can move around.
// Always set bounds of movement of AI by calling this function, otherwise it won't move.
void AIPlayer::SetBounds(D3DXVECTOR3* pos)
{
   // Bounds of movement for ship
   float xzBound = 60;
   _mv->top = xzBound;
   _mv->bottom = 5;
   _mv->left = -xzBound;
   _mv->right = xzBound;
   _mv->back = _mv->left;
   _mv->front = _mv->right;
   
   // considered when straightening the ship
   _mv->straightDownHi = _mv->top - 0.2f;
   _mv->straightDownLow = _mv->top - 0.4f;
   _mv->straightUpHi = 0.4f;
   _mv->straightUpLow = 0.2f;
}

// Heart of the AI. Updates AI behaviour.
// TODO: Behaviour evaluator to evaluate what behaviour to activate.
void AIPlayer::Update(float timeDelta)
{
	D3DXVECTOR3 target = _fsm.eval();
   HWND hWnd = _fsm.getGameState()->getHWND();
   
   if(KeepInBounds(hWnd))
   { 
      Seek(target);
      /*
	   switch(_fsm.getCurrentState())
	   {
		case FLEE:
			Flee(hWnd, target);
			break;
		case WAND:
			Wander(hWnd);
			break;
		case ATCK:
			//Attack();
			break;
		case SEEK:
			Seek(target);
			break;
	   }*/
   }

   MainPlayerClass::Update(timeDelta);
}

// Returns true if the ship is inside the bounds.
// If it's not inside the bounds, it makes the ship seek the inside of the bound area.
bool AIPlayer::KeepInBounds(HWND hWnd)
{
   
   // If going outside xz-horizontal bound, make it seek the middle of map.
   if(getPositionVector().x > _mv->right ||
      getPositionVector().x < _mv->left ||
      getPositionVector().z > _mv->front ||
      getPositionVector().z < _mv->back)
   {
      //Move(hWnd, DIR_NONE, NULL);
      Seek(D3DXVECTOR3(1, 10, 1));
      return false;
   }
   

   // If it's out of y-bottom bound, straighten up.
   if(getPositionVector().y < _mv->bottom)
   {   
      StraightenUp();
      return false;
   }

   // If it's out of y-top bound, straighten down.
   if(getPositionVector().y > _mv->top)
   {
      StraightenDown();
      return false;
   }
   
   return true;
}

// AI Behaviour: makes AI wander around randomly.
void AIPlayer::Wander(HWND hWnd)
{   
   bool thing = false;

   if(_mv->count <= 0 )
   {  
      _mv->count = std::rand() % 20;
      _mv->dir =  std::rand() % 12;
      if(_mv->dir > 8)
         _mv->dir = 8;
      if(_mv->dir == UP )
         _mv->count *= 5;
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
         this->down(true);//this->up(0.04f);
      return UP;
   case DWN: //1
         this->up(true);//this->down(0.04f);
      return DWN;
   case LFT: //2
      this->left(true);//this->left(0.05f);
      return LFT;
   case RGHT: //3
      this->right(true);//this->right(0.05f);
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
      left(false);
      right(false);
      up(false);
      down(false);
      return DIR_NONE;
   }
}

void AIPlayer::Seek(D3DXVECTOR3 enemyPos) {
   /*float x = getPositionVector().x;//getPositionX();
	float z = getPositionVector().z;
	float y = getPositionVector().y;
	float xDif = enemyPos.x - x;
   float yDif = enemyPos.y - y;
   float zDif = enemyPos.z - z;*/

     
	float dist;// = sqrt(xDif*xDif + yDif*yDif + zDif*zDif);
   D3DXVECTOR3 d;// = enemyPos - getPositionVector();
   GetMeToPosVector(&enemyPos, &d, &dist);
   /*d.x /= dist; // for some reason.. need to do this reflection first
   d.y /= dist; // Setting univector from this ship to enemy position.
   d.z /= dist;*/
   D3DXVECTOR3 d2 = d;
   float off = 0.02f;
   float angle, angle2;
   // Calculating horizontal rotation
   angle = atan2(getDirectionVector().z, getDirectionVector().x);
   Rotate2DvectorXZ(&d, -angle);
      
   angle2 = atan2(d.z, d.x) - atan2(0, 1.0f);
   if(angle2 > off)
      if(getUpVector().y > 0) // Check up vector to see if ship is upside down
         left(true);
      else
         right(true);
   else if(angle2 < -off)
      if(getUpVector().y > 0) // Check up vector to see if ship is upside down
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
   
   angle2 = atan2(d2.y, d2.z) - atan2(0, 1.0f);
   if(angle2 > off)
      down(true);
   else if(angle2 < -off)
      up(true);
   else
   {
      up(false);
      down(false);
   }
}

// Rotates the X and Z of the vector to the given angle.
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

// Rotates Z and Y of the vector given the angle.
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

/*
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

}
*/

// Straightens the ship setting it with an slight up-direction.
bool AIPlayer::StraightenUp()
{
   float off = 0.2f;
   Move(NULL, DIR_NONE, NULL);
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
      /*up(false);
      down(false);
      left(false);
      right(false);*/
      Move(NULL, DIR_NONE, NULL);
      return true;
   }
}

// Straightens the ship setting it with an slight down-direction.
bool AIPlayer::StraightenDown()
{
   float off = 0.2f;
   Move(NULL, DIR_NONE, NULL);
   if(getDirectionVector().y > 0)// || getDirectionVector().y < -0.4f)
   {
      if(getUpVector().y > 0)
         Move(NULL, DWN, NULL);
      else
         Move(NULL, UP, NULL);
      return false;
   }
   else
   {
      Move(NULL, DIR_NONE, NULL);
      /*up(false);
      down(false);
      left(false);
      right(false);*/
      return true;
   }
}

void AIPlayer::Shoot(D3DXVECTOR3 target, float* timeDelta)
{
	D3DXVECTOR3 dist;
	float fDist;
	GetMeToPosVector(&target, &dist, &fDist);

	D3DXVECTOR3	comp = getPositionVector();
    D3DXVECTOR3 diff = comp - dist;

    if(abs(diff.x) < _shootArea.x && abs(diff.y) < _shootArea.y) {
       MainPlayerClass::shoot(*timeDelta);
	}
}

// Gets a vector from this AI position to the position given in loc.
// Resulting vector 'res' is returned as an UNIVECTOR.
// Out pointer paramer 'dist' saves the distance between both locations.
void AIPlayer::GetMeToPosVector(D3DXVECTOR3* loc, D3DXVECTOR3* res, float* dist)
{
   *res = *loc - getPositionVector();
	*dist = sqrt(res->x*res->x + res->y*res->y + res->z*res->z);
   
   res->x /= *dist; // need to do this reflection first
   res->y /= *dist; // Setting univector from this ship to enemy position.
   res->z /= *dist;
}

// Initializes AI. Function usually called in constructor.
void AIPlayer::IniAI()
{
   //_mv = (PMovement) malloc (sizeof(PMovement));
   float shootarea = 0.2f;
   srand(timeGetTime());
   _mv = (Movement*) malloc (sizeof(Movement));
   _mv->dir = -1;
   _shootArea.x = shootarea;
   _shootArea.y = shootarea;
}

AIPlayer::~AIPlayer(void)
{
}
