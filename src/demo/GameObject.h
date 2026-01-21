#pragma once
// BASE GAME OBJECT CLASES

// =======================================================
// Libraries / Headers to include 
//#include "Game.h"

#include <SDL_rect.h>
#include <SDL_render.h>

// Game Settings Constants

constexpr auto GAME_TITLE = "CI517 SDL Game Example";
constexpr auto SCREEN_WIDTH = 800;
constexpr auto SCREEN_HEIGHT = 600;
constexpr auto SPRITE_SIZE = 32;
constexpr auto SPRITE_SCREEN_SIZE = 32;


class GameObject
{
public:
	GameObject();
	GameObject(const char* spriteFileName, int xPos, int yPos);

	void loadSpriteTexture(const char* spriteFileName);
	void update();
	void render();


	// Getters & Setters	
	bool getAliveState() const { return isActive; }
	void setAlive(bool newState) { isActive = newState; }
	void setSize(int width, int height);

	void setX(float newX) { x = newX; }
	void setY(float newY) { y = newY; }
	float getX() const { return x; }
	float getY() const { return y; }
	void addX(float xAmount) { x += xAmount; }
	void addY(float yAmount) { y += yAmount; }
	float getAngle()  const { return angle; }
	void setAngle(float newAngle) {  angle = newAngle; }
	void setSpeed(float newSpeed) { speed = newSpeed; }

	// Movement Limits
	float getVelX() const { return xVel; }
	float getVelY()  const { return yVel; }
	void setVelX(float newXVel) { xVel = newXVel; }
	void setVelY(float newYVel) { yVel = newYVel; }
	float getOldX() const { return oldX; }
	float getOldY() const { return oldY; }
	void stop();
	void setLastPosition();

	void screenLimit();
	void screenWrap();
	void screenBounce();
	void disableOffScreen();

	// Health 
	void drawHPBar() const;
	float getHP() const { return health; }
	void setHP(float newHP) { health = newHP; }
	void changeHP(float hpChange) { health += hpChange; }


public: // derived classes can access but nothing else
	bool isActive = false;
	float x = -100, y = -100;	 // off the screen by default
	float xVel = 0, yVel = 0;
	float speed = 100;
	float angle = 0;
	float oldX = 0, oldY = 0;

	float health = 100; // new
	float healthMax = 100;

	SDL_Rect srcRect = { 0,0,0,0 };
	SDL_Rect destRect = { 0,0,0,0 };
	SDL_Texture* spriteTexture = nullptr;
};

// =======================================================
