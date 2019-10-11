#pragma once
#include <list>
#include <Box2D/Box2D.h>
#include <math.h>

class PhysicsWorld {

public:
	PhysicsWorld();
	PhysicsWorld(float gravity[2]);
	~PhysicsWorld();
	
	void CreateBody(b2BodyType type, float position[2], float density, float restitution, float friction, float scale, b2Shape *shape);
	void CreateBox(b2BodyType type, float position[2], float dimensions[2], float density, float restitution, float friction, float scale);
	void CreateCircle(b2BodyType type, float position[2], float radius, float density, float restitution, float friction, float scale);
	void CreateLine(b2BodyType type, float position[2], float destination[2], float density, float restitution, float friction, float scale);

	b2Body *Step(float32 timeStep = 1.0f / 60.0f, float32 velocityIterations = 10, float32 positionIterations = 8);

private:

	b2World *world;
	b2Vec2 gravity;
};