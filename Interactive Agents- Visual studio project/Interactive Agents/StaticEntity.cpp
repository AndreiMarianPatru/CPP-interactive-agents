#include "StaticEntity.h"

void StaticEntity::Initialize()
{
	// load the texture into memory
	texture.loadFromFile(filename);
	texture.setSmooth(true);
	// and provide the sprite with a pointer to the texture object
	// if the texture object is destroyed (goes out of scope etc) then the sprite will display weirdly
	sprite.setTexture(texture);
	// set up our colour tint
	sprite.setColor(colourTint);

	setOrigin(4, 4);

	// add the entity to the list of renderables.
	//Renderables.push_back(this);
}
