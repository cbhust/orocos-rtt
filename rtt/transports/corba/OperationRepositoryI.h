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
// ../../../ACE_wrappers/TAO/TAO_IDL/be/be_codegen.cpp:1133

#ifndef ORO_CORBA_OPERATIONREPOSITORYI_H_
#define ORO_CORBA_OPERATIONREPOSITORYI_H_

#include "corba.h"
#ifdef CORBA_IS_TAO
#include "OperationRepositoryS.h"
#else
#include "OperationRepositoryC.h"
#endif
#include "../../interface/OperationRepository.hpp"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  RTT_corba_CSendHandle_i
  : public virtual POA_RTT::corba::CSendHandle,
  public virtual PortableServer::RefCountServantBase
{
public:
  // Constructor
  RTT_corba_CSendHandle_i (void);

  // Destructor
  virtual ~RTT_corba_CSendHandle_i (void);

  virtual
  ::RTT::corba::CSendStatus collect (
      ::RTT::corba::CAnyArguments & args);

  virtual
  ::RTT::corba::CSendStatus collectIfDone (
      ::RTT::corba::CAnyArguments & args);

  virtual
  ::RTT::corba::CSendStatus checkStatus (
      void);

  virtual
  ::CORBA::Any * ret (
      void);
};

class  RTT_corba_COperationRepository_i
  : public virtual POA_RTT::corba::COperationRepository,
  public virtual PortableServer::RefCountServantBase
{
      RTT::interface::OperationRepository* mfact;
      PortableServer::POA_var mpoa;
  public:
    //Constructor
      RTT_corba_COperationRepository_i(RTT::interface::OperationRepository* mfact, PortableServer::POA_ptr the_poa);

      PortableServer::POA_ptr _default_POA();

  // Destructor
  virtual ~RTT_corba_COperationRepository_i (void);

  virtual
  RTT::corba::COperationRepository::COperationList * getOperations (
      void);

  virtual
  ::RTT::corba::CDescriptions * getArguments (
      const char * operation);

  virtual
  char * getResultType (
      const char * operation);

  virtual
  char* getArgumentType(
          const char*,
          CORBA::UShort);

  virtual
  char* getCollectType(
          const char*,
          CORBA::UShort);

  virtual
  char * getDescription (
      const char * operation);

  virtual
  void checkOperation (
      const char * operation,
      ::RTT::corba::CAnyArguments & args);

  virtual
  ::CORBA::Any * callOperation (
      const char * operation,
      ::RTT::corba::CAnyArguments & args);

  virtual
  ::RTT::corba::CSendHandle_ptr sendOperation (
      const char * operation,
      ::RTT::corba::CAnyArguments & args);
};


#endif /* OPERATIONREPOSITORYI_H_  */
