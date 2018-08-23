#include "stadfx.h"
#include "ReadConfig.h"
#include "Obmap.h"
#include "STCplan.h"
#include "MultiAuction.h"
#include "MultiAuctionSTC.h"
#include "MultiAuctionSTCEst.h"
int main(int argc, char * argv[])
{
	
	string str_conDir = conDir;
	string str_txt = "cfg2.txt";
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
	str_deg += "cfg2";
	str_deg += "data.txt";
	e_deg.open(str_deg, std::ios::trunc);

	pl::Obmap obmap(readCfg, str_txt);
	obmap.map2tGrid();
	obmap.map2sGrid();
	obmap.writeEdges(pl::graphType::span);
	obmap.writeEdgesInPnt(pl::graphType::span);
	//auto wtf = obmap;
	cout << "obMapName = " << obmap._m_name << endl;
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
	e_deg << "makeSpan ";
	for (size_t i = 0; i < 20; i++)
	{
		pl::MultiAuctionSTCEst multi_aucSTC(obmap, startPnt);
		multi_aucSTC.setRandomSeed(i);
		multi_aucSTC.process();
		multi_aucSTC.writeRobGraph();
		e_deg << multi_aucSTC._makeSpan<<"  ";
	}

	e_deg.close();
	cout << "ggq' code" << endl;
	if (argc <= 1) {
		size_t input_val;
		std::cin >> input_val;
	}
}


