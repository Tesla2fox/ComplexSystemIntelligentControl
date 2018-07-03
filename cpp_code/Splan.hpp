#pragma once
#include "Obmap.h"
#include "bgeometry.h"
#include "stadfx.h"
#include "simple_svg.hpp"

namespace pl
{
	enum DirType{left,bottom,right,top,center};
	class Splan
	{
	public:
		Splan(Obmap  &ob_map):
		_mainMap(ob_map), doc("plan.svg", svg::Layout(svg::Dimensions(ob_map.getWsPoint3().x(), ob_map.getWsPoint3().y())
			,svg::Layout::BottomLeft)), _ob_tGraph(ob_map.getGraph(graphType::base)), _ob_sGraph(ob_map.getGraph(graphType::span))
			, _ob_tGrid(ob_map.getGridMap(graphType::base)), _ob_sGrid(ob_map.getGridMap(graphType::span)),
			_ob_sgraph2map(ob_map.sgraph2map),_ob_smap2graph(ob_map.smap2graph),_ob_tgraph2map(ob_map.tgraph2map),
			_ob_tmap2graph(ob_map.tmap2graph)
		{

		}

		void setStartPnt(bex::DPoint const& pnt) { _startPnt = pnt; }

		void pathPlanning();
		void drawGraph(size_t const &type,bool const &b_edge);
		void drawTree();
		void drawPath();
		void savePic() { doc.save(); }

		
	private:

		//tgrid and the tgraph means the based grid
		//sgrid and sgraph means the spanning grid
		//from the obmap
		bex::Graph &_ob_tGraph;
		GridMap &_ob_tGrid;

		bex::Graph &_ob_sGraph;
		GridMap &_ob_sGrid;

		
		//
		std::map<std::pair<int, int>, int> &_ob_tmap2graph;
		std::map<int, std::pair<int, int>> &_ob_tgraph2map;

		std::map<std::pair<int, int>, int> &_ob_smap2graph;
		std::map<int, std::pair<int, int>> &_ob_sgraph2map;


		//menber graph
		bex::Graph  _m_tGraph;

		//

		void spanningTreePrim();
		void getNewGraph();
		void searchPath();

		//
		bex::DPoint _startPnt;
		Obmap  &_mainMap;
		svg::Document doc;

		//
		vector<size_t> _vTree;
		vector<bex::DSegment> _vTreeSg;
		bool treeIntersection(bex::DSegment const sg) const;

		//get four neighbour grid indexs;
		vector<GridIndex> getVerticalNeighbour(GridIndex const &cen_index);

		//get four neighbour grid indexs with directions
		vector<pair<GridIndex, size_t>> getVerticalNeighbourWithDir(GridIndex const &cen_index);

		size_t getDir(bex::VertexDescriptor const & cen_index, bex::VertexDescriptor const & n_index);

		bool inSameMegaBox(bex::VertexDescriptor const &vd0, bex::VertexDescriptor const &vd1);
		//
		bex::DLineString _path;
		vector<bex::VertexDescriptor> _pathIndex;
	};

}