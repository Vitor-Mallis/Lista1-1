#pragma once
#include <SFML/Graphics.hpp>

class Renderer {

public:

	Renderer();
	~Renderer();

	void Render(sf::RenderWindow *window);
	
	void DrawCircle(float radius, float centerX, float centerY, sf::RenderWindow *window, sf::Color color);
	void DrawPolygon(int vertexCount, float **vertices, sf::RenderWindow *window, sf::Color color);
	void DrawLine(int vertexCount, float pointStart[2], float pointFinish[2], sf::RenderWindow *window, sf::Color color);

private:

};