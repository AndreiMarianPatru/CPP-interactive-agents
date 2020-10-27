#pragma once
#include "BaseEntity.h"
class StaticEntity :
	public BaseEntity
{
public:
	using BaseEntity::BaseEntity;

	virtual void Think() override {};

	virtual void Initialize() override;
};

