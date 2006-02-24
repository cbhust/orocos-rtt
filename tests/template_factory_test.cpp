/***************************************************************************
  tag: Peter Soetens  Wed Jan 18 14:11:41 CET 2006  template_factory_test.cpp 

                        template_factory_test.cpp -  description
                           -------------------
    begin                : Wed January 18 2006
    copyright            : (C) 2006 Peter Soetens
    email                : peter.soetens@mech.kuleuven.be
 
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
 

#include "template_factory_test.hpp"
#include <unistd.h>
#include <iostream>
#include <execution/FunctionGraph.hpp>
#include <execution/TemplateFactories.hpp>
#include <execution/DataSourceAdaptor.hpp>
#include <execution/DataSourceGenerator.hpp>

#include <corelib/TaskSimulation.hpp>
#include <corelib/SimulationThread.hpp>

#include <pkgconf/system.h>
#ifdef OROPKG_GEOMETRY
#include <geometry/frames.h>
using namespace ORO_Geometry;
#endif

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( Template_FactoryTest );


void 
Template_FactoryTest::setUp()
{
    tc =  new TaskContext( "root" );
    tc->methods()->registerObject("methods", this->createMethodFactory() );
    tc->commands()->registerObject("commands", this->createCommandFactory() );
    tc->datasources()->registerObject("data", this->createDataSourceFactory() );
    tc->events()->addEvent("FloatEvent", &t_event_float);
    tsim = new TaskSimulation(0.001, tc->engine() );
    event_proc = new EventProcessor();
}


void 
Template_FactoryTest::tearDown()
{
//     if ( tc->getPeer("programs") )
//         delete tc->getPeer("programs");
    tsim->stop();
    SimulationThread::Instance()->stop();
    delete tc;
    delete tsim;
    delete event_proc;
}

bool Template_FactoryTest::assertBool( bool b) { 
    return b;
}

MethodFactoryInterface* Template_FactoryTest::createMethodFactory()
{
    TemplateMethodFactory< Template_FactoryTest >* dat =
        newMethodFactory( this );

    dat->add( "assert", method( &Template_FactoryTest::assertBool, "assert","b","bd") );

    dat->add( "m0", method( &Template_FactoryTest::m0, "M0") );
    dat->add( "m1", method( &Template_FactoryTest::m1, "M1","a","ad") );
    dat->add( "m2", method( &Template_FactoryTest::m2, "M2","a","ad","a","ad") );
    dat->add( "m3", method( &Template_FactoryTest::m3, "M3","a","ad","a","ad","a","ad") );
    dat->add( "m4", method( &Template_FactoryTest::m4, "M4","a","ad","a","ad","a","ad","a","ad") );
    return dat;
}

DataSourceFactoryInterface* Template_FactoryTest::createDataSourceFactory()
{
    TemplateDataSourceFactory< Template_FactoryTest >* dat =
        newDataSourceFactory( this );

    dat->add( "d0", data( &Template_FactoryTest::d0, "d0") );
    dat->add( "d1", data( &Template_FactoryTest::d1, "d1","a","ad") );
    dat->add( "d2", data( &Template_FactoryTest::d2, "d2","a","ad","a","ad") );
    dat->add( "d3", data( &Template_FactoryTest::d3, "d3","a","ad","a","ad","a","ad") );
    dat->add( "d4", data( &Template_FactoryTest::d4, "d4","a","ad","a","ad","a","ad","a","ad") );

    return dat;
}

CommandFactoryInterface* Template_FactoryTest::createCommandFactory()
{
    TemplateCommandFactory< Template_FactoryTest >* dat =
        newCommandFactory( this );

    dat->add( "c00", command( &Template_FactoryTest::cd0, &Template_FactoryTest::cn0, "c0d") );
    dat->add( "c10", command( &Template_FactoryTest::cd1, &Template_FactoryTest::cn0, "c1d","a","ad") );
    dat->add( "c11", command( &Template_FactoryTest::cd1, &Template_FactoryTest::cn1, "c1d","a","ad") );
    dat->add( "c20", command( &Template_FactoryTest::cd2, &Template_FactoryTest::cn0, "c2d","a","ad","a","ad") );
    dat->add( "c21", command( &Template_FactoryTest::cd2, &Template_FactoryTest::cn1, "c2d","a","ad","a","ad") );
    dat->add( "c22", command( &Template_FactoryTest::cd2, &Template_FactoryTest::cn2, "c2d","a","ad","a","ad") );
    dat->add( "c30", command( &Template_FactoryTest::cd3, &Template_FactoryTest::cn0, "c3d","a","ad","a","ad","a","ad") );
    dat->add( "c31", command( &Template_FactoryTest::cd3, &Template_FactoryTest::cn1, "c3d","a","ad","a","ad","a","ad") );
    dat->add( "c33", command( &Template_FactoryTest::cd3, &Template_FactoryTest::cn3, "c3d","a","ad","a","ad","a","ad") );
    return dat;
}

void Template_FactoryTest::executePrograms(const Parser::ParsedPrograms& pg_list )
{
    tc->getExecutionEngine()->getProgramProcessor()->loadProgram( *pg_list.begin() );
    SimulationThread::Instance()->start();
    tsim->start();
    CPPUNIT_ASSERT( (*pg_list.begin())->start() );
    sleep(1);
    CPPUNIT_ASSERT( (*pg_list.begin())->stop() );
    tsim->stop();
    SimulationThread::Instance()->stop();
    if ( (*pg_list.begin())->inError() ) {
        stringstream errormsg;
        errormsg << " Program error on line " << (*pg_list.begin())->getLineNumber() <<"."<<endl;
        CPPUNIT_ASSERT_MESSAGE( errormsg.str(), false );
    }
}


void Template_FactoryTest::testMethods()
{
    string prog = std::string("program x { ")
        +" var double r = 0.0\n"
        +" set r = methods.m0\n"
        +" set r = methods.m0()\n"
        +" do methods.assert( r == -1.0 )\n"
        +" set r = methods.m1(1)\n"
        +" do methods.assert( r == -2.0 )\n"
        +" set r = methods.m2(1, 1.0)\n"
        +" do methods.assert( r == -3.0 )\n"
        +" set r = methods.m3(1, 1.0, true)\n"
        +" do methods.assert( r == -4.0 )\n"
        +" set r = methods.m4(1, 1.0, true, \"hello\")\n"
        +" do methods.assert( r == -5.0 )\n"
        +"}";
    stringstream progs(prog);
    Parser::ParsedPrograms pg_list;
    try {
        pg_list = parser.parseProgram( progs, tc);
    }
    catch( const file_parse_exception& exc )
        {
            CPPUNIT_ASSERT_MESSAGE(exc.what(), false );
        }
    if ( pg_list.empty() )
        {
            CPPUNIT_ASSERT( false );
        }
    // execute
    executePrograms( pg_list );
}

void Template_FactoryTest::testMethodsC()
{
    MethodC mc;
    double r = 0.0;
    mc = tc->methods()->create("methods", "m0").ret( r );
    CPPUNIT_ASSERT( mc.execute() );
    CPPUNIT_ASSERT( r == -1.0 );

    mc = tc->methods()->create("methods", "m2").argC(1).argC(1.0).ret( r );
    CPPUNIT_ASSERT( mc.execute() );
    CPPUNIT_ASSERT( r == -3.0 );

    mc = tc->methods()->create("methods", "m3").ret( r ).argC(1).argC(1.0).argC(true);
    CPPUNIT_ASSERT( mc.execute() );
    CPPUNIT_ASSERT( r == -4.0 );
    
#if 0
        +" set r = methods.m0()\n"
        +" do methods.assert( r == -1.0 )\n"
        +" set r = methods.m1(1)\n"
        +" do methods.assert( r == -2.0 )\n"
        +" set r = methods.m2(1, 1.0)\n"
        +" do methods.assert( r == -3.0 )\n"
        +" set r = methods.m3(1, 1.0, true)\n"
        +" do methods.assert( r == -4.0 )\n"
        +" set r = methods.m4(1, 1.0, true, \"hello\")\n"
        +" do methods.assert( r == -5.0 )\n"
#endif
}

void Template_FactoryTest::testData()
{
    string prog = string("program x { ")
        +" var double r = 0.0\n"
        +" set r = data.d0\n"
        +" set r = data.d0()\n"
        +" do methods.assert( r == 1.0 )\n"
        +" set r = data.d1(1)\n"
        +" do methods.assert( r == 2.0 )\n"
        +" set r = data.d2(1, 1.0)\n"
        +" do methods.assert( r == 3.0 )\n"
        +" set r = data.d3(1, 1.0, true)\n"
        +" do methods.assert( r == 4.0 )\n"
        +" set r = data.d4(1, 1.0, true, \"hello\")\n"
        +" do methods.assert( r == 5.0 )\n"
        +"}";
    stringstream progs(prog);
    Parser::ParsedPrograms pg_list;
    try {
        pg_list = parser.parseProgram( progs, tc);
    }
    catch( const file_parse_exception& exc )
        {
            CPPUNIT_ASSERT_MESSAGE(exc.what(), false );
        }
    if ( pg_list.empty() )
        {
            CPPUNIT_ASSERT( false );
        }
    // execute
        executePrograms( pg_list );
}

void Template_FactoryTest::testCommands()
{
    string prog = string("program x { ")
        +" do commands.c00()\n"
        +" do commands.c10(1)\n"
        +" do commands.c11(1)\n"
        +" do commands.c20(1, 1.0)\n"
        +" do commands.c21(1, 1.0)\n"
        +" do commands.c22(1, 1.0)\n"
        +" do commands.c30(1, 1.0, 'a')\n"
        +" do commands.c31(1, 1.0, 'a')\n"
        +" do commands.c33(1, 1.0, 'a')\n"
        +"}";
    stringstream progs(prog);
    Parser::ParsedPrograms pg_list;
    try {
        pg_list = parser.parseProgram( progs, tc);
    }
    catch( const file_parse_exception& exc )
        {
            CPPUNIT_ASSERT_MESSAGE(exc.what(), false );
        }
    if ( pg_list.empty() )
        {
            CPPUNIT_ASSERT( false );
        }
    // execute
        executePrograms( pg_list );
}

void Template_FactoryTest::testCommandsC()
{
    CommandC cc = tc->commands()->create("commands","c00");
    CommandC c20 = tc->commands()->create("commands","c20").argC(1).argC(1.0);
    CommandC c32 = tc->commands()->create("commands","c31").argC(1).argC(1.0).argC('a');
    CommandC c33 = tc->commands()->create("commands","c33").argC(1).argC(1.0).argC('a');
    SimulationThread::Instance()->start();
    tsim->start();
    CPPUNIT_ASSERT( cc.execute() );
    CPPUNIT_ASSERT( c20.execute() );
    CPPUNIT_ASSERT( c32.execute() );
    CPPUNIT_ASSERT( c33.execute() );
    sleep(1);
    CPPUNIT_ASSERT( cc.accepted() );
    CPPUNIT_ASSERT( c20.accepted() );
    CPPUNIT_ASSERT( c32.accepted() );
    CPPUNIT_ASSERT( c33.accepted() );
    CPPUNIT_ASSERT( cc.valid() );
    CPPUNIT_ASSERT( c20.valid() );
    CPPUNIT_ASSERT( c32.valid() );
    CPPUNIT_ASSERT( c33.valid() );
    CPPUNIT_ASSERT( cc.evaluate() );
    CPPUNIT_ASSERT( c20.evaluate() );
    CPPUNIT_ASSERT( c32.evaluate() );
    CPPUNIT_ASSERT( c33.evaluate() );
    tsim->stop();
    SimulationThread::Instance()->stop();
#if 0
    string prog = string("program x { ")
        +" do commands.c00()\n"
        +" do commands.c10(1)\n"
        +" do commands.c11(1)\n"
        +" do commands.c20(1, 1.0)\n"
        +" do commands.c21(1, 1.0)\n"
        +" do commands.c22(1, 1.0)\n"
        +" do commands.c30(1, 1.0, 'a')\n"
        +" do commands.c31(1, 1.0, 'a')\n"
        +" do commands.c33(1, 1.0, 'a')\n"
        +"}";
#endif
}

void Template_FactoryTest::testManual()
{
    // test manual invocation of factory methods by users:

    // C++ POD :
    vector<DataSourceBase::shared_ptr> res;
    res = GenerateDataSource()( 1 );
    CPPUNIT_ASSERT( res.size() == 1 );
    res = GenerateDataSource()(1, 1.0 );
    CPPUNIT_ASSERT( res.size() == 2 );
    res = GenerateDataSource()(1, 1.0, true );
    CPPUNIT_ASSERT( res.size() == 3 );
    res = GenerateDataSource()(1, 1.0, true, "c" );
    CPPUNIT_ASSERT( res.size() == 4 );

    // C++ DataSources :
    DataSourceBase::shared_ptr empty1;
    DataSource<double>::shared_ptr empty2;
    AssignableDataSource<bool>::shared_ptr empty3;
    VariableDataSource<string>::shared_ptr empty4;

    res = GenerateDataSource()( empty1.get() );
    CPPUNIT_ASSERT( res.size() == 1 );
    res = GenerateDataSource()( empty1.get(), empty2.get() );
    CPPUNIT_ASSERT( res.size() == 2 );
    res = GenerateDataSource()( empty1.get(), empty2.get(), empty3.get() );
    CPPUNIT_ASSERT( res.size() == 3 );
    res = GenerateDataSource()( empty1.get(), empty2.get(), empty3.get(), empty4.get() );
    CPPUNIT_ASSERT( res.size() == 4 );
}


int Template_FactoryTest::float_listener(float a, float b)
{
    Logger::log() << Logger::Debug << "float_listener "<< a<<", "<<b<<Logger::endl;
    float_sum += a + b;
    return 1;
}

int Template_FactoryTest::float_completer(float a, float b)
{
    Logger::log() << Logger::Debug << "float_completer "<< a<<", "<<b<<Logger::endl;
    float_sub -= (a + b);
    return 1; // ignored...
}


void Template_FactoryTest::testEventC()
{
    float_sum = 0;
    float_sub = 0;
    // use CompletionProcessor for completer
    CompletionProcessor::Instance()->stop();
    ConnectionC cc = tc->events()->setupConnection("FloatEvent");
    cc.callback( this, &Template_FactoryTest::float_listener );
    cc.callback( this, &Template_FactoryTest::float_completer, CompletionProcessor::Instance()->getEventProcessor() );
    Handle h = cc.handle();

    h.connect();

    EventC mevent = tc->events()->setupEmit("FloatEvent").argC(float(1.0)).argC(float(4.0));
    mevent.emit();
    CPPUNIT_ASSERT_EQUAL( float(5.0), float_sum );
    CPPUNIT_ASSERT_EQUAL( float(0.0),  float_sub );

    float a = 10.0, b = 5.0;
    mevent = tc->events()->setupEmit("FloatEvent").arg(a).arg(b);
    mevent.emit();
    CPPUNIT_ASSERT_EQUAL( float(20.0), float_sum );
    CPPUNIT_ASSERT_EQUAL( float(0.0),  float_sub );

    CompletionProcessor::Instance()->start();
    
    h.disconnect();
    float_sum = 0;
    float_sub = 0;

    // use event processor
    event_proc->initialize();

    cc = tc->events()->setupConnection("FloatEvent").callback( this, &Template_FactoryTest::float_listener);
    cc.callback( this, &Template_FactoryTest::float_completer, event_proc, ORO_CoreLib::EventProcessor::OnlyLast );
    h = cc.handle();

    h.connect();

    // simulate overrun :
    mevent = tc->events()->setupEmit("FloatEvent").argC(float(1.0)).argC(float(4.0));
    mevent.emit();
    CPPUNIT_ASSERT_EQUAL( float(5.0), float_sum );

    mevent = tc->events()->setupEmit("FloatEvent").arg(a).arg(b);
    mevent.emit();
    CPPUNIT_ASSERT_EQUAL( float(20.0), float_sum );

    event_proc->step();
    event_proc->finalize();
    // asyn handlers should reach only last total.
    CPPUNIT_ASSERT_EQUAL( float(-15.0), float_sub );
    h.disconnect();
}

