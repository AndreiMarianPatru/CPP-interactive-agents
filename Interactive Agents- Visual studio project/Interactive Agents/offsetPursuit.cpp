#include "offsetPursuit.h"
#include "UtilRandom.h"
#include <iostream>
#include <stdexcept>
#include "seeker.h"


void offsetPursuit::Think()
{
	for (auto entity : seeker::seekers)
	{
		if (seeker::seekers.size() > 1)
		{		
			std::cout << "too many seekers";
		}
		else
		{
			sf::Vector2f target;
			
			target = seeker::seekers[0]->getPosition();
			target.x += 20;
			target.y += 20;
			float dist = sqrtf((getPosition().x - target.x) * (getPosition().x - target.x) + (getPosition().y - target.y) * (getPosition().y - target.y));
			float lookahead = dist / 0.05f;
			
			target.x = target.x + (lookahead * seeker::seekers[0]->velocity.x);
			target.y = target.y + (lookahead * seeker::seekers[0]->velocity.y);

			sf::Vector2f targetminusboid;
			targetminusboid.x = target.x - getPosition().x;
			targetminusboid.y = target.y - getPosition().y;
			float vectorr = sqrtf(targetminusboid.x * targetminusboid.x + targetminusboid.y * targetminusboid.y);
			targetminusboid.x /= vectorr;
			targetminusboid.y /= vectorr;


			sf::Vector2f steering = targetminusboid - velocity;
			velocity += steering;
			sf::Vector2f pos = getPosition() + (velocity * 0.1f);
			if (seeker::seekers[0]->getPosition().x <= seeker::seekers[0]->returnTraget().x - 1 || seeker::seekers[0]->getPosition().x >= seeker::seekers[0]->returnTraget().x + 1)
			{
				setPosition(pos);
			}
	
			float angle = atan2f(velocity.y, velocity.x);
			setRotation(angle * (float)(180 / M_PI));

		}

	}	

}

void offsetPursuit::Initialize()
{
	// set a default position - could be randomised.
	int x = UtilRandom::instance()->GetRange(100, 900);
	int y = UtilRandom::instance()->GetRange(100, 900);

	setPosition(x, y);

	// load the texture into memory
	texture.loadFromFile(filename);
	texture.setSmooth(true);
	// and provide the sprite with a pointer to the texture object
	// if the texture object is destroyed (goes out of scope etc) then the sprite will display weirdly
	sprite.setTexture(texture);
	// set up our colour tint
	sprite.setColor(sf::Color::Cyan);

	setOrigin(16, 16);

	// select a random angle
	float angle = UtilRandom::instance()->GetRange(0, 361);
	// set our rotation value
	setRotation(angle);
	// and assign a velocity, we need to convert angle to radians so it plays nicely with cos and sin.
	velocity = sf::Vector2f(cos(angle * M_PI / 180), sin(angle * M_PI / 180));

	// add the entity to the list of renderables.
	Renderables.push_back(this);
}
