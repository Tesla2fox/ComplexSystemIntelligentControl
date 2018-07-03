#pragma once
#include "Obmap.h"

namespace pl {
	class STCPlan
	{
	public:
		STCPlan(Obmap  & obmap):
			_mainMap(obmap)
		{};
		~STCPlan();
		
		void plan();
	private:
		Obmap  &_mainMap;
	};
}
