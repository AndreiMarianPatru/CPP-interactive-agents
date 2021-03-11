#pragma once
#include "BaseEntity.h"
class arrival :
	public BaseEntity
{
public:
	void Think() override;
	void Initialize() override;
	void SetTarget(sf::Vector2f newtarget);
	sf::Vector2f returnTraget();

};
static sf::Vector2f target;



