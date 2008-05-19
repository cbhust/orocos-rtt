/***************************************************************************
  tag: FMTC  do nov 2 13:06:20 CET 2006  DataFlowI.h 

                        DataFlowI.h -  description
                           -------------------
    begin                : do november 02 2006
    copyright            : (C) 2006 FMTC
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

#ifndef DATAFLOWI_H_
#define DATAFLOWI_H_

#include "../rtt-config.h"
#include "DataFlowS.h"
#include "../DataFlowInterface.hpp"
#include "../BufferInterface.hpp"
#include "CorbaConversion.hpp"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/*
 * Represent a buffered data channel.
 */
template< class T>
class  RTT_Corba_BufferChannel_i : public virtual POA_RTT::Corba::BufferChannel, public virtual PortableServer::RefCountServantBase
{
    typename RTT::BufferInterface<T>::shared_ptr mbuf;
public:
  //Constructor 
  RTT_Corba_BufferChannel_i (typename RTT::BufferInterface<T>::shared_ptr buf)
      : mbuf(buf)
    {}
        
  
  //Destructor 
    virtual ~RTT_Corba_BufferChannel_i (void) {}
  
    virtual CORBA::Boolean push (const ::CORBA::Any & data)
        ACE_THROW_SPEC ((
                         CORBA::SystemException
                         ))
    {
        using namespace RTT;
        Logger::In in("CorbaBufferChannelI::push");
        log(Debug) << "Receiving buffer value."<<endlog();

        T value;
        if ( AnyConversion<T>::update( data, value ) == false ) {
            Logger::log() <<Logger::Error << "Could not accept remote value: wrong data type."<<Logger::endl;
            return false;
        }

        return mbuf->Push( value );
    }

    virtual CORBA::Boolean pull ( ::CORBA::Any_out data)
        ACE_THROW_SPEC ((
                         CORBA::SystemException
                         ))
    {
        using namespace RTT;
        Logger::In in("CorbaBufferChannelI::pull");
        log(Debug) << "Returning buffer value."<<endlog();
        T result;
        if ( mbuf->Pop( result ) == false ) {
            data = new CORBA::Any();
            return false;
        }
        CORBA::Any_var toset = AnyConversion<T>::createAny( result );
        data = toset._retn();
        return true;
    }
  
  virtual
  ::CORBA::Any_ptr front(
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return RTT::AnyConversion<T>::createAny( mbuf->front() );
  }
  
  virtual
  CORBA::Boolean full (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return mbuf->full();
  }
  
  virtual
  CORBA::Boolean empty (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return mbuf->empty();
  }
  
  virtual
  CORBA::Long capacity (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )){
      return mbuf->capacity();
  }
  
  virtual
  CORBA::Long size (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return mbuf->size();
  }
  
  virtual
  void clear (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      mbuf->clear();
  }
};

/**
 * Specialise for when we really don't know this C++ type.
 */
template<>
class  RTT_Corba_BufferChannel_i<RTT::detail::UnknownType> : public virtual POA_RTT::Corba::BufferChannel, public virtual PortableServer::RefCountServantBase
{
    RTT::BufferBase::shared_ptr mbuf;
public:
  //Constructor 
  RTT_Corba_BufferChannel_i ( RTT::BufferBase::shared_ptr buf)
      : mbuf(buf)
    {}
  
  //Destructor 
    virtual ~RTT_Corba_BufferChannel_i (void) {}
  
    virtual CORBA::Boolean push (const ::CORBA::Any & data)
        ACE_THROW_SPEC ((
                         CORBA::SystemException
                         ))
    {
        using namespace RTT;
        Logger::In in("CorbaBufferChannelI::push");
        log(Error) << "Don't know how to interprete this data type."<<endlog();
        return false;
    }

    virtual CORBA::Boolean pull ( ::CORBA::Any_out data)
        ACE_THROW_SPEC ((
                         CORBA::SystemException
                         ))
    {
        using namespace RTT;
        data = new CORBA::Any();
        Logger::In in("CorbaBufferChannelI::pull");
        log(Error) << "Don't know how to transfer this data type."<<endlog();
        return false;
    }
  
  virtual
  ::CORBA::Any_ptr front(
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return new CORBA::Any();
  }
  
  virtual
  CORBA::Boolean full (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return mbuf->full();
  }
  
  virtual
  CORBA::Boolean empty (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return mbuf->full();
  }
  
  virtual
  CORBA::Long capacity (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )){
      return mbuf->capacity();
  }
  
  virtual
  CORBA::Long size (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return mbuf->size();
  }
  
  virtual
  void clear (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      mbuf->clear();
  }
};


class  RTT_Corba_DataFlowInterface_i : public virtual POA_RTT::Corba::DataFlowInterface, public virtual PortableServer::RefCountServantBase
{
    RTT::DataFlowInterface* mdf;
public:
  //Constructor 
  RTT_Corba_DataFlowInterface_i (RTT::DataFlowInterface* dfi);
  
  //Destructor 
  virtual ~RTT_Corba_DataFlowInterface_i (void);
  
  virtual
  ::RTT::Corba::DataFlowInterface::PortNames * getPorts (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  ::RTT::Corba::AssignableExpression_ptr createDataChannel (
      const char * port_name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  ::RTT::Corba::BufferChannel_ptr createBufferChannel (
      const char * port_name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  ::RTT::Corba::Expression_ptr createDataObject (
      const char * port_name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  ::RTT::Corba::DataFlowInterface::ConnectionModel getConnectionModel (
      const char * port_name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  ::RTT::Corba::DataFlowInterface::PortType getPortType (
      const char * port_name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  char* getDataType (
      const char * port_name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean isConnected (
      const char * port_name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  void disconnect (
      const char * port_name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean connectDataPort (
     const char * port_name,
     ::RTT::Corba::AssignableExpression_ptr data
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean connectBufferPort (
     const char * port_name,
     ::RTT::Corba::BufferChannel_ptr buffer
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  CORBA::Boolean connectPorts (
     const char * local_port, 
     ::RTT::Corba::DataFlowInterface_ptr remote_ports, 
     const char * remote_port
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
};


#endif /* DATAFLOWI_H_  */

