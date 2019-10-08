#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	world = new b2World(b2Vec2(0, -10));
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

	line.Set(b2Vec2(position[0]/scale, position[1]/scale), b2Vec2(destination[0]/scale, destination[1]/scale));
	
	CreateBody(type, position, density, restitution, friction, scale, &line);
}

b2Body *PhysicsWorld::Step(float32 timeStep, float32 velocityIterations, float32 positionIterations)
{
	world->Step(timeStep, velocityIterations, positionIterations);
	world->ClearForces();

	return world->GetBodyList();
}
