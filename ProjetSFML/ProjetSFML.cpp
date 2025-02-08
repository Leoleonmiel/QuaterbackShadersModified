#include "common.h"
#include <vector>
#define S_WIDTH 1920
#define S_HEIGHT 1080

//sf::VertexArray cell(sf::Quads);

std::vector<sf::VertexArray> cells;
float tileWidth = 70;
float tileHeight = 70 / 2;

struct Cell 
{
	sf::VertexArray quad;
	sf::Color color;
	sf::CircleShape particles[50]; 
};

sf::Vector2f Screen2World(sf::Vector2f _screenPos)
{
	sf::Vector2f screenPos;
	screenPos.x = (_screenPos.x / static_cast<float>(tileWidth) / 1.75f) + (_screenPos.y / static_cast<float>(tileHeight));
	screenPos.y = (_screenPos.y / static_cast<float>(tileHeight) / 1.75f) - (_screenPos.x / static_cast<float>(tileWidth));

	return screenPos;
}

sf::Vector2f World2Screen(float _x, float _y, float _z)
{
	sf::Vector2f screenPos{ 0.f, 0.f };

	screenPos.x = (_x * (tileWidth)-_y * (tileWidth));
	screenPos.y = static_cast<float>(_x * (tileWidth / 2.f) + _y * (tileWidth / 2.f) - _z * tileWidth * 0.577f);

	return screenPos;
}
void tile(sf::Vector2f _p, int& index, int i, int j)
{
	cells[index][0].position = sf::Vector2f(_p.x, _p.y - tileHeight);
	cells[index][1].position = sf::Vector2f(_p.x + tileWidth, _p.y);
	cells[index][2].position = sf::Vector2f(_p.x, _p.y + tileHeight);
	cells[index][3].position = sf::Vector2f(_p.x - tileWidth, _p.y);

	std::vector<sf::Color> colors;
	for (int k = 0; k < 4; ++k)
	{
		colors.push_back(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	}
}

void InitGrid(int rows, int cols, const sf::View& view, sf::RenderWindow& _window)
{
	float startX = 3;
	float startY = 0;

	cells.resize(rows * cols, sf::VertexArray(sf::Quads, 4)); // Each cell has 4 vertices

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			float worldX = 11 + i;
			float worldY = 10 - j;
			float worldZ = 0.0f; 

			sf::Vector2f screenPos = World2Screen(worldX, worldY, worldZ);

			int index = (i * cols + j);
			tile(screenPos, index, i, j);
		}
	}
}
float clamp(float value, float min, float max) 
{
	return (value < min) ? min : ((value > max) ? max : value);
}
float lerp(float a, float b, float t) 
{
	return a + t * (b - a);
}
float smoothstep(float edge0, float edge1, float x) 
{
	x = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	return x * x * (3 - 2 * x);
}
int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "My window");
	sf::View camera;
	sf::Clock clock;
	float zoomLevel = -2.f;
	camera.zoom(zoomLevel);

	sf::Vector2f cameraPosition(0.f, 0.f);
	camera.setCenter(cameraPosition);

	int rows = 7;
	int cols = 20;

	InitGrid(rows, cols, window.getView(), window);
	std::cout << "SIZE:" << cells.size() << std::endl;

	float zoomSpeed = 0.1f; 

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		float elapsedTime = clock.getElapsedTime().asSeconds();

		float speedR = 0.1f;
		float speedG = 0.2f;
		float speedB = 0.3f;

		// Update colors over time
		//for (int i = 0; i < cells.size(); ++i)
		//{
		//	for (int k = 0; k < 4; ++k)
		//	{
		//		sf::Color& currentColor = cells[i][k].color;

		//		currentColor.a = 0;
		//		currentColor.r = static_cast<sf::Uint8>(255 * 0.5 * (1 + sin(speedR * elapsedTime)));
		//		currentColor.g = static_cast<sf::Uint8>(255 * 0.5 * (1 + sin(speedG * elapsedTime)));
		//		currentColor.b = static_cast<sf::Uint8>(255 * 0.5 * (1 + sin(speedB * elapsedTime)));
		//	}
		//}


		//for (int i = 0; i < cells.size(); ++i)
		//{
		//	for (int k = 0; k < 4; ++k)
		//	{
		//		sf::Color& currentColor = cells[i][k].color;

		//		float speedR = 0.1f;
		//		float speedG = 0.2f;
		//		float speedB = 0.3f;

		//		sf::Vector2f quadCenter = (cells[i][0].position + cells[i][2].position) * 0.5f;
		//		float distance = std::sqrt(std::pow(cells[i][k].position.x - quadCenter.x, 2) + std::pow(cells[i][k].position.y - quadCenter.y, 2));
		//		float gradientIntensity = 1.5f - (distance / 6.f);

		//		currentColor.r = static_cast<sf::Uint8>(255 * 0.5 * (1 + gradientIntensity * sin(speedR * elapsedTime)));
		//		currentColor.g = static_cast<sf::Uint8>(255 * 0.5 * (1 + gradientIntensity * sin(speedG * elapsedTime)));
		//		currentColor.b = static_cast<sf::Uint8>(255 * 0.5 * (1 + gradientIntensity * sin(speedB * elapsedTime)));
		//		currentColor.a = static_cast<sf::Uint8>(255 * gradientIntensity);
		//	}
		//}

		// Update colors over time
		for (int i = 0; i < cells.size(); ++i)
		{
			for (int k = 0; k < 4; ++k)
			{
				sf::Color& currentColor = cells[i][k].color;

				float speedR = 0.1f;
				float speedG = 0.2f;
				float speedB = 0.3f;

				sf::Vector2f quadCenter = (cells[i][0].position + cells[i][2].position) * 0.5f;
				float distance = std::sqrt(std::pow(cells[i][k].position.x - quadCenter.x, 2) + std::pow(cells[i][k].position.y - quadCenter.y, 2));
				float gradientIntensity = 1.5f - (distance / 6.f);
				float cellTimeOffset = 0.2f * i;

				currentColor.r = static_cast<sf::Uint8>(255 * 0.5 * (1 + gradientIntensity * sin(speedR * (elapsedTime + cellTimeOffset))));
				currentColor.g = static_cast<sf::Uint8>(255 * 0.5 * (1 + gradientIntensity * sin(speedG * (elapsedTime + cellTimeOffset))));
				currentColor.b = static_cast<sf::Uint8>(255 * 0.5 * (1 + gradientIntensity * sin(speedB * (elapsedTime + cellTimeOffset))));
				currentColor.a = static_cast<sf::Uint8>(255 * gradientIntensity);
			}
		}
		 
		window.clear(sf::Color::Black);

		for (const auto& cell : cells)
		{
			window.draw(cell);
		}

		sf::Color outlineColor = sf::Color::White; 
		for (int i = 0; i < cells.size(); ++i)
		{
			for (int k = 0; k < 4; ++k)
			{
				window.draw(cells[i]);

				sf::VertexArray outline(sf::LineStrip, 5);
				for (int k = 0; k < 4; ++k)
				{
					outline[k].position = cells[i][k].position;
					outline[k].color = outlineColor;
				}
				outline[4] = outline[0]; 
				window.draw(outline);
			}
		}

		window.display();
	}

	return 0;
}

