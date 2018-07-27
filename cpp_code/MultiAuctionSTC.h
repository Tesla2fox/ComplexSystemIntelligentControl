#pragma once
#include "Obmap.h"
#include "Gwrite.hpp"
#include "bgraph.h"

template<typename T>
bool writeDebug(std::ofstream &deg, string const &str, vector<T> const &v_val);


template<typename T>
bool writeDebug(std::ofstream &deg, string const &str, T const &v_val);

namespace pl
{
	class MultiAuctionSTC {
	public:
		MultiAuctionSTC(Obmap  & ob_map, vector<GridIndex> const &vStartPnt) :
			_mainMap(ob_map),_ob_sgraph2map(ob_map.sgraph2map), _ob_smap2graph(ob_map.smap2graph), _ob_tgraph2map(ob_map.tgraph2map),
			_ob_tmap2graph(ob_map.tmap2graph),_vStartPnt(vStartPnt),_robNum(vStartPnt.size()),
			_ob_tGraph(ob_map.getGraph(graphType::base)), _ob_sGraph(ob_map.getGraph(graphType::span))
			, _ob_tGrid(ob_map.getGridMap(graphType::base)), _ob_sGrid(ob_map.getGridMap(graphType::span))
		{
			string  str_deg = conDir;
			str_deg += "auctionSTCDeg.txt";
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
		//


		//tgrid and the tgraph means the based grid
		//sgrid and sgraph means the spanning grid
		//from the obmap
		bex::Graph &_ob_tGraph;
		GridMap &_ob_tGrid;

		bex::Graph &_ob_sGraph;
		GridMap &_ob_sGrid;
		

		std::map<std::pair<int, int>, int> &_ob_tmap2graph;
		std::map<int, std::pair<int, int>> &_ob_tgraph2map;

		std::map<std::pair<int, int>, int> &_ob_smap2graph;
		std::map<int, std::pair<int, int>> &_ob_sgraph2map;

		
		void auction();
		void auctioneer(const size_t & robID);
		void bidding(const size_t &robID);


		Obmap &_mainMap;
		

		//
		size_t randomSeed = 0;

		//
		shared_ptr<vector<set<size_t>>> _vRobSetPtr = nullptr;
		shared_ptr<vector<vector<GridIndex>>> _vRobGridPtr = nullptr;
		shared_ptr<vector<set<bex::VertexDescriptor>>> _vRobNeiPtr = nullptr;
		shared_ptr<vector<bool>> _vRobSleepPtr = nullptr;
		map<bex::VertexDescriptor, int> _GridMap;
		//shared_ptr<vector<set<size_t>>>_vRobNghSetPtr = nullptr;

	
		//shared_ptr<vector<set<size_t>>> _vRob

		const vector<GridIndex> _vStartPnt;
		//const vector<bex::DPoint> _vStartPnt;
		const size_t _robNum;
		bool calAucVertID(size_t const & aucNeer, size_t &aucVertID);
		size_t maxBiddingRob(size_t const & aucVertID);
		double calBidding(size_t const &bidding);

		//
		double calUnopPriority(size_t const &robID, bex::VertexDescriptor const &vd);
		double calOpPriority(size_t const &aucNeer, size_t const & OpRobId, bex::VertexDescriptor const &vd);
		
		bool updateNeiGraph(size_t const &robID);
		bool updateNeiGraph(size_t const &succBidID, bex::VertexDescriptor const & vd);
		bool updateNeiGraphWithErase(size_t const &loseID, bex::VertexDescriptor const & vd);
	};
	
}