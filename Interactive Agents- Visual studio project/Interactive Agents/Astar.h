#pragma once
#include <SFML/Graphics.hpp>
#include "BaseEntity.h"
#include "StaticEntity.h"
#include <iostream>
#include <math.h>
#include <sstream>
#include "UtilRandom.h"



class tile
{
public:
	int idx;
	int idy;
	std::vector<tile*> neighbours;
	float tentative_distance;
	bool visited;
	tile* parent = nullptr;
	sf::Vector2f position;
};
class grid: public tile
{
public:
	
	int tilesize=100;
	static const int gridsize = 13;//how big the grid is
	tile* tiles[gridsize+1 ][gridsize +1];
	void populate_grid();
	tile* findTileForEntity(StaticEntity* entity);

	void drawgrid(sf::RenderWindow& window, grid mygrid, float xoffset, float yoffset);
	
};


class Astar :public grid
{
	
	public:
		std::vector<tile*> road;
		BaseEntity base;

		std::vector<StaticEntity*> drawables;
		std::vector<StaticEntity*> pathtargets;
	std::vector<StaticEntity*>Obstacles;

	int adjMatr[gridsize][gridsize] =
   {9999,9999,9999,9999,9999,9999,9999,9999,9999,9999,9999,9999,9999,
	9999,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,9999,
	9999,   1,9999,9999,9999,   1,9999,   1,9999,9999,9999,   1,9999,
	9999,   1,9999,   1,   1,   1,9999,   1,   1,   1,9999,   1,9999,
	9999,   1,9999,   1,9999,   1,9999,   1,9999,   1,9999,   1,9999,
	9999,   1,   1,   1,   1,   1,9999,   1,   1,   1,   1,   1,9999,
	9999,   1,9999,9999,9999,9999,   1,9999,9999,9999,9999,   1,9999,
	9999,   1,   1,   1,   1,   1,9999,   1,   1,   1,   1,   1,9999,
	9999,   1,9999,   1,9999,   1,9999,   1,9999,   1,9999,   1,9999,
	9999,   1,9999,   1,   1,   1,9999,   1,   1,   1,9999,   1,9999,
	9999,   1,9999,9999,9999,   1,9999,   1,9999,9999,9999,   1,9999,
	9999,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,9999,
	9999,9999,9999,9999,9999,9999,9999,9999,9999,9999,9999,9999,9999,
	};
	std::vector<sf::Vector2f> freepositions;//store sf::vector2 positions that are not occupied by a Obstacle

	Astar();
	~Astar();
	std::vector<BaseEntity*> GetObstacles(grid mygrid);
	std::vector<tile*> findroute(grid* grid,tile*start, tile* finish);
	void addtile(sf::Vector2f);
	void graphicpresentation(std::vector<tile*> tilesvector, sf::RenderWindow& window);
	void deleteSqr(BaseEntity* entity);
	void PrintMatrix();
	void PrintDestinations(BaseEntity* entity);
	bool closeEnough(sf::Vector2f v1, sf::Vector2f v2, int range);// check if 2 vectors have similar values
	StaticEntity* AddTarget(BaseEntity* entity);
	void PopulateRoadDestination(BaseEntity* entity, grid* mygrid);
	void GetRoad(BaseEntity* entity);
};

