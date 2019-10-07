#include "Game.h"

Game::Game()
{
	world = new PhysicsWorld();
	window = new sf::RenderWindow();

	createWindow();
}

Game::~Game()
{
	delete world;
	delete window;
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
	b2Body *bodies;

	window->clear(sf::Color::White);

	//Physics engine update tick
	bodies = world->Step();

	//Going through the bodies list and drawing each one at its updated position and rotation
	for (b2Body *bodyIterator = bodies; bodyIterator != 0; bodyIterator = bodyIterator->GetNext()) {
		sf::RectangleShape shape;

		/*sf::Sprite sprite;
		sprite.setOrigin(16.0f, 16.0f);
		sprite.setPosition(SCALE * bodyIterator->GetPosition().x, SCALE * bodyIterator->GetPosition().y);
		sprite.setRotation(bodyIterator->GetAngle() * 180 / b2_pi);*/

		shape.setOrigin(16.0f, 16.0f);
		shape.setPosition(SCALE * bodyIterator->GetPosition().x, SCALE * bodyIterator->GetPosition().y);
		shape.setRotation(bodyIterator->GetAngle() * 180 / b2_pi);
		shape.setSize(sf::Vector2<float32>(SCALE * bodyIterator->GetTransform().p.x, SCALE * bodyIterator->GetTransform().p.y));
		shape.setFillColor(sf::Color::Black);

		window->draw(shape);
	}

	window->display();
}

void Game::Exercicio1()
{
	float boxPosition[2] = { 400.f, 300.f };
	float boxDimensions[2] = { 32.f, 32.f };
	
	float groundPosition[2] = { 100.f, 100.f };
	float groundDestination[2] = { 150.f, 150.f };

	world->CreateLine(b2_staticBody, groundPosition, groundDestination, 1.0f, 0.0f, 0.0f, SCALE);
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
