#include "Astar.h"



void grid::populate_grid()
{
	{		
		for (int i = 0; i < gridsize; i++) //columns
			for (int j = 0; j < gridsize; j++) //rows
			{				
				tiles[i][j] = new tile;
				tiles[i][j]->idx = i;
				tiles[i][j]->idy = j;
				tiles[i][j]->position.x = i * tilesize;
				tiles[i][j]->position.y = j * tilesize;				
			}

		for (int i = 0; i < gridsize; i++)//set the neighbors for each tile
			for (int j = 0; j < gridsize; j++)
			{
				if (i>0 && tiles[i - 1][j])
				{
					tiles[i][j]->neighbours.push_back(tiles[i - 1][j]);				
				}				
				if (tiles[i + 1][j])
					tiles[i][j]->neighbours.push_back(tiles[i + 1][j]);

				if (j>0 && tiles[i][j - 1])
					tiles[i][j]->neighbours.push_back(tiles[i][j - 1]);
				
				
				if (tiles[i][j + 1])
					tiles[i][j]->neighbours.push_back(tiles[i][j + 1]);
				
				if (tiles[i + 1][j + 1])
					tiles[i][j]->neighbours.push_back(tiles[i + 1][j + 1]);
				
				if (j>0 && tiles[i + 1][j - 1])
					tiles[i][j]->neighbours.push_back(tiles[i + 1][j - 1]);
				
				if (i>0 && tiles[i - 1][j + 1])
					tiles[i][j]->neighbours.push_back(tiles[i - 1][j + 1]);
				
				if (i>0 && j>0 && tiles[i - 1][j - 1])
					tiles[i][j]->neighbours.push_back(tiles[i - 1][j - 1]);
			}
		

	}
}

void grid::drawgrid(sf::RenderWindow& window, grid mygrid, float xoffset, float yoffset)
{
	{

		for (int i = 0; i < mygrid.gridsize; i++)
			for (int j = 0; j < mygrid.gridsize; j++)
			{

				//top line
				sf::Vertex line3[] =
				{
					sf::Vertex(sf::Vector2f((mygrid.tiles[i][j]->position.x + xoffset + mygrid.tilesize / 2) , (mygrid.tiles[i][j]->position.y + yoffset - mygrid.tilesize / 2))),
					sf::Vertex(sf::Vector2f((mygrid.tiles[i][j]->position.x + xoffset - mygrid.tilesize / 2) , (mygrid.tiles[i][j]->position.y + yoffset - mygrid.tilesize / 2)))
				};
				window.draw(line3, 2, sf::Lines);

				//left line
				sf::Vertex line4[] =
				{
					sf::Vertex(sf::Vector2f((mygrid.tiles[i][j]->position.x + xoffset - mygrid.tilesize / 2) , (mygrid.tiles[i][j]->position.y + yoffset - mygrid.tilesize / 2))),
					sf::Vertex(sf::Vector2f((mygrid.tiles[i][j]->position.x + xoffset - mygrid.tilesize / 2) , (mygrid.tiles[i][j]->position.y + yoffset + mygrid.tilesize / 2)))
				};
				window.draw(line4, 2, sf::Lines);


				if (i == mygrid.gridsize-1)
				{

					//right line
					sf::Vertex line2[] =
					{
						sf::Vertex(sf::Vector2f((mygrid.tiles[i][j]->position.x + xoffset + mygrid.tilesize / 2) , (mygrid.tiles[i][j]->position.y + yoffset + mygrid.tilesize / 2))),
						sf::Vertex(sf::Vector2f((mygrid.tiles[i][j]->position.x + xoffset + mygrid.tilesize / 2) , (mygrid.tiles[i][j]->position.y + yoffset - mygrid.tilesize / 2)))
					};
					window.draw(line2, 2, sf::Lines);

				}
				if (j == mygrid.gridsize-1)
				{
					//bottom line
					sf::Vertex line1[] =
					{
						sf::Vertex(sf::Vector2f((mygrid.tiles[i][j]->position.x + xoffset - mygrid.tilesize / 2), (mygrid.tiles[i][j]->position.y + yoffset + mygrid.tilesize / 2))),
						sf::Vertex(sf::Vector2f((mygrid.tiles[i][j]->position.x + xoffset + mygrid.tilesize / 2), (mygrid.tiles[i][j]->position.y + yoffset + mygrid.tilesize / 2)))
					};
					window.draw(line1, 2, sf::Lines);
				}

			}

	}
}

Astar::Astar(): base(base)
{
	for (int i = 0; i <gridsize; i++)//get the free position that can be used for path-finding/spawning
		for (int j = 0; j <gridsize; j++)
		{
			if (adjMatr[i][j] == 1)
			{
				sf::Vector2f temppos;
				temppos.x = i *tilesize;
				temppos.y = j *tilesize;
				freepositions.push_back(temppos);
			}
			
		}

}

Astar::~Astar()
{
}




std::vector<tile*> Astar::findroute(grid* grid,tile* start, tile* finish)
{	
		tile* currentTile = start;
		std::vector<tile*> result;
		//assign every tile a tentative distance value (0 for the root, infinity for the rest of the graph)
		std::vector<tile*>unvisited;
		for (int i = 0; i < grid->gridsize; i++)
			for (int j = 0; j < grid->gridsize; j++)
			{
				if (grid->tiles[i][j] == start)
				{
					grid->tiles[i][j]->tentative_distance = 0;
				}
				else
				{
					grid->tiles[i][j]->tentative_distance = INT_MAX;
					grid->tiles[i][j]->visited = false;
					unvisited.push_back(grid->tiles[i][j]);
				}
			}
		float newtentativedistance=0 ;
		while (finish->visited == false)
		{
			for (std::vector<tile*>::size_type i = 0; i < currentTile->neighbours.size(); i++)
			{
				if (currentTile->neighbours[i]->visited == false)
				{
					
				    newtentativedistance =currentTile->tentative_distance + adjMatr[currentTile->neighbours[i]->idx][currentTile->neighbours[i]->idy];
					if (newtentativedistance < currentTile->neighbours[i]->tentative_distance)
					{
						currentTile->neighbours[i]->tentative_distance = newtentativedistance;
						currentTile->neighbours[i]->parent = currentTile;
					}
				}
			}
			currentTile->visited = true;
			unvisited.erase(std::remove(unvisited.begin(), unvisited.end(), currentTile), unvisited.end());
			//if the target is found, rebuild the road to it and print it
			if (finish->visited == true)
			{
				while (true)//we don't know how long the road from finish to start is so we will use while true
				{
					result.push_back(currentTile);
					currentTile = currentTile->parent;
					if (currentTile == start)
					{
						result.push_back(currentTile);
						break;
					}
				}
				
				return result;
				
			}
			//search for the lowest tentative distance
			int tempTentDist = INT_MAX;
			for (std::vector<tile*>::size_type i = 0; i < unvisited.size(); i++)
			{
				if (unvisited[i]->tentative_distance < tempTentDist)
					tempTentDist = unvisited[i]->tentative_distance;
			}
			
			for (std::vector<tile*>::size_type i = 0; i < currentTile->neighbours.size(); i++)
				if (currentTile->neighbours[i]->visited == false)				
					//set the tile with the lowest tentative distance as currenttile
					for (std::vector<tile*>::size_type j = 0; j < unvisited.size(); j++)
						if (unvisited[j]->tentative_distance == tempTentDist)
						{
							currentTile = unvisited[j];
						}
		}	
}

void Astar::addtile(sf::Vector2f pos)//create a new entity. this will be used for getting the start position of path-finding
{
	if(pos.x<tilesize*gridsize&& pos.y < tilesize * gridsize)
	{
		StaticEntity* Entity_Test = new StaticEntity("static", "assets\\static.png");
		Entity_Test->SetColour(sf::Color::Green);
		int tempx = ((int)pos.x + 50) / 100;
		int tempy = ((int)pos.y + 50) / 100;

	
		Entity_Test->setPosition(tempx * 100, tempy * 100);
		Entity_Test->Initialize();
		Entity_Test->setScale(10, 10);
		drawables.push_back(Entity_Test);

	}

}

tile* grid::findTileForEntity(StaticEntity* entity)//get the tile in which an entity is positioned. works only if the origins are the same
{
	for (int i = 0; i < gridsize; i++)
		for (int j = 0; j < gridsize; j++)
		{
			if ((tiles[i][j]->position.x == entity->getPosition().x) && (tiles[i][j]->position.y == entity->getPosition().y))
				return  tiles[i][j];					
		}
}

void Astar::graphicpresentation(std::vector<tile*>tilesvector, sf::RenderWindow& window)//draw the road resuted after path-finding. debugging purposes
{
	if(drawables.size()==2)
	for (std::vector<tile*>::size_type i = 0; i < tilesvector.size(); i++)
	{
		StaticEntity* Entity_Test = new StaticEntity("static","assets\\static.png");
		Entity_Test->setOrigin(16, 16);
		Entity_Test->SetColour(sf::Color::White);
		Entity_Test->setPosition(tilesvector[i]->position);
		Entity_Test->Initialize();
		Entity_Test->setScale(10, 10);
		drawables.push_back(Entity_Test);
	
		
	}
}

void Astar::deleteSqr(BaseEntity* entity)//clear data used for path-finding
{
	entity->roadDestinations.clear();
	road.clear();

	for (int i = 0; i < drawables.size(); i++)
	{
		delete drawables[i];
		drawables[i] = nullptr;
		for(auto _entity: BaseEntity::Renderables)
			if(_entity==drawables[i])
			{
				
				delete _entity;
				_entity = nullptr;
			}
	}
	drawables.clear();
	
}

void Astar::PrintMatrix()
{
	for (int i = 0; i <gridsize; i++)
	{
		for (int j = 0; j <gridsize; j++)
			std::cout <<adjMatr[i][j] << " ";
		std::cout << std::endl;
	}
}

void Astar::PrintDestinations(BaseEntity* entity)//prints information about the road resulted after path-finding. only for debugging
{
	if (entity->roadDestinations.size() > 0)
	{
		std::cout << entity->roadDestinations.size();
		std::cout << std::endl;
		for (int i = 0; i <= entity->roadDestinations.size() - 1; i++)
		{
			std::cout << entity->roadDestinations[i].x << " " << entity->roadDestinations[i].x << std::endl;
		}

	}
}

bool Astar::closeEnough(sf::Vector2f v1, sf::Vector2f v2,int range)//check if 2 vectors have close enough values
{
	if (((int)v1.x <= (int)v2.x + range) && ((int)v1.x >= (int)v2.x - range))
		if (((int)v1.y <= (int)v2.y + range) && ((int)v1.y >= (int)v2.y - range))
			return true;
	return false;
	
}

StaticEntity* Astar::AddTarget(BaseEntity* entity)//choose a random tile and make it a target for path-finding, this function can be called only after "addtile" function
{
	int x = UtilRandom::instance()->GetRange(0, gridsize - 1);
	int y = UtilRandom::instance()->GetRange(0, gridsize - 1);
	sf::Vector2f tempvec;
	tempvec.x = x *tilesize;
	tempvec.y = y *tilesize;
	if (adjMatr[x][y] != 1 || tempvec == entity->lasttarget)
	{
		 AddTarget(entity);
	}
	else
	{
		StaticEntity* Destination = new StaticEntity("static", "assets\\static.png");
		Destination->setOrigin(16, 16);
		Destination->SetColour(sf::Color::Blue);

		Destination->setPosition(tempvec);
		Destination->Initialize();
		Destination->setScale(10, 10);
		if(drawables[0]->getPosition()==Destination->getPosition())//check if the newly created target is different from the current position
		{
			AddTarget(entity);
		}
		else
		{
			drawables.push_back(Destination);
			return Destination;
		}
		
	}
}

void Astar::PopulateRoadDestination(BaseEntity* entity,grid* mygrid)//find the path between 2 tiles
{
	road =findroute(mygrid, mygrid->findTileForEntity(drawables[0]), mygrid->findTileForEntity(drawables[1]));
	for (int i = road.size() - 1; i >= 0; i--)
	{
		sf::Vector2f tempvector;
		tempvector.x = road[i]->idx * mygrid->tilesize;
		tempvector.y = road[i]->idy * mygrid->tilesize;
		entity->roadDestinations.push_back(tempvector);
	}
}

void Astar::GetRoad(BaseEntity* entity)//create the 2 drawable which will serve as start and finish for pathfinding
{
	addtile(entity->getPosition());
	AddTarget(entity);
}

std::vector<BaseEntity*> Astar::GetBananas(grid mygrid)
{
	std::vector<BaseEntity*> bananas;
	for (int i = 0; i < gridsize; i++)
		for (int j = 0; j < gridsize; j++)
		{if(adjMatr[i][j]==9999)
		{
			StaticEntity* banana = new StaticEntity("banana","assets\\banana.png");
			banana->Initialize();
			banana->setScale(2, 2);
			banana->setOrigin(32, 32);
			banana->setPosition(i * mygrid.tilesize, j * mygrid.tilesize);
			bananas.push_back(banana);
		}						
		}
	return bananas;
}
