#include "Splan.hpp"

namespace pl{
	void pl::Splan::pathPlanning()
	{
		_path.clear();
		GridIndex  cenGridIndex = _mainMap.pnt2Index(_startPnt, graphType::base);
		spanningTreePrim();
		getNewGraph();
		inSameMegaBox(7, 8);
		searchPath();
	}

void Splan::drawGraph(size_t const &type,bool const &b_edge)
{
	bex::Graph *graphPtr;
	double gridStep;
	switch (type)
	{
	case graphType::base:{		
		gridStep = _mainMap.getGridStep();
		graphPtr = &_mainMap.getGraph(graphType::base);
		break;
	}
	case graphType::span: {
		gridStep = _mainMap.getGridStep() * 2;
		graphPtr = &_mainMap.getGraph(graphType::span);
		break;
	}
	case graphType::tree: {
		gridStep = _mainMap.getGridStep();
		graphPtr = &_m_tGraph;
		break;
	}
	default:
		break;
	}
	auto &graph = (*graphPtr);

	std::pair<bex::VertexIterator, bex::VertexIterator> _b2e_vi = boost::vertices(graph);
	size_t  i = 0;

	for (bex::VertexIterator vit = _b2e_vi.first; vit != _b2e_vi.second; vit++)
	{
		bex::VertexDescriptor vd = *vit;
		bex::VertexProperty &vp = graph[vd];
		double px = vp.pnt.x();
		double py = vp.pnt.y();
		//			std::cout << "px" << px << std::endl;
		//			std::cout << "py" << py << std::endl;

		//deg::conf_debug << "px" << px << std::endl;
		//deg::conf_debug << "py" << py << std::endl;

		svg::Point pntc(px, py);
		svg::Point pnt(px - gridStep / 2, py + gridStep / 2);
		svg::Fill fill();
		svg::Stroke stroke(1, svg::Color::Cyan);

		if ((type == graphType::base)||(type ==graphType::tree))
		{
			switch (vp.Type)
			{
			case bex::vertType::ObVert:
			{
				doc << svg::Rectangle(pnt, gridStep, gridStep, svg::Fill(svg::Color::Fuchsia), svg::Stroke(0.1, svg::Color::Cyan));
				//doc << svg::Circle(pntc, gridStep, svg::Fill(svg::Color::Fuchsia), svg::Stroke(0.1, svg::Color::Cyan));
				break;
			}
			case bex::vertType::WayVert:
			{
				doc << svg::Rectangle(pnt, gridStep, gridStep, svg::Fill(), svg::Stroke(0.1, svg::Color::Cyan));
				//doc << svg::Circle(pntc, gridStep, svg::Fill(), svg::Stroke(0.1, svg::Color::Cyan));
				break;
			}
			default:
				break;
			}
		}
		else {
			switch (vp.Type)
			{
			case bex::vertType::ObVert:
			{
				doc << svg::Rectangle(pnt, gridStep, gridStep, svg::Fill(svg::Color::Fuchsia), svg::Stroke(0.5, svg::Color::Cyan));
//				doc << svg::Circle(pntc, gridStep, svg::Fill(svg::Color::Fuchsia), svg::Stroke(0.3, svg::Color::Cyan));
				break;
			}
			case bex::vertType::WayVert:
			{
				doc << svg::Rectangle(pnt, gridStep, gridStep, svg::Fill(), svg::Stroke(0.4, svg::Color::Brown));
//				doc << svg::Circle(pntc, gridStep, svg::Fill(), svg::Stroke(0.1, svg::Color::Brown));
				break;
			}
			default:
				break;
			}
		}
	}

	if (b_edge)
	{
		std::pair<bex::EdgeIterator, bex::EdgeIterator> _b2e_ei = boost::edges(graph);

		for (auto eit = _b2e_ei.first; eit != _b2e_ei.second; eit++)
		{
			bex::EdgeDescriptor ed = *eit;
			bex::EdgeProperty &ep = graph[ed];
			bex::VertexDescriptor sVertd = boost::source(ed, graph);
			bex::VertexDescriptor tVertd = boost::target(ed, graph);

			bex::VertexProperty &sVert = graph[sVertd];
			bex::VertexProperty &tVert = graph[tVertd];

			svg::Polyline polyline_a(svg::Stroke(.2, svg::Color::Blue));

			double spx = sVert.pnt.x();
			double spy = sVert.pnt.y();
			//std::cout << "px" << px << std::endl;
			//std::cout << "py" << py << std::endl;
			svg::Point spnt(spx, spy);

			double tpx = tVert.pnt.x();
			double tpy = tVert.pnt.y();
			//std::cout << "px" << px << std::endl;
			//std::cout << "py" << py << std::endl;
			svg::Point tpnt(tpx, tpy);
			polyline_a << spnt << tpnt;
			doc << polyline_a;
		}
	}
}

void Splan::drawTree()
{
	auto &graph = _mainMap.getGraph(pl::graphType::span);
	for (size_t i = 0; i < _vTree.size(); i++)
	{
		bex::VertexProperty &sVert = graph[i];
		bex::VertexProperty &tVert = graph[_vTree[i]];


		svg::Polyline polyline_a(svg::Stroke(.3, svg::Color::Red));

		double spx = sVert.pnt.x();
		double spy = sVert.pnt.y();
		//std::cout << "px" << px << std::endl;
		//std::cout << "py" << py << std::endl;
		svg::Point spnt(spx, spy);

		double tpx = tVert.pnt.x();
		double tpy = tVert.pnt.y();
		//std::cout << "px" << px << std::endl;
		//std::cout << "py" << py << std::endl;
		svg::Point tpnt(tpx, tpy);
		polyline_a << spnt << tpnt;
		doc << polyline_a;
	}
}

void Splan::drawPath()
{
	svg::Polyline polyline_Path(svg::Stroke(.5, svg::Color::Purple));
	for (size_t i = 0; i < _path.size(); i++)
	{
		svg::Point spnt(_path[i].x(), _path[i].y());
		polyline_Path << spnt;
	}
	doc << polyline_Path;
	double gridStep = _mainMap.getGridStep();
	svg::Point pntc(_startPnt.x(), _startPnt.y());
	doc << svg::Circle(pntc, gridStep, svg::Fill(svg::Color::Transparent), svg::Stroke(0.1, svg::Color::Cyan));

	//doc << svg::Text(svg::Point(_startPnt.x(), _startPnt.y()), "Simple SVG", svg::Color::Silver, svg::Font(0.1, "Verdana"));
}

void pl::Splan::spanningTreePrim()
{

	typedef bt::adjacency_list < bt::vecS, bt::vecS, bt::undirectedS,
		bt::property<bt::vertex_distance_t, int>, bt::property < bt::edge_weight_t, int > > SGraph;
	typedef std::pair < int, int >SE;
	auto &graph = _mainMap.getGraph(graphType::span);
	const int num_nodes = bt::num_vertices(graph);

	SE edges[] = { SE(0, 2), SE(1, 3), SE(1, 4), SE(2, 1), SE(2, 3),
		SE(3, 4), SE(4, 0)
	};

	int weights[] = { 1, 1, 2, 7, 3, 1, 1 };


	std::pair<bex::EdgeIterator, bex::EdgeIterator> _b2e_ei = boost::edges(graph);

	vector<size_t> vSvd, vTvd;
	auto edgeNum = bt::num_edges(graph);
	//auto edgesPtr = make_shared<SE>(new SE[edgeNum]);
	SE * edgesPtr = new SE[edgeNum];
	int * weightPtr = new int[edgeNum];
	//auto edgesPtr = make_shared<std::array<SE,edgeNum>>();
	size_t i = 0;
	for (auto eit = _b2e_ei.first; eit != _b2e_ei.second; eit++)
	{
		bex::EdgeDescriptor ed = *eit;
		bex::EdgeProperty &ep = graph[ed];
		bex::VertexDescriptor sVertd = boost::source(ed, graph);
		bex::VertexDescriptor tVertd = boost::target(ed, graph);
		vSvd.push_back(sVertd);
		vTvd.push_back(tVertd);
		edgesPtr[i] = SE(sVertd, tVertd);
		weightPtr[i] = ep.weight;
		i++;
		//cout << "w" << endl;
	}


#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
	Graph g(num_nodes);
	property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
	for (std::size_t j = 0; j < sizeof(edges) / sizeof(E); ++j) {
		graph_traits<Graph>::edge_descriptor e; bool inserted;
		boost::tie(e, inserted) = add_edge(edges[j].first, edges[j].second, g);
		weightmap[e] = weights[j];
	}
#else
	//		SGraph g(edgesPtr, edgesPtr + sizeof(edgesPtr) / sizeof(SE), weightPtr, num_nodes);
	SGraph g(edgesPtr, edgesPtr + edgeNum, weightPtr, num_nodes);

#endif
	std::vector < bt::graph_traits < SGraph >::vertex_descriptor >
		p(num_vertices(g));

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
	property_map<Graph, vertex_distance_t>::type distance = get(vertex_distance, g);
	property_map<Graph, vertex_index_t>::type indexmap = get(vertex_index, g);
	prim_minimum_spanning_tree
	(g, *vertices(g).first, &p[0], distance, weightmap, indexmap,
		default_dijkstra_visitor());
#else
	bt::prim_minimum_spanning_tree(g, &p[0]);
#endif
	_vTree.clear();
	_vTreeSg.clear();
	for (std::size_t i = 0; i != p.size(); ++i)
	{
		this->_vTree.push_back(p[i]);
#ifdef _DEBUG
		//cout << "_ob_sGraph_0   x =" << _ob_sGraph[i].pnt.x() << " y " << _ob_sGraph[i].pnt.y() << endl;
		//cout << "_ob_sGraph_1   x =" << _ob_sGraph[p[i]].pnt.x() << " y " << _ob_sGraph[p[i]].pnt.y() << endl;
#endif // _DEBUG
		this->_vTreeSg.push_back(bex::DSegment(_ob_sGraph[i].pnt, _ob_sGraph[p[i]].pnt));
		if (p[i] != i)
			std::cout << "parent[" << i << "] = " << p[i] << std::endl;
		else
			std::cout << "parent[" << i << "] = no parent" << std::endl;
	}
}

void Splan::getNewGraph()
{
	std::pair<bex::VertexIterator, bex::VertexIterator> _b2e_vi = boost::vertices(_ob_tGraph);
	size_t  i = 0;
	for (bex::VertexIterator vit = _b2e_vi.first; vit != _b2e_vi.second; vit++)
	{
		bex::VertexDescriptor vd = *vit;
		bex::VertexProperty vp = _ob_tGraph[vd];
		vp.EdgeState = false;
		vp.NeighbourState = false;
		vp.QueueState = false;
		boost::add_vertex(vp, _m_tGraph);
	}

	std::pair<bex::VertexIterator, bex::VertexIterator> vi = boost::vertices(_m_tGraph);
	for (bex::VertexIterator vit = vi.first; vit != vi.second; vit++)
	{
		bex::VertexDescriptor vd = *vit;
		if (_m_tGraph[vd].Type == bex::vertType::WayVert)
		{
			auto localIndex = _ob_tgraph2map[vd];
			auto vlocalIndex = getVerticalNeighbour(localIndex);
				//getSearchNeighbor(localIndex, graphType::span);
			std::vector<int> vvd;
			for (auto &it : vlocalIndex)
			{
				vvd.push_back(_ob_tmap2graph[it]);
			}

			for (auto &it : vvd)
			{
				if (_m_tGraph[it].EdgeState == false)
				{
					bex::DSegment sg(_m_tGraph[it].pnt, _m_tGraph[vd].pnt);
					if (this->treeIntersection(sg))
					//≈–∂œ «∑Òœ‡Ωª
					{
						//std::cout << "0-0" << endl;
					}
					else
					{
						bex::EdgeProperty ep;
						//ep.weight = bg::distance(sGraph[vd].pnt, sGraph[it].pnt);
						boost::add_edge(it, vd, ep, _m_tGraph);
					}
				}
			}
		}
		_m_tGraph[vd].EdgeState = true;
	}

}

void Splan::searchPath()
{
	GridIndex  cenGridIndex =  _mainMap.pnt2Index(_startPnt, graphType::base);
	bex::VertexDescriptor cenVd = this->_ob_tmap2graph[cenGridIndex];
	bex::VertexDescriptor canVd(cenVd);
	size_t cenDir = DirType::center;
	size_t canDir = DirType::center;

	size_t i = 0;
	do
	{
#ifdef _DEBUG
		cout << "i = " << i++ << endl;
#endif // _DEBUG
		cenVd = canVd;
		cenDir = canDir;
		//canDir = cenDir;
		_pathIndex.push_back(cenVd);
		auto neighborsIter = boost::adjacent_vertices(cenVd, _m_tGraph);
		bex::VertexProperty &cenVp = _m_tGraph[cenVd];
		cenVp.NeighbourState = true;
		this->_path.push_back(cenVp.pnt);
		vector<bex::VertexDescriptor> vNeighbors;
		for (auto ni = neighborsIter.first; ni != neighborsIter.second; ++ni)
		{
			if (!_m_tGraph[*ni].NeighbourState)
			{
				vNeighbors.push_back(*ni);
			}
		}
		canVd = cenVd;
		vector<size_t> vCanVd;
		bool chsSameMega = false;
		//
		for (auto &it : vNeighbors)
		{
			auto &vp = _m_tGraph[it];
			//has not been covered
			if (vp.NeighbourState == false)
			{
				if (inSameMegaBox(it, cenVd))
				{
					canVd = it;
					chsSameMega = true;
					break;
				}
				else
				{
					vCanVd.push_back(it);
				}
			}
		}
		//no vertex in the same mega box
		if (!chsSameMega)
		{
			if (!vCanVd.empty())
			{
				if (vCanVd.size() == 1)
				{
					canVd = vCanVd.back();
				}
				else
				{
					vector<size_t> vDir;
					for (auto &it : vCanVd)
					{
						if (cenDir == getDir(cenVd, it))
							canVd = it;
					}
				}
			}
		}
		canDir = getDir(cenVd, canVd);
	} while (canVd != cenVd);

}

bool Splan::treeIntersection(bex::DSegment const sg) const
{

	for (auto &it : this->_vTreeSg)
	{
		if (bg::intersects(it, sg))
		{
			return true;
		}
	}
	return false;

}

vector<GridIndex> Splan::getVerticalNeighbour(GridIndex const & cen_index)
{
	auto &grid = this->_ob_tGrid;

	vector<GridIndex> vIndex;
	auto &i = cen_index.first;
	auto &j = cen_index.second;
	auto neighbour = [=](GridIndex &ind, vector<GridIndex>  &vInd) {
		if (grid.count(ind))
		{
			if (grid.at(ind).type == bex::vertType::WayVert)
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

vector<pair<GridIndex, size_t>> Splan::getVerticalNeighbourWithDir(GridIndex const & cen_index)
{
	auto &grid = this->_ob_tGrid;

	vector<pair<GridIndex, size_t>> res;
	vector<GridIndex> vIndex;
	vector<size_t> vDir;
	
	auto &i = cen_index.first;
	auto &j = cen_index.second;
	auto neighbour = [=](GridIndex &ind, vector<GridIndex>  &vInd) {
		if (grid.count(ind))
		{
			if (grid.at(ind).type == bex::vertType::WayVert)
			{
				vInd.push_back(ind);
				return true;
			}
		}
		return false;
	};

	//left
	if(neighbour(GridIndex(i - 1, j), vIndex)) vDir.push_back(DirType::left);
	//top 
	if(neighbour(GridIndex(i, j + 1), vIndex)) vDir.push_back(DirType::top);
	//right
	if(neighbour(GridIndex(i + 1, j), vIndex)) vDir.push_back(DirType::right);
	//botton
	if(neighbour(GridIndex(i, j - 1), vIndex)) vDir.push_back(DirType::bottom);
	
	for (size_t i = 0; i < vDir.size(); i++)
	{
		res.push_back(pair<GridIndex, size_t>(vIndex[i], vDir[i]));
	}
	return res;
}

size_t Splan::getDir(bex::VertexDescriptor const & cen_index, bex::VertexDescriptor const & n_index)
{
	GridIndex cenGridInd = _ob_tgraph2map[cen_index];
	GridIndex nGridInd = _ob_tgraph2map[n_index];
	// x equal 
	if (cenGridInd.first == nGridInd.first)
	{
		if (cenGridInd.second == (nGridInd.second - 1)) {
			return DirType::bottom;
		}		
		if (cenGridInd.second == (nGridInd.second + 1)) {
			return DirType::top;
		}
	}
	if (cenGridInd.second == nGridInd.second)
	{
		if (cenGridInd.first == (nGridInd.first - 1)) {
			return DirType::left;
		}
		if (cenGridInd.first == (nGridInd.first + 1)) {
			return DirType::right;
		}
	}
	return DirType::center;
}

bool Splan::inSameMegaBox(bex::VertexDescriptor const & vd0, bex::VertexDescriptor const & vd1)
{
	GridIndex index0 = _mainMap.pnt2Index(_m_tGraph[vd0].pnt, graphType::span);
	//cout << " x0 =" << _m_tGraph[vd0].pnt.x() << "	y0 = " << _m_tGraph[vd0].pnt.y() << endl;
	GridIndex index1 = _mainMap.pnt2Index(_m_tGraph[vd1].pnt, graphType::span);
	//cout << " x1 =" << _m_tGraph[vd1].pnt.x() << "	y1 = " << _m_tGraph[vd1].pnt.y() << endl;
	if ((index0.first == index1.first) && (index0.second == index1.second))
	{
		return true;
	}
	return false;
}

}