#include "stadfx.h"
#include "ReadConfig.h"
#include "Obmap.h"
#include "STCplan.h"
int main(int argc, char * argv[])
{
	
	string str_conDir = conDir;
	str_conDir += "5_19_19_80_Outdoor_Cfg.txt";
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

	pl::Obmap obmap(readCfg);
	obmap.map2tGrid();
	obmap.map2sGrid();
	obmap.writeEdges(pl::graphType::span);
	obmap.writeEdgesInPnt(pl::graphType::span);

	pl::STCPlan stcPlan(obmap);

	cout << "ggq' code" << endl;
	if (argc <= 1) {
		size_t input_val;
		std::cin >> input_val;
	}
}


