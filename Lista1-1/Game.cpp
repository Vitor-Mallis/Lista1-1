#include "Game.h"

Game::Game()
{
	world = new PhysicsWorld();
	window = new sf::RenderWindow();
	window->setFramerateLimit(60);

	createWindow();

	renderer = new Renderer();
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
	b2Body *bodies = world->Step();;

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
			b2Vec2 vertices[b2_maxPolygonVertices];

			for (int32 i = 0; i < vertexCount; ++i)
			{
				vertices[i] = b2Mul(xf, poly->m_vertices[i]);
			}

			//renderer->DrawPolygon();

			sf::ConvexShape shape;

			//shape.setOrigin(16.0f, 16.0f);

			shape.setPointCount(vertexCount);

			for (int iterator = 0; iterator < vertexCount; iterator++) {
				shape.setPoint(iterator, sf::Vector2<float>(SCALE * vertices[iterator].x, SCALE * vertices[iterator].y));
			}


			// shape.setPosition(SCALE * bodyIterator->GetPosition().x, SCALE * bodyIterator->GetPosition().y);
			shape.setRotation(fixtureIterator->GetBody()->GetAngle() * 180 / b2_pi);
			//shape.setSize(sf::Vector2<float32>(SCALE * bodyIterator->GetTransform().p.x, SCALE * bodyIterator->GetTransform().p.y));
			shape.setFillColor(sf::Color::Black);

			shape.setOutlineThickness(1.0f);
			shape.setOutlineColor(sf::Color::Black);

			window->draw(shape);

			//renderer.DrawPolygon(vertices, vertexCount, color);
		}

		break;
		case b2Shape::e_edge:
		{
			b2EdgeShape* edge = (b2EdgeShape*)fixtureIterator->GetShape();
			int32 vertexCount;

			b2Vec2 vertices[b2_maxPolygonVertices];
			int i = 0;

			if (edge->m_hasVertex0)
			{
				vertices[i] = b2Mul(xf, edge->m_vertex0);
				i++;
			}
			vertices[i] = b2Mul(xf, edge->m_vertex1); i++;
			vertices[i] = b2Mul(xf, edge->m_vertex2); i++;
			if (edge->m_hasVertex3)
			{
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

void Game::Exercicio1()
{
	float boxPosition[2] = { 400.f, 300.f };
	float boxDimensions[2] = { 32.f, 32.f };
	
	float groundCoords1[2] = { 5.f, 595.f };
	float groundCoords2[2] = { 795.f, 595.f };

	world->CreateLine(b2_staticBody, groundCoords1, groundCoords2, 1.0f, 0.0f, 0.0f, SCALE);
	world->CreateBox(b2_dynamicBody, boxPosition, boxDimensions, 1.0f, 0.0f, 0.0f, SCALE);

	while (window->isOpen()) {
		Update();
		Render();
	}
}

void Game::Run()
{
	Exercicio1();
}

void Game::createWindow()
{
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "Física - Lista 1");
}
