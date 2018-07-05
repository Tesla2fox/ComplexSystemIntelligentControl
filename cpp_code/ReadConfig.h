#pragma once
#include "stadfx.h"
#include <tuple>
#include "ssconfig.hpp"
#include "bgeometry.h"

namespace cfg
{
	class MatPnt
	{
	public:
		size_t row;
		size_t col;
		MatPnt();
		MatPnt(size_t const &r, size_t const &c) :
			row(r), col(c)
		{}
	private:
	};

	class ReadConfig
	{
	public:
		ReadConfig(const char *filename) :
			_m_fileName(filename)
		{}
		~ReadConfig();
		void read();
		//cfgTuple getCfg();
		//vTaskAgentPtr _m_vTaskAgentPtr;
		//vTaskPntPtr _m_vTaskPntPtr;
		//DisMapPtr _taskDisMatPtr;
		//DisMapPtr _ag2taskDisMatPtr;
		//vector<size_t> _encode;
		//shared_ptr<vector<size_t>> 
		shared_ptr<vector<double>> _vRange_xPtr = nullptr;
		shared_ptr<vector<double>> _vRange_yPtr = nullptr;
		shared_ptr<bex::DRing> _RangePtr = nullptr;
		shared_ptr<vector<bex::DPoint>> _vStartPntPtr = nullptr;
		shared_ptr<vector<bex::DPoint>> _vObPntPtr = nullptr;
		shared_ptr<vector<MatPnt>> _vObGridInd = nullptr;
		double _gridStep;

		shared_ptr<vector<size_t>> _vGridPtr = nullptr;
		shared_ptr<vector<MatPnt>> _vStartMatPntPtr = nullptr;
		size_t _row;
		size_t _col;
	private:
		const char *_m_fileName;
	};
}