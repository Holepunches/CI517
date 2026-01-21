#include "MyEngineSystem.h"

// -------------------------
// ----- Engine System -----
// -------------------------

MyEngineSystem::MyEngineSystem() {
	gravity.y = CONST_GRAVITY;
}

// Physics

bool MyEngineSystem::isColliding(PhysicsObj* obj1, PhysicsObj* obj2) {
	SDL_Rect selfRect = { obj1->center.x, obj1->center.y, obj1->width, obj1->height };
	SDL_Rect otherRect = { obj2->center.x, obj2->center.y, obj2->width, obj2->height };

	return SDL_HasIntersection(&selfRect, &otherRect);
}

void MyEngineSystem::collisionReaction(PhysicsObj* obj1, PhysicsObj* obj2) {
	float distance = 0;
	float dx = 0;
	float dy = 0;
	int objPush = 0;
	float objMult = 1;

	// get velocities of each object
	float obj1V = sqrt((obj1->velocity.x * obj1->velocity.x) + (obj1->velocity.y * obj1->velocity.y));
	float obj2V = sqrt((obj2->velocity.x * obj2->velocity.x) + (obj2->velocity.y * obj2->velocity.y));

	// apply a certain amount of force to each object depending on velocities (higher the velocity, higher the force)
	if (obj1V > obj2V) {
		objPush = obj1V / 2;
		if (objPush < 2) objPush = 0;
		objMult = 34 - obj1V;
	}
	else if (obj2V > obj1V) {
		objPush = obj2V / 2;
		if (objPush < 2) objPush = 0;
		objMult = 34 - obj2V;
	}

	distance = getDistance(obj1->center, obj2->center);
	if (!obj1->isActive) return;
	// check if distance between circles is less than sum of radii
	if (obj1->shape == "circle" && obj2->shape == "circle") {
		if (distance < obj1->halfWidth + obj2->halfWidth) {
			dx = obj2->center.x - obj1->center.x;
			dy = obj2->center.y - obj1->center.y;
			obj1->center.x -= dx / objMult;
			obj1->center.y = obj1->lastCenter.y + objPush;
			obj2->center.x += dx / objMult;
			obj2->center.y = obj2->lastCenter.y - objPush;
		}
	}

	// collision reactions for two squares
	if (obj1->shape == "square" && obj2->shape == "square") {
		dx = obj2->center.x - obj1->center.x;
		dy = obj2->center.y - obj1->center.y;
		if (obj1->center.y < obj2->center.y - obj2->halfHeight || obj2->center.y < obj1->center.y - obj1->halfHeight) {
			obj1->center.y = obj1->lastCenter.y;
			obj2->center.y = obj2->lastCenter.y;
		}
		else if (obj1->center.x < obj2->center.x - obj2->halfWidth || obj2->center.x < obj1->center.x - obj1->halfWidth) {
			if (obj1->center.y < obj2->center.y - obj2->halfHeight);
			if (obj2->center.y < obj1->center.y - obj1->halfHeight);
			else {
				obj1->center.x -= dx / 10;
				obj2->center.x += dx / 10;
			}
		}
	}
	
	// collisions for square and circle
	if (obj1->shape == "square" && obj2->shape == "circle" || obj1->shape == "circle" && obj2->shape == "square") {
		PhysicsObj* square;
		PhysicsObj* circle;
		if (obj1->shape == "square") {
			square = obj1;
			circle = obj2;
		}
		else {
			square = obj2;
			circle = obj1;
		}
		dx = square->center.x - circle->center.x;
		dy = square->center.y - circle->center.y;
		if (circle->center.x > square->center.x - square->halfWidth
			&& circle->center.y < square->center.y - square->halfHeight
			|| circle->center.x < square->center.x + square->halfWidth
			&& circle->center.y < square->center.y - square->halfHeight);
		else {
			circle->center.x -= dx / 5;
			square->center.x += dx / 10;
		}
		circle->center.y = circle->lastCenter.y;
		square->center.y = square->lastCenter.y;
	}
}

float MyEngineSystem::getDistance(SDL_Point p1, SDL_Point p2) {
	float distance = 0;
	float dy = p2.y - p1.y;
	float dx = p2.x - p1.x;
	distance = sqrt((dy * dy) + (dx * dx)); // pythagoras

	return distance;
}


// ------------------------------
// ------- Physics Object -------
// ------------------------------

PhysicsObj::PhysicsObj() {} // default constructor
PhysicsObj::PhysicsObj(const SDL_Point& center, float x, float y, std::string shape) : center(center), lastCenter(center), width(x), height(x), halfWidth(x / 2), halfHeight(x / 2), force(0.0f, 0.0f), shape(shape) {
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = width;
	srcRect.h = height;

	destRect.x = center.x - halfWidth;
	destRect.y = center.y - halfHeight;
	destRect.w = width;
	destRect.h = height;
}

void PhysicsObj::applyForce(float x, float y) {
	force.x += x;
	force.y -= y;
}

void PhysicsObj::applyGravity(float g) { 
	applyForce(0, g);
}

void PhysicsObj::collideWalls(int screenWidth, int screenHeight) {
	// collide with all 4 edges of screen and set position inside wall
	if (center.y + halfHeight > screenHeight) center.y = screenHeight - getHalfWidth();
	if (center.x + halfWidth > screenWidth) center.x = screenWidth - halfWidth - 3;
	if (center.x - halfWidth < 0) center.x = halfWidth + 2;
	if (center.y - halfHeight < 0) center.y = halfHeight;
}

void PhysicsObj::update(float frameTime, int screenWidth, int screenHeight) {
	if (!isActive) return; // do not update inactive objects
	destRect.x = center.x - halfWidth;
	destRect.y = center.y - halfHeight;
	if (isStatic) return; // do not update fully if black hole (doesn't move)
	collideWalls(screenWidth, screenHeight); // collide with walls
	updateMovement(frameTime); // move object based on current and previous position
	applyGravity(CONST_GRAVITY); // apply gravity
}

Point2 PhysicsObj::launchMove(bool launch, int startX, int startY, int targetX, int targetY)
{
	// get angle using different between positions
	angle = atan2(targetY - startY, targetX - startX);
	angle *= 180 / M_PI;
	angle += 90;
	if (angle < 0) angle += 360;

	// calculate distance from pc
	float distance = 0;
	float a = targetX - startX;
	float b = targetY - startY;
	distance = sqrt((a * a) + (b * b));
	if (distance > 200) distance = 200;
	Point2 trajectoryPoint;

	if (launch) {  // allows for angle to be read before the force is applied
		force.x -= sin(angle * M_PI / 180) * distance * 1500;
		force.y += cos(angle * M_PI / 180) * distance * 1500;
		return trajectoryPoint;
	}
	else if (!launch) { // create trajectory path
		if (a > 200) a = 200;
		if (b > 200) b = 200;
		trajectoryPoint.x = startX + a * -1;
		trajectoryPoint.y = startY + b * -1;
		return trajectoryPoint;
	}
}

void PhysicsObj::updateMovement(float dt) {
	velocity.x = center.x - lastCenter.x;
	velocity.y = center.y - lastCenter.y;

	// Save current pos
	lastCenter = center;

	// Air Res
	velocity.x *= airRes;

	// Verlet integration
	center.x = center.x + velocity.x + force.x * dt * dt;
	center.y = center.y + velocity.y + force.y * dt * dt;

	// Reset force (accel)
	force = {};

}

void PhysicsObj::blackHole(PhysicsObj* other) { // Applies a force proportional to distance towards the black hole object
	float dx = center.x - other->center.x;
	float dy = center.y - other->center.y;

	// Get Angle to black hole
	float bangle = atan2(dy, dx);
	bangle *= 180 / M_PI;
	bangle += 90;
	if (bangle < 0) bangle += 360;

	// Apply force based on distance from black hole object
	float distance = 10;
	distance = sqrt((dx * dx) + (dy * dy));
	distance = 1 / distance * 400;
	other->force.x = sin(bangle * M_PI / 180) * distance * 2;
	other->force.y = -cos(bangle * M_PI / 180) * distance * 2;
}