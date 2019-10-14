#pragma once
#include <Box2D/Box2D.h>

class PhysicsWorld {

public:
	PhysicsWorld();
	PhysicsWorld(b2Vec2 gravity);
	~PhysicsWorld();
	
	b2Body *CreateBody(b2BodyType type, b2Vec2 position, float32 scale);
	b2Body *CreateBox(b2BodyType type, b2Vec2 position, b2Vec2 dimensions, float32 density, float32 restitution, float32 friction, float32 scale);
	b2Body *CreateCircle(b2BodyType type, b2Vec2 position, float32 radius, float32 density, float32 restitution, float32 friction, float32 scale);
	b2Body *CreateLine(b2BodyType type, b2Vec2 position, b2Vec2 destination, float32 density, float32 restitution, float32 friction, float32 scale);
	
	b2FixtureDef *CreateRectangleFixture(b2Vec2 position, float32 density, float32 restitution, float32 friction, b2Vec2 dimensions, b2Body *body, float32 scale);
	b2FixtureDef *CreateCircleFixture(b2Vec2 position, float32 density, float32 restitution, float32 friction);
	b2FixtureDef *CreatePolygonFixture(float32 density, float32 restitution, float32 friction, b2Vec2 *vertices, int32 vertexCount);

	void SetGravity(b2Vec2 gravity);


	b2Body *Step(float32 timeStep = 1.0f / 60.0f, int32 velocityIterations = 10, int32 positionIterations = 8);

private:

	b2World *world;
	b2Vec2 gravity;
};