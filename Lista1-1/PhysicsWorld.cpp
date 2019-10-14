#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	world = new b2World(b2Vec2(0.0f, 9.8f));
	world->SetAllowSleeping(false);
}

PhysicsWorld::PhysicsWorld(b2Vec2 gravity)
{
	world = new b2World(b2Vec2(gravity.x, gravity.y));
	world->SetAllowSleeping(false);
}

PhysicsWorld::~PhysicsWorld()
{
	world->~b2World();
}

b2Body *PhysicsWorld::CreateBody(b2BodyType type, b2Vec2 position)
{
	b2BodyDef bodyDef;
	b2Body *body;

	bodyDef.type = type;
	bodyDef.position = position;

	body = world->CreateBody(&bodyDef);

	return body;
}

b2Body *PhysicsWorld::CreateBox(b2BodyType type, b2Vec2 position, b2Vec2 dimensions, float32 density, float32 restitution, float32 friction)
{
	b2PolygonShape rectangle;
	b2Body *body;

	rectangle.SetAsBox(dimensions.x / 2, dimensions.y / 2);

	body = CreateBody(type, position);

	b2FixtureDef fixture;

	fixture.shape = &rectangle;
	fixture.density = density;
	fixture.restitution = restitution;
	fixture.friction = friction;

	body->CreateFixture(&fixture);

	return body;
}

b2Body *PhysicsWorld::CreateCircle(b2BodyType type, b2Vec2 position, float32 radius, float32 density, float32 restitution, float32 friction)
{
	b2CircleShape circle;
	b2Body *body;

	circle.m_radius = radius;

	body = CreateBody(type, position);

	b2FixtureDef fixture;

	fixture.shape = &circle;
	fixture.density = density;
	fixture.restitution = restitution;
	fixture.friction = friction;

	body->CreateFixture(&fixture);

	return body;
}

b2Body *PhysicsWorld::CreateLine(b2BodyType type, b2Vec2 position, b2Vec2 destination, float32 density, float32 restitution, float32 friction)
{
	b2EdgeShape line;

	b2BodyDef bodyDef;
	b2Body *body;

	b2FixtureDef fixture;

	// Setting the line vertices
	line.Set(position, destination);

	bodyDef.type = type;

	body = world->CreateBody(&bodyDef);

	fixture.shape = &line;
	fixture.density = density;
	fixture.restitution = restitution;
	fixture.friction = friction;

	body->CreateFixture(&fixture);
	
	return body;
}

void PhysicsWorld::CreateRectangleFixture(b2Vec2 position, float32 density, float32 restitution, float32 friction, b2Vec2 dimensions, float32 angle, b2Body *body)
{
	b2FixtureDef fixture;
	b2PolygonShape shape;
	
	shape.SetAsBox(dimensions.x / 2, dimensions.y / 2, position, angle);

	fixture.shape = &shape;
	fixture.density = density;
	fixture.restitution = restitution;
	fixture.friction = friction;

	body->CreateFixture(&fixture);
}

void PhysicsWorld::CreateCircleFixture(b2Vec2 position, float32 radius, float32 density, float32 restitution, float32 friction, b2Body *body)
{
	b2FixtureDef fixture;
	b2CircleShape shape;

	shape.m_p = position;
	shape.m_radius = radius;

	fixture.shape = &shape;
	fixture.density = density;
	fixture.restitution = restitution;
	fixture.friction = friction;

	body->CreateFixture(&fixture);
}

void PhysicsWorld::CreatePolygonFixture(float32 density, float32 restitution, float32 friction, b2Vec2 *vertices, int32 vertexCount, b2Body *body)
{
	b2FixtureDef fixture;
	b2PolygonShape shape;
	
	shape.Set(vertices, vertexCount);

	fixture.shape = &shape;
	fixture.density = density;
	fixture.restitution = restitution;
	fixture.friction = friction;

	body->CreateFixture(&fixture);
}

void PhysicsWorld::SetGravity(b2Vec2 gravity)
{
	world->SetGravity(gravity);
}

b2Body *PhysicsWorld::Step(float32 timeStep, int32 velocityIterations, int32 positionIterations)
{
	world->Step(timeStep, velocityIterations, positionIterations);
	world->ClearForces();

	return world->GetBodyList();
}
