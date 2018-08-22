#pragma once
#include "Obmap.h"
#include "Gwrite.hpp"
#include "bgraph.h"
template<typename T>
bool writeDebug(std::ofstream &deg, string const &str, vector<T> const &v_val);


template<typename T>
bool writeDebug(std::ofstream &deg, string const &str, T const &v_val);

namespace pl {


	enum AnchorIndex { aiBottomLeft, aiBottom, aiBottomRight, aiLeft, aicentre, aiRight, aiTopLeft, aiTop, aiTopRight };

	typedef std::pair<int, double> Anchor2Dis;


	class ANode
	{
	public:
		ANode();
		~ANode();
		//ANode(bex::PointVert &vert, bex::DPoint &tar, double &disg, int &dir);
		ANode(bex::PointVert vert, bex::DPoint tar, double disg, int dir, GridIndex par);
		ANode(GridIndex &ind, bex::DPoint &tar, double &disg, int &dir, GridIndex &par);

		//	:disG(disg)
		//{
		//	//this->disG = disg;
		//	this->parent = par;
		//	this->vertPnt.pnt.x(ind.first + 0.5);
		//	this->vertPnt.pnt.y(ind.second + 0.5);
		//	this->targetPnt = tar;
		//	this->direction = dir;
		//	this->caldisF();
		//};

		bex::PointVert vertPnt;
		bex::DPoint targetPnt;
		double disH ;
		double disG ;
		double disF ;
		int direction;
		double getdisF() const { return this->disF; }
		void caldisF();
		void setdisG(double const &);
		GridIndex parent;
	};



	bool ANodeFunction(ANode &a, ANode &b);
	typedef std::pair<GridIndex, ANode> mNode;

	class AstarPlan
	{
	public:
		AstarPlan(Obmap  & ob_map) :
			_mainMap(ob_map), _ob_tGrid(ob_map.getGridMap(graphType::base)) {
			initDisMap();
		};
		bool getStratGrid(GridIndex const startInd);
		bool getTargetGrid(GridIndex const targetInd);


		bool plan();

		GridIndex m_Tindex;
		GridIndex m_Sindex;

		set<GridIndex> i_setSize;
		std::vector<GridIndex> m_pathIndex;
		std::vector<bex::DPoint> m_path;
		void init(GridIndex const &startInd, GridIndex const &end);
	private:

		Obmap &_mainMap;
		GridMap &_ob_tGrid;

		bool groupMode = false;

		bex::DPoint m_startPnt;
		bex::DPoint m_targetPnt;
		std::map<int, double> m_disMap;
		bool initDisMap();

		bool AstarSearch();
		bool AstarPathFinder();

		bool AstarShorten();

		std::list<ANode> m_openList;
		// std::set<ob::gridIndex> m_openSet;
		std::list<GridIndex> m_openSet;
		std::map<GridIndex, ANode> m_closeList;

		std::set<GridIndex> m_closeSet;
		std::vector<ANode> m_closeVect;

		void SortedAddToOpenList(ANode const & mNode);
		std::vector<GridIndex> getNeighbor(GridIndex const &mindex);
		std::vector<GridIndex> getNeighbor(GridIndex const &mindex, std::vector<int> &vdirIndex);

		//return ID
		int shortenList(std::list<bex::DPoint> & ldpoint, int const & ID);

		int shortenList(std::list<bex::DPoint> & ldpoint, std::list<bex::DPoint>::const_iterator);

		// max search time
		size_t maxSearhTimes = 20000;
		bool DPointEqual(const bex::DPoint &pnt1, const bex::DPoint &pnt2);
	};
}

