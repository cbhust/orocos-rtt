/***************************************************************************
  tag: Peter Soetens  Mon Jan 10 15:59:18 CET 2005  dispatch_test.hpp 

                        dispatch_test.hpp -  description
                           -------------------
    begin                : Mon January 10 2005
    copyright            : (C) 2005 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be
 
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
 
#ifndef DISPATCHTEST_H
#define DISPATCHTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include <scripting/Parser.hpp>
#include <CommandProcessor.hpp>
#include <TaskContext.hpp>
#include <string>
#include <SimulationActivity.hpp>

using namespace RTT;
using namespace RTT;

class DispatchTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( DispatchTest );
    CPPUNIT_TEST( testParseDispatch );
    CPPUNIT_TEST( testDispatchFailure );
    CPPUNIT_TEST( testDispatchCondition );
    CPPUNIT_TEST( testDispatchAnd );
    CPPUNIT_TEST( testDispatchTry );
    CPPUNIT_TEST( testDispatchUntil );
    CPPUNIT_TEST( testDispatchUntilFail );
    CPPUNIT_TEST( testDispatchMany );
    CPPUNIT_TEST_SUITE_END();

    //CPPUNIT_TEST( testSendDispatch );

    Parser parser;
    TaskContext gtc;
    TaskContext mtc;
    TaskContext ltc;
    SimulationActivity gtask;
    SimulationActivity mtask;
    SimulationActivity ltask;
    TaskObject* createObject(std::string a, CommandProcessor* cp);

    bool true_genCom() { return true; }
    bool false_genCom() { return false; }
    bool true_gen() const { return true; }
    bool false_gen() const { return false; }

    bool bool_gen( bool b ) const { return b; }

    bool assertBool( bool );
    bool assertMsg( bool, const std::string& msg);
    void doDispatch( const std::string& prog, TaskContext* );
    void finishDispatch( TaskContext* , std::string );
public:
    DispatchTest();

    void setUp();
    void tearDown();

    void testParseDispatch();
    void testDispatchCondition();
    void testDispatchMany();
    void testDispatchFailure();
    void testDispatchAnd();
    void testDispatchTry();
    void testDispatchUntil();
    void testDispatchUntilFail();

};

#endif
