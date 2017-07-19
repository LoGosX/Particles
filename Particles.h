#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include <utility>
#include <cmath>
#include <iostream> // for possible debug logs
using namespace std;

class Particles
{
	class MyTime
	{
		sf::Clock clock;
		float lastFrameTime{ 0 };
		float currentFrameTime{ 0 };
	public:
		void PrepareFrame() // has to be called at the beginning of game loop
		{
			currentFrameTime = clock.getElapsedTime().asSeconds();
		}
		void EndFrame() // has to be called at the end of game loop
		{
			lastFrameTime = currentFrameTime;
		}
		float DeltaTime() { return currentFrameTime - lastFrameTime; }
	};

public:

	struct Particle
	{
		sf::Vector2f position{ 0,0 };
		sf::Vector2f velocity{ 0,0 };
		sf::Color color{ sf::Color::White };
		Particle() {}
		Particle(sf::Vector2f _position, sf::Vector2f _velocity, sf::Color _color) : position(_position), velocity(_velocity), color(_color) {}
	};

	Particles();
	Particles(float _pixelsPerSecondMaxSpeed = 1, sf::Color _noSpeedColor = sf::Color::Yellow, sf::Color _maxSpeedColor = sf::Color::Red)
		: MAX_SPEED(_pixelsPerSecondMaxSpeed), NO_SPEED_COLOR(_noSpeedColor), MAX_SPEED_COLOR(_maxSpeedColor) {};
	~Particles();

	void Update();
	void AddVelocity(int _particleId, sf::Vector2f _velocity);
	void AddVelocity(Particle& _particle, sf::Vector2f _velocity);
	void PullParticles(sf::Vector2f _forcePosition, float _forcePower);
	void PushParticles(sf::Vector2f _forcePosition, float _forcePower);
	const vector<Particle>* GetParticles();
	void SpawnParticle(sf::Vector2f _position = { 0,0 }, sf::Vector2f _velocity = { 0,0 });
	void FreezeParticles();
	void ClearParticles();

	MyTime timer;

	bool keep60fps{ true };

private:

	const float MAX_SPEED = 10; // in pixels per second
	const sf::Color NO_SPEED_COLOR; //color of particle when it doesn't move
	const sf::Color MAX_SPEED_COLOR; //color of particle when it moves at MAX_SPEED

	vector<Particle> particles;

	sf::Vector2f Normalize(sf::Vector2f _vector);
	float GetMagnitude(sf::Vector2f _vector);
	sf::Color ColorLerp(sf::Color a, sf::Color b, float t);
};

