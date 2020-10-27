#include "seeker.h"
#include "UtilRandom.h"

std::vector<seeker*> seeker::seekers;

void seeker::Think()
{
	
	sf::Vector2f targetminusboid;
	targetminusboid.x = target.x - getPosition().x;
	targetminusboid.y = target.y - getPosition().y;
	float vectorr = sqrtf(targetminusboid.x * targetminusboid.x + targetminusboid.y * targetminusboid.y);
	targetminusboid.x /= vectorr;
	targetminusboid.y /= vectorr;
	sf::Vector2f steering = targetminusboid - velocity;
	velocity += steering;
	sf::Vector2f pos = getPosition() + (velocity * 0.1f);
	setPosition(pos);
	//if(getPosition()!=target)
		
	/*if (getPosition().x <= target.x - 1  || getPosition().x >= target.x + 1)
		if (getPosition().y <= target.y - 1 || getPosition().y >= target.y + 1)			
			setPosition(pos);*/
		//else
			//setPosition(target);
		
			
		

	float angle = atan2f(velocity.y, velocity.x);
	setRotation(angle * (float)(180 / M_PI));
	

}
void seeker::Initialize()
{
	lasttarget = getPosition();
	
		target.x = UtilRandom::instance()->GetRange(0, 1000);
		target.y = UtilRandom::instance()->GetRange(0, 1000);
	

	// set a default position - could be randomised.

	//int x = UtilRandom::instance()->GetRange(100, 900);
	//int y = UtilRandom::instance()->GetRange(100, 900);

	setPosition(101,101);

	// load the texture into memory
	texture.loadFromFile(filename);
	texture.setSmooth(true);
	// and provide the sprite with a pointer to the texture object
	// if the texture object is destroyed (goes out of scope etc) then the sprite will display weirdly
	sprite.setTexture(texture);
	// set up our colour tint
	sprite.setColor(sf::Color::Red);

	setOrigin(16, 16);

	// select a random angle
	float angle = UtilRandom::instance()->GetRange(0, 361);
	// set our rotation value
	setRotation(angle);
	// and assign a velocity, we need to convert angle to radians so it plays nicely with cos and sin.
	velocity = sf::Vector2f(cos(angle * M_PI / 180), sin(angle * M_PI / 180));

	// add the entity to the list of renderables.
	Renderables.push_back(this);
	seekers.push_back(this);
}

sf::Vector2f seeker::returnTraget()
{
	return target;
}
void seeker::SetTarget(sf::Vector2f newtarget)
{
	target = newtarget;
}


