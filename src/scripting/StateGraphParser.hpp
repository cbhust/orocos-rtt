/***************************************************************************
  tag: Peter Soetens  Thu Apr 22 20:40:59 CEST 2004  StateGraphParser.hpp

                        StateGraphParser.hpp -  description
                           -------------------
    begin                : Thu April 22 2004
    copyright            : (C) 2004 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be

 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public                   *
 *   License as published by the Free Software Foundation;                 *
 *   version 2 of the License.                                             *
 *                                                                         *
 *   As a special exception, you may use this file as part of a free       *
 *   software library without restriction.  Specifically, if other files   *
 *   instantiate templates or use macros or inline functions from this     *
 *   file, or you compile this file and link it with other files to        *
 *   produce an executable, this file does not by itself cause the         *
 *   resulting executable to be covered by the GNU General Public          *
 *   License.  This exception does not however invalidate any other        *
 *   reasons why the executable file might be covered by the GNU General   *
 *   Public License.                                                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public             *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/

#ifndef EXECUTION_STATE_PARSER_HPP
#define EXECUTION_STATE_PARSER_HPP

#include "parser-types.hpp"

#include <map>
#include <string>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "../DataSource.hpp"

namespace RTT
{
    class StateDescription;
    class TaskContext;
    class FunctionGraph;
    class ParsedStateMachine;
    class ProgramInterface;
    class StateMachineTask;
}
namespace RTT { namespace detail
{
    class StateMachineBuilder;
    class ProgramGraphParser;
    class ConditionParser;
    class ValueChangeParser;
    class ExpressionParser;
    class CommonParser;
    class PeerParser;
    class ArgumentsParser;

  /**
   * This is not a parser in the Boost.spirit sense of the word, it's
   * just a class used to hold the parser and semantic actions..
   *
   * This class does the actual work.  It generates a Finite State Graph, by
   * constructing the State nodes.  We just go over the code one
   * single time, and construct the FSM as we go.  For every
   * statement, we construct a new GraphNode, and fill it up as we
   * get the information we need.
   */
  class StateGraphParser
  {
      // Our task we are loaded in :
      TaskContext* context;
      // The TC of the current StateMachine
      StateMachineTask* curcontext ;
      TaskContext* peer;
      our_pos_iter_t& mpositer;
      our_pos_iter_t saveStartPos;
      // offset relative to StateMachine text.
      int ln_offset;

      typedef boost::shared_ptr<ParsedStateMachine> ParsedStateMachinePtr;
      typedef std::map<std::string, ParsedStateMachinePtr> contextnamemap_t;
      typedef std::map<std::string, AttributeBase*> contextparams_t;
      typedef std::map<std::string, DataSourceBase::shared_ptr> contextparamvalues_t;
      typedef std::map<std::string, StateDescription*> contextstatesmap_t;
      typedef std::map<std::string, StateMachineBuilder*> contextbuilders_t;

      contextnamemap_t rootcontexts;
      contextbuilders_t contextbuilders;
      ParsedStateMachinePtr curtemplate;
      std::vector<CommandInterface*> paraminitcommands;
      std::vector<CommandInterface*> varinitcommands;
      ParsedStateMachinePtr curinstantiatedcontext;
      StateMachineBuilder* curcontextbuilder;
      std::string curinstcontextname;
      contextparamvalues_t curinstcontextparams;
      std::string curcontextinstargumentname;
      std::string curcontextname;
      bool curinitialstateflag;
      bool curfinalstateflag;
      StateDescription* curstate;
      StateDescription* curnonprecstate;
      ProgramGraphParser* progParser;
      boost::shared_ptr<ProgramInterface> transProgram;
      StateDescription* elsestate;
      boost::shared_ptr<ProgramInterface> elseProgram;
      ConditionInterface* curcondition;
#if 0
      std::string curscvccontextname;
      std::string curscvcparamname;
#endif
      /**
       * used to sort conditions as they are generated and
       * inserted in the StateMachine.
       */
      int rank;
      bool isroot; //! are we instantiating a rootmachine ?
      unsigned int selectln; //! store line number of select's 'if' statement.

      std::vector<DataSourceBase::shared_ptr> evargs;
      std::string evname;

      rule_t production;
      rule_t newline;
      rule_t rootcontextinstantiation;
      rule_t statecontext;
      rule_t contextinstantiation;
      rule_t statecontextcontent;
      rule_t varline;
      rule_t state;
      rule_t vardec;
      rule_t subMachinedecl;
      rule_t statecontent;
      rule_t statecontentline;
      rule_t entry;
      rule_t preconditions;
      rule_t precondition;
      rule_t run;
      rule_t transitions;
      rule_t transition;
      rule_t transprog;
      rule_t elseprog;
      rule_t argslist;
      rule_t handle;
      rule_t exit;
      rule_t programBody;
      rule_t transline;
      rule_t selectcommand;
      rule_t brancher;
      rule_t selector;
      rule_t contextinstarguments;
      rule_t contextinstargument;
      rule_t contextmemvar;
      rule_t contextvariable;
      rule_t contextparam;
      rule_t contextconstant;
      rule_t contextalias;
      rule_t subMachinevarchange;

      ConditionParser* conditionparser;
      CommonParser* commonparser;
      ValueChangeParser* valuechangeparser;
      ExpressionParser* expressionparser;
      ArgumentsParser* argsparser;
      PeerParser* peerparser;

      void clear();

      void seeninitialstate();
      void seenfinalstate();
      void statedef( iter_t s, iter_t f);
      void seenstateend();

      void inprogram( const std::string& name );
      void seenentry();
      void seenexit();
      void seenhandle();
      void seenrun();
      boost::shared_ptr<ProgramInterface> finishProgram();

      void seencondition();
      void seenendcondition();
      void seentransprog();
      void seenelseprog();
      void seenelse();
      void seenselect( iter_t s, iter_t f);
      void seenprecondition();
      void seeneventname(iter_t s, iter_t f);
      void seeneventtrans();
      void seeneventargs();

      void seenstatecontextname( iter_t begin, iter_t end );
      void storeOffset();
      void saveText( iter_t begin, iter_t end );
      void startrootcontextinstantiation();
      void seenrootcontextinstantiation();
      void seenstatecontextend();
      void seensubMachineinstantiation();

      void inpreconditions();
      void seenpreconditions();

      void seencontexttypename( iter_t begin, iter_t end );
      void seeninstcontextname( iter_t begin, iter_t end );
      void seencontextinstantiation();
      void seencontextinstargumentname( iter_t begin,  iter_t end );
      void seencontextinstargumentvalue();

      void seencontextvariable();
      void seencontextparam();

#if 0
      void seenscvcsubMachinename( iter_t begin, iter_t end );
      void seenscvcparamname( iter_t begin, iter_t end );
#endif
  public:
    StateGraphParser( iter_t& positer, TaskContext* tc );
    ~StateGraphParser();

    // tries to parse, returns the instantiated root contexts.  On all
    // returned contexts setName() will have been called with the
    // correct name.
    // will throw an file_parse_exception on error
    std::vector<ParsedStateMachinePtr> parse( iter_t& begin, iter_t end );
  };
}}

#endif
