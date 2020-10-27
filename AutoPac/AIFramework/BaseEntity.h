#pragma once

/**
 * BaseEntity, the basic building block of any AI agent in the framework.
 * 
 * This is the parent class for any agents desired to be added to the system,
 * it inherits from sf::Transformable to give us access to set / get position and related functionality.
 */

#include <string>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>



// we need to tell the compiler that it's ok to use the default defines from math.h, if we don't, we don't get access to M_PI!
#define _USE_MATH_DEFINES
#include <math.h>

class Astar;
class grid;
class State;
class seekClass;

class BaseEntity : public sf::Transformable
{
protected:
	std::string filename;
	sf::Texture texture;
	sf::Color colourTint;
	sf::Vector2f velocity;
public:
	std::string Statename;
	Astar* astar;
	grid* grid;
	int score;
	State* current_state;
	std::string name;
	bool readyToUpdate;
	bool readyForNextState;
	std::vector<sf::Vector2f> roadDestinations;
	sf::Vector2f lasttarget;
	sf::Sprite sprite;
	sf::Vector2f target;

	
public:
	/**
	 * static std vector of BaseEntity pointers, currently used to loop all entities for rendering and ticking.
	 */
	static std::vector<BaseEntity*> Renderables;
	static std::vector<BaseEntity*> playersVect;

public:
	/**
	 * Base constructor, defaults all variables.
	 */
	BaseEntity(std::string name);
	/**
	 * Constructor that accepts a texture file name
	 * @param file a string filename
	 */
	explicit BaseEntity(std::string name,std::string file);
	/**
	 * Constructor that accepts a texture file name and a colour tint
	 * @param file a string filename
	 * @param colour a sf::Color value
	 */
	explicit BaseEntity(std::string name,std::string file, sf::Color colour);

	/**
	 * Deconstructor
	 */
	virtual ~BaseEntity();
	/**
	 * Initialize assures our texture is loaded using the assigned file name and initial colour tint
	 */
	virtual void Initialize();
	/** 
	 * Think is the standard "update" function for all agents
	 * All agents must implement their own Think function - there is nothing provided in the base class.
	 */
	virtual void Think();

	/**
	 * GetSprite provides access to the entities sprite object
	 * @return sf::Sprite current entities sprite
	 */
	sf::Sprite GetSprite() { return sprite; }

	/**
	 * SetColour allows for post-construction alteration of the colour tint
	 * @param colour a sf::Colour value
	 */
	void SetColour(sf::Color colour) { colourTint = colour; }

	/**
	* SetTextureFile is used to set the filename variable
	* @param string a filename
	*/
	void SetTextureFile(std::string file) { filename = file; }
	//void SetTarget(sf::Vector2f target);
	

	//movement function
	void seek(sf::Vector2f target, float speed);


	//fsm
	void setCurrentState(std::string state,float boost);
	void CheckInfo(sf::Window& window, sf::Vector2i mousepos, int range);

	//info
	void CheckInfo(sf::Window& window, sf::Vector2f mousepos, int range);
		
};

class State
{
public:

	BaseEntity* Stateowner;
	float stateboost;
	virtual void onenter();
	virtual void update();
	virtual void onexit();
};
class seekClass :public State
{
public:
    
	BaseEntity* target;
	seekClass(BaseEntity* owner);
	~seekClass();
	void onenter() override;
	void update() override;
	void onexit() override;
};
class pathfindingClass :public State
{
public:
	explicit pathfindingClass(BaseEntity* owner);
	~pathfindingClass();
	void onenter() override;
	void update() override;
	void onexit() override;
};