/***************************************************************************
  tag: Peter Soetens  Thu Oct 10 16:16:58 CEST 2002  StateContext.hpp

                        StateContext.hpp -  description
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

#ifndef STATECONTEXT_HPP
#define STATECONTEXT_HPP

#include "StateInterface.hpp"
#include "ConditionInterface.hpp"

#include <map>
#include <vector>
#include <string>
#include <boost/tuple/tuple.hpp>

namespace ORO_CoreLib
{
    /**
     * @brief A StateContext keeps track of the current StateInterface and all
     * transitions from this StateInterface to another.
     *
     * One can request
     * a transition from one StateInterface to another which will fail
     * or succeed depending on a previously set condition.
     *
     * By default, any state transition fails.
     */
    class StateContext
    {
        /**
         * The key is the current state, the value is the transition condition to
         * another state with a certain priority (int).
         */
        typedef std::vector< boost::tuple<ConditionInterface*, StateInterface*, int> > TransList;
        typedef std::map< StateInterface*, TransList > TransitionMap;
    public:

        /**
         * Create a StateContext instance with an empty initial state.
         */
        StateContext();

        virtual ~StateContext();

        /**
         * Create a StateContext instance with a given initial and final state.
         *
         * @param s_init
         *        The first state which must be entered.
         * @param s_fini
         *        The final state
         * @post The StateContext is in state <s_init>
         */
        StateContext( StateInterface* s_init, StateInterface* s_fini );

        /**
         * Is this StateContext active ?
         */
        bool isActive() const;
        /**
         * Start this StateContext. The Initial state will be entered.
         *
         */
        bool activate();

        /**
         * Stop this StateContext. The current state (which should be 
         * the Final state) is left.
         *
         */
        bool deactivate();

        /**
         * Search from the current state a candidate next state.
         * If none is found, the current state is taken.
         * Next, handle the resulting state.
         *
         * This call is equivalent to
         * this->requestState( this->nextState() ), but more
         * efficient.
         *
         * @return The current state.
         */
        StateInterface* requestNextState();

        /**
         * Request going to the Final State. This will always
         * proceed.
         */
        void requestFinalState();

        /**
         * Request going to the Initial State. This function will only
         * proceed if the current state is the Final State or the
         * Initial State. If it fails, one can try
         * to requestNextState() which may lead to the
         * initial state anyway if the transition is set.
         * This path is not tried by this function.
         *
         * @return The true on success, false if not allowed.
         */
        bool requestInitialState();

        /**
         * Search from the current state a candidate next state.
         * If none is found, the current state is returned.
         */
        StateInterface* nextState();

        /**
         * Request a state transition to a new state.
         * If the transition is not set by transitionSet(), acquiering
         * the state will fail.
         *
         * @param  s_n
         *         The state to change to
         * @return true
         *          if the transition is successfull
         *         false
         *          if the transition is not allowed
         */
        bool requestState( StateInterface * s_n );

        /**
         * Express a possible transition from one state to another under
         * a certain condition.
         *
         * @param from
         *        The state which should be left
         * @param to
         *        The state which should be entered
         * @param cnd
         *        The Condition under which the transition may succeed
         * @param priority
         *        The priority of this transition; low number (like -1000) is low priority
         *        high number is high priority (like + 1000). Transitions of equal
         *        priority are traversed in an unspecified way.
         * @post  All transitions from <from> to <to> will succeed under
         *        condition <cnd>
         */
        void transitionSet( StateInterface* from, StateInterface* to, ConditionInterface* cnd, int priority=0 );

        /**
         * Set the initial state of this StateContext.
         */
        void setInitialState( StateInterface* s );

        /**
         * Set the final state of this StateContext.
         */
        void setFinalState( StateInterface* s );

        /**
         * Retrieve the current state of the context
         */
        StateInterface* currentState() const;

        /**
         * Retrieve the initial state of the context.
         */
        StateInterface* getInitialState() const {
            return initstate;
        }

        /**
         * Retrieve the final state of the context.
         */
        StateInterface* getFinalState() const {
            return finistate;
        }

    protected:
        void leaveState( StateInterface* s );

        void enterState( StateInterface* s );

        /**
         * The Initial State.
         */
        StateInterface* initstate;

        /**
         * The Final State.
         */
        StateInterface* finistate;

        /**
         * The current state the Context is in.
         * current == 0 means that the context is currently inactive...
         */
        StateInterface* current;


    protected:
        /**
         * A map keeping track of all conditional transitions
         * between two states
         */
        TransitionMap stateMap;
    };
}

#endif

