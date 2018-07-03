#include "Obmap.h"
#include "Gwrite.hpp"

namespace pl
{
	bool pl::Obmap::map2tGrid()
	{
		this->_tGrid.clear();
		auto &vGrid = *_vGridPtr;
		size_t p = 0;
		for (size_t i = 0; i < m_MaxRow; i++)
		{
			for (size_t j = 0; j < m_MaxCol; j++)
			{
				GridIndex ind(i, j);
				this->_tGrid.insert(pair<GridIndex, size_t>(ind, vGrid[p++]));
			}
		}
		//create graph
		int i = 0;
		for (auto & it : this->_tGrid)
		{
			bex::VertexProperty vp;
			std::pair<size_t, size_t> ind(it.first.first,it.first.second);
			vp.PntIndex = ind;
			vp.Type = it.second;
			vp.EdgeState = false;
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
				this->_tGrid.insert(pair<GridIndex, size_t>(ind, 1));
			}
		}

		//create graph
		int i = 0;
		for (auto & it : this->_sGrid)
		{
			bex::VertexProperty vp;
			std::pair<size_t, size_t> ind(it.first.first, it.first.second);
			vp.PntIndex = ind;
			vp.Type = it.second;
			vp.EdgeState = false;
			boost::add_vertex(vp, this->_tGraph);
			std::pair<int, int> localIndex;
			localIndex = it.first;
			smap2graph.insert(std::pair<std::pair<int, int>, int>(localIndex, i));
			sgraph2map.insert(std::pair<int, std::pair<int, int>>(i, localIndex));
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


	void Obmap::writeEgdes()
	{		
		std::pair<bex::EdgeIterator, bex::EdgeIterator> _b2e_ei = boost::edges(this->_tGraph);

		vector<size_t> sRow, sCol, tRow, tCol;
		for (auto eit = _b2e_ei.first; eit != _b2e_ei.second; eit++)
		{
			bex::EdgeDescriptor ed = *eit;
			bex::EdgeProperty &ep = _tGraph[ed];
			bex::VertexDescriptor sVertd = boost::source(ed, _tGraph);
			bex::VertexDescriptor tVertd = boost::target(ed, _tGraph);

			bex::VertexProperty &sVert = _tGraph[sVertd];
			bex::VertexProperty &tVert = _tGraph[tVertd];

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

	vector<GridIndex> Obmap::getTgraphGridInd(GridIndex const & cenInd)
	{
		GridIndex sgridInd = cenInd;
		GridIndex tgridInd(sgridInd.first * 2, sgridInd.second * 2);
		vector<GridIndex> res;
		if(_tGrid.count(tgridInd))
			res.push_back(tgridInd);
		if (_tGrid.count(GridIndex(tgridInd.first + 1, tgridInd.second)))
			res.push_back(GridIndex(tgridInd.first + 1, tgridInd.second));

		if (_tGrid.count(GridIndex(tgridInd.first, tgridInd.second + 1)))
			res.push_back(GridIndex(tgridInd.first, tgridInd.second + 1));

		if (_tGrid.count(GridIndex(tgridInd.first + 1, tgridInd.second + 1)))
			res.push_back(GridIndex(tgridInd.first + 1, tgridInd.second + 1));

		res.push_back(GridIndex(tgridInd.first + 1, tgridInd.second));
		res.push_back(GridIndex(tgridInd.first, tgridInd.second + 1));
		res.push_back(GridIndex(tgridInd.first + 1, tgridInd.second + 1));
		return res;

	}
	
}