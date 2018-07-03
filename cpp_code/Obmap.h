#pragma once
#include "stadfx.h"
#include "bgeometry.h"
#include "bgraph.h"
#include "simple_svg.hpp"
#include "ReadConfig.h"
#include "Gwrite.hpp"

template<typename T>
bool writeDebug(std::ofstream &deg, string const &str, vector<T> const &v_val);

namespace pl
{

	using GridIndex = pair<int, int>;
	enum graphType { span, base, tree };
	class STCVert
	{
	public:
		//Left bottom
		GridIndex LB;
		//left top
		GridIndex LT;
		//right bottom
		GridIndex RB;
		//right top
		GridIndex RT;
		double x;
		double y;
	private:

	};
	using  DRingPtr = shared_ptr<bex::DRing>;
	//using GridMap = std::map
	using  GridMap = std::map<std::pair<int, int>,size_t>;
	using STCGridMap = std::map<std::pair<int, int>, STCVert>;
	

	
	class Obmap {
	public:
		Obmap(cfg::ReadConfig const& readCfg):
			m_MaxRow(readCfg._row), m_MaxCol(readCfg._col),_vGridPtr(readCfg._vGridPtr),
			m_sMaxRow(ceil(readCfg._row/2)), m_sMaxCol(ceil(readCfg._col / 2))
		{
			
			string  str_deg = conDir;
			str_deg += "obmapDeg.txt";
			c_deg.open(str_deg, std::ios::trunc);

		}
		template<typename T>
		friend  bool ::writeDebug(std::ofstream &deg, string const &str, vector<T> const &v_val);

		bool map2tGrid();
		bool map2sGrid();

		///
		std::map<std::pair<int, int>, int> tmap2graph;
		std::map<int, std::pair<int, int>> tgraph2map;

		///
		std::map<std::pair<int, int>, int> smap2graph;
		std::map<int, std::pair<int, int>> sgraph2map;

		//

		bex::Graph& gettGraph() { return _tGraph; }
		size_t getGridNum() const { return _tGrid.size(); }
		//
		void writeEgdes();

		
	private:

		//write txt
		std::ofstream c_deg;

		//

		const  shared_ptr<vector<size_t>>_vGridPtr;
		GridMap _tGrid;
		bex::Graph _tGraph;

		GridMap _sGrid;
		bex::Graph _sGraph;

		//最大行
		const size_t m_MaxRow;
		//最大列
		const size_t m_MaxCol;
		//最大s行
		const size_t m_sMaxRow;
		//最大s列
		const size_t m_sMaxCol;

		std::vector<GridIndex> getSearchVerticalNeighbor(GridIndex const &cen_index, size_t const &gridType);

		vector<GridIndex> getTgraphGridInd(GridIndex const & cenInd);
	};
}
