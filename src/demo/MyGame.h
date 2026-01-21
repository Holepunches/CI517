#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

#include"GameObject.h"

class MyGame : public AbstractGame {
private:
	Uint64 frameStart;
	float deltaTime;

	GameObject* background;

	PhysicsObj* pc;
	PhysicsObj* blocks[20];
	PhysicsObj* circles[10];

	Vector2i velocity;

	std::vector<std::shared_ptr<GameKey>> gameKeys;

	/* GAMEPLAY */

	bool mouseLPressed = false;
	bool mouseRPressed = false;
	bool spacePressed = false;
	Point2 trajectoryPoint;
	Point2 pcPos;

	void CreateGameOjects();
	void handleKeyEvents();
	void update(float deltaTime);
	void render();
	void renderUI();
	void collisionCheck();

public:
	MyGame();
	~MyGame();
};

#endif