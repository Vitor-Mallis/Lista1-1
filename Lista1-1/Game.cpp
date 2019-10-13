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

void Game::UpdateSFMLEvents()
{
	//Checking if the user closes the window
	while (window->pollEvent(sfEvent)) {
		if (sfEvent.type == sf::Event::Closed) window->close();
	}
}

void Game::Update()
{
	UpdateSFMLEvents();
}

void Game::Render()
{
	//Physics engine update tick
	b2Body *bodies = world->Step();

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
			float rotation = fixtureIterator->GetBody()->GetAngle() * 180 / b2_pi;
			
			//Calling the SFML renderer
			renderer->DrawCircle(radius, centerX, centerY, rotation, window, sf::Color::Black);
		}
		break;

		case b2Shape::e_polygon:
		{
			b2PolygonShape* poly = (b2PolygonShape*)fixtureIterator->GetShape();
			int32 vertexCount = poly->m_count;
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			//b2Vec2 vertices[b2_maxPolygonVertices];
			float **vertices;
			vertices = new float*[vertexCount];
			float rotation = fixtureIterator->GetBody()->GetAngle() * 180 / b2_pi;

			for (int i = 0; i < vertexCount; i++) {
				b2Vec2 multipliedVertex = b2Mul(xf, poly->m_vertices[i]);

				vertices[i] = new float[2];
				vertices[i][0] = SCALE * multipliedVertex.x;
				vertices[i][1] = SCALE * multipliedVertex.y;
			}


			renderer->DrawPolygon(vertexCount, vertices, rotation, window, sf::Color::Black);
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

void Game::Exercicio1_3()
{	
	float groundCoords1[2] = { 5.f, 595.f };
	float groundCoords2[2] = { 795.f, 595.f };

	float wallRightCoords1[2] = { 795.f, 595.f };
	float wallRightCoords2[2] = { 795.f, 5.f };

	float ceilingCoords1[2] = { 795.f, 5.f };
	float ceilingCoords2[2] = { 5.f, 5.f };

	float wallLeftCoords1[2] = { 5.f, 5.f };
	float wallLeftCoords2[2] = { 5.f, 595.f };

	b2Vec2 gravityExercicio;

	//Creating the border

	world->CreateLine(b2_staticBody, groundCoords1, groundCoords2, 1.0f, 0.0f, 0.0f, SCALE);
	world->CreateLine(b2_staticBody, wallRightCoords1, wallRightCoords2, 1.0f, 0.0f, 0.0f, SCALE);
	world->CreateLine(b2_staticBody, ceilingCoords1, ceilingCoords2, 1.0f, 0.0f, 0.0f, SCALE);
	world->CreateLine(b2_staticBody, wallLeftCoords1, wallLeftCoords2, 1.0f, 0.0f, 0.0f, SCALE);

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
					float boxPosition[2] = { rand() % 771 + 20, rand() % 571 + 20 };
					float boxDimensions[2] = { rand() % 51 + 20, rand() % 51 + 20 };

					//Setting the three paramaters to a number from 0.0f to 1.0f

					float boxDensity = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float boxRestitution = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float boxFriction = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

					world->CreateBox(b2_dynamicBody, boxPosition, boxDimensions, 1.0f, 0.0f, 0.0f, SCALE);
				}

				if (sfEvent.key.code == sf::Keyboard::C) {
					float circlePosition[2] = { rand() % 771 + 20, rand() % 571 + 20 };
					float circleRadius = rand() % 26 + 5;

					//Setting the three paramaters to a number from 0.0f to 1.0f

					float circleDensity = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float circleRestitution = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float circleFriction = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

					world->CreateCircle(b2_dynamicBody, circlePosition, circleRadius, circleDensity, circleRestitution, circleFriction, SCALE);
				}

				if (sfEvent.key.code == sf::Keyboard::L) {
					float linePosition[2] = { rand() % 771 + 20, rand() % 571 + 20 };
					float lineDestination[2] = { rand() % 771 + 20, rand() % 571 + 20 };

					//Setting the three paramaters to a number from 0.0f to 1.0f

					float lineDensity = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float lineRestitution = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float lineFriction = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

					world->CreateLine(b2_dynamicBody, linePosition, lineDestination, lineDensity, lineRestitution, lineFriction, SCALE);
				}
			}
		}

		//Commenting Update for now, because events will be specific to the exercises
		//Update();
		Render();
	}
}

void Game::Run()
{
	Exercicio1_3();
}

void Game::createWindow()
{
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "Física - Lista 1");
}
