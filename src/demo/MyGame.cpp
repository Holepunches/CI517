#include "MyGame.h"



MyGame::MyGame() : AbstractGame(), score(0), lives(3), numKeys(15), gameWon(false), box(15, 15, 30, 30)
{

	TTF_Font* font = ResourceManager::loadFont("res/fonts/arial.ttf", 28);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	background = new GameObject("res/images/background.png", 0, 0);
	background->spriteTexture = ResourceManager::loadTexture("res/images/background.png", SDL_COLOR_GREEN);
	background->setSize(800, 600);
	CreateGameOjects();
}

void MyGame::CreateGameOjects()
{	
	SDL_Point start;
	start.x = 200;
	start.y = 584;
	SDL_Point start2;
	start2.x = 200;
	start2.y = 300;

	// create pc physics object
	pc = new PhysicsObj(start2, 32, 32, "circle");
	pc->spriteTexture = ResourceManager::loadTexture("res/images/Pawn_Blue.png", SDL_COLOR_BLUE);  // load texture for phys obj
	pc->setID(-1);
    pc->setActive(true);

	// Create all squares
	for (int i = 0; i < sizeof(blocks) / sizeof(blocks[0]); i++)
	{
		if (i <= 1) {
			blocks[i] = new PhysicsObj(start, 32, 32, "square");
			blocks[i]->spriteTexture = ResourceManager::loadTexture("res/images/crate.png", SDL_COLOR_GRAY);
			blocks[i]->setActive(true);
			blocks[i]->setStatic(true);
			start.y -= 32;
		}
		else {
			start.x = -100;
			blocks[i] = new PhysicsObj(start, 32, 32, "square");
			blocks[i]->spriteTexture = ResourceManager::loadTexture("res/images/crate.png", SDL_COLOR_GRAY);
			blocks[i]->setActive(false);

		}
		blocks[i]->setID(i);
	}

	// Create all Circles
	for (int i = 0; i < sizeof(circles) / sizeof(circles[0]); i++) {
		circles[i] = new PhysicsObj(start, 32, 32, "circle");
		circles[i]->spriteTexture = ResourceManager::loadTexture("res/images/tyre.png", SDL_COLOR_GRAY);
		circles[i]->setActive(false);
		circles[i]->setID(i);
	}

}


MyGame::~MyGame() {

}

void MyGame::handleKeyEvents() {
	
	if (eventSystem->isPressed(BTN_LEFT)) {
		trajectoryPoint = pc->launchMove(false, pc->getCenter().x, pc->getCenter().y, eventSystem->getMousePos().x, eventSystem->getMousePos().y);
		mouseLPressed = true;
	}
	if (!eventSystem->isPressed(BTN_LEFT) && mouseLPressed) {
		pc->launchMove(true, pc->getCenter().x, pc->getCenter().y, eventSystem->getMousePos().x, eventSystem->getMousePos().y);
		mouseLPressed = false;
	}
	if (eventSystem->isPressed(BTN_RIGHT) && !mouseRPressed) {
		float startX = eventSystem->getMousePos().x;
		float startY = eventSystem->getMousePos().y;
		for (PhysicsObj* block : blocks) {
			if (!block->isActive) {
				block->setCenter(startX, startY);
				block->setLastCenter(startX, startY);
				block->setActive(true);
				break;
			}
		}
		mouseRPressed = true;
	}
	if (!eventSystem->isPressed(BTN_RIGHT) && mouseRPressed) mouseRPressed = false;
	
	if (eventSystem->isPressed(Key::SPACE) && !spacePressed) {
		float startX = eventSystem->getMousePos().x;
		float startY = eventSystem->getMousePos().y;
		for (PhysicsObj* circle : circles) {
			if (!circle->isActive) {
				circle->setCenter(startX, startY);
				circle->setLastCenter(startX, startY);
				circle->setActive(true);
				break;
			}
		}
		spacePressed = true;
	}
	if (!eventSystem->isPressed(Key::SPACE) && spacePressed) spacePressed = false;
}

void MyGame::update(float deltaTime) {
	if (deltaTime >= 0.1) deltaTime = 0.016;
	pc->update(deltaTime, SCREEN_WIDTH, SCREEN_HEIGHT);
	for (PhysicsObj* block : blocks) {
		block->update(deltaTime, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	for (PhysicsObj* circle : circles) {
		circle->update(deltaTime, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	pcPos.x = pc->getCenter().x;
	pcPos.y = pc->getCenter().y;
	collisionCheck();
}

void MyGame::collisionCheck() {
	for (PhysicsObj* block : blocks) {
		if (!block->isActive) continue;
		if (mySystem->isColliding(pc, block)) mySystem->collisionReaction(pc, block);
		for (PhysicsObj* block2 : blocks) { 
			if (!block2->isActive) continue;
			if (block->getID() == block2->getID()) continue;
			if (block->getID() > block2->getID()) continue;// check if already collided with block
			if (mySystem->isColliding(block, block2)) mySystem->collisionReaction(block, block2);
		}
		for (PhysicsObj* circle : circles) {
			if (!circle->isActive) continue;
			if (mySystem->isColliding(block, circle)) mySystem->collisionReaction(block, circle);
		}
	}

	for (PhysicsObj* circle : circles) {
		if (!circle->isActive) continue;
		if (mySystem->isColliding(pc, circle)) mySystem->collisionReaction(pc, circle);
		for (PhysicsObj* circle2 : circles) {
			if (!circle2->isActive) continue;
			if (circle->getID() == circle2->getID()) continue;
			if (circle->getID() > circle2->getID()) continue;
			if (mySystem->isColliding(circle, circle2)) mySystem->collisionReaction(circle, circle2);
		}
	}

	
}


void MyGame::render() {
	gfx->setDrawColor(SDL_COLOR_AQUA);
	gfx->drawTexture(background->spriteTexture, &background->srcRect, &background->destRect);
	gfx->setDrawColor(SDL_COLOR_PURPLE);
	gfx->drawTexture(pc->spriteTexture, &pc->srcRect, &pc->destRect);
	gfx->setDrawColor(SDL_COLOR_GRAY);
	for (PhysicsObj* block : blocks) {
		gfx->drawTexture(block->spriteTexture, &block->srcRect, &block->destRect, block->getAngle(), &block->getCenter());
	}
	for (PhysicsObj* circle : circles) {
		gfx->drawTexture(circle->spriteTexture, &circle->srcRect, &circle->destRect);
	}
	gfx->setDrawColor(SDL_COLOR_WHITE);
	if (mouseLPressed) gfx->drawLine(pcPos, trajectoryPoint);
}

void MyGame::renderUI() {
	gfx->setDrawColor(SDL_COLOR_WHITE);
	std::string controlStr = "Left Click to Launch PC";
	std::string controlStr2 = "Right Click to Spawn Squares";
	std::string controlStr3 = "Space to Spawn Circles";
	gfx->drawText(controlStr, 25, 25);
	gfx->drawText(controlStr2, 25, 50);
	gfx->drawText(controlStr3, 25, 75);
}