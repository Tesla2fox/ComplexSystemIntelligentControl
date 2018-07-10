#include "Obmap.h"
#include "Gwrite.hpp"

namespace pl
{
	bool pl::Obmap::map2tGrid()
	{
		this->_tGrid.clear();
		//auto &vGrid = *_vGridPtr;
		//size_t p = 0;
		//for (size_t i = 0; i < m_MaxRow; i++)
		//{
		//	for (size_t j = 0; j < m_MaxCol; j++)
		//	{
		//		GridIndex ind(i, j);
		//		this->_tGrid.insert(pair<GridIndex, size_t>(ind, vGrid[p++]));
		//	}
		//}
		bool rowChange = false;
		bool colChange = false;
		if (m_MaxRow % 2)
		{
			rowChange = true;
			m_MaxRow++;
		}
		if (m_MaxCol % 2)
		{
			colChange = true;
			m_MaxCol++;
		}
		for (size_t i = 0; i < m_MaxRow; i++)
		{
			for (size_t j = 0; j < m_MaxCol; j++)
			{
				GridIndex ind(i, j);
				this->_tGrid.insert(pair<GridIndex, size_t>(ind, bex::vertType::WayVert));
			}
		}

		for (auto &it : (*_vObGridInd)) 
			_tGrid[GridIndex(it.row,it.col)] = bex::vertType::ObVert;
		
		_obVertNum = _vObGridInd->size();
		if (rowChange)
		{
			for (size_t i = 0; i < m_MaxCol; i++)
			{
				_tGrid[GridIndex(m_MaxRow - 1, i)] = bex::vertType::ObVert;
				_obVertNum++;
			}
		}
		if (colChange)
		{
			for (size_t i = 0; i < m_MaxRow; i++)
			{
				_tGrid[GridIndex(i , m_MaxCol -1)] = bex::vertType::ObVert;
				_obVertNum++;
			}
		}
		
		_reachableVertNum = m_MaxRow * m_MaxCol - _obVertNum;
		//create graph
		int i = 0;
		for (auto & it : this->_tGrid)
		{
			bex::VertexProperty vp;
			std::pair<size_t, size_t> ind(it.first.first, it.first.second);
			vp.PntIndex = ind;
			vp.Type = it.second;
			vp.EdgeState = false;
			vp.pnt.x(it.first.first + 0.5);
			vp.pnt.y(it.first.second + 0.5);
			boost::add_vertex(vp, this->_tGraph);
			std::pair<int, int> localIndex;
			localIndex = it.first;
			tmap2graph.insert(std::pair<std::pair<int, int>, int>(localIndex, i));
			tgraph2map.insert(std::pair<int, std::pair<int, int>>(i, localIndex));
			i++;
		}
		//add edges of the graph 

		std::pair<bex::VertexIterator, bex::VertexIterator> vi = boost::vertices(this->_tGraph);
		for (bex::VertexIterator vit = vi.first; vit != vi.second; vit++)
		{
			bex::VertexDescriptor vd = *vit;
			if (this->_tGraph[vd].Type != bex::vertType::ObVert)
			{
				auto localIndex = tgraph2map[vd];
				//				auto vlocalIndex = getSearchNeighbor(localIndex, graphType::base);
				auto vlocalIndex = getSearchVerticalNeighbor(localIndex, graphType::base);

				std::vector<int> vvd;
				for (auto &it : vlocalIndex)
				{
					vvd.push_back(tmap2graph[it]);
				}

				for (auto &it : vvd)
				{
					if (this->_tGraph[it].EdgeState == false)
					{
						bex::EdgeProperty ep;
						ep.weight = bg::distance(_tGraph[vd].pnt, _tGraph[it].pnt);
						boost::add_edge(it, vd, ep, _tGraph);
					}
				}
			}
			this->_tGraph[vd].EdgeState = true;
		}
		auto num_edges = boost::num_edges(this->_tGraph);
		cout << "num_edges = " << num_edges << endl;
		auto num_vertex = boost::num_vertices(_tGraph);
		cout << "num_vertex = " << num_vertex << endl;
		return false;
	}

	bool Obmap::map2sGrid()
	{
		this->_sGrid.clear();
		for (size_t i = 0; i < m_sMaxRow; i++)
		{
			for (size_t j = 0; j < m_sMaxCol; j++)
			{
				GridIndex ind(i, j);
				STCVert vert;
				vert.x = 2 * i + 1;
				vert.y = 2 * j + 1;
				vert.LB.first = 2 * i;
				vert.LT.first = 2 * i;
				vert.RB.first = 2 * i + 1;
				vert.RT.first = 2 * i + 1;
				vert.LB.second = 2 * j;
				vert.LT.second = 2 * j + 1;
				vert.RB.second = 2 * j;
				vert.RT.second = 2 * j + 1;
				auto tGridIndex = getTgraphGridInd(ind);
				vert._vGridIndex = tGridIndex;
				this->_STCGrid.insert(pair<GridIndex, STCVert>(ind, vert));
				//this->_tGrid.insert(pair<GridIndex, size_t>(ind, vGrid[p++]));
			}
		}

		//create graph
		int i = 0;
		for (auto & it : this->_STCGrid)
		{
			bex::VertexProperty vp;
			std::pair<size_t, size_t> ind(it.first.first, it.first.second);

			if (it.first.first == 9 && it.first.second == 0)
				cout << "bug" << endl;
			vp.PntIndex = ind;
			//auto tGridIndex = getTgraphGridInd(ind);
			vp.pnt.x(it.second.x);
			vp.pnt.y(it.second.y);
			if (allObstacle(it.second._vGridIndex))
				vp.Type = bex::vertType::ObVert;
			else
				vp.Type = bex::vertType::WayVert;
			vp.EdgeState = false;
			boost::add_vertex(vp, this->_sGraph);
			std::pair<int, int> localIndex;
			localIndex = it.first;
			smap2graph.insert(std::pair<std::pair<int, int>, int>(localIndex, i));
			sgraph2map.insert(std::pair<int, std::pair<int, int>>(i, localIndex));
			i++;
		}
		//add edges of the graph 

		size_t vertNum = 0;
		std::pair<bex::VertexIterator, bex::VertexIterator> vi = boost::vertices(this->_sGraph);
		for (bex::VertexIterator vit = vi.first; vit != vi.second; vit++)
		{
			bex::VertexDescriptor vd = *vit;
			if (this->_sGraph[vd].Type != bex::vertType::ObVert)
			{
				auto localIndex = sgraph2map[vd];
				if (localIndex.first == 3 && localIndex.second == 0)
					cout << "bug" << endl;
				auto vlocalIndex = getSTCVerticalNeighbor(localIndex);
				std::vector<int> vvd;
				for (auto &it : vlocalIndex)
				{
					vvd.push_back(smap2graph[it]);
				}

				for (auto &it : vvd)
				{
					if (this->_sGraph[it].EdgeState == false)
					{
						bex::EdgeProperty ep;
						//ep.weight = bg::distance(_tGraph[vd].pnt, _tGraph[it].pnt);
						boost::add_edge(it, vd, ep, _sGraph);
					}
				}
			}
			this->_sGraph[vd].EdgeState = true;
		}
		auto num_edges = boost::num_edges(this->_sGraph);
		cout << "num_edges = " << num_edges << endl;
		auto num_vertex = boost::num_vertices(_sGraph);
		cout << "num_vertex = " << num_vertex << endl;
		return false;
	}


	void Obmap::writeEdges(size_t const & type)
	{		
		bex::Graph *graphPtr;
		if (type == graphType::base)
			graphPtr = &this->_tGraph;
		else
			graphPtr = &this->_sGraph;
		auto &graph = *graphPtr;
		std::pair<bex::EdgeIterator, bex::EdgeIterator> _b2e_ei = boost::edges(graph);

		vector<size_t> sRow, sCol, tRow, tCol;
		for (auto eit = _b2e_ei.first; eit != _b2e_ei.second; eit++)
		{
			bex::EdgeDescriptor ed = *eit;
			bex::EdgeProperty &ep = graph[ed];
			bex::VertexDescriptor sVertd = boost::source(ed, graph);
			bex::VertexDescriptor tVertd = boost::target(ed, graph);

			bex::VertexProperty &sVert = graph[sVertd];
			bex::VertexProperty &tVert = graph[tVertd];

			sRow.push_back(sVert.PntIndex.first);
			sCol.push_back(sVert.PntIndex.second);

			tRow.push_back(tVert.PntIndex.first);
			tCol.push_back(tVert.PntIndex.second);

		}
		writeDebug(c_deg, "sRow", sRow);
		writeDebug(c_deg, "sCol", sCol);
		writeDebug(c_deg, "tRow", tRow);
		writeDebug(c_deg, "tCol", tCol);
	}

	void Obmap::writeEdgesInPnt(size_t const & type)
	{
		bex::Graph *graphPtr;
		if (type == graphType::base)
			graphPtr = &this->_tGraph;
		else
			graphPtr = &this->_sGraph;
		auto &graph = *graphPtr;
		std::pair<bex::EdgeIterator, bex::EdgeIterator> _b2e_ei = boost::edges(graph);

		vector<double> sPntx, sPnty, tPntx, tPnty;
		for (auto eit = _b2e_ei.first; eit != _b2e_ei.second; eit++)
		{
			bex::EdgeDescriptor ed = *eit;
			bex::EdgeProperty &ep = graph[ed];
			bex::VertexDescriptor sVertd = boost::source(ed, graph);
			bex::VertexDescriptor tVertd = boost::target(ed, graph);

			bex::VertexProperty &sVert = graph[sVertd];
			bex::VertexProperty &tVert = graph[tVertd];

			sPntx.push_back(sVert.pnt.x());
			sPnty.push_back(sVert.pnt.y());

			//cout << "pnt.x " << tVert.pnt.x() << "	pnt.y" << tVert.pnt.y() << endl;
			tPntx.push_back(tVert.pnt.x());
			tPnty.push_back(tVert.pnt.y());

		}
		writeDebug(c_deg, "sPntx", sPntx);
		writeDebug(c_deg, "sPnty", sPnty);
		writeDebug(c_deg, "tPntx", tPntx);
		writeDebug(c_deg, "tPnty", tPnty);

	}

	bool Obmap::allConnected(vector<bex::VertexDescriptor> const & v_vd)
	{
		if (v_vd.size() == 1)
			return true;
		using CGraph = boost::adjacency_list<bt::vecS, bt::vecS, bt::undirectedS>;
		CGraph cg;
		for (size_t i = 0; i < v_vd.size(); i++)
		{
			for (size_t j = i; j < v_vd.size(); j++)
			{
				
				if (isConnected(v_vd[i], v_vd[j], pl::graphType::span))
				{
					bt::add_edge(i, j, cg);
					//cout << "t = " << i << " s = " << j << endl;
				}
			}
		}
		vector<int> component(bt::num_vertices(cg));
		if (component.size() < v_vd.size())
			return false;
		int num = bt::connected_components(cg, &component[0]);
		if (num == 1) 
			return true;
		return false;
	}

	bool Obmap::allConnectedBase(vector<bex::VertexDescriptor> const & v_vd)
	{
		if (v_vd.size() == 1)
			return true;
		using CGraph = boost::adjacency_list<bt::vecS, bt::vecS, bt::undirectedS>;
		CGraph cg;
		for (size_t i = 0; i < v_vd.size(); i++)
		{
			for (size_t j = i; j < v_vd.size(); j++)
			{

				if (isConnected(v_vd[i], v_vd[j], pl::graphType::base))
				{
					bt::add_edge(i, j, cg);
					//cout << "t = " << i << " s = " << j << endl;
				}
			}
		}
		vector<int> component(bt::num_vertices(cg));
		if (component.size() < v_vd.size())
			return false;
		int num = bt::connected_components(cg, &component[0]);
		if (num == 1)
			return true;
		return false;
	}

	std::vector<GridIndex> Obmap::getSearchVerticalNeighbor(GridIndex const & cen_index, size_t const & gridType)
	{
		GridMap *gridPtr;
		if (gridType == graphType::base)
		{
			gridPtr = &this->_tGrid;
		}
		else
		{
			gridPtr = &this->_sGrid;
		}
		auto &grid = (*gridPtr);

		vector<GridIndex> vIndex;

		auto &i = cen_index.first;
		auto &j = cen_index.second;
		auto neighbour = [=](GridIndex &ind, vector<GridIndex>  &vInd) {
			if (grid.count(ind))
			{
				if (grid.at(ind) != bex::vertType::ObVert)
				{
					vInd.push_back(ind);
					return true;
				}
			}
			return false;
		};

		//left
		neighbour(GridIndex(i - 1, j), vIndex);
		//top 
		neighbour(GridIndex(i, j + 1), vIndex);
		//right
		neighbour(GridIndex(i + 1, j), vIndex);
		//botton
		neighbour(GridIndex(i, j - 1), vIndex);

		return vIndex;
	}

	std::vector<GridIndex> Obmap::getSTCVerticalNeighbor(GridIndex const & cen_index)
	{
		auto &vert = this->_STCGrid[cen_index];


		vector<GridIndex> vIndex;

		auto &i = cen_index.first;
		auto &j = cen_index.second;

		//left 
		if (adjacent(cen_index,GridIndex(i - 1,j),left))
			vIndex.push_back(GridIndex(i - 1, j));
		//right
		if (adjacent(cen_index, GridIndex(i + 1, j), right))
			vIndex.push_back(GridIndex(i + 1, j));
		//top
		if (adjacent(cen_index, GridIndex(i, j + 1), top))
			vIndex.push_back(GridIndex(i, j + 1));
		//bottom
		if (adjacent(cen_index, GridIndex(i, j - 1), bottom))
			vIndex.push_back(GridIndex(i, j - 1));
		return vIndex;
	}

	vector<GridIndex> Obmap::getTgraphGridInd(GridIndex const & cenInd)
	{
		GridIndex sgridInd = cenInd;
		
		auto maxRow = sgridInd.first * 2;
		auto maxCol = sgridInd.second * 2;

		vector<GridIndex> res;
		GridIndex tgridInd(sgridInd.first * 2, sgridInd.second * 2);
		if ((maxRow == m_MaxRow)&&(maxCol == m_MaxCol))
		{
			res.push_back(tgridInd);
			return res;
		}
		if (maxRow == m_MaxRow)
		{
			res.push_back(tgridInd);
			res.push_back(GridIndex(tgridInd.first, tgridInd.second + 1));
			return res;
		}
		if (maxCol == m_MaxCol)
		{
			res.push_back(tgridInd);
			res.push_back(GridIndex(tgridInd.first + 1, tgridInd.second));
			return res;
		}
		res.push_back(tgridInd);
		res.push_back(GridIndex(tgridInd.first + 1, tgridInd.second));
		res.push_back(GridIndex(tgridInd.first + 1, tgridInd.second + 1));
		res.push_back(GridIndex(tgridInd.first, tgridInd.second + 1));
		return res;
	}

	bool Obmap::allObstacle(vector<GridIndex>  const &vindex)
	{
		for (auto &it: vindex)
		{
			if (this->_tGrid[it] == bex::vertType::WayVert)
				return false;
		}
		return true;
	}

	bool Obmap::gridObstacle(GridIndex const & cenInd)
	{
		if (cenInd.first == -1 && cenInd.second == -1)
			return true;
		if (this->_tGrid[cenInd] == bex::ObVert)
			return true;
		return false;
	}

	int Obmap::gridObNum(vector<GridIndex> const & vindex)
	{
		size_t obNum = 0;
		for (auto &it : vindex)
		{
			if (_tGrid[it] == bex::ObVert)
				obNum++;
		}
		return obNum;
	}

	bool Obmap::obstacleOccupy(GridIndex const & cenInd, int const & dir)
	{
		if (_STCGrid.count(cenInd) == 0)
			return true;
		auto &vert = this->_STCGrid[cenInd];
		switch (dir)
		{
		case left:
		{
			if (gridObstacle(vert.LB)&& gridObstacle(vert.LT))
				return true;
			else
				return false;
		}
		case right:
		{
			if (gridObstacle(vert.RB) && gridObstacle(vert.RT))
				return true;
			else
				return false;
		}
		case top:
		{
			if (gridObstacle(vert.RT) && gridObstacle(vert.LT))
				return true;
			else
				return false;
		}
		case bottom:
		{
			if (gridObstacle(vert.LB) && gridObstacle(vert.RB))
				return true;
			else
				return false;
		}
		default:
			break;
		}
		return false;
	}

	bool Obmap::adjacent(GridIndex const & sInd, GridIndex const & tInd, int const & dir)
	{
		if (_STCGrid.count(sInd) == 0)
			return false;
		if (_STCGrid.count(tInd) == 0)
			return false;
		auto &sVert = this->_STCGrid[sInd];
		auto &tVert = this->_STCGrid[tInd];
		switch (dir)
		{
		case left:
		{
			if (!obstacleOccupy(sInd, left) && !obstacleOccupy(tInd, right))
			{
				if (gridObstacle(sVert.LT) && gridObstacle(tVert.RB))
					return false;
				if (gridObstacle(sVert.LB) && gridObstacle(tVert.RT))
					return false;
				return true;
			}
			return false;
		}
		case right:
		{
			if (!obstacleOccupy(sInd, right) && !obstacleOccupy(tInd, left))
			{
				if (gridObstacle(sVert.RT) && gridObstacle(tVert.LB))
					return false;
				if (gridObstacle(sVert.RB) && gridObstacle(tVert.LT))
					return false;
				return true;
			}
			return false;
		}
		case top:
		{
			if (!obstacleOccupy(sInd, top) && !obstacleOccupy(tInd, bottom))
			{
				if (gridObstacle(sVert.RT) && gridObstacle(tVert.LB))
					return false;
				if (gridObstacle(sVert.LT) && gridObstacle(tVert.RB))
					return false;
				return true;
			}
			return false;
		}
		case bottom:
		{
			if (!obstacleOccupy(sInd, bottom) && !obstacleOccupy(tInd, top))
			{
				if (gridObstacle(sVert.RB) && gridObstacle(tVert.LT))
					return false;
				if (gridObstacle(sVert.LB) && gridObstacle(tVert.RT))
					return false;
				return true;
			}
			return false;
		}
		default:
			break;
		}
		return false;
	}

	bool Obmap::isConnected(bex::VertexDescriptor const & vd1, bex::VertexDescriptor const & vd2, int const &gridType)
	{
		bex::Graph *graphPtr;
		
		if (gridType == graphType::base)
			graphPtr = &this->_tGraph;
		else
			graphPtr = &this->_sGraph;
		auto &graph = *graphPtr;		
		auto neighborIter = boost::adjacent_vertices(vd1, graph);		
		std::vector<size_t> vVd;
		for (auto ni = neighborIter.first; ni != neighborIter.second; ++ni)
			vVd.push_back(*ni);
		 auto iter = std::find(vVd.begin(), vVd.end(), vd2);
		 if(iter == vVd.end())
			 return false;
		 return true;
	}
	
}