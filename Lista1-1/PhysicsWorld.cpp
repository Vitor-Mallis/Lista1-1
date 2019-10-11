#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	world = new b2World(b2Vec2(0, 9.8f));
}

PhysicsWorld::PhysicsWorld(float gravity[2])
{
	world = new b2World(b2Vec2(gravity[0], gravity[1]));
}

PhysicsWorld::~PhysicsWorld()
{
	world->~b2World();
	delete world;
}

void PhysicsWorld::CreateBody(b2BodyType type, float position[2], float density, float restitution, float friction, float scale, b2Shape *shape)
{
	b2BodyDef bodyDef;
	b2Body *body;

	b2FixtureDef fixture;

	bodyDef.type = type;
	bodyDef.position = b2Vec2(position[0] / scale, position[1] / scale);

	body = world->CreateBody(&bodyDef);

	fixture.shape = shape;
	fixture.density = density;
	fixture.restitution = restitution;
	fixture.friction = friction;

	body->CreateFixture(&fixture);
}

void PhysicsWorld::CreateBox(b2BodyType type, float position[2], float dimensions[2], float density, float restitution, float friction, float scale)
{
	
	b2PolygonShape rectangle;
	
	rectangle.SetAsBox((dimensions[0]/2)/scale, (dimensions[1]/2)/scale);

	CreateBody(type, position, density, restitution, friction, scale, &rectangle);
}

void PhysicsWorld::CreateCircle(b2BodyType type, float position[2], float radius, float density, float restitution, float friction, float scale)
{
	b2CircleShape circle;

	circle.m_radius = radius/scale;

	CreateBody(type, position, density, restitution, friction, scale, &circle);
}

void PhysicsWorld::CreateLine(b2BodyType type, float position[2], float destination[2], float density, float restitution, float friction, float scale)
{
	b2EdgeShape line;
	
	// CreateBody(type, position, density, restitution, friction, scale, &line);

	b2BodyDef bodyDef;
	b2Body *body;

	b2FixtureDef fixture;

	// Setting the body position to the the center of the line
	float posX = (position[0] + destination[0] / 2.f);
	float posY = (position[1] + destination[1] / 2.f);
	
	// Calculating the line length
	float length = (float)sqrt((position[0] - destination[0])*(position[0] - destination[0]) + (position[1] - destination[1])*(position[1] - destination[1]));
	float boxLength = length / scale;

	// Setting the line vertices offset from the center
	line.Set(b2Vec2(position[0] / scale, position[1] / scale), b2Vec2(destination[0] / scale, destination[1] / scale));


	bodyDef.type = type;
	//bodyDef.position = b2Vec2(posX / scale, posY / scale);

	body = world->CreateBody(&bodyDef);

	fixture.shape = &line;
	fixture.density = density;
	fixture.restitution = restitution;
	fixture.friction = friction;

	body->CreateFixture(&fixture);
}

b2Body *PhysicsWorld::Step(float32 timeStep, float32 velocityIterations, float32 positionIterations)
{
	world->Step(timeStep, velocityIterations, positionIterations);
	world->ClearForces();

	return world->GetBodyList();
}
