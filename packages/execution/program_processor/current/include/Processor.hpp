/***************************************************************************
  tag: Peter Soetens  Thu Oct 10 16:19:04 CEST 2002  Processor.hpp

                        Processor.hpp -  description
                           -------------------
    begin                : Thu October 10 2002
    copyright            : (C) 2002 Peter Soetens
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

#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "corelib/RunnableInterface.hpp"
#include "corelib/CommandInterface.hpp"
#include <os/Mutex.hpp>

#include <list>
#include <string>
#include <vector>

#include <pkgconf/execution_program_processor.h>

namespace ORO_CoreLib
{
    template< class T>
    class AtomicQueue;
}

namespace ORO_Execution
{
    class StateContextTree;
    class ProgramInterface;
    class FunctionGraph;

    using ORO_CoreLib::CommandInterface;

    class program_load_exception
    {
        std::string merror;
    public:
        program_load_exception( const std::string& error )
            : merror( error )
        {
        }
        const std::string what() const
        {
            return merror;
        }
    };

    class program_unload_exception
    {
        std::string merror;
    public:
        program_unload_exception( const std::string& error )
            : merror( error )
        {
        }
        const std::string what() const
        {
            return merror;
        }
    };

    /**
     * @brief This class implements a controllable program processor.
     * It executes Realtime Programs and State Machines and can 
     * accept and execute external commands when running.
     *
     * @todo Make a more efficient implementation instead of iterating
     * over the programs and state lists. Also provide object based
     * control methods instead of string based.
     */
    class Processor
         : public ORO_CoreLib::RunnableInterface
    {
    public:
        /**
         * The status of a Program.
         */
        struct ProgramStatus {
            enum status { unloaded, stopped, running, stepmode, error };
        };
        /**
         * The status of a StateContext.
         */
        struct StateContextStatus {
            enum status { unloaded, inactive, active, running, stopped, paused, todelete, error };
        };

        /**
         * Constructs a new Processor
         * @param queue_size The size of the command queue.
         *
         */
        Processor(int queue_size = ORONUM_EXECUTION_PROC_QUEUE_SIZE);

        virtual ~Processor();

        bool pauseProgram(const std::string& name);

        bool loadStateContext( StateContextTree* sc );
        bool unloadStateContext( const std::string& name );
        bool activateStateContext(const std::string& name);
        bool deactivateStateContext(const std::string& name);
        bool startStateContext(const std::string& name);
        bool pauseStateContext(const std::string& name);
        bool stopStateContext(const std::string& name);
        bool stepStateContext(const std::string& name);
        bool resetStateContext(const std::string& name);
        bool deleteStateContext(const std::string& name);
        bool steppedStateContext(const std::string& name);
        bool continuousStateContext(const std::string& name);

        ProgramStatus::status getProgramStatus(const std::string& name) const;
        StateContextStatus::status getStateContextStatus(const std::string& name) const;

        bool loadProgram( ProgramInterface* pi ) ;
        bool startProgram(const std::string& name);
        bool stopProgram(const std::string& name);
        bool stepProgram(const std::string& name);
        bool deleteProgram(const std::string& name);

        bool isProgramRunning( const std::string& name) const;
        bool isStateContextRunning( const std::string& name) const;
        bool isStateContextStepped(const std::string& name) const;

        virtual bool initialize();
        virtual void step();
        virtual void finalize();

        /**
         * Process a given command.
         * @return The command number. You can check if the given command was processed
         * by calling Processor::isProcessed( command number ).
         */
        int process(CommandInterface* c);
        bool isProcessed( int i );

        bool runFunction(ProgramInterface* f);
        bool isFunctionFinished( ProgramInterface* f );

        std::vector<std::string> getProgramList();
        std::vector<std::string> getStateContextList();

        ProgramInterface* getProgram(const std::string& name) const;

        /**
         * Should the Processor accept or reject commands in \a process().
         * @param true_false true to accept, false to reject.
         */
        void acceptCommands( bool true_false) { accept=true_false; }

        class ProgramInfo;
        class StateInfo;
    private:
        void recursiveLoadStateContext( StateContextTree* sc );
        void recursiveCheckLoadStateContext( StateContextTree* sc );
        void recursiveUnloadStateContext( StateContextTree* sc );
        void recursiveCheckUnloadStateContext(const StateInfo& si );

        typedef std::list<ProgramInfo>::iterator program_iter;
        typedef std::list<StateInfo>::iterator state_iter;
        typedef std::list<ProgramInfo>::const_iterator cprogram_iter;
        typedef std::list<StateInfo>::const_iterator cstate_iter;
        std::list<ProgramInfo>* programs;
        std::list<StateInfo>*   states;
        std::vector<ProgramInterface*> funcs;
        std::vector<ProgramInterface*>::iterator f_it;

        bool accept;
        ORO_CoreLib::AtomicQueue<CommandInterface*>* a_queue;
        ORO_CoreLib::AtomicQueue<ProgramInterface*>* f_queue;

        /**
         * Counting how much commands we processed.
         */
        int coms_processed;

        /**
         * Guard state list
         */
        ORO_OS::MutexRecursive statemonitor;
        ORO_OS::Mutex progmonitor;
        ORO_OS::Mutex procmonitor;
    };

}

#endif
