#include "AstarPlan.h"
#include "Obmap.h"

namespace pl {

	ANode::ANode()
	{
	}

	ANode::~ANode()
	{
	}

	//ANode::ANode(bex::PointVert &vert, bex::DPoint &tar, double &disg, int &dir)
	//{
	//	this->disG = disg;
	//	this->vertPnt = vert;
	//	this->targetPnt = tar;
	//	this->direction = dir;
	//	this->caldisF();
	//}

	//ANode::ANode(GridIndex ind, bex::DPoint tar, double disg, int dir, GridIndex par)
	//{
	//	this->disG = disg;
	//	this->parent = par;
	//	this->vertPnt.pnt.x(ind.first + 0.5);
	//	this->vertPnt.pnt.y(ind.second + 0.5);
	//	this->targetPnt = tar;
	//	this->direction = dir;
	//	this->caldisF();
	//}

	ANode::ANode(bex::PointVert vert, bex::DPoint tar, double disg, int dir, GridIndex par)
	{
		this->disG = disg;
		this->parent = par;
		this->vertPnt = vert;
		this->targetPnt = tar;
		this->direction = dir;
		this->caldisF();
	}

	void ANode::caldisF()
	{
		disH = bg::distance(this->vertPnt.pnt, this->targetPnt);
		this->disF = this->disG + this->disH;
		//return disF;
	}
	void ANode::setdisG(double const &dval)
	{
		this->disH = dval;
	}
	bool ANodeFunction(ANode &a, ANode &b)
	{
		auto af = a.getdisF();
		auto bf = b.getdisF();
		if (af < bf)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool pl::AstarPlan::plan()
	{
		this->m_path.clear();
		this->m_openSet.clear();
		this->m_openList.clear();

		this->m_closeList.clear();
		this->m_closeSet.clear();
		this->m_closeVect.clear();

		if (AstarSearch())
		{
			if (AstarPathFinder())
			{
				return true;
			}
		}
		return false;
	}
	void AstarPlan::init(GridIndex const & startInd, GridIndex const & endInd)
	{
		m_Sindex = startInd;
		m_Tindex = endInd;
		m_startPnt.x(startInd.first + 0.5);
		m_startPnt.y(startInd.second + 0.5);
		m_targetPnt.x(endInd.first + 0.5);
		m_targetPnt.y(endInd.second + 0.5);
	}
	bool AstarPlan::initDisMap()
	{
		this->m_disMap.insert(Anchor2Dis(aiTop, 0.5));
		this->m_disMap.insert(Anchor2Dis(aiLeft, 0.5));
		this->m_disMap.insert(Anchor2Dis(aiRight, 0.5));
		this->m_disMap.insert(Anchor2Dis(aiBottom, 0.5));
		this->m_disMap.insert(Anchor2Dis(aiTopLeft, 0.707));
		this->m_disMap.insert(Anchor2Dis(aiTopRight, 0.707));
		this->m_disMap.insert(Anchor2Dis(aiBottomRight, 0.707));
		this->m_disMap.insert(Anchor2Dis(aiBottomLeft, 0.707));
		this->m_disMap.insert(Anchor2Dis(aicentre, 0));
		return false;

		//return false;
	}
	bool AstarPlan::AstarSearch()
	{
		auto &grid = _ob_tGrid;
		bex::PointVert vert;
		vert.pnt.x(m_Sindex.first + 0.5);
		vert.pnt.y(m_Sindex.second + 0.5);
		vert.PntIndex = m_Sindex;
		ANode sNode(vert, m_targetPnt, 0.0, AnchorIndex::aicentre, m_Sindex);
		
		//m_openSet.insert(m_Sindex);
		m_openSet.push_back(m_Sindex);
		m_openList.push_back(sNode);
		size_t searchTimes = 0;
		size_t maxSearch = 10000;

		bool already_find = false;
		double extra_search_ratio = 0.3;
		size_t extra_search = 0;
		size_t extra_counter = 0;
		// qDebug()<<" TIndex.first = " <<m_Tindex.first <<" Tindex.second ="<< m_Tindex.second;

		while (!m_openList.empty())
		{
			std::vector<int> vdirIndex;
			auto aIndex = m_openList.front().vertPnt.PntIndex;
#ifdef _DEBUG
			cout << "aIndex.first = " << aIndex.first << " aIndex.second" << aIndex.second << endl;
#endif // _DEBUG

			auto PreDir = m_openList.front().direction;
			double currDis = m_openList.front().disG;

			// Move the first item of openList to closedList
			m_closeVect.push_back(m_openList.front());
			m_closeList.insert(mNode(m_openList.front().vertPnt.PntIndex, m_openList.front()));
			m_openSet.remove(GridIndex{ aIndex.first, aIndex.second });
			m_openList.pop_front();

			auto vIndex = getNeighbor(aIndex, vdirIndex);
			// qDebug()<<"aIndex.first = "<<aIndex.first <<" aIndex.second ="<<aIndex.second;


			/// the index vector
			//            vIndex

			/// the direction vector
			/// vdirIndex
			searchTimes++;

#ifdef _DEBUG
			if (searchTimes == 3)
				cout << ":" << endl;
			cout << "searchTime = " << searchTimes << endl;
#endif // _DEBUG

			if (searchTimes > this->maxSearhTimes)
			{
				//qDebug()<<"over the maxSearchTimes";
				std::cout << "over the maxmaxSearchTimes" << std::endl;
				goto Fcase;
			}
			size_t dirIndex = 0;
			//for (auto &it : vIndex)
			for (; dirIndex < vIndex.size(); ++dirIndex)
			{
				auto& it = vIndex[dirIndex];
				if (grid[it] != bex::vertType::WayVert)
					continue;
				bool notInOpenSet = std::find(m_openSet.begin(), m_openSet.end(), it) == m_openSet.end();
				bool notInClosedList = (m_closeList.count(it) == 0);

				double disg = currDis + 1;
				//if (PreDir != vdirIndex.at(dirIndex)) {
				//	disg += 0.15;
				//}
				if (notInOpenSet && notInClosedList)
				{
					bex::PointVert vert_t;
					vert_t.pnt.x(it.first + 0.5);
					vert_t.pnt.y(it.second + 0.5);
//					ANode wtf(GridIndex(m_Sindex.first, m_Sindex.second),m_targetPnt,0.0,0,m_Sindex);
					vert_t.PntIndex = it;
					ANode u_node(vert_t, m_targetPnt, disg, vdirIndex.at(dirIndex), aIndex);
					SortedAddToOpenList(u_node);
					//m_openSet.insert(it);
					m_openSet.push_back(it);
				}
				else
				{
					if (!notInOpenSet) {
						auto existed = std::find_if(m_openList.begin(), m_openList.end(), [&](const ANode& xx) {
							return xx.vertPnt.PntIndex.first == it.first &&
								xx.vertPnt.PntIndex.second == it.second;
						});
						if (disg < existed->disG) {
							m_openList.erase(existed);
							bex::PointVert vert_t;
							vert_t.pnt.x(it.first + 0.5);
							vert_t.pnt.y(it.second + 0.5);
							vert_t.PntIndex = it;

							ANode u_node(vert_t, m_targetPnt, disg, vdirIndex.at(dirIndex), aIndex);
							SortedAddToOpenList(u_node);
						}
					}
					else {
						auto existed = m_closeList.find(it);
						if (disg < existed->second.disG) {
							m_closeList.erase(existed);

							bex::PointVert vert_t;
							vert_t.pnt.x(it.first + 0.5);
							vert_t.pnt.y(it.second + 0.5);
							vert_t.PntIndex = it;

							ANode u_node(vert_t, m_targetPnt, disg, vdirIndex.at(dirIndex), aIndex);
							SortedAddToOpenList(u_node);
							//m_openSet.insert(it);
							m_openSet.push_back(it);
						}
					}
				}
				//dirIndex++;
				//cout << dirIndex++ << endl;
			}
			if ((aIndex.first == m_Tindex.first) && (aIndex.second == m_Tindex.second))
			{
				already_find = true;
				extra_search = (size_t)(searchTimes * extra_search_ratio);
				//                            break;
				break;
			}
			if (already_find) {

				extra_counter++;
				if (searchTimes == maxSearch - 1 ||
					extra_counter == extra_search) {
					break;
				}
			}
		}
		return true;

	Fcase:
		return false;
	}

	bool AstarPlan::AstarPathFinder()
	{
		std::stack<bex::DPoint> sDPoint;
		std::stack<int> sDirIndex;
		auto u_node = m_closeList[m_Tindex];
		sDPoint.push(u_node.vertPnt.pnt);
		sDirIndex.push(u_node.direction);

		//		qDebug() << "u_first= " << u_node.vertPnt.PntIndex.first << " u_second " << u_node.vertPnt.PntIndex.second;

		//		qDebug() << " u_x " << u_node.vertPnt.pnt.x() << " u_y " << u_node.vertPnt.pnt.y();

		bex::DPoint  spnt(m_Sindex.first + 0.5, m_Sindex.second + 0.5);
		size_t successIndex = 0;
		do
		{
			successIndex++;
			if (successIndex>this->maxSearhTimes)
			{
				return false;
			}
			//qDebug() << "u_first= " << u_node.vertPnt.PntIndex.first<< " u_second " << u_node.vertPnt.PntIndex.second;
			//qDebug() << "p_first= " << u_node.parent.first << " p_second " << u_node.parent.second;
			//qDebug() << "s_first" << m_Sindex.first << "s_second" << m_Sindex.second;
			u_node = m_closeList[u_node.parent];
			sDPoint.push(u_node.vertPnt.pnt);
			sDirIndex.push(u_node.direction);
		} while (!DPointEqual(u_node.vertPnt.pnt, spnt));
		std::vector<bex::DPoint> vpathTem;
		std::vector<int> vdirIndex;
		do
		{
			vpathTem.push_back(sDPoint.top());
			sDPoint.pop();
			vdirIndex.push_back(sDirIndex.top());
			sDirIndex.pop();
		} while (!sDPoint.empty());

		//m_path.push_back(m_startPnt);
		std::list<bex::DPoint> l_path;
		m_path.push_back(m_startPnt);
		for (size_t i = 1; i < (vdirIndex.size() - 1); i++)
		{
			m_path.push_back(vpathTem.at(i));
		}
		m_path.push_back(vpathTem.back());

		return true;
	}

	int AstarPlan::shortenList(std::list<bex::DPoint> & ldpoint, int const & ID)
	{

		return 0;
	}
	bool AstarPlan::DPointEqual(const bex::DPoint & a, const bex::DPoint & b)
	{
		if (a.x() == b.x())
		{
			if (a.y() == b.y())
			{
				return true;
			}
		}
		return false;
	}
	bool AstarPlan::AstarShorten()
	{
	//	std::vector<bgeo::DPoint> pathTemp;

		return false;
	}

	void AstarPlan::SortedAddToOpenList(ANode const & mNode)
	{
		if (!m_openList.empty())
		{
			if (mNode.disF < m_openList.front().disF)
			{
				m_openList.push_front(mNode);
				return;
			}
			if (mNode.disF >= m_openList.back().disF)
			{
				m_openList.push_back(mNode);
				return;
			}
			for (auto it = m_openList.begin(); it != m_openList.end(); it++)
			{
				if (mNode.disF < it->disF)
				{
					auto p = it;
					//p--;
					m_openList.insert(p, mNode);
					return;
				}
			}
		}
		m_openList.push_back(mNode);
	}
	std::vector<GridIndex>  AstarPlan::getNeighbor(GridIndex const &mindex)
	{
		auto res = _mainMap.getSearchVerticalNeighbor(mindex, graphType::base);
		return res;
	}

	std::vector<GridIndex> AstarPlan::getNeighbor(GridIndex const &mindex, std::vector<int> &vdirIndex)
	{
		vdirIndex.clear();
		auto res = _mainMap.getSearchVerticalNeighbor(mindex, graphType::base);

		std::vector<GridIndex> resDel;

		for (size_t i = 0; i < res.size(); i++)
		{
			if (i_setSize.count(res[i]) == 1)
			{
				resDel.push_back(res[i]);
				vdirIndex.push_back(aicentre);
			}
		}
		return resDel;
	}
}