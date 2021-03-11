#include "BaseEntity.h"
#include "UtilRandom.h"
#include "Astar.h"


std::vector<BaseEntity*> BaseEntity::Renderables;
std::vector<BaseEntity*> BaseEntity::playersVect;


BaseEntity::BaseEntity(std::string name)
{
	// with no further information, we make some assumptions and set default values.
	filename = "assets\\boid.png";
	colourTint = sf::Color::White;
	this->name = name;
}

BaseEntity::BaseEntity(std::string name,std::string file) : filename(file)
{
	// with just a texture file, we default to a white tint (so no tint at all).
	colourTint = sf::Color::White;
}

BaseEntity::BaseEntity(std::string name,std::string file, sf::Color colour) : filename(file), colourTint(colour)
{
}

BaseEntity::~BaseEntity()
{
}

void BaseEntity::Think()
{
	if(current_state!=nullptr)
		current_state->update();
}



void BaseEntity::Initialize()
{
	readyToUpdate = false;
	readyForNextState = false;
	score = 0;

	// set a default position - could be randomised.
	float x = UtilRandom::instance()->GetRange(200, 600);
	float y = UtilRandom::instance()->GetRange(200, 600);
	// load the texture into memory
	texture.loadFromFile(filename);
	texture.setSmooth(true);
	// and provide the sprite with a pointer to the texture object
	// if the texture object is destroyed (goes out of scope etc) then the sprite will display weirdly
	sprite.setTexture(texture);
	// set up our colour tint
	sprite.setColor(colourTint);
	setOrigin(16, 16);
	// select a random angle
	float angle = UtilRandom::instance()->GetRange(10, 80);
	// set our rotation value
	setRotation(angle);
	// and assign a velocity, we need to convert angle to radians so it plays nicely with cos and sin.
	velocity = sf::Vector2f(cos(angle * M_PI / 180), sin(angle * M_PI / 180));

	// add the entity to the list of renderables.
	
	Renderables.push_back(this);
}


void BaseEntity::seek(sf::Vector2f target,float boost)
{
	
	sf::Vector2f targetminusboid = target - getPosition();
	float vectorr = sqrtf(targetminusboid.x * targetminusboid.x + targetminusboid.y * targetminusboid.y);
	targetminusboid.x /= vectorr;
	targetminusboid.y /= vectorr;
	sf::Vector2f steering = targetminusboid - velocity;
	velocity += steering;
	sf::Vector2f pos = getPosition() + (velocity * boost);
	setPosition(pos);
	float angle = atan2f(velocity.y, velocity.x);
	setRotation(angle * (float)(180 / M_PI));
}



void BaseEntity::setCurrentState(std::string state,float boost)
{
	if(state=="seek")
	{
		seekClass* seek = new seekClass(this);
		seek->stateboost = boost;
		current_state = seek;
	}
	if(state=="pathfinding")
	{
		pathfindingClass* pathf = new pathfindingClass(this);
		pathf->stateboost = boost;
		current_state = pathf;
	}
}

void BaseEntity::CheckInfo(sf::Window& window, sf::Vector2f mousepos,int range)
{

	if ((mousepos.x <=getPosition().x + range)&&(mousepos.x >=getPosition().x - range)&&(mousepos.y <=getPosition().y + range)&&(mousepos.y >=getPosition().y - range))
	{
		std::cout << "===============================" << std::endl;
		std::cout << this->name << std::endl;
		std::cout << "Position: x: " << getPosition().x << " y: " << getPosition().y << std::endl;
		std::cout << "Score: " << this->score<<std::endl;
		std::cout << "State: " << this->Statename << std::endl;
		std::cout << "Target: x: " << this->target.x << " y: " << this->target.y << std::endl;
		std::cout << "===============================" << std::endl;


	}
	
}


void State::onenter()
{
}

void State::update()
{
}

void State::onexit()
{
}

seekClass::seekClass(BaseEntity* owner)
{
	Stateowner = owner;
	Stateowner->readyForNextState = false;
	Stateowner->Statename = "seeking";
	onenter();
}

seekClass::~seekClass()
{
}

void seekClass::onenter()
{
		std::cout << "Entity " << Stateowner->name << " is now seeking"<<std::endl;
		Stateowner->sprite.setColor(sf::Color::Red);
		for (auto player : BaseEntity::playersVect)	
			if (player->Statename=="pathfinding")
				target = player;
}

void seekClass::update()
{
	if (Stateowner->astar->closeEnough(Stateowner->getPosition(),target->getPosition(), 50))
	{
		onexit();
	}
	else
		Stateowner->seek(target->getPosition(), stateboost);

}

void seekClass::onexit()
{
	Stateowner->sprite.setColor(sf::Color::Green);
	for (auto entity : BaseEntity::playersVect)
		Stateowner->astar->deleteSqr(entity);

	Stateowner->readyForNextState = true;
}

pathfindingClass::pathfindingClass(BaseEntity* owner)
{
	
	Stateowner = owner;
	Stateowner->readyForNextState = false;
	Stateowner->Statename = "pathfinding";

	onenter();

}

void pathfindingClass::onenter()
{
	std::cout << "Entity " << Stateowner->name << " is now pathfinding" << std::endl;
	Stateowner->sprite.setColor(sf::Color::Blue);
	
	Stateowner->astar->GetRoad(Stateowner);
	Stateowner->astar->PopulateRoadDestination(Stateowner, Stateowner->grid);
	if (Stateowner->astar->road.size() > 0 )
	{
		Stateowner->setPosition(Stateowner->roadDestinations[0].x, Stateowner->roadDestinations[0].y);
		Stateowner->target = Stateowner->roadDestinations[1];
		Stateowner->roadDestinations.erase(Stateowner->roadDestinations.begin());
		
	}
	
}

void pathfindingClass::update()
{
	if (Stateowner->roadDestinations.size() > 0)
		Stateowner->seek(Stateowner->target, stateboost);
	if (Stateowner->astar->closeEnough(Stateowner->target, Stateowner->getPosition(), 2))
	{
		if (Stateowner->roadDestinations.size() > 1)
		{
			Stateowner->target = Stateowner->roadDestinations[1];
			Stateowner->roadDestinations.erase(Stateowner->roadDestinations.begin());
		}
		else
		{
			onexit();
		}
	}
}

void pathfindingClass::onexit()
{

	Stateowner->sprite.setColor(sf::Color::Green);
	Stateowner->astar->deleteSqr(Stateowner);

	Stateowner->lasttarget = Stateowner->target;

	Stateowner->readyForNextState = true;


}


