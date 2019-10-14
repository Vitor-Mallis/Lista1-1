#pragma once

#include "Renderer.h"
#include "PhysicsWorld.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Game {

public:
	Game();
	~Game();

	void UpdateDeltaTime();
	void UpdateSFMLEvents();
	void Update();
	void Render();

	void DrawBody(b2Body *body);
	
	void Exercicio1_3();
	void Exercicio4();
	void Exercicio5();
	void Exercicio6();
	void Exercicio7();
	void Exercicio8();

	void Run();

private:
	//Scale for converting from world coordinates to Box2D coordinates
	const float32 SCALE = 30.0f;
	
	void createWindow();

	//DeltaTime
	float dt;

	sf::Clock dtClock;

	PhysicsWorld *world;

	Renderer *renderer;

	sf::RenderWindow *window;
	sf::Event sfEvent;
};