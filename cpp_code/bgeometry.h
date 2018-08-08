#ifndef BGEOMETRY_H
#define BGEOMETRY_H


//几何head——files
#include "boost/geometry/geometry.hpp"
#include "boost/geometry/geometries/point_xy.hpp"
#include "boost/geometry/geometries/linestring.hpp"
#include "boost/geometry/geometries/linestring.hpp"
#include "boost/geometry/geometries/box.hpp"
#include "boost/geometry/geometries/ring.hpp"
#include "boost/geometry/geometries/polygon.hpp"


//std
#include <vector>
#include <map>
namespace bg = boost::geometry;
namespace bt = boost;

namespace bex {

	/*geometry lib */

	//点
	typedef bg::model::d2::point_xy<double> DPoint;
	//线段
	typedef bg::model::segment<DPoint> DSegment;
	//折线段
	typedef bg::model::linestring<DPoint> DLineString;
	//
	typedef bg::model::ring<DPoint, false> DRing;
	

	typedef bg::model::polygon<DPoint, false> DPolygon;

	

	std::vector<DPoint> segment2VDPoint(DSegment const &seg, double const step);

	DPoint DPointStep(DPoint const & pnt, double const step, DPoint const & pntB);

	//DPoint 


	DSegment v2Segment(std::vector<double> &vx, std::vector<double> &vy);

	DLineString v2LineString(std::vector<double> const &vx, std::vector<double> const &vy);

	DRing v2Ring(std::vector<double> const &vx, std::vector<double> const &vy);

	bool DPointEqual(DPoint const &a, DPoint const &b);




	////
	//// ggq define
	enum   vertType {ObVert,WayVert,EdgeObVert,ShoulderVert,NearVert,DoubleSameOb,SingleOb,DoubleDiffOb};

	class PointVert
	{
	public:
		DPoint pnt;
		int type;
		std::pair<size_t, size_t> PntIndex;
		
	private:

	};




    //几何到图的转换

    //VertexProperty PointVert2PointVert(const PointVert & pntVert);

}
#endif // BGEOMETRY_H
