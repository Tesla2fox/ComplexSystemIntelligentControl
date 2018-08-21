#include "stadfx.h"
#include "ReadConfig.h"
#include "Obmap.h"
#include "STCplan.h"
#include "MultiAuction.h"
#include "MultiAuctionSTCEst.h"
int main(int argc, char * argv[])
{
	
	string str_conDir = conDir;
	string str_txt = "8_40_40_320_Outdoor_Cfg.txt";
	str_conDir += str_txt;
	char * conFileName = new char[str_conDir.size() + 1];
	memcpy(conFileName, str_conDir.c_str(), str_conDir.size() + 1);

	if (argc > 1) {
		cout << argv[1] << endl;
		conFileName = argv[1];	
	}
	cfg::ReadConfig readCfg(conFileName);
	readCfg.read();
	std::ofstream e_deg;

	string  str_deg = conDir;
	str_deg += "data.txt";
	e_deg.open(str_deg, std::ios::trunc);

	pl::Obmap obmap(readCfg, str_txt);
	obmap.map2tGrid();
	obmap.map2sGrid();
	obmap.writeEdges(pl::graphType::span);
	obmap.writeEdgesInPnt(pl::graphType::span);

	cout << "obMapName = " << obmap._m_name << endl;
	//print("obmapString")
	pl::STCPlan stcPlan(obmap);
	vector<pl::GridIndex> startPnt;
	for (size_t i = 0; i < readCfg._vStartMatPntPtr->size(); i++)
	{
		startPnt.push_back(pl::GridIndex(readCfg._vStartMatPntPtr->at(i).row, readCfg._vStartMatPntPtr->at(i).col));
	}
	//pl::MultiAuction multi_auc(obmap, startPnt);	
	//multi_auc.setRandomSeed(1);
	//multi_auc.process();
	//multi_auc.writeRobGraph();
	
	pl::MultiAuctionSTCEst multi_aucSTC(obmap,startPnt);
	multi_aucSTC.setRandomSeed(1);
	multi_aucSTC.process();
	multi_aucSTC.writeRobGraph();

	cout << "ggq' code" << endl;
	if (argc <= 1) {
		size_t input_val;
		std::cin >> input_val;
	}
}


