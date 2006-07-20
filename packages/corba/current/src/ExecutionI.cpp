// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from 
// be/be_codegen.cpp:910

#include "rtt/corba/ExecutionI.h"
#include "rtt/CommandC.hpp"
#include "rtt/DataSourceCondition.hpp"
#include "rtt/Logger.hpp"





// Implementation skeleton constructor
Orocos_Action_i::Orocos_Action_i (CommandInterface* com )
    : mcom(com)
{
}

// Implementation skeleton destructor
Orocos_Action_i::~Orocos_Action_i (void)
{
    delete mcom;
}

CORBA::Boolean Orocos_Action_i::execute (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    return mcom->execute();
}

void Orocos_Action_i::reset (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    mcom->reset();
}

// Implementation skeleton constructor
Orocos_Command_i::Orocos_Command_i ( CommandC& orig)
    : morig( new CommandC(orig) )
{
}

// Implementation skeleton destructor
Orocos_Command_i::~Orocos_Command_i (void)
{
    delete morig;
}

CORBA::Boolean Orocos_Command_i::execute (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    //Logger::In in("Orocos_Command_i");
    //Logger::log() <<Logger::Debug << "Executing CommandC."<<Logger::endl;
    return morig->execute();
}

CORBA::Boolean Orocos_Command_i::evaluate (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    //Logger::In in("Orocos_Command_i");
    //Logger::log() <<Logger::Debug << "Evaluating CommandC:"<<morig->evaluate()<<Logger::endl;
    return morig->evaluate();
}

CORBA::Boolean Orocos_Command_i::executed (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    return morig->executed();
}

CORBA::Boolean Orocos_Command_i::sent (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    return morig->sent();
}

CORBA::Boolean Orocos_Command_i::accepted (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    return morig->accepted();
}

CORBA::Boolean Orocos_Command_i::valid (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    return morig->valid();
}

void Orocos_Command_i::reset (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    return morig->reset();
}

::Orocos::Action_ptr Orocos_Command_i::createAction (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    Logger::In in("Orocos_Command_i");
    Logger::log() <<Logger::Debug << "Creating Orocos::Action servant."<<Logger::endl;
    Orocos_Action_i* act = new Orocos_Action_i( morig->createCommand() );
    return act->_this();
}

::Orocos::Expression_ptr Orocos_Command_i::createCondition (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    Logger::In in("Orocos_Command_i");
    Logger::log() <<Logger::Debug << "Creating Orocos::Expression servant."<<Logger::endl;
    Orocos_Expression_i<bool>* exp = new Orocos_Expression_i<bool>( new DataSourceCondition( morig->createCondition() ) );
    return exp->_this();
}




