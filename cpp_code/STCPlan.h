#pragma once
#include "Obmap.h"
#include "Gwrite.hpp"
template<typename T>
bool writeDebug(std::ofstream &deg, string const &str, vector<T> const &v_val);

namespace pl {
	class STCPlan
	{
	public:
		STCPlan(Obmap  & obmap):
			_mainMap(obmap)
		{
			string  str_deg = conDir;
			str_deg += "STCDeg.txt";
			c_deg.open(str_deg, std::ios::trunc);
		};
		~STCPlan();

		template<typename T>
		friend  bool ::writeDebug(std::ofstream &deg, string const &str, vector<T> const &v_val);

		void setRange(vector<size_t> const &vVertInd);
		void setRange(vector<GridIndex> const &vInd);

		void setStartPnt(size_t const &startRow, size_t const & startCol);
		//the same type pair can equal 
		void setStartPnt(GridIndex const &ind) { _startPntInd = ind; };
		void plan();
	private:


		//write txt
		std::ofstream c_deg;
		

		Obmap  &_mainMap;
		vector<size_t>  _vVertInd;
		vector<GridIndex> _vGridInd;
		GridIndex _startPntInd;
		void constructGraph();		
	};
}
