#pragma once
#include "Obmap.h"
#include "Splan.hpp"
#include "bgeometry.h"
#include "stadfx.h"
#include "simple_svg.hpp"

namespace pl
{
	class MultiPlan
	{
	public:
		MultiPlan(Obmap &ob_map, vector<bex::DPoint> const &_vRobPnt) :
			_mainMap(ob_map), doc("MultiPlan.svg", svg::Layout(svg::Dimensions(ob_map.getWsPoint3().x(), ob_map.getWsPoint3().y())
				, svg::Layout::BottomLeft)), _ob_tGraph(ob_map.getGraph(graphType::base)), _ob_sGraph(ob_map.getGraph(graphType::span))
			, _ob_tGrid(ob_map.getGridMap(graphType::base)), _ob_sGrid(ob_map.getGridMap(graphType::span)),
			_ob_sgraph2map(ob_map.sgraph2map), _ob_smap2graph(ob_map.smap2graph), _ob_tgraph2map(ob_map.tgraph2map),
			_ob_tmap2graph(ob_map.tmap2graph)
			,_vStartPnt(_vRobPnt),_robNum(_vRobPnt.size())
		{			
			_colorMap.insert(pair<size_t, int>(0, svg::Color::Green));
			_colorMap.insert(pair<size_t, int>(1, svg::Color::Blue));
			_colorMap.insert(pair<size_t, int>(2, svg::Color::Red));
			_colorMap.insert(pair<size_t, int>(3, svg::Color::Yellow));

			c_deg.open("D:\\py_code\\ASCII\\data\\planDebug.txt", std::ios::trunc);

			vector<double> vx, vy;
			for (auto &it : this->_vStartPnt)
			{
				vx.push_back(it.x());
				vy.push_back(it.y());
			}
			this->writeDebug("start_x", vx);
			this->writeDebug("start_y", vy);
		}

		void drawGraph(size_t const &type, bool const &b_edge);
		void drawRobGraph();
		void drawStartLocation();
		void drawRobSet(bool const &neibool);
		void drawPath();
		void savePic() { doc.save(); }

		void writePath();
		void writeSgsTree();

		void cenPathPlanning();
		void disPathPlanning();
		void setRandomSeed(const size_t &val) { randomSeed = val; }
		vector<size_t> getSetNum() {
			vector<size_t> _vRobSetNum;
			for (size_t i = 0; i < _robNum; i++)
				_vRobSetNum.push_back(_vRobSetPtr->at(i).size());
			return _vRobSetNum;
		}
		
	private:

		//obMap
		//tgrid and the tgraph means the based grid
		//sgrid and sgraph means the spanning grid
		//from the obmap
		bex::Graph &_ob_tGraph;
		GridMap &_ob_tGrid;

		bex::Graph &_ob_sGraph;
		GridMap &_ob_sGrid;

		size_t randomSeed = 0;
		//
		std::map<std::pair<int, int>, int> &_ob_tmap2graph;
		std::map<int, std::pair<int, int>> &_ob_tgraph2map;

		std::map<std::pair<int, int>, int> &_ob_smap2graph;
		std::map<int, std::pair<int, int>> &_ob_sgraph2map;

		Obmap  &_mainMap;

		//
		const vector<bex::DPoint> _vStartPnt;
		const size_t _robNum;

		vector<bex::DLineString> _vTreeLineStr;
		vector<vector<bex::DSegment>> _vTreeSgs;
		vector<bex::Graph> _m_vGraph;
		vector<map<size_t, size_t>> _vT2local;
		vector<map<size_t, size_t>> _vlocal2T;

		vector<bex::DLineString> _vpath;
		vector<vector<bex::VertexDescriptor>> _vpathIndex;
		
		///

		
		//write txt
		std::ofstream c_deg;
		void writeDebug(string const &str, vector<double> const &v_val);
		//
		//
		//shared_ptr<vector<vector<size_t>>> _vRobSetPtr = nullptr;
		
		//
		//shared_ptr<vector<map<size_t,size_t>>> _vRobSetPtr = nullptr;
		shared_ptr<vector<map<size_t,size_t>>> _vRobNeiSetPtr = nullptr;
		shared_ptr<vector<set<size_t>>>_vRobNghSetPtr = nullptr;
		shared_ptr<vector<bool>> _vRobSleepPtr = nullptr;

		shared_ptr<vector<set<size_t>>> _vRobSetPtr = nullptr;
		shared_ptr<set<size_t>> _notBidSetPtr = nullptr;
		//shared_ptr<map<size_t,size_t>> _notBidPtr
		//shared_ptr < vector<map<size_t, size_t>> = nullptr;
		using TreeEdge = pair<size_t, size_t>;
//		shared_ptr<vector<set<pair<size_t, size_t>>>> _vRobNeiSetPtr = nullptr;
//		vector<vector<pair<size_t,size_t>>> _vRobSpanTree;

		map<size_t, int> _colorMap;
		//
		svg::Document doc;

		void auction();
		void cenAuction();
		void  getSpanningTreeSgs();
		void getNewGraph();
		void searchPath();


		bool treeIntersection(bex::DSegment const sg,size_t const robID) const;

		bool inSameMegaBox(bex::VertexDescriptor const &vd0, bex::VertexDescriptor const &vd1,size_t const &robID);

		size_t getDir(bex::VertexDescriptor const & cen_index, bex::VertexDescriptor const & n_index,size_t const &robID);

		bool getMinLeaf(set<size_t> const & robSet, bex::VertexDescriptor &vd) const;


		bool getMinLeaf(size_t const &robID, bex::VertexDescriptor &min_MegaBoxVd);

		bool getAuctionInd(bex::VertexDescriptor &min_MegaBoxVd);

		double calFitNess(size_t const &robID, bex::VertexDescriptor const  &megaBoxVd);
		double calPriority(size_t const &robID, bex::VertexDescriptor const &megaBoxVd);

		bool getAllLeaf(set<size_t> const & robSet, vector<bex::VertexDescriptor> &vVd) const;

		//
		// return min bid rob
		size_t minBidRob(size_t const &robAucNeer, bex::VertexDescriptor const &megaBoxID);

		void updateSet(size_t const &succBidID, bex::VertexDescriptor const& megaBoxVd);
		void updateSetWithErase(size_t const &succBidID, bex::VertexDescriptor const& megaBoxVd);


		double calSolutionFitNess();

	};
	//	enum DirType{left,bottom,right,top,center};
	//class Splan
	//{
	//public:
	//	Splan(Obmap  &ob_map):
	//	_mainMap(ob_map), doc("plan.svg", svg::Layout(svg::Dimensions(ob_map.getWsPoint3().x(), ob_map.getWsPoint3().y())
	//		,svg::Layout::BottomLeft)), _ob_tGraph(ob_map.getGraph(graphType::base)), _ob_sGraph(ob_map.getGraph(graphType::span))
	//		, _ob_tGrid(ob_map.getGridMap(graphType::base)), _ob_sGrid(ob_map.getGridMap(graphType::span)),
	//		_ob_sgraph2map(ob_map.sgraph2map),_ob_smap2graph(ob_map.smap2graph),_ob_tgraph2map(ob_map.tgraph2map),
	//		_ob_tmap2graph(ob_map.tmap2graph)
	//	{

	//	}

	//	void setStartPnt(bex::DPoint const& pnt) { _startPnt = pnt; }

	//	void pathPlanning();
	//	void drawGraph(size_t const &type,bool const &b_edge);
	//	void drawTree();
	//	void drawPath();
	//	void savePic() { doc.save(); }

	//	
	//private:

	//	//tgrid and the tgraph means the based grid
	//	//sgrid and sgraph means the spanning grid
	//	//from the obmap
	//	bex::Graph &_ob_tGraph;
	//	GridMap &_ob_tGrid;

	//	bex::Graph &_ob_sGraph;
	//	GridMap &_ob_sGrid;

	//	
	//	//
	//	std::map<std::pair<int, int>, int> &_ob_tmap2graph;
	//	std::map<int, std::pair<int, int>> &_ob_tgraph2map;

	//	std::map<std::pair<int, int>, int> &_ob_smap2graph;
	//	std::map<int, std::pair<int, int>> &_ob_sgraph2map;


	//	//menber graph
	//	bex::Graph  _m_tGraph;

	//	//

	//	void spanningTreePrim();
	//	void getNewGraph();
	//	void searchPath();

	//	//
	//	bex::DPoint _startPnt;
	//	Obmap  &_mainMap;
	//	svg::Document doc;

	//	//
	//	vector<size_t> _vTree;
	//	vector<bex::DSegment> _vTreeSg;
	//	bool treeIntersection(bex::DSegment const sg) const;

	//	//get four neighbour grid indexs;
	//	vector<GridIndex> getVerticalNeighbour(GridIndex const &cen_index);

	//	//get four neighbour grid indexs with directions
	//	vector<pair<GridIndex, size_t>> getVerticalNeighbourWithDir(GridIndex const &cen_index);

	//	size_t getDir(bex::VertexDescriptor const & cen_index, bex::VertexDescriptor const & n_index);

	//	bool inSameMegaBox(bex::VertexDescriptor const &vd0, bex::VertexDescriptor const &vd1);
	//	//
	//	bex::DLineString _path;
	//	vector<bex::VertexDescriptor> _pathIndex;
	//};

}