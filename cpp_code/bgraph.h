#ifndef BGRAPH_H
#define BGRAPH_H



//graph  head files
#include "boost/graph/graph_traits.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include "boost/graph/kruskal_min_spanning_tree.hpp"
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>

#include "bgeometry.h"


#include <vector>
#include <map>

namespace bg = boost::geometry;
namespace bt = boost;

namespace bex {

	///*图库*/
    class VertexProperty { // 图节点中保存的信息
    public:
        unsigned int index;
        //size_t        color;
        bool EdgeState;
        bool QueueState;
        int Type;
        bool NeighbourState;
        bex::DPoint  pnt;
        std::pair<size_t, size_t> PntIndex;
    };

    class EdgeProperty {  // 图边中保存的信息
    public:
        unsigned int index;
        float         weight;
        size_t dir;
    };

    // 图的定义。后面两个参数分别是节点属性和边属性
    ///
    /// bidirectionalS	表示一个既有出边也有入边的有向图
    /// directedS	表示一个只能存取出边的有向图
    /// undirectedS	表示一个无向图
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProperty, EdgeProperty> Graph;

    // 节点描述符
    typedef boost::graph_traits<Graph>::vertex_descriptor   VertexDescriptor;
    // 边描述符
    typedef boost::graph_traits<Graph>::edge_descriptor     EdgeDescriptor;

    // 下面是邻接链表的一些遍历器
    typedef boost::graph_traits<Graph>::vertex_iterator     VertexIterator;
    typedef boost::graph_traits<Graph>::edge_iterator      EdgeIterator;
    typedef boost::graph_traits<Graph>::adjacency_iterator  AdjacencyIterator;
    typedef boost::graph_traits<Graph>::out_edge_iterator   OutEdgeIterator;

    //几何到图的转换

}
#endif // BGRAPH_H
