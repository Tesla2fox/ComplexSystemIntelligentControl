#pragma once
#include "Obmap.h"
#include "Gwrite.hpp"

namespace pl {
	class GroupVert
	{
	public:
		GroupVert(Obmap &ob_map):
			_mainMap(ob_map)
		{
			c_deg.open("D:\\py_code\\adversarialCover\\data\\map_deg.txt", std::ios::trunc);
		};
		
		template<typename T>
		friend  bool ::writeDebug(std::ofstream &deg, string const &str, vector<T> const &v_val);

		bool to_Group();
		~GroupVert();
	private:
		//write txt
		std::ofstream c_deg;

		Obmap &_mainMap;
		vector<vector<GridIndex>> _Group;
	};
}

