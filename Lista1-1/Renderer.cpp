#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Render(sf::RenderWindow *window)
{
	

}

void Renderer::DrawCircle(float radius, float centerX, float centerY, float rotation, sf::RenderWindow *window, sf::Color color)
{
	//Creating the SF shape and drawing it
	sf::CircleShape shape;

	//shape.setOrigin(16.0f, 16.0f);
	shape.setPosition(centerX, centerY);
	shape.setRotation(rotation);
	shape.setRadius(radius);
	//shape.setSize(sf::Vector2<float32>(SCALE * bodyIterator->GetTransform().p.x, SCALE * bodyIterator->GetTransform().p.y));
	shape.setFillColor(sf::Color::Black);

	window->draw(shape);
}

void Renderer::DrawLine(int vertexCount, float pointStart[2], float pointFinish[2], sf::RenderWindow *window, sf::Color color) {
	sf::VertexArray points(sf::LinesStrip, vertexCount);

	//shape.setOrigin(16.0f, 16.0f);

	points[0] = sf::Vector2<float>(pointStart[0], pointStart[1]);
	points[1] = sf::Vector2<float>(pointFinish[0], pointFinish[1]);

	points[0].color = color;
	points[1].color = color;

	window->draw(points);
}
