#pragma once

#include "Renderer.h"
#include "PhysicsWorld.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Game {

public:
	Game();
	~Game();

	void UpdateSFMLEvents();
	void Update();
	void Render();

	void DrawBody(b2Body *body);
	
	void Exercicio1_3();

	void Run();

private:
	const float SCALE = 30.0f;
	
	void createWindow();

	PhysicsWorld *world;

	Renderer *renderer;

	sf::RenderWindow *window;
	sf::Event sfEvent;
};