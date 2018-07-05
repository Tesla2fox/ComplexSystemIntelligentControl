#include "ReadConfig.h"
//#include "EncodeMat.hpp"

namespace cfg
{
	ReadConfig::~ReadConfig()
	{
	}
	void ReadConfig::read()
	{
		sscfg::ConfigFile co_list = sscfg::ConfigFile::load(_m_fileName);

		cout << "fileName is " << _m_fileName << endl;
		_row = 0;
		co_list.get("row", _row);
		co_list.get("col", _col);

		_vGridPtr = make_shared<vector<size_t>>();		
		co_list.get("grid", *_vGridPtr);
		
		vector<size_t> vRow, vCol;

		_vStartMatPntPtr = make_shared<vector<MatPnt>>();
		co_list.get("robRow", vRow);
		co_list.get("robCol", vCol);
		for (size_t i = 0; i < vRow.size(); i++)
		{
			_vStartMatPntPtr->push_back(MatPnt(vRow[i], vCol[i]));
		}

		vRow.clear();
		vCol.clear();

		_vObGridInd = make_shared<vector<MatPnt>>();
		co_list.get("obRow", vRow);
		co_list.get("obCol", vCol);
		for (size_t i = 0; i < vRow.size(); i++)
		{
			_vObGridInd->push_back(MatPnt(vRow[i], vCol[i]));
		}
		cout << "Grid Size = " << _vGridPtr->size() << endl;
		cout << "read successs" << endl;
	}
}
