// ======================================================= 
// Libraries / Headers to include 
#include "GameObject.h"
#include <SDL.h>



// BASE GAME OBJECT CLASS
// ======================================================= 


//  Movement Methods
// ======================================================= 
void GameObject::screenLimit()
{
	// Limit to edges
	bool stopMoving = false;
	if (x > SCREEN_WIDTH - SPRITE_SIZE)
	{
		stopMoving = true;
		x = SCREEN_WIDTH - SPRITE_SIZE;
	}
	if (x < 0)
	{
		stopMoving = true;
		x = 0;
	}
	if (y > SCREEN_HEIGHT - SPRITE_SIZE)
	{
		stopMoving = true;
		y = SCREEN_HEIGHT - SPRITE_SIZE;
	}
	if (y < 0)
	{
		stopMoving = true;
		y = 0;
	}

	if (stopMoving)
	{
		xVel = 0;
		yVel = 0;
	}
}//---

void GameObject::screenBounce()
{
	// bounce of edges by reversing velocity
	if (x > SCREEN_WIDTH - SPRITE_SIZE)  xVel = -xVel;
	if (x < 0) xVel = -xVel;
	if (y > SCREEN_HEIGHT - SPRITE_SIZE) yVel = -yVel;
	if (y < 0) yVel = -yVel;
}//---

void GameObject::screenWrap()
{
	// Screen Wrap to opposite side if sprite leaves screen
	if (x > SCREEN_WIDTH - SPRITE_SIZE)  x = 0;
	if (x < 0) x = SCREEN_WIDTH - SPRITE_SIZE;
	if (y > SCREEN_HEIGHT - SPRITE_SIZE) y = 0;
	if (y < 0) y = SCREEN_HEIGHT - SPRITE_SIZE;
}//---


void GameObject::disableOffScreen()
{
	// disable if sprite leave the screen area
	if (x > SCREEN_WIDTH || x < 0 || y > SCREEN_HEIGHT || y < 0)
	{
		isActive = false;
	}
}//---


void GameObject::stop()
{
	xVel = yVel = 0;
	x = oldX;
	y = oldY;
}//--


void GameObject::setLastPosition()
{
	x = oldX;
	y = oldY;
}//--


// ======================================================= 
void GameObject::update()
{	// update the display Rectangle Position
	destRect.x = x;
	destRect.y = y;
}//---


// ======================================================= 

GameObject::GameObject() {} // default Constructor; 


GameObject::GameObject(const char* spriteFileName, int xPos, int yPos)
{
	x = float(xPos); 	y = float(yPos);
	loadSpriteTexture(spriteFileName);
}//---

// ======================================================= 
void GameObject::setSize(int width, int height) // if sprite is non standard size
{
	// The source file
	srcRect.w = float(width);
	srcRect.h = float(height);
	// Destination Screen display 
	destRect.w = float(width) * SPRITE_SCREEN_SIZE / SPRITE_SIZE;
	destRect.h = float(height) * SPRITE_SCREEN_SIZE / SPRITE_SIZE;
}//----



// ======================================================= 

void GameObject::loadSpriteTexture(const char* spriteFileName)
{
	// Load Image from File to create the sprite
	//SDL_Surface* tempSurface = IMG_Load(spriteFileName);

	//spriteTexture = SDL_CreateTextureFromSurface(SDL_Renderer* renderer, tempSurface);

	//if (spriteTexture == NULL) std::cout <<" Image Load Error  - SDL Error: %s\n" << SDL_GetError();

	//SDL_FreeSurface(tempSurface);
	// Which part of the file to display
	srcRect.h = SPRITE_SIZE;
	srcRect.w = SPRITE_SIZE;
	srcRect.x = 0;
	srcRect.y = 0;

	// Where to display the Sprite on screen
	destRect.h = SPRITE_SCREEN_SIZE;
	destRect.w = SPRITE_SCREEN_SIZE;
	destRect.x = x;
	destRect.y = y;
}//---




// ======================================================= 
void GameObject::render()
{
	// add the Sprite to the Render Image
	//SDL_RenderCopy(Game::renderer, spriteTexture, &srcRect, &destRect);
}//---


// ======================================================= 
void GameObject::drawHPBar() const 
{
	SDL_Rect hpBar{ 0,0,0,0 };
	// Draw HP Bar
	hpBar.w = SPRITE_SIZE * health / 100; ;
	hpBar.h = 2;
	hpBar.x = x;
	hpBar.y = y - 6;

	//// Colour of HP Bar
	//if (health >= 50) // Green
	//	SDL_SetRenderDrawColor(Game::renderer, 0, 200, 00, 255);

	//else if (health >= 25 && health < 50) // Yellow
	//	SDL_SetRenderDrawColor(Game::renderer, 250, 250, 0, 255);

	//else // Red
	//	SDL_SetRenderDrawColor(Game::renderer, 250, 0, 0, 255);

	//SDL_RenderDrawRect(Game::renderer, &hpBar);
}//---