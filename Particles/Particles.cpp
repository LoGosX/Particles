#include "Particles.h"

Particles::Particles()
{
}


Particles::~Particles()
{
}

sf::Vector2f Particles::Normalize(sf::Vector2f _vector)
{
	float _magnitude = GetMagnitude(_vector);
	return sf::Vector2f(_vector.x / _magnitude, _vector.y / _magnitude);
}

float Particles::GetMagnitude(sf::Vector2f _vector)
{
	return sqrtf(_vector.x *_vector.x + _vector.y * _vector.y);
}

sf::Color Particles::ColorLerp(sf::Color _a, sf::Color _b, float _t)
{
	if (_t > 1) _t = 1;
	_a.r = _a.r + (_b.r - _a.r) * _t;
	_a.g = _a.g + (_b.g - _a.g) * _t;
	_a.b = _a.b + (_b.b - _a.b) * _t;
	return _a;
}

void Particles::Update()
{
	for (auto& particle : particles)
	{
		particle.position += particle.velocity * timer.DeltaTime();
	}
	if (keep60fps == true && 1 < 60 * timer.DeltaTime()) // <-- (1 / timer.DeltaTime()) < 60
	{
		particles.resize(particles.size() * 0.9);
	}
}

void Particles::AddVelocity(int _particleId, sf::Vector2f _velocity)
{
	AddVelocity(particles[_particleId], _velocity);
}

void Particles::AddVelocity(Particle & _particle, sf::Vector2f _velocity)
{
	_velocity *= timer.DeltaTime();
	_particle.velocity += _velocity;
	float _particleSpeed = GetMagnitude(_particle.velocity);
	if (_particleSpeed > MAX_SPEED)
		_particle.velocity = Normalize(_particle.velocity) * MAX_SPEED;
	_particle.color = ColorLerp(NO_SPEED_COLOR, MAX_SPEED_COLOR, _particleSpeed / MAX_SPEED);
}

void Particles::PullParticles(sf::Vector2f _forcePosition, float _forcePower)
{
	for (int i = 0; i < particles.size(); i++)
	{
		sf::Vector2f _direction = Normalize(_forcePosition - particles[i].position);
		AddVelocity(particles[i], _direction * _forcePower);
	}
}

void Particles::PushParticles(sf::Vector2f _forcePosition, float _forcePower)
{
	for (int i = 0; i < particles.size(); i++)
	{
		sf::Vector2f _direction = Normalize(particles[i].position - _forcePosition);
		AddVelocity(particles[i], _direction * _forcePower);
	}
}

const vector<Particles::Particle>* Particles::GetParticles()
{
	return &particles;
}

void Particles::SpawnParticle(sf::Vector2f _position, sf::Vector2f _velocity)
{
	sf::Color _color = ColorLerp(NO_SPEED_COLOR, MAX_SPEED_COLOR, GetMagnitude(_velocity) / MAX_SPEED);
	particles.push_back(Particle(_position, _velocity, _color));
}

void Particles::FreezeParticles()
{
	for (auto& particle : particles)
		particle.velocity = { 0,0 }, particle.color = NO_SPEED_COLOR;
}

void Particles::ClearParticles()
{
	particles.clear();
}
