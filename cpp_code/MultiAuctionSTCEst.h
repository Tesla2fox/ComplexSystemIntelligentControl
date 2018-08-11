#pragma once
#include "Obmap.h"
#include "Gwrite.hpp"
#include "bgraph.h"
#include "AstarPlan.h"
template<typename T>
bool writeDebug(std::ofstream &deg, string const &str, vector<T> const &v_val);


template<typename T>
bool writeDebug(std::ofstream &deg, string const &str, T const &v_val);

namespace pl
{
	using STCEdge = pair<bex::VertexDescriptor, bex::VertexDescriptor>;
	// this auction uses estimate
	class MultiAuctionSTCEst {
	public:
		MultiAuctionSTCEst(Obmap  & ob_map, vector<GridIndex> const &vStartPnt) :
			_mainMap(ob_map), _ob_sgraph2map(ob_map.sgraph2map), _ob_smap2graph(ob_map.smap2graph), _ob_tgraph2map(ob_map.tgraph2map),
			_ob_gridInd2GraphVd(ob_map.gridInd2GraphVd), _ob_graphVd2GridInd(ob_map.graphVd2GridInd), _ob_STCGrid(ob_map._STCVirtualGrid),
			_ob_tmap2graph(ob_map.tmap2graph),_vStartPnt(vStartPnt),_robNum(vStartPnt.size()),
			_ob_tGraph(ob_map.getGraph(graphType::base)), _ob_sGraph(ob_map.getGraph(graphType::span))
			, _ob_tGrid(ob_map.getGridMap(graphType::base)), _ob_sGrid(ob_map.getGridMap(graphType::span)),aplan(ob_map)
		{
			string  str_deg = conDir;
			str_deg += "auctionSTCEstDeg.txt";
			c_deg.open(str_deg, std::ios::trunc);
			
		};

		template<typename T>
		friend  bool ::writeDebug(std::ofstream &deg, string const &str, vector<T> const &v_val);
		template<typename T>
		friend  bool ::writeDebug(std::ofstream &deg, string const &str, T const &v_val);
		//void setStartPnt(vector<size_t> const&vStartPntRow, vector<size_t> const&vStartPntCol);
		//void setStartPnt(vector<GridIndex> const &vStartPnt) { _vStartPnt.clear(); _vStartPnt = vStartPnt; }

		void process();

		void writeRobGraph();

		void setRandomSeed(const size_t &val) { randomSeed = val; }

	private:
		//write txt
		std::ofstream c_deg;

		void formSpanningTree();
		void testAstar();
		void writeMultiGraph();
		void writeMultiPath();
		void wirteMultiGraphSeg();
		void writeMultiGraphEdge();
		void wirteGlobalSTCGraph();
		//
		AstarPlan aplan;

		//tgrid and the tgraph means the based grid
		//sgrid and sgraph means the spanning grid
		//from the obmap
		bex::Graph &_ob_tGraph;
		GridMap &_ob_tGrid;

		bex::Graph &_ob_sGraph;
		GridMap &_ob_sGrid;
		STCGridMap &_ob_STCGrid;

		std::map<std::pair<int, int>, int> &_ob_tmap2graph;
		std::map<int, std::pair<int, int>> &_ob_tgraph2map;

		std::map<std::pair<int, int>, int> &_ob_smap2graph;
		std::map<int, std::pair<int, int>> &_ob_sgraph2map;

		map<pair<pair<int, int>, int>, int> &_ob_gridInd2GraphVd;
		map<int, pair<pair<int, int>, int>> &_ob_graphVd2GridInd;


		
		void auction();
		void getSpanningTreeSgs();
		void getNewGraph();
		void searchVitualPath();
		void generateRealPath();
		void auctioneer(const size_t & robID);


		Obmap &_mainMap;
		

		//
		size_t randomSeed = 0;

		//
		shared_ptr<vector<size_t>> _vRobEstCostPtr = nullptr;

		shared_ptr<vector<set<size_t>>> _vRobSetPtr = nullptr;
		//the stc vertex ind and it is a leaf node or not.
		shared_ptr<vector<map<size_t, size_t>>> _vRobMapPtr = nullptr;
		shared_ptr<vector<vector<GridIndex>>> _vRobGridPtr = nullptr;
		shared_ptr<vector<set<bex::VertexDescriptor>>> _vRobNeiPtr = nullptr;

		shared_ptr<vector<vector<STCEdge>>> _vRobEdgePtr = nullptr;
		// the first means in the robot graph
		// the second means out of the robot graph
		shared_ptr<vector<vector<pair<bex::VertexDescriptor, bex::VertexDescriptor>>>> _vRobNeiEdgePtr = nullptr;

		shared_ptr<vector<bool>> _vRobSleepPtr = nullptr;
		map<bex::VertexDescriptor, int> _GridMap;
		//shared_ptr<vector<set<size_t>>>_vRobNghSetPtr = nullptr;

	
		//shared_ptr<vector<set<size_t>>> _vRob
		

		// the val for the STC algorithm 
		const vector<GridIndex> _vStartPnt;
		vector<vector<bex::DSegment>> _vTreeSgs;
		vector<bex::Graph> _m_vGraph;
		vector<map<size_t, size_t>> _vT2local;
		vector<map<size_t, size_t>> _vlocal2T;

		vector<bex::DLineString> _vpath;
		vector<vector<bex::VertexDescriptor>> _vpathIndex;
		vector<vector<size_t>> _vLeafSet;
		vector<set<size_t>> _vLeafSTCSet;
		vector<set<size_t>> _vNoLeafSTCSet;
		vector<set<size_t>> _vNoPathInd;
		vector<vector<bool>> _vvBoolTree;
		/// function for the STC
		//const vector<bex::DPoint> _vStartPnt;


		bool treeIntersection(bex::DSegment const sg, size_t const robID) const;

		size_t getDir(bex::VertexDescriptor const & lcen_index, bex::VertexDescriptor const & ln_index, size_t const & robID);

		bool isAdjacent(bex::VertexDescriptor const & vd1, bex::VertexDescriptor const & vd2);
		//end the STC function

		///
		///
		const size_t _robNum;
		// auction function begin
		bool calAucVertID(size_t const & aucNeer, size_t &aucVertID);

//		bool calAucVertID(size_t const & aucNeer, STCEdge &aucEdgeID);
		bool maxBiddingRob(size_t const &aucVertID, size_t &winnerRob, size_t &sVertID, size_t &minCost);
		size_t maxBiddingRob(size_t const & aucVertID);
		// sVertID means the vert in the rob graph
		size_t calCost(const size_t &robID,const size_t &aucVertID, int &sVertID);

		double calBidding(size_t const &bidding);

		//
		pair<bool, double> calUnopPriority(size_t const &robID, bex::VertexDescriptor const &vd);

		double calOpPriority(size_t const &aucNeer, size_t const & OpRobId, bex::VertexDescriptor const &vd);
		bool calOpPriority(size_t const & OpRobId, bex::VertexDescriptor const &vd,double &priority);

		size_t leafCost(bex::VertexDescriptor const &tvd);
		//esatimate the add cost of rob path
		//the svd means the vertex out of the graph
		//the tvd means the vertex in the rob graph
		size_t estAddCost(size_t const &robID, bex::VertexDescriptor const &svd,bex::VertexDescriptor const &tvd);
		size_t estEraseCost(size_t const &robID, bex::VertexDescriptor const &svd, bex::VertexDescriptor const &tvd);

		bool updateNeiGraph(size_t const &robID);
		bool updateNeiGraph(size_t const &succBidID, bex::VertexDescriptor const & vd);
		bool updateNeiGraphWithErase(size_t const &loseID, bex::VertexDescriptor const & vd);

		bool loserEarseVert(size_t const &loserID, size_t const&vertID);
		bool isLeafNode(size_t const &robID, size_t const &vertID);
		//
		bool exchange2TreeSgs();
		bool calMakeSpan();
		// auction function end
	};
	
}