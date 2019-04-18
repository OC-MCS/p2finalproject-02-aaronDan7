#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

// Missiles to shoot aliens; spawning / despawning dependent on Missiles class
class Missile
{
private:
	Sprite  missileSprite; // sprite depicting missile, holding location
public:
	// constructor; pass texture by reference; ship pos determines spawn location
	Missile(Texture &missileTexture, Vector2f shipPos)
	{
		missileSprite.setTexture(missileTexture);
		missileSprite.setPosition(shipPos.x + (2 * missileTexture.getSize().x), shipPos.y);
	}

	// change sprite's position, return false if off screen to initiate destruction
	bool moveUp()
	{
		bool continueFlight = true; // tell program to delete dynamically alocated sprite if true
		if (missileSprite.getPosition().y > 0)
		{
			missileSprite.move(0, -4.0f);
		}
		else
		{
			continueFlight = false;
		}
		return continueFlight;
	}
	// for drawing the missile's sprite
	Sprite getSprite()
	{
		return missileSprite;
	}

	// for collisions (check for collision with aliens)
	FloatRect getCollision()
	{
		return missileSprite.getGlobalBounds();
	}
};