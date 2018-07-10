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
	enum DirType { left, bottom, right, top, center };
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
		vector<GridIndex> _vGridIndex;
	private:

	};
	using  DRingPtr = shared_ptr<bex::DRing>;
	//using GridMap = std::map
	using  GridMap = std::map<std::pair<int, int>,size_t>;
	using STCGridMap = std::map<std::pair<int, int>, STCVert>;
	
	
	// used for judge connectness.
	using CGraph = boost::adjacency_list<bt::vecS, bt::vecS, bt::undirectedS>;

	
	class Obmap {
	public:
		Obmap(cfg::ReadConfig const& readCfg):
			m_MaxRow(readCfg._row), m_MaxCol(readCfg._col),_vGridPtr(readCfg._vGridPtr),
			m_sMaxRow(ceil(double(readCfg._row)/2)), m_sMaxCol(ceil(double(readCfg._col) / 2)), _vObGridInd(readCfg._vObGridInd)
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
		void writeEdges(size_t const & type = graphType::base);
		void writeEdgesInPnt(size_t const & type = graphType::base);
		//void writeEgdes(size_t const & type = graphType::base);

		GridIndex tGridInd2SGridInd(GridIndex const & ind);
		bex::Graph &getGraph(const size_t &type) { if (type == graphType::base) { return _tGraph; } return _sGraph; }
		GridMap &getGridMap(const size_t &type) { if (type == graphType::base) { return _tGrid; } return _sGrid; }
		//
		bool allConnected(vector<bex::VertexDescriptor> const &vvd);

		bool allConnectedBase(vector<bex::VertexDescriptor> const &vvd);

		size_t getReachableVertNum()const { return _reachableVertNum; };


	private:

		//write txt
		std::ofstream c_deg;

		//
		const shared_ptr<vector<size_t>>_vGridPtr;
		const shared_ptr<vector<cfg::MatPnt>> _vObGridInd;

		
		GridMap _tGrid;
		bex::Graph _tGraph;

		GridMap _sGrid;
		bex::Graph _sGraph;
		STCGridMap _STCGrid;

		size_t _reachableVertNum;
		size_t _obVertNum;

		//最大行
		size_t m_MaxRow;
		//最大列
		size_t m_MaxCol;
		//最大s行
		const size_t m_sMaxRow;
		//最大s列
		const size_t m_sMaxCol;

		std::vector<GridIndex> getSearchVerticalNeighbor(GridIndex const &cen_index, size_t const &gridType);
		std::vector<GridIndex> getSTCVerticalNeighbor(GridIndex const& cen_index);

		vector<GridIndex> getTgraphGridInd(GridIndex const & cenInd);
		bool allObstacle(vector<GridIndex> const &vindex);

		bool gridObstacle(GridIndex const & cenInd);
		int gridObNum(vector<GridIndex> const &vindex);
		bool obstacleOccupy(GridIndex const & cenInd, int const &dir);
		bool adjacent(GridIndex const &sInd, GridIndex const &tInd, int const &dir);

		bool isConnected(bex::VertexDescriptor const &vd1, bex::VertexDescriptor const &vd2,int const &gridType);
	};
}
