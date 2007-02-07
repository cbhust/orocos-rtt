/***************************************************************************
  tag: Peter Soetens  Mon Jun 26 13:25:58 CEST 2006  ExecutionEngineI.h 

                        ExecutionEngineI.h -  description
                           -------------------
    begin                : Mon June 26 2006
    copyright            : (C) 2006 Peter Soetens
    email                : peter.soetens@fmtc.be
 
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
// be/be_codegen.cpp:844

#ifndef EXECUTIONENGINEI_H_
#define EXECUTIONENGINEI_H_

#include "ExecutionEngineS.h"
#include "ExecutionEngine.hpp"
#include "StatementProcessor.hpp"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  Orocos_ExecutionEngine_i : public virtual POA_RTT::Corba::ExecutionEngine, public virtual PortableServer::RefCountServantBase
{
    ExecutionEngine& mee;
    StatementProcessor sproc;
public:
  //Constructor 
  Orocos_ExecutionEngine_i (ExecutionEngine& ee);
  
  //Destructor 
  virtual ~Orocos_ExecutionEngine_i (void);
  
  virtual
  void loadPrograms (
      const char * code,
      const char * filename
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ::RTT::Corba::ExecutionEngine::LoadException
    ));
  
  virtual
  void unloadProgram (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ::RTT::Corba::ExecutionEngine::LoadException
    ));
  
  virtual
  ::RTT::Corba::ExecutionEngine::ProgramNames * getPrograms (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  CORBA::Long getProgramLine (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  char * getProgramText (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  char * getProgramStatus (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void loadStateMachines (
      const char * code,
      const char * filename
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ::RTT::Corba::ExecutionEngine::LoadException
    ));
  
  virtual
  void unloadStateMachine (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ::RTT::Corba::ExecutionEngine::LoadException
    ));
  
  virtual
  ::RTT::Corba::ExecutionEngine::StateMachineNames * getStateMachines (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  char * getStateMachineText (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  char * getStateMachineStatus (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  CORBA::Long getStateMachineLine (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  char * getCurrentState (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  CORBA::Boolean execute (
      const char * code
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
};


#endif /* EXECUTIONENGINEI_H_  */

