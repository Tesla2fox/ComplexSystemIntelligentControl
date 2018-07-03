#include "MultiPlan.hpp"

namespace pl {

	void MultiPlan::drawGraph(size_t const & type, bool const & b_edge)
	{
		bex::Graph *graphPtr; 
		double gridStep;
		switch (type)
		{
		case graphType::base: {
			gridStep = _mainMap.getGridStep();
			graphPtr = &_mainMap.getGraph(graphType::base);
			break;
		}
		case graphType::span: {
			gridStep = _mainMap.getGridStep() * 2;
			graphPtr = &_mainMap.getGraph(graphType::span);
			break;
		}
							  //case graphType::tree: {
							  //	gridStep = _mainMap.getGridStep();
							  //	graphPtr = &_m_tGraph;
							  //	break;
							  //}
		default:
		{
			cout << "error type" << endl;
			return;
			break;
		}
		}
		auto &graph = (*graphPtr);

		std::pair<bex::VertexIterator, bex::VertexIterator> _b2e_vi = boost::vertices(graph);
		size_t  i = 0;
		if (true)
		{

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

				if ((type == graphType::base) || (type == graphType::tree))
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
					doc << svg::Text(pntc, std::to_string(vd), svg::Color::Brown, svg::Font(3));
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

				svg::Polyline polyline_a(svg::Stroke(.2, svg::Color::Yellow));

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

	void MultiPlan::drawRobGraph()
	{

		for (size_t p = 0; p < _robNum; p++)
		{

			bex::Graph *graphPtr;
			double gridStep;
			gridStep = _mainMap.getGridStep();
			graphPtr = &this->_m_vGraph[p];
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

				doc << svg::Rectangle(pnt, gridStep, gridStep, svg::Fill(), svg::Stroke(0.1, svg::Color::Cyan));

			}

			if (true)
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
	}

	void MultiPlan::drawStartLocation()
	{
		double gridStep = _mainMap.getGridStep();
		size_t iS = 0;
		for (auto &it : this->_vStartPnt)
		{
			svg::Point pntc(it.x()/*- gridStep/2*/ , it.y()/*- gridStep/2*/);
			doc << svg::Circle(pntc, gridStep, svg::Fill(svg::Color::Transparent), svg::Stroke(0.1, svg::Color::Purple));
			doc << svg::Text(pntc, std::to_string(iS++), svg::Color::Silver, svg::Font(3));
		}
	}
	void MultiPlan::drawRobSet(bool const &neibool)
	{
		double gridStep = _mainMap.getGridStep();
		auto &graph = _ob_sGraph;
		vector<set<size_t>> &_vRobSet(*_vRobSetPtr);

		//		svg::Color color(1);
		//		svg::Fill  file();
		size_t robID = 0;
		for (auto &it : _vRobSet) {
			for (auto itSet = it.begin(); itSet != it.end(); ++itSet)
			{
				const bex::VertexDescriptor &vd = (*itSet);
				bex::VertexProperty &vp = graph[vd];
				double px = vp.pnt.x();
				double py = vp.pnt.y();

				//svg::Polyline PloyTreeEdge(svg::Stroke(.4, svg::Color::Red));
				//bex::VertexDescriptor &tVertd = (*itSet).second;
				//bex::VertexProperty &tVert = graph[tVertd];
				//double tpx = tVert.pnt.x();
				//double tpy = tVert.pnt.y();
				//svg::Point spnt(px, py);
				//svg::Point tpnt(tpx, tpy);
				//PloyTreeEdge << spnt << tpnt;
				//doc << PloyTreeEdge;
				svg::Point pnt(px - gridStep / 2, py + gridStep / 2);
				doc << svg::Text(pnt, std::to_string(robID), svg::Color::Blue, svg::Font(3));

				if (true) {
					switch (robID)
					{
					case 0:
					{
						doc << svg::Rectangle(pnt, gridStep, gridStep, svg::Fill(), svg::Stroke(0.1, svg::Color::Fuchsia));
						break;
					}
					case 1:
					{
						doc << svg::Rectangle(pnt, gridStep, gridStep, svg::Fill(), svg::Stroke(0.1, svg::Color::Blue));
						break;
					}
					case 2:
					{
						doc << svg::Rectangle(pnt, gridStep, gridStep, svg::Fill(svg::Color::Red), svg::Stroke(0.1, svg::Color::Red));
						break;
					}
					default:
						doc << svg::Rectangle(pnt, gridStep, gridStep, svg::Fill(svg::Color::Yellow), svg::Stroke(0.3, svg::Color::Yellow));
						break;
					}
				}
			}
			if (neibool)
			{
				for (auto itSet = _vRobNghSetPtr->at(robID).begin(); itSet != _vRobNghSetPtr->at(robID).end(); ++itSet)
				{
					const bex::VertexDescriptor &vd = *itSet;
					bex::VertexProperty &vp = graph[vd];
					double px = vp.pnt.x();
					double py = vp.pnt.y();
					svg::Point pnt(px, py);
					doc << svg::Circle(pnt, gridStep, svg::Fill(), svg::Stroke(0.2, svg::Color::Green));
				}
				//for (auto itSet = _vRobNeiSetPtr->at(robID).begin(); itSet != _vRobNeiSetPtr->at(robID).end(); ++itSet)
				//{

				//	const bex::VertexDescriptor &vd = (*itSet).first;
				//	bex::VertexProperty &vp = graph[vd];
				//	double px = vp.pnt.x();
				//	double py = vp.pnt.y();
				//	svg::Point pnt(px, py);

				//	svg::Polyline PloyTreeEdge(svg::Stroke(.25, svg::Color::Green));
				//	bex::VertexDescriptor &tVertd = (*itSet).second;
				//	bex::VertexProperty &tVert = graph[tVertd];
				//	double tpx = tVert.pnt.x();
				//	double tpy = tVert.pnt.y();

				//	svg::Point spnt(px, py);
				//	svg::Point tpnt(tpx, tpy);
				//	PloyTreeEdge << spnt << tpnt;
				//	doc << PloyTreeEdge;
				//	doc << svg::Circle(pnt, gridStep, svg::Fill(), svg::Stroke(0.2, svg::Color::Green));
				//}
			}
			robID++;
		}

	}
	void MultiPlan::drawPath()
	{
		for (size_t p = 0; p < _robNum; p++)
		{
			auto &path = this->_vpath[p];
			vector<double> vx, vy;
			for (size_t i = 0; i < path.size(); i++)
			{
				vx.push_back(path[i].x());
				vy.push_back(path[i].y());
			}
			auto str_ind = std::to_string(p);
			string path_x_str = "path_x_" + str_ind;
			string path_y_str = "path_y_" + str_ind;
			
			c_deg << path_x_str;
			for (auto & it : vx)
			{
				c_deg << " " << it;
			}
			c_deg << endl;

			c_deg << path_y_str;
			for (auto & it : vy)
			{
				c_deg << " " << it;
			}
			c_deg << endl;
		}
		for (size_t p = 0; p < _robNum; p++)
		{
			auto &_path = this->_vpath[p];
			switch (p)
			{
			case 0:
			{
				svg::Polyline polyline_Path(svg::Stroke(.5, svg::Color::Red));
				for (size_t i = 0; i < _path.size(); i++)
				{
					svg::Point spnt(_path[i].x(), _path[i].y());
					polyline_Path << spnt;
				}
				doc << polyline_Path;
				break;
			}
			case 1:
			{
				svg::Polyline polyline_Path(svg::Stroke(.5, svg::Color::Green));
				for (size_t i = 0; i < _path.size(); i++)
				{
					svg::Point spnt(_path[i].x(), _path[i].y());
					polyline_Path << spnt;
				}
				doc << polyline_Path;
				break;
			}
			case 2:
			{
				svg::Polyline polyline_Path(svg::Stroke(.5, svg::Color::Blue));
				for (size_t i = 0; i < _path.size(); i++)
				{
					svg::Point spnt(_path[i].x(), _path[i].y());
					polyline_Path << spnt;
				}
				doc << polyline_Path;
				break;
			}
			default:
			{
				svg::Polyline polyline_Path(svg::Stroke(.5, svg::Color::Aqua));
				for (size_t i = 0; i < _path.size(); i++)
				{
					svg::Point spnt(_path[i].x(), _path[i].y());
					polyline_Path << spnt;
				}
				doc << polyline_Path;
				break;
			}

				break;
			}
		}

	}
	void MultiPlan::writePath()
	{

	}
	void MultiPlan::writeSgsTree()
	{

		for (size_t p = 0; p < _robNum; p++)
		{
			auto &TreeSgs = this->_vTreeSgs[p];
			
			vector<double> tvx, tvy, svx, svy;
			for (size_t i = 0; i < TreeSgs.size(); i++)
			{
				tvx.push_back(TreeSgs[i].first.x());
				svx.push_back(TreeSgs[i].second.x());
				tvy.push_back(TreeSgs[i].first.y());
				svy.push_back(TreeSgs[i].second.y());
			}
			

			auto str_ind = std::to_string(p);
			string tvx_str = "tvx_" + str_ind;
			string tvy_str = "tvy_" + str_ind;
			string svx_str = "svx_" + str_ind;
			string svy_str = "svy_" + str_ind;

			c_deg << tvx_str;
			for (auto & it : tvx)
			{
				c_deg << " " << it;
			}
			c_deg << endl;

			c_deg << tvy_str;
			for (auto & it : tvy)
			{
				c_deg << " " << it;
			}
			c_deg << endl;

			c_deg << svx_str;
			for (auto & it : svx)
			{
				c_deg << " " << it;
			}
			c_deg << endl;

			c_deg << svy_str;
			for (auto & it : svy)
			{
				c_deg << " " << it;
			}
			c_deg << endl;


		}

	}
	void MultiPlan::cenPathPlanning()
	{
	    cenAuction();
		//auction();
		this->_vTreeSgs.clear();
		this->_vTreeSgs.resize(this->_robNum);
		getSpanningTreeSgs();
		getNewGraph();
		searchPath();		
	}

	void MultiPlan::disPathPlanning()
	{
		auction();
		//return;
		this->_vTreeSgs.clear();
		this->_vTreeSgs.resize(_robNum);
		getSpanningTreeSgs();
		getNewGraph();
		searchPath();

	}

	void MultiPlan::writeDebug(string const & str, vector<double> const & v_val)
	{
		c_deg << str;
		for (auto & it : v_val)
		{
			c_deg << " " << it;
		}
		c_deg << endl;
	}

	void MultiPlan::auction()
	{
		
		_vRobSetPtr = make_shared<vector<set<size_t>>>(_robNum);
		_vRobNeiSetPtr = make_shared<vector<map<size_t, size_t>>>(_robNum);
		_vRobNghSetPtr = make_shared<vector<set<size_t>>>(_robNum);
		_notBidSetPtr = make_shared<set<size_t>>();
		_vRobSleepPtr = make_shared<vector<bool>>(_robNum, false);
		vector<set<size_t>> &_vRobSet(*_vRobSetPtr);
		vector<map<size_t, size_t>> &_vRobNeiSet(*_vRobNeiSetPtr);
		vector<set<size_t>> &_vRobNghSet(*_vRobNghSetPtr);

		vector<bool> allAucEd(bt::num_vertices(_ob_sGraph), false);

		auto sObSet = _mainMap.getsObSet();
		for (auto &it : sObSet) {
			allAucEd[_ob_smap2graph[it]] = true;
		}
		auto aucCompleted = [](vector<bool> const &vb) {
			auto iter = std::find(vb.begin(), vb.end(), false);
			if (iter == vb.end()) 
 				return true;
			return false;
		};


		for (size_t i = 0; i < this->_vStartPnt.size(); i++)
		{

			//vector<pair<size_t, size_t>> &robSpanTree = _vRobSpanTree[i];
			GridIndex robGridInd = _mainMap.pnt2Index(_vStartPnt[i], pl::graphType::span);
			//c_deg<<"start"
			//c_deg << "index 0 - " << robGridInd.first << " index 1 - " << robGridInd.second << endl;
			bex::VertexDescriptor robVd = _ob_smap2graph[robGridInd];
			//_vRobSet[i].insert(pair<size_t, size_t>(robVd, robVd));
			_vRobSet[i].insert(robVd);
			auto neighborsIter = bt::adjacent_vertices(robVd, _ob_sGraph);
			for (auto ni = neighborsIter.first; ni != neighborsIter.second; ++ni)
			{
				_vRobNeiSet[i].insert(pair<size_t, size_t>(*ni, robVd));
				_vRobNghSet[i].insert(*ni);
			}
			_notBidSetPtr->insert(robVd);
			allAucEd[robVd] = true;
		}

		//vector<set<size_t>> _vRobNei;
		size_t circleTime = 0;
		size_t maxcircleTime = _mainMap.vertNum() * 2;
		
		std::default_random_engine eng;
		eng.seed(randomSeed);
		std::uniform_int_distribution<int> dis(0, _robNum - 1);
		//cout << "_robNum = " << _robNum << endl;
		//return;
		clock_t start, finish;
		start = clock();
		size_t aucNeerC = 0;
		do
		{

			//size_t aucNeer = (aucNeerC++) % _robNum;
			size_t aucNeer = dis(eng);
			if (aucNeer == _robNum) aucNeer = _robNum - 1;
			//size_t aucNeer = 1;
			bex::VertexDescriptor aucInd;
			auto &robSet = _vRobSet[aucNeer];

			
			//cout << "0-0" << endl;

			bool allCovered = getMinLeaf(aucNeer, aucInd);
			//bool allCovered = getAuctionInd(aucInd);
			
			//c_deg << " times  = " << circleTime;
			//for (size_t i = 0; i < _robNum; i++)
			//{
			//	c_deg << "	" << _vRobSleepPtr->at(i);
			//}
			//c_deg << endl;

			//cout << "bug 0-0 " << endl;
			if (_vRobSleepPtr->at(aucNeer))
			{ 
//				cout << "sleep in " << aucNeer << endl;
				continue;
			}

			size_t successBidRobID = minBidRob(aucNeer, aucInd);
			
			
			if (allCovered)
			{

				updateSetWithErase(successBidRobID, aucInd);
				//if (successBidRobID == aucNeer) {
				//	drawRobSet(false);
				//	savePic();
				//	cout << "stop" << endl;
				//}				
			}
			else {
				updateSet(successBidRobID, aucInd);
			}

			
			allAucEd[aucInd] = true;
			circleTime++;
			//cout << "circleTime  =" << circleTime << endl;
			//if (circleTime == 30000)
			//{
			//	cout << "bug" << endl;
			//	drawGraph(pl::graphType::base, false);
			//	drawGraph(pl::graphType::span, false);
			//	drawRobSet(true);
			//	// demonPlan.cenPathPlanning();
			//	//	multi_plan.disPathPlanning();
			//	drawStartLocation();
			//	savePic();
			//	//break;
			//}
			if (circleTime>maxcircleTime)
			{
				break;
			}
			//if (!(circleTime % 20))
			//{
			//	c_deg << circleTime << "fitnees = " << calSolutionFitNess() << endl;
			//}
			//if (circleTime == 220)
			//	cout << "bug" << endl;

			//if (aucCompleted(*_vRobSleepPtr))
			//{
			//	c_deg << "bug is here" << endl;
			//	break;
			//}
			//for (size_t i = 0; i < _robNum; i++)
			//{
			//	vector<size_t> vvd;
			//	for (auto &it : _vRobSetPtr->at(i))
			//		vvd.push_back(it);
			//	if (!_mainMap.allConnected(vvd))
			//	{
			//		//					c_deg << i << "	rob true" << endl;
			//		c_deg << "aucInd = " << aucInd << endl;
			//		c_deg << "successBidRobID = " << successBidRobID << endl;
			//		c_deg << i << "	rob false" << endl;
			//		drawGraph(pl::graphType::base, false);
			//		drawRobSet(true);
			//		drawGraph(pl::graphType::span, false);
			//		// demonPlan.cenPathPlanning();
			//		//	multi_plan.disPathPlanning();
			//		drawStartLocation();
			//		savePic();
			//		return;
			//	}
			//}
		} while (!aucCompleted(*_vRobSleepPtr));

		finish = clock();
		double totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
		c_deg << "total time = " << totaltime << endl;

		vector<size_t> vSize;
		size_t allNumSize = 0;
		for (size_t i = 0; i < _robNum; i++)
		{
			c_deg << "rob" << i << "	set size = " << _vRobSetPtr->at(i).size() << endl;
			//c_deg << "rob" << i << "	set size = " << _vRobSetPtr->at(i).size() << endl;
			vSize.push_back(_vRobSetPtr->at(i).size());
			allNumSize += _vRobSetPtr->at(i).size();
		}
		auto maxSize = *std::max_element(vSize.begin(), vSize.end());
		auto minSize = *std::min_element(vSize.begin(), vSize.end());

		c_deg << "max  = " << maxSize << " min = " << minSize << endl;
		c_deg << "allNumSize = " << allNumSize << endl;

		//debug msg 

		for (size_t i = 0; i < _robNum; i++)
		{
			vector<size_t> vvd;
			for (auto &it : _vRobSetPtr->at(i))
				vvd.push_back(it);
			if (_mainMap.allConnected(vvd))
				c_deg << i << "	rob true" << endl;
			else
				c_deg << i << "	rob false" << endl;
		}
		drawGraph(pl::graphType::base, false);
		drawRobSet(true);
		drawGraph(pl::graphType::span, false);
		// demonPlan.cenPathPlanning();
		//	multi_plan.disPathPlanning();
		drawStartLocation();
		savePic();
		cout << "auction end" << endl;
	}

	void MultiPlan::cenAuction()
	{

		//_vRobSetPtr = make_shared<vector<map<size_t, size_t>>>(_robNum);
		//_vRobNeiSetPtr = make_shared<vector<map<size_t, size_t>>>(_robNum);
		//_notBidSetPtr = make_shared<set<size_t>>();
		//vector<map<size_t, size_t>> &_vRobSet(*_vRobSetPtr);
		//vector<map<size_t, size_t>> &_vRobNeiSet(*_vRobNeiSetPtr);

		//vector<bool> allAucEd(bt::num_vertices(_ob_sGraph), false);

		//auto sObSet = _mainMap.getsObSet();
		//for (auto &it : sObSet) {
		//	allAucEd[_ob_smap2graph[it]] = true;
		//}


		//auto aucCompleted = [](vector<bool> const &vb) {
		//	auto iter = std::find(vb.begin(), vb.end(), false);
		//	if (iter == vb.end()) return true;
		//	return false;
		//};


		//for (size_t i = 0; i < this->_vStartPnt.size(); i++)
		//{

		//	//vector<pair<size_t, size_t>> &robSpanTree = _vRobSpanTree[i];
		//	GridIndex robGridInd = _mainMap.pnt2Index(_vStartPnt[i], pl::graphType::span);
		//	bex::VertexDescriptor robVd = _ob_smap2graph[robGridInd];
		//	_vRobSet[i].insert(pair<size_t, size_t>(robVd, robVd));
		//	auto neighborsIter = bt::adjacent_vertices(robVd, _ob_sGraph);
		//	for (auto ni = neighborsIter.first; ni != neighborsIter.second; ++ni)
		//	{
		//		_vRobNeiSet[i].insert(pair<size_t, size_t>(*ni, robVd));
		//	}
		//	_notBidSetPtr->insert(robVd);
		//	allAucEd[robVd] = true;
		//}

		////vector<set<size_t>> _vRobNei;
		//size_t circleTime = 0;

		//std::default_random_engine eng;
		//eng.seed(2);
		//std::uniform_int_distribution<int> dis(0, _robNum);
		////return;
		//do
		//{

		//	size_t aucNeer = dis(eng);

		//	if (aucNeer == _robNum) aucNeer = _robNum - 1;
		//	//size_t aucNeer = 1;

		////	c_deg << "aucNeer  = " << aucNeer << endl;
		//	bex::VertexDescriptor aucInd;
		//	auto &robSet = _vRobSet[aucNeer];

		//	//bool allCovered = getMinLeaf(aucNeer, aucInd);
		//	bool allCovered = getAuctionInd(aucInd);


		//	size_t successBidRobID = minBidRob(aucNeer, aucInd);

		//	if (allCovered)
		//	{
		//		updateSetWithErase(successBidRobID, aucInd);
		//		//if (successBidRobID == aucNeer) {
		//		//	drawRobSet(false);
		//		//	savePic();
		//		//	cout << "stop" << endl;
		//		//}				
		//	}
		//	else {
		//		updateSet(successBidRobID, aucInd);
		//	}


		//	allAucEd[aucInd] = true;
		//	circleTime++;
		//	cout << "circleTime  =" << circleTime << endl;
		//	if (circleTime > 320)
		//	{
		//		break;
		//	}
		//} while (!aucCompleted(allAucEd));

		//for (size_t i = 0; i < _robNum; i++)
		//{
		//	cout << "rob" << i << "	set size = " << _vRobSetPtr->at(i).size() << endl;
		//}

		//cout << "auction end" << endl;
	}

	void MultiPlan::getSpanningTreeSgs()
	{


		typedef bt::adjacency_list < bt::vecS, bt::vecS, bt::undirectedS,
			bt::property<bt::vertex_distance_t, int>, bt::property < bt::edge_weight_t, int > > SGraph;
		typedef std::pair < int, int >SE;
		auto &graph = _ob_sGraph;
		for (size_t robID = 0; robID < _robNum; robID++)
		{
			cout << "rob is = " << robID << endl;
			//auto &graph = _mainMap.getGraph(graphType::span);
			//const int num_nodes = bt::num_vertices(graph);
			const int num_nodes = this->_vRobSetPtr->at(robID).size();

			vector<size_t> vSvd, vTvd;
			vector<bex::VertexDescriptor> vvd;
			for (auto &it :this->_vRobSetPtr->at(robID) )
			{
				vvd.push_back(it);
			}
			for (size_t i = 0; i < vvd.size(); i++)
			{
				for (size_t j = i; j < vvd.size(); j++)
				{
					if (_mainMap.IsConnected(vvd[i], vvd[j], pl::graphType::span))
					{
						vSvd.push_back(i);
						vTvd.push_back(j);
					}
				}
			}
			const int edgeNum = vSvd.size();
			SE * edgesPtr = new SE[edgeNum];
			int * weightPtr = new int[edgeNum];
			for (size_t i = 0; i < vSvd.size(); i++)
			{
				//cout << "index = " << i << endl;
				edgesPtr[i] = SE(vSvd[i], vTvd[i]);
				weightPtr[i] = 1;
			}


			SGraph g(edgesPtr, edgesPtr + edgeNum, weightPtr, num_nodes);

			std::vector < bt::graph_traits < SGraph >::vertex_descriptor >
				p(num_vertices(g));
			bt::prim_minimum_spanning_tree(g, &p[0]);
			for (std::size_t i = 0; i != p.size(); ++i)
			{
				//cout << i << " vvd[i]" << vvd[i] << " vvd[p[i]]" << vvd[p[i]] << endl;
				auto sg = bex::DSegment(_ob_sGraph[vvd[i]].pnt, _ob_sGraph[vvd[p[i]]].pnt);
				_vTreeSgs[robID].push_back(sg);
			}			
		}
	}

	void MultiPlan::getNewGraph()
	{
		auto &graph = _ob_tGraph;
		set<bex::VertexDescriptor> robBaseSet;
		_vT2local.clear();
		_vlocal2T.clear();
		for (size_t p = 0; p < _robNum; p++)
		{
			bex::Graph new_graph;
			auto &robSet = _vRobSetPtr->at(p);
			map<size_t, size_t> T2local;
			map<size_t, size_t> local2T;
			bex::VertexDescriptor localVd = 0;
			for (auto &it : robSet)
			{
				bex::VertexDescriptor svd = it;
				auto vvd = _mainMap.getTgraphVd(svd);
				for (auto &vd : vvd)
				{
					robBaseSet.insert(vd);
					bex::VertexProperty vp = graph[vd];
					vp.EdgeState = false;
					vp.NeighbourState = false;
					vp.QueueState = false;
					bt::add_vertex(vp, new_graph);
					T2local.insert(pair<size_t, size_t>(vd, localVd));
					local2T.insert(pair<size_t, size_t>(localVd, vd));
					localVd++;
				}
			}
			_vT2local.push_back(T2local);
			_vlocal2T.push_back(local2T);
			//add edge 
			std::pair<bex::VertexIterator, bex::VertexIterator> vi = boost::vertices(new_graph);
			for (bex::VertexIterator vit = vi.first; vit != vi.second; vit++)
			{
				bex::VertexDescriptor vd = *vit;
				if (new_graph[vd].Type == bex::vertType::WayVert)
				{
		/*			cout << "___" << endl;
					cout << " new_graph[vd].pnt x = " << new_graph[vd].pnt.x()
						<< " new_graph[vd].pnt y = " << new_graph[vd].pnt.y() << endl;
*/
					auto tvd = local2T[vd];
					auto nei_vvd = _mainMap.getSearchVerticalNeighbor(tvd, graphType::base);
					//auto vlocalIndex = getVerticalNeighbour(localIndex);
					//getSearchNeighbor(localIndex, graphType::span);
					std::vector<int> vvd;
					for (auto &it : nei_vvd)
					{
						if (robBaseSet.count(it) == 1)
						{
	//						cout << "nei__it " << it << endl;
							vvd.push_back(T2local[it]);
						}
					}
					for (auto &it : vvd)
					{
	//					cout << "vvd_it  = " << it << endl;

						if (new_graph[it].EdgeState == false)
						{
							bex::DSegment sg(new_graph[it].pnt, new_graph[vd].pnt);
							//cout << " new_graph[it].pnt x = " << new_graph[it].pnt.x()
							//	<< " new_graph[it].pnt y = " << new_graph[it].pnt.y() << endl;


							if (this->treeIntersection(sg, p))
								//≈–∂œ «∑Òœ‡Ωª
							{
//								std::cout << "inter" << endl;
							}
							else
							{
								bex::EdgeProperty ep;
								//ep.weight = bg::distance(sGraph[vd].pnt, sGraph[it].pnt);
								boost::add_edge(it, vd, ep, new_graph);
							}
						}
					}
				}
				new_graph[vd].EdgeState = true;
				size_t num_edges = bt::num_edges(new_graph);
				//cout << "num_edges = " << num_edges << endl;
			}

			this->_m_vGraph.push_back(new_graph);
		}

	}

	void MultiPlan::searchPath()
	{
		_vpathIndex.clear();
		_vpath.clear();
		_vpath.resize(_robNum);
		_vpathIndex.resize(_robNum);
		for (size_t p = 0; p < _robNum; p++)
		{
			auto _startPnt = this->_vStartPnt[p];
			GridIndex  cenGridIndex = _mainMap.pnt2Index(_startPnt, graphType::base);
			bex::VertexDescriptor totalCenVd = this->_ob_tmap2graph[cenGridIndex];

			bex::VertexDescriptor cenVd = _vT2local[p][totalCenVd];
			bex::VertexDescriptor canVd(cenVd);
			size_t cenDir = DirType::center;
			size_t canDir = DirType::center;

			auto &_pathIndex = _vpathIndex[p];
			auto &_path = _vpath[p];
			auto &localGraph = _m_vGraph[p];
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
				auto neighborsIter = boost::adjacent_vertices(cenVd, localGraph);
				bex::VertexProperty &cenVp = localGraph[cenVd];
				cenVp.NeighbourState = true;
				_path.push_back(cenVp.pnt);
				vector<bex::VertexDescriptor> vNeighbors;
				for (auto ni = neighborsIter.first; ni != neighborsIter.second; ++ni)
				{
					if (!localGraph[*ni].NeighbourState)
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
					auto &vp = localGraph[it];
					//has not been covered
					if (vp.NeighbourState == false)
					{
						if (inSameMegaBox(it, cenVd,p))
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
						if ((vCanVd.size() == 1) ||(cenDir == DirType::center))
						{
							canVd = vCanVd.back();
						}
						else
						{
							vector<size_t> vDir;
							for (auto &it : vCanVd)
							{
								if (cenDir == getDir(cenVd, it,p))
									canVd = it;
							}
						}
					}
				}
				canDir = getDir(cenVd, canVd,p);
			} while (canVd != cenVd);
		}
		//cout << "wtf" << endl;
	}

	bool MultiPlan::treeIntersection(bex::DSegment const sg, size_t const robID) const
	{
		auto &vTreeSg = this->_vTreeSgs[robID];
		for (auto &it : vTreeSg)
		{
			if (bg::intersects(it, sg))
			{
				return true;
			}
		}
		return false;
	}

	bool MultiPlan::inSameMegaBox(bex::VertexDescriptor const & vd0, bex::VertexDescriptor const & vd1, size_t const &robID)
	{
		auto & _m_tGraph = this->_m_vGraph[robID];
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

	size_t MultiPlan::getDir(bex::VertexDescriptor const & lcen_index, bex::VertexDescriptor const & ln_index, size_t const & robID)
	{
		auto &local2T = _vlocal2T[robID];
		bex::VertexDescriptor cen_index = local2T[lcen_index];
		bex::VertexDescriptor n_index = local2T[ln_index];

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

	//bool MultiPlan::getMinLeaf(set<size_t> const & robSet, bex::VertexDescriptor &vd) const 
	//{
	//	double minDis = 99999;
	//	int resVd = -1;
	//	auto &graph = _ob_sGraph;
	//	for (auto it = robSet.begin(); it != robSet.end(); it++)
	//	{
	//		auto &cenInd = *it;
	//		auto neighborsIter = bt::adjacent_vertices(*it, _ob_sGraph);
	//		for (auto ni = neighborsIter.first; ni != neighborsIter.second; ++ni)
	//		{
	//			if (robSet.count(*ni) == 0)
	//			{
	//				//robSet.count
	//				double dis = bg::distance(graph[cenInd].pnt, graph[*ni].pnt);
	//				if (dis < minDis)
	//				{
	//					minDis = dis;
	//					resVd = *ni;
	//				}
	//			}
	//		}
	//	}
	//	if (resVd < 0)
	//	{
	//		return false;
	//	}
	//	vd = resVd;
	//	return true;
	//}

	bool MultiPlan::getMinLeaf(size_t const & robID, bex::VertexDescriptor & min_MegaBoxVd)
	{
		double UnCoverMinFit = 99999;
		double coverMinFit = 999999;
		int UnCoverResVd = -1;
		int coverResVd = -1;
		bool allCovered = true;
		auto &graph = _ob_sGraph;
		auto &robSet = (*_vRobSetPtr)[robID];
		auto &robNeiSet = (*_vRobNeiSetPtr)[robID];
		auto &robNghSet = (*_vRobNghSetPtr)[robID];

		if (robNghSet.empty())
		{
			(*_vRobSleepPtr)[robID] = true;
			return false;
		}
		vector<pair<size_t,size_t>> robOpNeiSet;
		vector<size_t> robOpCandi;
		for (auto it = robNghSet.begin(); it != robNghSet.end(); it++)
		{
			bool inOtherSet = false;
			for (size_t i = 0; i < _robNum; i++)
			{
				if (i == robID) continue;
				if (_vRobSetPtr->at(i).count(*it) == 1)
				{
					inOtherSet = true;
					if (_notBidSetPtr->count(*it) == 1)
						break;
					robOpCandi.push_back(_vRobSetPtr->at(i).size());
//					cout << "index = " << i << endl;
					robOpNeiSet.push_back(pair<size_t,size_t>(*it,_vRobSetPtr->at(i).size()));
					break;
				}
			}
			if (!inOtherSet) {
				 double fit = -this->calFitNess(robID, *it);
				// need optimal
				//double dis = bg::distance(graph[it->first].pnt, graph[it->second].pnt);
				if (fit < UnCoverMinFit)
				{
					UnCoverMinFit = fit;
					UnCoverResVd = *it;
					allCovered = false;
				}
			}
		}
		if (UnCoverResVd != -1) { min_MegaBoxVd = UnCoverResVd; return allCovered;}

		
		//end the rule 1
		auto cmpOp = [](pair<size_t,size_t> ind0,pair<size_t,size_t> ind1)
		{
			if(ind0.second < ind1.second)
			{
				return true;
			}
			return false;
		};

		std::sort(robOpNeiSet.begin(), robOpNeiSet.end(), cmpOp);
		//for (size_t i = 0; i < robOpNeiSet.size(); i++)
		//{
		//	cout<<i << "	robOpNeiSet[i].first = " << robOpNeiSet[i].first << "	robOpNeiSet[i].second =  " << robOpNeiSet[i].second << endl;
		//}

		auto maxCandi = robOpNeiSet.back().second;
		if (robSet.size() >= (maxCandi - 1))
		{
			(*_vRobSleepPtr)[robID] = true;
			return allCovered;
		}
		else
		{
			(*_vRobSleepPtr)[robID] = false;
		}

		auto size = robOpNeiSet.size();

		for (size_t i = 0; i < robOpNeiSet.size(); i++)
		{
			if (calPriority(robID, robOpNeiSet.at(size - 1 - i).first) != std::numeric_limits<double>::max())
			{
				min_MegaBoxVd = robOpNeiSet.at(size - 1 - i).first;
				return allCovered;
			}
		}
		_vRobSleepPtr->at(robID) = true;
		return allCovered;
//		cout << "robOpSet  = " << robOpNeiSet.size() << endl;
//		auto maxCandi = *std::max_element(robOpCandi.begin(), robOpCandi.end());
//
//		vector<size_t> vCandVd;
//
//		size_t index = 0;
//		for (auto it = robNeiSet.begin(); it != robNeiSet.end(); it++)
//		{
//			if (_notBidSetPtr->count(it->first) == 1) continue;
//			if (robOpCandi[index++] == maxCandi)
//			{
//				vCandVd.push_back(it->first);
//			}
//		}
//
//		if (vCandVd.empty())
//		{
//			cout<<"bug empty"<<endl;
//		}
//
//
////		auto maxEle = std::max_element(robOpNeiSet.begin(), robOpNeiSet.begin() + 2, cmpOp);
//		//std::stable_sort(robOpNeiSet.begin(), robOpNeiSet.end(), cmpOp);
//
//		//size_t maxCardi = robOpNeiSet.back().second;
//		//vCandVd.push_back(robOpNeiSet.back().first);
//		//for (size_t i = 1; i < robOpNeiSet.size(); i++)
//		//{
//		//	//vCandVd.back()
//		//	if (robOpNeiSet[robOpNeiSet.size() - i - 1].second == maxCardi)
//		//	{
//		//		vCandVd.push_back(robOpNeiSet[robOpNeiSet.size() - i - 1].second);
//		//	}
//		//	else
//		//	{
//		//		break;
//		//	}
//		//}
//		if (vCandVd.size()==1)
//		{
//			min_MegaBoxVd = vCandVd.front();
//			//rule 2 end
//		}
//		else
//		{
//			double minPri = std::numeric_limits<double>::max();
//			double minPriInd = vCandVd.front();
//			for (size_t i = 0; i < vCandVd.size(); i++)
//			{
//				double pri = calPriority(robID, vCandVd[i]);
//				if (pri < minPri)
//				{
//					minPriInd = vCandVd[i];
//					minPri = pri;
//				}
//			}
//			min_MegaBoxVd = minPriInd;
//			if (minPri == std::numeric_limits<double>::max())
//			{
//				_vRobSleepPtr->at(robID) = true;
//
//				c_deg << "rule 3 bug" << endl;
//			}
//			//rule 3 end
//		}
//		return allCovered;
	}

	bool MultiPlan::getAuctionInd(bex::VertexDescriptor & min_MegaBoxVd)
	{
		
		double UnCoverMinFit = 99999;
		double coverMinFit = 999999;
		int UnCoverResVd = -1;
		int coverResVd = -1;
		bool allCovered = true;
		auto &graph = _ob_sGraph;
		for (size_t i = 0; i < _robNum; i++)
		{
			auto &robID = i;
			auto &robSet = (*_vRobSetPtr)[i];
			auto &robNeiSet = (*_vRobNeiSetPtr)[i];
			for (auto it = robNeiSet.begin(); it != robNeiSet.end(); it++)
			{
				bool inOtherSet = false;
				for (size_t p = 0; p < _robNum; p++)
				{
					if (p == robID) continue;
					if (_vRobSetPtr->at(p).count(it->first) == 1)
					{
						inOtherSet = true;
						break;
					}
				}
				if (!inOtherSet) {
					double fit = -this->calFitNess(robID, it->first);
					// need optimal
					//double dis = bg::distance(graph[it->first].pnt, graph[it->second].pnt);
					if (fit < UnCoverMinFit)
					{
						UnCoverMinFit = fit;
						UnCoverResVd = it->first;
						allCovered = false;
					}
				}
				else
				{
					if (_notBidSetPtr->count(it->first) == 1) continue;
					double dis = bg::distance(graph[it->first].pnt, graph[it->second].pnt);
					if (dis < coverMinFit)
					{
						coverMinFit = dis;
						coverResVd = it->first;
					}
				}
			}
		}
		if (UnCoverResVd != -1) min_MegaBoxVd = UnCoverResVd;
		else min_MegaBoxVd = coverResVd;
		return allCovered;
	}

	double MultiPlan::calFitNess(size_t const & robID, bex::VertexDescriptor const &megaBoxVd)
	{
		double fitNess = 0;
		auto &graph = _ob_sGraph;
		for (size_t i = 0; i < _robNum; i++)
		{
			auto &robSet = (*_vRobSetPtr)[i];
			if (i == robID) continue;
			for (auto it = robSet.begin(); it != robSet.end(); it++)
			{
				double dis = bg::distance(graph[*it].pnt, graph[megaBoxVd].pnt);
				fitNess += dis;
			}
		}
		return fitNess;
	}

	double MultiPlan::calPriority(size_t const & robID, bex::VertexDescriptor const & megaBoxVd)
	{
		size_t neiID;
		for (size_t i = 0; i < _robNum; i++)
		{
			if (_vRobSetPtr->at(i).count(megaBoxVd) == 1)
			{
				neiID = i;
				break;
			}

		}
		auto &robNeiSet = (*_vRobSetPtr)[neiID];
		vector<bex::VertexDescriptor> v_vd;

		for (auto it = robNeiSet.begin(); it != robNeiSet.end(); it++)
		{
			if (megaBoxVd == *it) continue;
			v_vd.push_back(*it);
		}
		//all connect can bid
		if (_mainMap.allConnected(v_vd)) {
			auto &robSet = (*_vRobSetPtr)[robID];
			auto &graph = _ob_sGraph;
			double fitNess = 0;
			return fitNess;
			//for (auto it = robSet.begin(); it != robSet.end(); it++)
			//{
			//	double dis = bg::distance(graph[it->first].pnt, graph[megaBoxVd].pnt);
			//	fitNess += dis;
			//}
			return fitNess;
		}
		return std::numeric_limits<double>::max();
	}

	size_t MultiPlan::minBidRob(size_t const & robAucNeer, bex::VertexDescriptor const & megaBoxID)
	{
		size_t minBid = 99999;
		size_t successBidID;
		for (size_t i = 0; i < _robNum; i++)
		{
//			if (robAucNeer == i)
			if(false)
			{
				size_t bid = _vRobSetPtr->at(i).size();
				if (bid < minBid) {
					successBidID = i;
					minBid = bid;
				}
			}
			else
			{
				if (_vRobNghSetPtr->at(i).count(megaBoxID) == 1)
				{
					size_t bid = _vRobSetPtr->at(i).size();
					if (bid < minBid) {
						successBidID = i;
						minBid = bid;
					}
				}
				if (_vRobSetPtr->at(i).count(megaBoxID) == 1)
				{
					auto &robSet = (*_vRobSetPtr)[i];
					vector<bex::VertexDescriptor> v_vd;
					for (auto it = robSet.begin(); it != robSet.end(); it++)
					{
						if (megaBoxID == *it) 
							continue;
						v_vd.push_back(*it);
					}
					//all connect can bid
					if (_mainMap.allConnected(v_vd)) {
						size_t bid = _vRobSetPtr->at(i).size() - 1;
						if (bid < minBid) {
							successBidID = i;
							minBid = bid;
						}
					}
					else
					{
						c_deg << "connected " << i;
						for (auto &itt:v_vd)
						{
							c_deg << itt;
						}
						c_deg << endl;
						minBid = 0;
						successBidID = i;
						//_notBidSetPtr->insert(megaBoxID);
					}
				}
			}
		}
		return successBidID;
	}

	void MultiPlan::updateSet(size_t const & succBidID, bex::VertexDescriptor const & megaBoxVd)
	{

			auto &robSet = _vRobSetPtr->at(succBidID);
			auto &robNghSet = _vRobNghSetPtr->at(succBidID);
			//robSet.insert(TreeEdge(megaBoxVd, robNeiSet[megaBoxVd]));
			//auto &robNeiSet = (*_vRobNeiSetPtr)[succBidID];
			robSet.insert(megaBoxVd);
			robNghSet.clear();
			//robNeiSet.clear();

			for (auto it = robSet.begin(); it != robSet.end(); it++)
			{
				auto &cenInd = *it;
				auto neighborsIter = bt::adjacent_vertices(cenInd, _ob_sGraph);
				for (auto ni = neighborsIter.first; ni != neighborsIter.second; ++ni)
				{
					if (robSet.count(*ni) == 0)
					{
						robNghSet.insert(*ni);
					}
				}
			}
	}

	void MultiPlan::updateSetWithErase(size_t const & succBidID, bex::VertexDescriptor const & megaBoxVd)
	{

		auto &robSet = _vRobSetPtr->at(succBidID);
		auto &robNghSet = _vRobNghSetPtr->at(succBidID);
		//robSet.insert(TreeEdge(megaBoxVd, robNeiSet[megaBoxVd]));
		//auto &robNeiSet = (*_vRobNeiSetPtr)[succBidID];
		if (robSet.count(megaBoxVd) == 1)
			return;


		size_t eraseID;
		for (size_t i = 0; i < _robNum; i++)
		{
			if (_vRobSetPtr->at(i).count(megaBoxVd) == 1)
			{
				eraseID = i;
				// do not need update
				if (succBidID == eraseID)
					return;
				_vRobSetPtr->at(i).erase(megaBoxVd);
				auto &eraseRobSet = _vRobSetPtr->at(i);

				auto &eraseRobNghSet = _vRobNghSetPtr->at(i);
				eraseRobNghSet.clear();

				for (auto it = eraseRobSet.begin(); it != eraseRobSet.end(); it++)
				{
					auto &cenInd = *it;
					auto neighborsIter = bt::adjacent_vertices(cenInd, _ob_sGraph);
					for (auto ni = neighborsIter.first; ni != neighborsIter.second; ++ni)
					{
						if (eraseRobSet.count(*ni) == 0)
						{
							eraseRobNghSet.insert(*ni);
						}
					}
				}
				//return;
				break;
			}
		}

		robSet.insert(megaBoxVd);
		robNghSet.clear();
		//robNeiSet.clear();

		for (auto it = robSet.begin(); it != robSet.end(); it++)
		{
			auto &cenInd = *it;
			auto neighborsIter = bt::adjacent_vertices(cenInd, _ob_sGraph);
			for (auto ni = neighborsIter.first; ni != neighborsIter.second; ++ni)
			{
				if (robSet.count(*ni) == 0)
				{
					robNghSet.insert(*ni);
				}
			}
		}


	}

	double MultiPlan::calSolutionFitNess()
	{
		double vertNum = _mainMap.vertNum() - _mainMap.obNum();
		double averVertNum = vertNum / _robNum;
		double fitNess = 0;
		
		for (size_t i = 0; i < _robNum; i++)
		{
			auto  val = (_vRobSetPtr->at(i).size() - averVertNum)*(_vRobSetPtr->at(i).size() - averVertNum);
			fitNess += val;
		}
		return fitNess;
	}

	//size_t MultiPlan::minBidRob(size_t const & megaBoxID, size_t const & robAucNeer)
	//{
	//	size_t minBid = 99999;
	//	size_t successBidID;
	//	for (size_t i = 0; i < _robNum; i++)
	//	{
	//		if (robAucNeer == i)
	//		{
	//			size_t bid = _vRobSetPtr->at(i).size();
	//			if (bid < minBid) {
	//				successBidID = i;
	//				minBid = bid;
	//			}
	//		}
	//		else
	//		{
	//			if (_vRobNeiSetPtr->at(i).count(megaBoxID) == 1)
	//			{
	//				size_t bid = _vRobSetPtr->at(i).size();
	//				if (bid < minBid) {
	//					successBidID = i;
	//					minBid = bid;
	//				}
	//			}
	//			if (_vRobSetPtr->at(i).count(megaBoxID) == 1)
	//			{
	//				size_t bid = _vRobSetPtr->at(i).size() - 1;
	//				if (bid < minBid) {
	//					successBidID = i;
	//					minBid = bid;
	//				}
	//			}
	//		}
	//	}
	//	return successBidID;
	//}

	//void MultiPlan::updateSet(size_t const & succBidID, bex::VertexDescriptor const & MegaBoxVd)
	//{
	//	auto &robSet = _vRobSetPtr->at(succBidID);
	//	robSet.insert(MegaBoxVd);
	//	auto &robNeiSet = (*_vRobNeiSetPtr)[succBidID];
	//	robNeiSet.clear();

	//	for (auto it = robSet.begin(); it != robSet.end(); it++)
	//	{
	//		auto &cenInd = *it;
	//		auto neighborsIter = bt::adjacent_vertices(*it, _ob_sGraph);
	//		for (auto ni = neighborsIter.first; ni != neighborsIter.second; ++ni)
	//		{
	//			if (robSet.count(*ni) == 0)
	//			{
	//				robNeiSet.insert(*ni);
	//			}
	//		}
	//	}

	//}

}