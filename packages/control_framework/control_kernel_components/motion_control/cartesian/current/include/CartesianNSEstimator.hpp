/***************************************************************************
  tag: Peter Soetens  Thu Apr 22 20:40:59 CEST 2004  CartesianNSEstimator.hpp 

                        CartesianNSEstimator.hpp -  description
                           -------------------
    begin                : Thu April 22 2004
    copyright            : (C) 2004 Peter Soetens
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
 
#ifndef CARTESIAN_NS_ESTIMATOR_HPP
#define CARTESIAN_NS_ESTIMATOR_HPP

#include <geometry/frames.h>

#include <kindyn/KinematicsComponent.hpp>
#include <kindyn/KinematicsFactory.hpp>

#include <corelib/Event.hpp>
#include <control_kernel/KernelInterfaces.hpp>
#include <control_kernel/ReportingExtension.hpp>
#include <control_kernel/PropertyExtension.hpp>
#include <control_kernel/BaseComponents.hpp>
#include <control_kernel/ExtensionComposition.hpp>
#include <corelib/PropertyComposition.hpp>

#include <pkgconf/control_kernel.h>
#ifdef OROPKG_CONTROL_KERNEL_EXTENSIONS_EXECUTION
#include <control_kernel/ExecutionExtension.hpp>
#include "execution/TemplateDataSourceFactory.hpp"
#include "execution/TemplateCommandFactory.hpp"
#endif

#include "CartesianNSDataObjects.hpp"

#ifdef ORO_PRAGMA_INTERFACE
#pragma interface
#endif

/**
 * @file CartesianNSEstimator.hpp
 * Contains an Estimator for calculating the
 * cartesian position from joint positions.
 */
    
namespace ORO_ControlKernel
{
    using namespace ORO_Geometry;
    using namespace ORO_CoreLib;
    using namespace ORO_KinDyn;
#ifdef OROPKG_CONTROL_KERNEL_EXTENSIONS_EXECUTION
    using namespace ORO_Execution;
#endif
    
    /**
     * @brief This Estimator calculates an end frame from
     * joint positions.
     * @ingroup kcomps kcomp_estimator
     */
    class CartesianEstimator
        : public Estimator<Expects<CartesianNSSensorInput>,
                           Expects<NoOutput>,
                           Writes<CartesianNSModel>,
                           MakeFacet<PropertyExtension,
                                     KernelBaseFunction,
#ifdef OROPKG_CONTROL_KERNEL_EXTENSIONS_EXECUTION
                                     ExecutionExtension
#endif
                                     >::Result >
    {
        typedef Estimator<Expects<CartesianNSSensorInput>,
                          Expects<NoOutput>,
                          Writes<CartesianNSModel>,
                          MakeFacet<PropertyExtension, KernelBaseFunction,
#ifdef OROPKG_CONTROL_KERNEL_EXTENSIONS_EXECUTION
                                    ExecutionExtension
#endif
                                    >::Result > Base;
    public:
            
        CartesianEstimator() 
            : Base("CartesianEstimator"),
              kineName("Kinematics","The name of the KinematicsStub to use","Kuka361"), kine(0),kineComp(0),
              jpos(6,0.0)
        {
            kine = KinematicsFactory::create( kineName );
            if (kine)
                kineComp = new KinematicsComponent( kine );
        }

        virtual ~CartesianEstimator()
        {
            delete kineComp;
        }

        virtual bool componentStartup()
        {
            // Get the JointPositions DataObject and the 
            if ( !Input->dObj()->Get( "JointPositions", jpos_DObj ) ||
                 !Model->dObj()->Get( "EndEffPosition", endframe_DObj ) )
                return false;
            pull();
            if ( !kineComp->positionForward( jpos, mp_base_frame) )
                return false;
            push();
            return true;
        }
            
        /**
         * @see KernelInterfaces.hpp class ModuleControlInterface
         */
        virtual void pull()      
        {
            jpos_DObj->Get( jpos );
        }
            
        /**
         * @see KernelInterfaces.hpp class ModuleControlInterface
         */
        virtual void calculate() 
        {
            if ( !kineComp->positionForward( jpos, mp_base_frame) )
                {
                    Event<void(void)>* es = Event<void(void)>::nameserver.getObject("SingularityDetected");
                    if ( es != 0 )
                        es->fire();
                }
        }
            
        /**
         * @see KernelInterfaces.hpp class ModuleControlInterface
         */
        virtual void push()      
        {
            endframe_DObj->Set( mp_base_frame );
        }
            
        /**
         * @see KernelPropertyExtension.hpp class PropertyComponentInterface
         */
        virtual bool updateProperties( const PropertyBag& bag)
        {
            if ( composeProperty(bag, kineName ) )
                {
                    return setKinematics( kineName );
                }
            return false;
        }

        virtual void exportProperties( PropertyBag& bag)
        {
            bag.add( &kineName );
        }
            

        bool setKinematics( const std::string& name )
        {
            KinematicsInterface* _k;
            if ( (_k = KinematicsFactory::create(name)) != 0 )
                {
                    delete kineComp;
                    kineName =  name;
                    kine = _k;
                    kineComp = new KinematicsComponent( kine );
                    return true;
                }
            return false;
        }
            
    protected:
        Property<string> kineName;
        KinematicsInterface* kine;
        KinematicsComponent* kineComp;

        ORO_KinDyn::JointPositions jpos;
        DataObjectInterface<ORO_KinDyn::JointPositions>* jpos_DObj;

        Frame mp_base_frame;
        DataObjectInterface<Frame>* endframe_DObj;

    };

}
#endif

