#include "flee.h"
#include "UtilRandom.h"

void flee::Think()
{
	sf::Vector2f target;
	target.x = 100;
	target.y = 100;
	sf::Vector2f targetminusboid;
	targetminusboid.x =  getPosition().x-target.x;
	targetminusboid.y =  getPosition().y-target.y;
	float vectorr = sqrtf(targetminusboid.x * targetminusboid.x + targetminusboid.y * targetminusboid.y);
	targetminusboid.x /= vectorr;
	targetminusboid.y /= vectorr;


	sf::Vector2f steering = targetminusboid - velocity;
	velocity += steering;
	sf::Vector2f pos = getPosition() + (velocity * 0.1f);
	setPosition(pos);
	float angle = atan2f(velocity.y, velocity.x);
	setRotation(angle * (float)(180 / M_PI));

}

void flee::Initialize()
{
	// set a default position - could be randomised.

	int x = UtilRandom::instance()->GetRange(200, 600);
	int y = UtilRandom::instance()->GetRange(200, 600);

	setPosition(x, y);

	// load the texture into memory
	texture.loadFromFile(filename);
	texture.setSmooth(true);
	// and provide the sprite with a pointer to the texture object
	// if the texture object is destroyed (goes out of scope etc) then the sprite will display weirdly
	sprite.setTexture(texture);
	// set up our colour tint
	sprite.setColor(sf::Color::Yellow);

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
