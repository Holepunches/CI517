#ifndef __MY_ENGINE_H__
#define __MY_ENGINE_H__


#include <string>
#include <memory>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "../EngineCommon.h"
#include "../GameMath.h"

class PhysicsObj;

static const float CONST_GRAVITY = -10000.0;
constexpr float FPS = 60.0;

class MyEngineSystem {
	friend class XCube2Engine;
	friend class PhysicsObj;

public:
	Vector2f gravity;

	MyEngineSystem();

	// Physics

	bool isColliding(PhysicsObj* obj1, PhysicsObj* obj2);
	void collisionReaction(PhysicsObj* obj1, PhysicsObj* obj2);

	float getDistance(SDL_Point p1, SDL_Point p2);

	void setGravity(float g) { gravity = { 0, g }; }
	Vector2f getGravity() { return gravity; }
};

class PhysicsObj {
	friend class MyEngineSystem;

	protected:
		SDL_Point center;  // x and y coords in SDL_Point
		SDL_Point lastCenter; // store last center for repositioning
		float width, height, halfWidth, halfHeight;  // width and height for collision
		Vector2f force;
		Vector2f velocity;
		std::string shape;
		float angle = 0;
		int blockID = 0;

	public:
		PhysicsObj();
		PhysicsObj(const SDL_Point& center, float x, float y, std::string shape);

		// Physics Application

		void applyForce(float x, float y);
		void applyGravity(float g);


		void collideWalls(int screenWidth, int screenHeight);

		// Verlet integration and move update
		void updateMovement(float dt);

		// Getters & Setters

		SDL_Point getCenter() { return center; }
		void setCenter(float x, float y) { center.x = x; center.y = y; }
		void setLastCenter(float x, float y) { lastCenter.x = x; lastCenter.y = y; }
		float getWidth() { return width; }
		float getHeight() { return height; }
		float getHalfWidth() { return halfHeight; }
		float getHalfHeight() { return halfWidth; }
		float getAngle() { return angle; }

		void setID(int i) { blockID = i; }
		int getID() { return blockID; }

		void setActive(bool a) { isActive = a; }
		void setStatic(bool a) { isStatic = a; }

		void update(float frameTime, int screenWidth, int screenHeight);

		// Movement

		Point2 launchMove(bool launch, int startX, int startY, int targetX, int targetY);

		SDL_Rect srcRect = { 0,0,0,0 };
		SDL_Rect destRect = { 0,0,0,0 };
		SDL_Texture* spriteTexture = nullptr;

		bool isGravity = true;
		float airRes = 0.95;
		bool isActive = false;
		bool isStatic = false;
};
#endif