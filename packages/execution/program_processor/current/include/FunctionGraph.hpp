/***************************************************************************
  tag: Peter Soetens  Mon Jan 19 14:11:21 CET 2004  FunctionGraph.hpp 

                        FunctionGraph.hpp -  description
                           -------------------
    begin                : Mon January 19 2004
    copyright            : (C) 2004 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be
 
 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/ 
 
#ifndef FUNCTION_GRAPH_HPP
#define FUNCTION_GRAPH_HPP

#include "VertexNode.hpp"
#include "EdgeCondition.hpp"
#include <corelib/CommandNOP.hpp>

namespace ORO_Execution
{
    using boost::tie;
    using boost::graph_traits;
    using boost::property_map;
    using std::find_if;
    using std::bind2nd;
    using boost::get;
    using boost::put;
    using ORO_CoreLib::CommandNOP;

    /**
     * This class represents a function. It has
     * much in common with a program but is only
     * used for storing a Graph.
     */
    class FunctionGraph
    {
        typedef EdgeCondition::EdgeProperty EdgeProperty;
        typedef VertexNode::VertProperty    VertProperty;

        typedef boost::adjacency_list<boost::vecS,
                                      boost::listS,
                                      boost::directedS,
                                      VertProperty,
                                      EdgeProperty> Graph;
        typedef graph_traits<Graph>::vertex_descriptor Vertex;
        typedef graph_traits<Graph>::edge_descriptor Edge;

        /**
         * The graph containing this function.
         */
        Graph function;

        Vertex start;
        Vertex exit;
    public:
        FunctionGraph( ) 
        {
            // the start vertex of our function graph
            start = add_vertex( function );
            put(vertex_exec, function, start, VertexNode::normal_node );
            exit = add_vertex( function );
            put(vertex_exec, function, exit, VertexNode::normal_node);
        }

        FunctionGraph( const FunctionGraph& orig )
            : function( orig.getGraph() )
        {
            graph_traits<Graph>::vertex_iterator v1,v2, it;
            tie(v1,v2) = vertices(function);
            for ( it=v1; it != v2; ++it)
                if ( get( vertex_exec, function, *it) == VertexNode::func_start_node )
                    break;
            start = *v1;
            for ( it=v1; it != v2; ++it)
                if ( get( vertex_exec, function, *it) == VertexNode::func_exit_node )
                    break;
            exit = *v1;
        }

        void finish()
        {
            put(vertex_exec, function, start, VertexNode::func_start_node );
            put(vertex_exec, function, exit, VertexNode::func_exit_node);

            // Because we use listS, we need to re-index the map :-(
            // If we do not do this, it can not be copied by the copy_graph
            // function.
            property_map<Graph, vertex_index_t>::type
                index = get(vertex_index, function);
            // initialize the vertex_index property values
            graph_traits<Graph>::vertex_iterator vi, vend;
            graph_traits<Graph>::vertices_size_type cnt = 0;
            for(tie(vi,vend) = vertices(function); vi != vend; ++vi)
                put(index, *vi, cnt++);
        }

        Vertex startNode() const
        {
            return start;
        }

        Vertex exitNode() const
        {
            return exit;
        }

        const Graph& getGraph() const
        {
            return function;
        }

        Graph& getGraph()
        {
            return function;
        }
    };


}

#endif
