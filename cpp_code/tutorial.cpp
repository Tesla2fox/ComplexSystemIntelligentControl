#include "stadfx.h"
#include "ReadConfig.h"
#include "Obmap.h"
#include "STCplan.h"
#include "MultiAuction.h"
#include "MultiAuctionSTC.h"
#include "MultiAuctionSTCEst.h"
#include "Gwrite.hpp"
int main(int argc, char * argv[])
{
	std::ofstream e_deg;
	string  str_deg = conDir;
	str_deg += "_40_40_301_Outdoor_";
	str_deg += "EstMakespanData.txt";
	e_deg.open(str_deg, std::ios::trunc);
	vector<size_t> vEstMakespan;
	vector<size_t> vMakespan;
	for (size_t i = 2; i < 11; i++)
	{
		string str_conDir = conDir;
		string str_txt = std::to_string(i);
		str_txt += "_40_40_301_Outdoor_Cfg.txt";
		//string str_txt = "7_40_40_301_Outdoor_Cfg.txt";
		str_conDir += str_txt;
		char * conFileName = new char[str_conDir.size() + 1];
		memcpy(conFileName, str_conDir.c_str(), str_conDir.size() + 1);

		if (argc > 1) {
			cout << argv[1] << endl;
			conFileName = argv[1];
		}
		cfg::ReadConfig readCfg(conFileName);
		readCfg.read();




		pl::Obmap obmap(readCfg, str_txt);
		obmap.map2tGrid();
		obmap.map2sGrid();
		obmap.writeEdges(pl::graphType::span);
		obmap.writeEdgesInPnt(pl::graphType::span);
		//auto wtf = obmap;
#ifdef _DEBUG
		cout << "obMapName = " << obmap._m_name << endl;
#endif // _DEBUG
		//print("obmapString")
		pl::STCPlan stcPlan(obmap);
		vector<pl::GridIndex> startPnt;
		for (size_t i = 0; i < readCfg._vStartMatPntPtr->size(); i++)
		{
			startPnt.push_back(pl::GridIndex(readCfg._vStartMatPntPtr->at(i).row, readCfg._vStartMatPntPtr->at(i).col));
		}
		//pl::MultiAuctionSTC multi_auc(obmap, startPnt);
		//multi_auc.setRandomSeed(1);
		//multi_auc.process();
		//multi_auc.writeRobGraph();
//		e_deg << "makeSpan ";
		vEstMakespan.clear();
		vMakespan.clear();
		for (size_t i = 0; i < 20; i++)
		{
			pl::MultiAuctionSTCEst multi_aucSTC(obmap, startPnt);
			multi_aucSTC.setRandomSeed(i);
			multi_aucSTC.process();
			multi_aucSTC.writeRobGraph();
//			e_deg << multi_aucSTC._makeSpan << "  ";
			vEstMakespan.push_back(multi_aucSTC._estMakeSpan);
			vMakespan.push_back(multi_aucSTC._makeSpan);
		}
		string est_ms = "EstMakespan" + std::to_string(i);
		string ms = "makespan" + std::to_string(i);
		writeDebug(e_deg, est_ms, vEstMakespan);
		writeDebug(e_deg, ms, vMakespan);
	}
	e_deg.close();

	cout << "ggq' code" << endl;
	if (argc <= 1) {
		size_t input_val;
		std::cin >> input_val;
	}
}


