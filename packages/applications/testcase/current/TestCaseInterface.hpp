/***************************************************************************
  tag: Peter Soetens  Thu Oct 10 16:17:00 CEST 2002  TestCaseInterface.hpp 

                        TestCaseInterface.hpp -  description
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
 
#ifndef TESTCASEINTERFACE_HPP
#define TESTCASEINTERFACE_HPP

#include <corelib/EventInterfaces.hpp>

#include <string>
#include <vector>

namespace UnitTesting
{

    using namespace ORO_CoreLib;

    /**
     * TestCaseInterface.
     * XXX This class exists mainly because of a circular dependency between TestCase and TestSuite (caseNameGet <-> addTest).
     */

    class TestCaseInterface
    {

        public:

        virtual ~TestCaseInterface() {}

            /**
             * Get the name of this test case.
             * @return   The name of the test case.
             */
            virtual const char* caseNameGet() = 0;

            /**
             * Start the test case.
             * @param ei  Event that has to warn the TestRunner that the TestCase has completed.
             */
            virtual void start( EventOperationInterface* ei ) = 0;

            /**
             * Get all signaled errors.
             * @return   The vector returned only lives during the 
             *     lifetime of the instance of this class.
                    */
            virtual std::vector<const std::string*>* errorsGet() = 0;

            /**
             * Get the number of errors encountered during this test.
             */
            virtual int errorsNumber() = 0;

    };

}

#endif // TESTCASEINTERFACE_HPP

