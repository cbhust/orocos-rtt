/***************************************************************************
  tag: Peter Soetens  Thu Oct 10 16:17:00 CEST 2002  TestSuite.hpp 

                        TestSuite.hpp -  description
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
 

#ifndef TESTSUITE_HPP
#define TESTSUITE_HPP

#include "TestCaseInterface.hpp"
#include <corelib/WriteInterface.hpp>

namespace UnitTesting
{

    using namespace ORO_CoreLib;

    /**
     * A test suite encapsulates a number of test cases.
     */
    class TestSuite
    {

            typedef std::vector<TestCaseInterface*> TestList;

        public:

            /**
             * Construct a TestSuite.
             */
            TestSuite(WriteInterface* );

            /**
             * Destroy the TestSuite and delete all registered TestCases.
             */
            ~TestSuite();

            /**
             * Add a test to this suite, the suite becomes the owner of <testCase>.
             * @param testCase  The test case.
             */
            void addTest( TestCaseInterface* testCase );

            /**
             * Get the next test.
             */
            TestCaseInterface* getNextTest();

            /**
             * Get the number of tests in this suite.
             */
            int testsNumber();

            /**
             * Returns true if we have traversed all the test cases.
             */
            bool atEnd();

            /**
             * Reset the iterator of the test cases to the beginning.
             */
            void reset();

            /**
             * Get the total number of errors.
             */
            int errorsNumber();

            /**
             * Get the number of failed test cases.
             */
            int testsFailed();

        /**
         * Creates all the necesarry tests.
         */
        void createTests(WriteInterface*);
        private:
            /**
             * The cases that are part of this suite.
             */
            TestList tests;

            /**
             * An iterator pointing to the next test.
             */
            TestList::iterator nextTest;

    };

}

#endif // TESTSUITE_HPP

