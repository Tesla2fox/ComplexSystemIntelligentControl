#include "STCPlan.h"

namespace pl
{
	pl::STCPlan::~STCPlan()
	{

	}

	void pl::STCPlan::setRange(vector<size_t> const & vVertInd)
	{
		_vVertInd.clear();
		_vGridInd.clear();
		_vVertInd = vVertInd;
		for (auto &it : _vVertInd)			
			_vGridInd.push_back(this->_mainMap.tgraph2map[it]);
		
	}

	void STCPlan::setRange(vector<GridIndex> const & vInd) {
		_vVertInd.clear();
		_vGridInd.clear();
		_vGridInd = vInd;
		for (auto &it : _vGridInd)
			_vVertInd.push_back(this->_mainMap.tmap2graph[it]);

	}

	void pl::STCPlan::setStartPnt(size_t const & startRow, size_t const & startCol)
	{
		_startPntInd.first = startRow;
		_startPntInd.second = startCol;
	}

	void pl::STCPlan::plan()
	{
		constructGraph();
	}

	void pl::STCPlan::constructGraph()
	{
		if (!_mainMap.allConnected(_vVertInd))
			return;
		vector<size_t> rangeRow, rangeCol;
		for (auto &it : this->_vGridInd)
		{
			rangeRow.push_back(it.first);
			rangeCol.push_back(it.second);
		}
		writeDebug(c_deg, "rangRow", rangeRow);
		writeDebug(c_deg, "rangeCol", rangeCol);		
	}
}
