#pragma once
#include "BaseEntity.h"
class offsetPursuit :
	public BaseEntity
{
public:
	void Think() override;
	void Initialize() override;
};

