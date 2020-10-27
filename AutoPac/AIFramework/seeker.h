#pragma once
#include "BaseEntity.h"
 class seeker :public BaseEntity	
{

public:
	
	static std::vector<seeker*> seekers;

	void Think() override;
	void Initialize() override;
	void SetTarget(sf::Vector2f newtarget);
	sf::Vector2f returnTraget();

	
};
 static sf::Vector2f target;




