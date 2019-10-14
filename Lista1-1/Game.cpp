#include "Game.h"
#include <iostream>

Game::Game()
{
	world = new PhysicsWorld();
	window = new sf::RenderWindow();
	window->setFramerateLimit(60);

	createWindow();

	renderer = new Renderer();

	srand(time(NULL));
}

Game::~Game()
{
	delete world;
	delete window;
	delete renderer;
}

void Game::UpdateDeltaTime()
{
	dt = dtClock.restart().asSeconds();
}

void Game::UpdateSFMLEvents()
{
	//Checking if the user closes the window
	while (window->pollEvent(sfEvent)) {
		if (sfEvent.type == sf::Event::Closed) window->close();
	}
}

void Game::Update()
{
	UpdateDeltaTime();
	//UpdateSFMLEvents();
}

void Game::Render()
{
	//Physics engine update tick
	b2Body *bodies = world->Step(dt);

	window->clear(sf::Color::White);

	//Going through the bodies list and drawing each one at its updated position and rotation
	for (b2Body *bodyIterator = bodies; bodyIterator != 0; bodyIterator = bodyIterator->GetNext()) {
		DrawBody(bodyIterator);
	}

	window->display();
}

void Game::DrawBody(b2Body * body)
{
	for (b2Fixture *fixtureIterator = body->GetFixtureList(); fixtureIterator != 0; fixtureIterator = fixtureIterator->GetNext()) {

		const b2Transform& xf = fixtureIterator->GetBody()->GetTransform();

		switch (fixtureIterator->GetType())
		{
		case b2Shape::e_circle:
		{
			b2CircleShape* circle = (b2CircleShape*)fixtureIterator->GetShape();

			b2Vec2 center = b2Mul(xf, circle->m_p);
			float radius = SCALE * circle->m_radius;
			float centerX = SCALE * center.x;
			float centerY = SCALE * center.y;
			
			//Calling the SFML renderer
			renderer->DrawCircle(radius, centerX - radius, centerY - radius, window, sf::Color::Black);
		}
		break;

		case b2Shape::e_polygon:
		{
			b2PolygonShape* poly = (b2PolygonShape*)fixtureIterator->GetShape();
			int32 vertexCount = poly->m_count;
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			
			float **vertices;
			vertices = new float*[vertexCount];
			float rotation = fixtureIterator->GetBody()->GetAngle() * 180 / b2_pi;

			for (int i = 0; i < vertexCount; i++) {
				b2Vec2 multipliedVertex = b2Mul(xf, poly->m_vertices[i]);

				vertices[i] = new float[2];
				vertices[i][0] = SCALE * multipliedVertex.x;
				vertices[i][1] = SCALE * multipliedVertex.y;
			}

			renderer->DrawPolygon(vertexCount, vertices, window, sf::Color::Black);
		}

		break;
		case b2Shape::e_edge:
		{
			b2EdgeShape* edge = (b2EdgeShape*)fixtureIterator->GetShape();
			int32 vertexCount;

			b2Vec2 vertices[b2_maxPolygonVertices];
			int i = 0;

			if (edge->m_hasVertex0) {
				vertices[i] = b2Mul(xf, edge->m_vertex0);
				i++;
			}

			vertices[i] = b2Mul(xf, edge->m_vertex1); i++;
			vertices[i] = b2Mul(xf, edge->m_vertex2); i++;

			if (edge->m_hasVertex3) {
				vertices[i] = b2Mul(xf, edge->m_vertex3);
				i++;
			}

			vertexCount = i;

			float pointStart[2] = { SCALE * vertices[0].x, SCALE * vertices[0].y };
			float pointFinish[2] = { SCALE * vertices[1].x, SCALE * vertices[1].y };

			renderer->DrawLine(vertexCount, pointStart, pointFinish, window, sf::Color::Black);
		}

		break;

		}
	}
}

float32 Game::WorldToBox(float32 value)
{
	value /= SCALE;
	return value;
}

b2Vec2 Game::WorldToBox(b2Vec2 vector)
{
	vector.x /= SCALE;
	vector.y /= SCALE;
	return vector;
}

b2Vec2 * Game::WorldToBox(b2Vec2 *vectors, float32 size)
{
	for (int i = 0; i < size; i++) {
		vectors[i].x /= SCALE;
		vectors[i].y /= SCALE;
	}

	return vectors;
}

void Game::Exercicio1_3()
{	
	b2Vec2 groundCoords1 = { 5.f, 595.f };
	b2Vec2 groundCoords2 = { 795.f, 595.f };

	b2Vec2 wallRightCoords1 = { 795.f, 595.f };
	b2Vec2 wallRightCoords2 = { 795.f, 5.f };

	b2Vec2 ceilingCoords1 = { 795.f, 5.f };
	b2Vec2 ceilingCoords2 = { 5.f, 5.f };

	b2Vec2 wallLeftCoords1 = { 5.f, 5.f };
	b2Vec2 wallLeftCoords2 = { 5.f, 595.f };

	b2Vec2 gravityExercicio;

	//Creating the border
	world->CreateLine(b2_staticBody, WorldToBox(groundCoords1), WorldToBox(groundCoords2), 1.0f, 0.0f, 0.0f);
	world->CreateLine(b2_staticBody, WorldToBox(wallRightCoords1), WorldToBox(wallRightCoords2), 1.0f, 0.0f, 0.0f);
	world->CreateLine(b2_staticBody, WorldToBox(ceilingCoords1), WorldToBox(ceilingCoords2), 1.0f, 0.0f, 0.0f);
	world->CreateLine(b2_staticBody, WorldToBox(wallLeftCoords1), WorldToBox(wallLeftCoords2), 1.0f, 0.0f, 0.0f);

	while (window->isOpen()) {

		//Polling exercise-specific events
		while (window->pollEvent(sfEvent)) {
			if (sfEvent.type == sf::Event::Closed) window->close();

			if (sfEvent.type == sf::Event::KeyPressed) {
				if (sfEvent.key.code == sf::Keyboard::G) {
					gravityExercicio.x = rand() % 21 - 10;
					gravityExercicio.y = rand() % 21 - 10;
					world->SetGravity(gravityExercicio);
				}

				if (sfEvent.key.code == sf::Keyboard::B) {
					b2Vec2 boxPosition = { static_cast <float32> (rand() % 771 + 20), static_cast <float32> (rand() % 571 + 20) };
					b2Vec2 boxDimensions = { static_cast <float32> (rand() % 51 + 20), static_cast <float32> (rand() % 51 + 20) };

					float32 boxDensity = rand() % 19 + 1;

					//Setting these paramaters to a number from 0.0f to 1.0f
					float32 boxRestitution = static_cast <float32> (rand()) / static_cast <float32> (RAND_MAX);
					float32 boxFriction = static_cast <float32> (rand()) / static_cast <float32> (RAND_MAX);

					world->CreateBox(b2_dynamicBody, WorldToBox(boxPosition), WorldToBox(boxDimensions), boxDensity, boxRestitution, boxFriction);
				}

				if (sfEvent.key.code == sf::Keyboard::C) {
					b2Vec2 circlePosition = { static_cast <float32> (rand() % 771 + 20), static_cast <float32> (rand() % 571 + 20) };
					float32 circleRadius = rand() % 26 + 5;

					

					float32 circleDensity = rand() % 19 + 1;

					//Setting these three paramaters to a number from 0.0f to 1.0f
					float32 circleRestitution = static_cast <float32> (rand()) / static_cast <float32> (RAND_MAX);
					float32 circleFriction = static_cast <float32> (rand()) / static_cast <float32> (RAND_MAX);

					world->CreateCircle(b2_dynamicBody, WorldToBox(circlePosition), WorldToBox(circleRadius), circleDensity, circleRestitution, circleFriction);
				}

				if (sfEvent.key.code == sf::Keyboard::L) {
					b2Vec2 linePosition = { static_cast <float32> (rand() % 771 + 20), static_cast <float32> (rand() % 571 + 20) };
					b2Vec2 lineDestination = { static_cast <float32> (rand() % 771 + 20), static_cast <float32> (rand() % 571 + 20) };

					float32 lineDensity = rand() % 19 + 1;

					//Setting these paramaters to a number from 0.0f to 1.0f
					float32 lineRestitution = static_cast <float32> (rand()) / static_cast <float> (RAND_MAX);
					float32 lineFriction = static_cast <float32> (rand()) / static_cast <float> (RAND_MAX);

					world->CreateLine(b2_dynamicBody, WorldToBox(linePosition), WorldToBox(lineDestination), lineDensity, lineRestitution, lineFriction);
				}
			}
		}

		Update();
		Render();
	}
}

void Game::Exercicio4() {

	b2Vec2 boxCoords = { 400.f, 300.f };
	b2Vec2 boxDimensions = { 20.f, 20.f };
	float32 boxRestitution = 0.f;

	b2Vec2 groundCoords1 = { 5.f, 595.f };
	b2Vec2 groundCoords2 = { 795.f, 595.f };

	world->CreateLine(b2_staticBody, WorldToBox(groundCoords1), WorldToBox(groundCoords2), 0.0f, 0.0f, 1.0f);

	while (window->isOpen()) {

		//Polling exercise-specific events
		while (window->pollEvent(sfEvent)) {
			if (sfEvent.type == sf::Event::Closed) window->close();

			if (sfEvent.type == sf::Event::KeyPressed) {
				if (sfEvent.key.code == sf::Keyboard::B) {

					world->CreateBox(b2_dynamicBody, WorldToBox(boxCoords), WorldToBox(boxDimensions), 5.f, boxRestitution, 1.f);
					if (boxRestitution < 1.f) boxRestitution += 0.1f;
				}
			}
		}
		Update();
		Render();
	}
}

void Game::Exercicio5() {

	b2Vec2 boxCoords = { 775.f, 140.f };
	b2Vec2 boxDimensions = { 20.f, 20.f };
	float32 boxFriction = 0.f;

	b2Vec2 rampCoords1 = { 50.f, 590.f };
	b2Vec2 rampCoords2 = { 785.f, 150.f };

	world->CreateLine(b2_staticBody, WorldToBox(rampCoords1), WorldToBox(rampCoords2), 0.0f, 0.0f, 1.0f);

	while (window->isOpen()) {

		//Polling exercise-specific events
		while (window->pollEvent(sfEvent)) {
			if (sfEvent.type == sf::Event::Closed) window->close();

			if (sfEvent.type == sf::Event::KeyPressed) {
				if (sfEvent.key.code == sf::Keyboard::B) {

					world->CreateBox(b2_dynamicBody, WorldToBox(boxCoords), WorldToBox(boxDimensions), 5.f, 0.f, boxFriction);
					if (boxFriction < 1.f) boxFriction += 0.1f;
				}
			}
		}
		Update();
		Render();
	}
}

void Game::Exercicio6() {

	b2Vec2 boxCoords = { 200.f, 555.f };
	b2Vec2 boxDimensions = { 20.f, 20.f };

	b2Vec2 circleCoords = { 600.f, 555.f };
	float32 circleRadius = 10.f;
	
	b2Vec2 groundCoords1 = { 5.f, 595.f };
	b2Vec2 groundCoords2 = { 795.f, 595.f };

	world->CreateLine(b2_staticBody, WorldToBox(groundCoords1), WorldToBox(groundCoords2), 0.0f, 0.0f, 1.0f);

	for (int i = 0; i < 6; i++) {
		world->CreateBox(b2_dynamicBody, WorldToBox(boxCoords), WorldToBox(boxDimensions), 5.f, 0.f, 1.f);
		boxCoords.y -= 40.f;
	}

	for (int i = 0; i < 6; i++) {
		world->CreateCircle(b2_dynamicBody, WorldToBox(circleCoords), WorldToBox(circleRadius), 5.f, 0.f, 1.f);
		circleCoords.y -= 40.f;
	}

	while (window->isOpen()) {

		//Polling exercise-specific events
		while (window->pollEvent(sfEvent)) {
			if (sfEvent.type == sf::Event::Closed) window->close();
		}

		Update();
		Render();
	}
}

void Game::Exercicio7()
{
	b2Vec2 boxCoords = { 200.f, 555.f };
	b2Vec2 boxDimensions = { 20.f, 20.f };

	b2Vec2 groundCoords1 = { 5.f, 595.f };
	b2Vec2 groundCoords2 = { 795.f, 595.f };

	world->CreateLine(b2_staticBody, WorldToBox(groundCoords1), WorldToBox(groundCoords2), 0.0f, 0.0f, 1.0f);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			world->CreateBox(b2_dynamicBody, WorldToBox(boxCoords), WorldToBox(boxDimensions), 5.f, 0.f, 1.f);
			boxCoords.x += 21.f;
		}
		boxCoords.y -= 40.f;
		boxCoords.x = 200.f;
	}

	while (window->isOpen()) {

		//Polling exercise-specific events
		while (window->pollEvent(sfEvent)) {
			if (sfEvent.type == sf::Event::Closed) window->close();
		}

		Update();
		Render();
	}
}

void Game::Exercicio8()
{
	b2Vec2 objCoords = { 200.f, 555.f };

	b2Vec2 groundCoords1 = { 5.f, 595.f };
	b2Vec2 groundCoords2 = { 795.f, 595.f };

	b2Body *object;

	//Defining the fixtures of the object

	b2Vec2 bodyCoords = { 400.f, 300.f };

	b2Vec2 mainCoords = { 0.f, 0.f };
	b2Vec2 mainDimensions = { 100.f, 50.f };

	b2Vec2 leftWheelCoords = { -50.f, 25.f };
	float32 leftWheelRadius = 10.f;

	b2Vec2 rightWheelCoords = { 50.f, 25.f };
	float32 rightWheelRadius = 10.f;

	float32 triangleCount = 3;

	b2Vec2 *triangleCoords = new b2Vec2[triangleCount];

	triangleCoords[0] = b2Vec2(-25.f, -25.f);
	triangleCoords[1] = b2Vec2(25.f, -25.f);
	triangleCoords[2] = b2Vec2(0.f, -60.f);
		
	//Creating the multi-fixture object
	world->CreateLine(b2_staticBody, WorldToBox(groundCoords1), WorldToBox(groundCoords2), 0.0f, 0.0f, 1.0f);

	object = world->CreateBody(b2_dynamicBody, WorldToBox(bodyCoords));

	world->CreateRectangleFixture(WorldToBox(mainCoords), 5.f, 0.f, 1.f, WorldToBox(mainDimensions), 0.f, object);;

	world->CreateCircleFixture(WorldToBox(leftWheelCoords), WorldToBox(leftWheelRadius), 5.f, 0.f, 1.f, object);

	world->CreateCircleFixture(WorldToBox(rightWheelCoords), WorldToBox(rightWheelRadius), 5.f, 0.f, 1.f, object);

	world->CreatePolygonFixture(5.f, 0.f, 1.f, WorldToBox(triangleCoords, triangleCount), triangleCount, object);

	while (window->isOpen()) {

		//Polling exercise-specific events
		while (window->pollEvent(sfEvent)) {
			if (sfEvent.type == sf::Event::Closed) window->close();
		}

		Update();
		Render();
	}
}

void Game::Run()
{
	//Exercicio1_3();
	//Exercicio4();
	//Exercicio5();
	//Exercicio6();
	//Exercicio7();
	Exercicio8();
}

void Game::createWindow()
{
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "Física - Lista 1");
}
