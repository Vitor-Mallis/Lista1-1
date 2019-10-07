#pragma once

#include "PhysicsWorld.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Game {

public:
	Game();
	~Game();

	void UpdateSFMLEvents();
	void Update();
	void Render();
	
	void Exercicio1();

	void Run();

private:
	const float SCALE = 30.0f;
	
	void createWindow();

	PhysicsWorld *world;

	sf::RenderWindow *window;
	sf::Event sfEvent;
};