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
		void setRange(vector<size_t> const &vVertInd);
		void plan();
	private:
		Obmap  &_mainMap;
		vector<size_t>  _vVertInd;
	};
}
