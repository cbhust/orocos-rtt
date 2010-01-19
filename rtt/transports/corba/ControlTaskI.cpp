/***************************************************************************
  tag: FMTC  Tue Mar 11 21:49:22 CET 2008  ControlTaskI.cpp

                        ControlTaskI.cpp -  description
                           -------------------
    begin                : Tue March 11 2008
    copyright            : (C) 2008 FMTC
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
// be/be_codegen.cpp:910

#include "ControlTaskI.h"
#include "ControlTaskServer.hpp"
#include "ControlTaskProxy.hpp"
#include "AttributesI.h"
#include "OperationsI.h"
#include "OperationInterfaceI.h"
#include "ScriptingAccessI.h"
#include "ServicesI.h"
#include "DataFlowI.h"
#include "../../Method.hpp"
#include "../../rtt-detail-fwd.hpp"


using namespace RTT;
using namespace RTT::detail;

// CControlObject:
Orocos_CControlObject_i::Orocos_CControlObject_i (RTT::interface::OperationInterface* orig, PortableServer::POA_ptr the_poa )
    : mpoa( PortableServer::POA::_duplicate(the_poa) )
    , mobj( orig ), mAttrs_i(0), mMFact_i(0), mCFact_i(0)
{
}

// Implementation skeleton destructor
Orocos_CControlObject_i::~Orocos_CControlObject_i (void)
{
    if (!CORBA::is_nil(mAttrs))
    {
        PortableServer::ObjectId_var oid = mpoa->servant_to_id(mAttrs_i.in());
        mpoa->deactivate_object(oid.in());
    }
    if (!CORBA::is_nil(mMFact))
    {
        PortableServer::ObjectId_var oid = mpoa->servant_to_id(mMFact_i.in());
        mpoa->deactivate_object(oid.in());
    }

    // FIXME free up cache ? This is done by refcountservantbase ?
    for( CTObjMap::iterator it = ctobjmap.begin(); it != ctobjmap.end(); ++it) {
        PortableServer::ObjectId_var oid = mpoa->servant_to_id( it->second );
        mpoa->deactivate_object(oid.in());
    }
}

PortableServer::POA_ptr Orocos_CControlObject_i::_default_POA()
{
    return PortableServer::POA::_duplicate(mpoa);
}

 char* Orocos_CControlObject_i::getName (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      ))
{
    CORBA::String_var ret = CORBA::string_dup( mobj->getName().c_str() );
    return ret._retn();
}

 char* Orocos_CControlObject_i::getDescription (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      ))
{
    CORBA::String_var ret = CORBA::string_dup( mobj->getDescription().c_str() );
    return ret._retn();
}

::RTT::corba::CAttributeInterface_ptr Orocos_CControlObject_i::attributes (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    if (CORBA::is_nil(mAttrs)) {
        log(Debug) << "Creating CAttributeInterface."<<endlog();
        Orocos_CAttributeInterface_i* mserv;
        mAttrs_i = mserv = new Orocos_CAttributeInterface_i( mobj->attributes(), mpoa );
        try {
            mAttrs   = mserv->activate_this();
        } catch( ... ) {
            log(Error) << "Failed to create CAttributeInterface." <<endlog();
        }
    }
    return CAttributeInterface::_duplicate(mAttrs.in());
}

::RTT::corba::CMethodInterface_ptr Orocos_CControlObject_i::methods (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    if ( CORBA::is_nil( mMFact ) ) {
        log(Debug) << "Creating MethodInterface."<<endlog();
        Orocos_CMethodInterface_i* mserv;
        mMFact_i = mserv = new Orocos_CMethodInterface_i( mobj->methods(), mpoa );
        try {
            mMFact = mserv->_this();
        } catch( ... ) {
            log(Error) << "Failed to create MethodInterface." <<endlog();
        }
    }
    return CMethodInterface::_duplicate( mMFact.in() );
}

::RTT::corba::CControlObject_ptr Orocos_CControlObject_i::getObject (
    const char * name
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    std::string pname(name);
    // detect 'this':
    if ( pname == "this" )
        return this->_this();

    // Cache other objects
    OperationInterface* task = mobj->getObject( pname );
    if ( task ) {
        // do caching....
        Orocos_CControlObject_i* ret;
        if ( ctobjmap[pname] == 0 || ctobjmap[pname]->mobj != task) {
            // create or lookup new server for this object.
            // FIXME free up cache ? This is done by refcountservantbase !
            //delete ctobjmap[pname];
            ctobjmap[pname] = new Orocos_CControlObject_i(task, mpoa );
        }
        ret = ctobjmap[pname];
        return ret->_this();
    }
    // clear cache if possible.
    if ( ctobjmap.count(pname) ) {
        PortableServer::ObjectId_var oid = mpoa->servant_to_id( ctobjmap[pname] );
        mpoa->deactivate_object(oid.in());
        ctobjmap.erase( pname );
    }
    return RTT::corba::CControlObject::_nil();
}


::RTT::corba::CObjectList * Orocos_CControlObject_i::getObjectList (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    TaskContext::ObjectList objects = mobj->getObjectList();
    ::RTT::corba::CObjectList_var result = new ::RTT::corba::CObjectList();
    result->length( objects.size() );
    for (unsigned int i=0; i != objects.size(); ++i )
        result[i] = CORBA::string_dup( objects[i].c_str() );

    return result._retn();
}

CORBA::Boolean Orocos_CControlObject_i::hasObject (
    const char * name
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    std::string mname(name);
    return mobj->getObject(mname);
}



// Implementation skeleton constructor
Orocos_CControlTask_i::Orocos_CControlTask_i (TaskContext* orig, PortableServer::POA_ptr the_poa)
    : Orocos_CControlObject_i(orig, the_poa), mtask( orig ), mEEFact()
{
    // Add the corba object to the interface:
    mtask->addOperation("shutdown", &Orocos_CControlTask_i::shutdownCORBA, this).doc("Shutdown CORBA ORB. This function makes RunOrb() return.");

}

  void Orocos_CControlTask_i::shutdownCORBA() {
	  ControlTaskServer::ShutdownOrb(false);
  }


// Implementation skeleton destructor
Orocos_CControlTask_i::~Orocos_CControlTask_i (void)
{
    if (!CORBA::is_nil(mEEFact))
    {
        PortableServer::ObjectId_var oid = mpoa->servant_to_id(mEEFact_i.in());
        mpoa->deactivate_object(oid.in());
    }
    if (!CORBA::is_nil(mService))
    {
        PortableServer::ObjectId_var oid = mpoa->servant_to_id(mService_i.in());
        mpoa->deactivate_object(oid.in());
    }
    if (!CORBA::is_nil(mDataFlow))
    {
        CDataFlowInterface_i::deregisterServant(mtask->ports());
        PortableServer::ObjectId_var oid = mpoa->servant_to_id(mDataFlow_i.in());
        mpoa->deactivate_object(oid.in());
    }
}

::RTT::corba::CTaskState Orocos_CControlTask_i::getTaskState (
    )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    return ::RTT::corba::CTaskState(mtask->getTaskState());
}

CORBA::Boolean Orocos_CControlTask_i::start (
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    return mtask->start();
}

CORBA::Boolean Orocos_CControlTask_i::stop (
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    return mtask->stop();
}

CORBA::Boolean Orocos_CControlTask_i::isActive (
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    return mtask->isActive();
}

CORBA::Boolean Orocos_CControlTask_i::isRunning (
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    return mtask->isRunning();
}

CORBA::Boolean Orocos_CControlTask_i::configure (
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    return mtask->configure();
}

CORBA::Boolean Orocos_CControlTask_i::cleanup (
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    return mtask->cleanup();
}

CORBA::Boolean Orocos_CControlTask_i::isConfigured (
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    return mtask->isConfigured();
}


CORBA::Boolean Orocos_CControlTask_i::resetError (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
    return mtask->resetError();
}


CORBA::Boolean Orocos_CControlTask_i::activate (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
    return mtask->activate();
}

CORBA::Boolean Orocos_CControlTask_i::inFatalError (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
    return mtask->inFatalError();
}


CORBA::Boolean Orocos_CControlTask_i::inRunTimeWarning (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
    return mtask->inRunTimeWarning();
}



CORBA::Boolean Orocos_CControlTask_i::inRunTimeError (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
    return mtask->inRunTimeError();
}


CORBA::Long Orocos_CControlTask_i::getWarningCount (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
    return mtask->getWarningCount();
}



CORBA::Long Orocos_CControlTask_i::getErrorCount (
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
    return mtask->getErrorCount();
}


::RTT::corba::CScriptingAccess_ptr Orocos_CControlTask_i::scripting (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    if ( CORBA::is_nil( mEEFact ) ) {
        log(Debug) << "Creating CScriptingAccess."<<endlog();
        Orocos_CScriptingAccess_i* mserv;
        mEEFact_i = mserv = new Orocos_CScriptingAccess_i( mtask->scripting(), mpoa );
        mEEFact = mserv->_this();
        //mserv->_remove_ref();
    }
    return ::RTT::corba::CScriptingAccess::_duplicate( mEEFact.in() );
}

::RTT::corba::CServiceInterface_ptr Orocos_CControlTask_i::services (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
    if ( CORBA::is_nil( mService ) ) {
        log(Debug) << "Creating Services."<<endlog();
        RTT_Corba_CServiceInterface_i* mserv;
        mService_i = mserv = new RTT_Corba_CServiceInterface_i( mpoa );
        mService = mserv->_this();
        //mService->_remove_ref();
    }
    return ::RTT::corba::CServiceInterface::_duplicate( mService.in() );
}

::RTT::corba::CDataFlowInterface_ptr Orocos_CControlTask_i::ports (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
    if ( CORBA::is_nil( mDataFlow ) ) {
        log(Debug) << "Creating CDataFlowInterface."<<endlog();
        RTT::corba::CDataFlowInterface_i* mserv;
        mDataFlow_i = mserv = new RTT::corba::CDataFlowInterface_i( mtask->ports(), mpoa );
        mDataFlow = mserv->activate_this();
        CDataFlowInterface_i::registerServant(mDataFlow, mtask->ports());
    }
    return ::RTT::corba::CDataFlowInterface::_duplicate( mDataFlow.in() );
}



::RTT::corba::CControlTask::CControlTaskNames * Orocos_CControlTask_i::getPeerList (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    TaskContext::PeerList peers = mtask->getPeerList();
    ::RTT::corba::CControlTask::CControlTaskNames_var result = new ::RTT::corba::CControlTask::CControlTaskNames();
    result->length( peers.size() );
    for (unsigned int i=0; i != peers.size(); ++i )
        result[i] = CORBA::string_dup( peers[i].c_str() );

    return result._retn();
}


::RTT::corba::CControlTask_ptr Orocos_CControlTask_i::getPeer (
    const char * name
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    std::string pname(name);
    TaskContext* task = mtask->getPeer( pname );
    if ( task ) {
        // create or lookup new server for this peer.
        // do not export it to the naming service.
        return ControlTaskServer::CreateServer( task, false );
    }
    return RTT::corba::CControlTask::_nil();
}

CORBA::Boolean Orocos_CControlTask_i::addPeer (
    ::RTT::corba::CControlTask_ptr p,
    const char * alias
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    std::string malias(alias);
    if (mtask->hasPeer(alias) == false )
        return mtask->addPeer( ControlTaskProxy::Create(p), alias );
    return false;
}

CORBA::Boolean Orocos_CControlTask_i::hasPeer (
    const char * name
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    std::string mname(name);
    return mtask->hasPeer(mname);
}

CORBA::Boolean Orocos_CControlTask_i::removePeer (
    const char * name
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    std::string mname(name);
    mtask->removePeer( mname );
    return true;
}

CORBA::Boolean Orocos_CControlTask_i::connectPeers (
    ::RTT::corba::CControlTask_ptr p
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    TaskContext* t = ControlTaskProxy::Create( p );
    return mtask->connectPeers( t );
}

CORBA::Boolean Orocos_CControlTask_i::disconnectPeers (
    const char * name
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    std::string pname(name);
    mtask->disconnectPeers( pname );
    return true;
}

CORBA::Boolean Orocos_CControlTask_i::connectPorts (
    ::RTT::corba::CControlTask_ptr p
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    TaskContext* t = ControlTaskProxy::Create( p );
    return mtask->connectPorts( t );
}


