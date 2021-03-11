#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include "BaseEntity.h"
#include "UtilRandom.h"


#include "StaticEntity.h"
#include "Astar.h"
#include "seeker.h"

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
}


int main()
{

	
	grid* mygrid = new grid();
	Astar* myastar = new Astar();

	mygrid->populate_grid();
	std::vector<BaseEntity*> bb;
	bb = myastar->GetObstacles(*mygrid);
	






	// construct a window that uses a resolution of 800 x 600
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "AI Framework");

	// because we're running as a console application, we still get access to the console, so can use std::cout to print to it
	std::cout << "Constructed SFML Window" << std::endl;
	std::cout << "Click on a player for more info!" << std::endl;
	window.setFramerateLimit(60);
	sf::View view(window.getDefaultView());
	const float zoomAmount{ 1.1f };



	BaseEntity* player1 = new BaseEntity("player1");
	player1->Initialize();
	player1->setPosition(101, 101);
	player1->astar = myastar;
	player1->grid = mygrid;
	BaseEntity::playersVect.push_back(player1);



	BaseEntity* player2 = new BaseEntity("player2");
	player2->Initialize();
	player2->setPosition(1101, 1101);
	player2->astar = myastar;
	player2->grid = mygrid;
	BaseEntity::playersVect.push_back(player2);

	BaseEntity* player3 = new BaseEntity("player3");
	player3->Initialize();
	player3->setPosition(1101, 1101);
	player3->astar = myastar;
	player3->grid = mygrid;
	BaseEntity::playersVect.push_back(player3);

	player1->setCurrentState("pathfinding",1.75);
	player2->setCurrentState("seek",1.25);
	player3->setCurrentState("seek", 1.25);



	

	
	//myastar->PopulateRoadDestination(sic,mygrid);
	// our game loop
	while (window.isOpen())
	{
		// poll for events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
			{
				window.close();
				break;
			}
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::BackSpace)//get back to normal view
				{
					view = window.getDefaultView();
					window.setView(view);
					break;
				}
			case sf::Event::MouseWheelScrolled://zoom using scroll wheel			
			{
				if (event.mouseWheelScroll.delta > 0)
					zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / zoomAmount));
				else if (event.mouseWheelScroll.delta < 0)
					zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, zoomAmount);
				break;
			}
			case sf::Event::MouseButtonPressed:
				switch (event.key.code)
				{
				case sf::Mouse::Left:
					for (auto player : BaseEntity::playersVect)
					{
						auto mouse_pos = sf::Mouse::getPosition(window); // Mouse position relative to the window
						auto translated_pos = window.mapPixelToCoords(mouse_pos); // Mouse position translated into world coordinates
						player->CheckInfo(window, translated_pos, 50);
					}
						
					break;
				}

			}
			

		}

	


		for(auto player : BaseEntity::playersVect)//chack if any player completed a task, if not continue "thinking" 
		{
			if(player->readyForNextState==true)//a task is completed
			{
				player->score++;
				std::cout << "=========================================" << std::endl;

				//make the players ready for a new state
				player1->current_state = nullptr;
				player2->current_state = nullptr;
				player3->current_state = nullptr;

				//respawn them randomly
				int temp1 = UtilRandom::instance()->GetRange(0, myastar->freepositions.size());
				int temp2 = UtilRandom::instance()->GetRange(0, myastar->freepositions.size());
				int temp3 = UtilRandom::instance()->GetRange(0, myastar->freepositions.size());
				player1->setPosition(myastar->freepositions[temp1]);
				player2->setPosition(myastar->freepositions[temp2]);
				player3->setPosition(myastar->freepositions[temp3]);
				
				
				std::cout << "Entity " << player1->name << " score: " << player1->score<<std::endl;
				std::cout << "Entity " << player2->name << " score: " << player2->score << std::endl;
				std::cout << "Entity " << player3->name << " score: " << player3->score << std::endl;
				std::cout << "=========================================" << std::endl;



				int temp = UtilRandom::instance()->GetRange(0, 3);
				switch(temp)
				{
				case 0:
					player1->setCurrentState("pathfinding", 1.75);
					player2->setCurrentState("seek", 1.25);
					player3->setCurrentState("seek", 1.25);
					break;
				case 1:
					player2->setCurrentState("pathfinding", 1.75);
					player1->setCurrentState("seek", 1.25);
					player3->setCurrentState("seek", 1.25);
					break;
				case 2:
					player3->setCurrentState("pathfinding", 1.75);
					player2->setCurrentState("seek", 1.25);
					player1->setCurrentState("seek", 1.25);
					break;
				}
			}
			else
			{
				player1->Think();
				player2->Think();
				player3->Think();
			}

		}

		
		/*if (sic->roadDestinations.size() > 0)
			sic->Think();
		else
		{
			myastar->deleteSqr(sic);
			myastar->GetRoad(sic);
			myastar->PopulateRoadDestination(sic,mygrid);

		}

		if (myastar->road.size() > 0 && put == true)
		{
			sic->setPosition(sic->roadDestinations[0].x, sic->roadDestinations[0].y);
			sic->SetTarget(sic->roadDestinations[1]);
			sic->roadDestinations.erase(sic->roadDestinations.begin());		
			put = false;
		}


		if (myastar->closeEnough(sic->returnTraget(), sic->getPosition()))
		{
			myastar->PrintDestinations(sic);
			if (sic->roadDestinations.size() > 1)
			{
				
				std::cout << "next " << std::endl;
				sic->SetTarget(sic->roadDestinations[1]);
				sic->roadDestinations.erase(sic->roadDestinations.begin());
			}
			else
			{
				sic->roadDestinations.clear();


			}*/
	
		
		// clear the window before drawing anything to it
		window.clear();



		mygrid->drawgrid(window, *mygrid, 0, 0);
		//myastar->graphicpresentation(myastar->road, window); // "draw" the road resulted after path-finding

		for (auto entity : myastar->drawables)//draw the start and finish for path-finding
		{
			window.draw(entity->GetSprite(), entity->getTransform());
		}

		for(auto Obstacle :bb)//draw the obstacles
		{
			window.draw(Obstacle->GetSprite(), Obstacle->getTransform());

		}

		
		for(auto player :BaseEntity::Renderables)//draw players
		{
			window.draw(player->GetSprite(), player->getTransform());

		}


		// and then draw them

		// swap the frame buffers
		window.display();
	}

	return 0;
}
