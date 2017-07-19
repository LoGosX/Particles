#include <SFML/Graphics.hpp>
#include "Particles.h"
#include <iostream>
#include <cmath>
using namespace std;

void SpawnProjectilesHorizontal(Particles& particleManager, sf::Vector2f _position = { 0,0 }, int _amount = 100)
{
	for (sf::Vector2f _pos = _position; _pos.x < _position.x + _amount * 10; _pos.x += 10)
		particleManager.SpawnParticle(_pos);
}

void SpawnProjectilesVertical(Particles& particleManager, sf::Vector2f _position = { 0,0 }, int _amount = 100)
{
	for (sf::Vector2f _pos = _position; _pos.y < _position.y + _amount * 10; _pos.y += 10)
		particleManager.SpawnParticle(_pos);
}

void SpawnProjectilesAtCircle(Particles& particleManager, sf::Vector2f _position = { 0,0 }, int _amount = 100, float _radious = 5)
{
	constexpr float PI = 3.14;
	float angleGain = 2 * PI / (_amount + 1);
	sf::Vector2f r{ _radious,0 };
	for (float angle = 0; angle <= 2 * PI; angle += angleGain)
	{
		particleManager.SpawnParticle(_position + r);
		float ca = cos(angleGain);
		float sa = sin(angleGain);
		r = sf::Vector2f(ca*r.x - sa*r.y, sa*r.x + ca*r.y);
	}
}

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom) //https://github.com/SFML/SFML/wiki/Source:-Zoom-View-At-(specified-pixel)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
}

struct FakeMouse
{
	sf::Vector2f position{ 0,0 };
	bool enabled{ false };
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1200), "SFML works!", sf::Style::Fullscreen);

	sf::View view;

	view.reset(sf::FloatRect(window.getSize().x / 2, window.getSize().y / 2, window.getSize().x, window.getSize().y));
	window.setView(view);

	Particles particleManager(5000,sf::Color::Blue, sf::Color::Red);
	const vector<Particles::Particle>* particles = particleManager.GetParticles();

	bool mouseDown = false;
	float pullPower = 2400; // acceleration of 'pullPower' pixels per second
	float zoomAmount = 1.25;
	FakeMouse fakeMouse;

	sf::Clock clock;
	sf::Time tick;

	int avgSum = 0;
	int ticks = 0;
	int fps = 0;
	int maxParticleCount = 0;

	while (window.isOpen())
	{
		fps = 1 / clock.restart().asSeconds();
		avgSum += fps;
		ticks++;

		cout << "Avg: " << avgSum / ticks << "\tFPS: " << fps << "\tParticle count: " << particles->size() << "\tParticles per fps: " << float(particles->size()) / fps << "\tMax particle count: " << (maxParticleCount = maxParticleCount < particleManager.GetParticles()->size() ? particleManager.GetParticles()->size() : maxParticleCount) << endl;
		
		particleManager.SpawnParticle();

		particleManager.timer.PrepareFrame();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				#pragma region keyboardControls

				if (event.key.code == sf::Keyboard::A)
				{
					sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					particleManager.SpawnParticle(mousePosition);
				}
				else if (event.key.code == sf::Keyboard::F)
				{
					particleManager.FreezeParticles();
				}
				else if (event.key.code == sf::Keyboard::C)
				{
					particleManager.ClearParticles();
				}
				else if (event.key.code == sf::Keyboard::Z)
				{
					fakeMouse.enabled = !fakeMouse.enabled;
					fakeMouse.position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				}
				else if (event.key.code == sf::Keyboard::X)
				{
					sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					particleManager.PushParticles(mousePosition, 10000*pullPower);
				}
				else if (event.key.code == sf::Keyboard::Num1)
				{
					sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					SpawnProjectilesHorizontal(particleManager, mousePosition,1000);
				}
				else if (event.key.code == sf::Keyboard::Num2)
				{
					sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					SpawnProjectilesVertical(particleManager, mousePosition, 1000);
				}
				else if (event.key.code == sf::Keyboard::Num3)
				{
					sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					SpawnProjectilesAtCircle(particleManager, mousePosition, 1000, 100);
				}
				else if (event.key.code == sf::Keyboard::Num4)
				{
					sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					SpawnProjectilesAtCircle(particleManager, mousePosition, 2000, 200);
				}
				else if (event.key.code == sf::Keyboard::Num5)
				{
					sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					SpawnProjectilesAtCircle(particleManager, mousePosition, 4000, 400);
				}
				else if (event.key.code == sf::Keyboard::Num6)
				{
					sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					SpawnProjectilesAtCircle(particleManager, mousePosition, 80000, 8000);
				}
				
				#pragma endregion keyboardControls
			}
			
			if (event.type == sf::Event::MouseButtonPressed)
			{
				mouseDown = true;
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				mouseDown = false;
			}
			else if (event.type == sf::Event::MouseButtonReleased && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			{
				// TODO mouse camera movement
			}

			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.delta > 0)
					zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / zoomAmount));
				else if (event.mouseWheelScroll.delta < 0)
					zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, zoomAmount);
			}
		}

		if (mouseDown)
		{
			sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			particleManager.PullParticles(mousePosition, pullPower);
		}

		if (fakeMouse.enabled)
		{
			particleManager.PullParticles(fakeMouse.position, pullPower);
		}

		particleManager.Update();
		particleManager.timer.EndFrame();

		window.clear();
		
		sf::VertexArray vertices; //particles
		for (auto& particle : *particles)
			vertices.append(sf::Vertex(particle.position, particle.color));

		window.draw(vertices);
		window.display();

	}

	return 0;
}