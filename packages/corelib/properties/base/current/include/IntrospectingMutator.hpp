/***************************************************************************
  tag: Peter Soetens  Mon Jan 19 14:11:20 CET 2004  IntrospectingMutator.hpp 

                        IntrospectingMutator.hpp -  description
                           -------------------
    begin                : Mon January 19 2004
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
 

#include "PropertyIntrospection.hpp"

#if 0
namespace ORO_CoreLib
{

    /**
     * A helper class for introspecting a PropertyBase and assigning a value
     * to it if it matches the anticipated type. Assigns the default value
     * if the types do not match.
     */
    class IntrospectingMutator
    {
        bool bool_val;
        char char_val;
        int  int_val;
        double double_val;
        std::string string_val;
        PropertyBag bag_val;

        public:
            IntrospectingMutator()
                :bool_val(), char_val(), int_val(), double_val(), string_val(), bag_val()
            {}

            IntrospectingMutator( bool set_val, PropertyBase* target)
                :bool_val(set_val), char_val(), int_val(), double_val(), string_val(), bag_val()
            {
                target->identify(this);
            }

            IntrospectingMutator( char set_val, PropertyBase* target)
                :bool_val(), char_val(set_val), int_val(), double_val(), string_val(), bag_val()
            {
                target->identify(this);
            }

            IntrospectingMutator( int set_val, PropertyBase* target)
                :bool_val(), char_val(), int_val(set_val), double_val(), string_val(), bag_val()
            {
                target->identify(this);
            }

            IntrospectingMutator( double set_val, PropertyBase* target)
                :bool_val(), char_val(), int_val(), double_val(set_val), string_val(), bag_val()
            {
                target->identify(this);
            }

            IntrospectingMutator( const std::string& set_val, PropertyBase* target)
                :bool_val(), char_val(), int_val(), double_val(), string_val(set_val), bag_val()
            {
                target->identify(this);
            }

            IntrospectingMutator( const PropertyBag& set_val, PropertyBase* target)
                :bool_val(), char_val(), int_val(), double_val(), string_val(set_val), bag_val(set_val)
            {
                target->identify(this);
            }

            void mutate( bool& set_val, PropertyBase* target)
            {
                bool_val = set_val;
                target->identify(this);
                set_val = bool();
            }

            void mutate( char& set_val, PropertyBase* target)
            {
                char_val = set_val;
                target->identify(this);
                set_val = char();
            }

            void mutate( int& set_val, PropertyBase* target)
            {
                int_val = set_val;
                target->identify(this);
                set_val = int();
            }

            void mutate( std::string& set_val, PropertyBase* target)
            {
                string_val = set_val;
                target->identify(this);
                set_val = std::string();
            }

            void mutate( double& set_val, PropertyBase* target)
            {
                double_val = set_val;
                target->identify(this);
                set_val = double();
            }

            void mutate( PropertyBag& set_val, PropertyBase* target)
            {
                bag_val = set_val;
                target->identify(this);
                set_val = bag();
            }

			virtual void introspect(const Property<bool> &v)
            {
                v.value() = bool_val;
            }
			
			virtual void introspect(const Property<char> &v)
            {
                v.value() = char_val;
            }
			
			virtual void introspect(const Property<int> &v)
            {
                v.value() = int_val;
            }
			
			virtual void introspect(const Property<double> &v)
            {
                v.value() = double_val;
            }
			
			virtual void introspect(const Property<std::string> &v)
            {
                v.value() = string_val;
            }
			
            virtual void introspect(const Property<PropertyBag> &v)
            {
                v.value() = bag_val;
            }

            template< class T >
            void introspect( const Property<T> &v )
            {
                decomposeProperty(this, v);
            }
	};

                










}

    /**
     * A helper class for introspecting a PropertyBase and assigning a value
     * to it if it matches the anticipated type. Assigns the default value
     * if the types do not match.
     */
    class IntrospectingMutator
    {
        bool bool_val;
        char char_val;
        int  int_val;
        double double_val;
        std::string string_val;
        PropertyBag bag_val;

        public:
            IntrospectingMutator()
                :bool_val(), char_val(), int_val(), double_val(), string_val(), bag_val()
            {}

            IntrospectingMutator( bool set_val, PropertyBase* target)
                :bool_val(set_val), char_val(), int_val(), double_val(), string_val(), bag_val()
            {
                target->identify(this);
            }

            IntrospectingMutator( char set_val, PropertyBase* target)
                :bool_val(), char_val(set_val), int_val(), double_val(), string_val(), bag_val()
            {
                target->identify(this);
            }

            IntrospectingMutator( int set_val, PropertyBase* target)
                :bool_val(), char_val(), int_val(set_val), double_val(), string_val(), bag_val()
            {
                target->identify(this);
            }

            IntrospectingMutator( double set_val, PropertyBase* target)
                :bool_val(), char_val(), int_val(), double_val(set_val), string_val(), bag_val()
            {
                target->identify(this);
            }

            IntrospectingMutator( const std::string& set_val, PropertyBase* target)
                :bool_val(), char_val(), int_val(), double_val(), string_val(set_val), bag_val()
            {
                target->identify(this);
            }

            IntrospectingMutator( const PropertyBag& set_val, PropertyBase* target)
                :bool_val(), char_val(), int_val(), double_val(), string_val(set_val), bag_val(set_val)
            {
                target->identify(this);
            }

            modify( bool set_val, PropertyBase* target)
            {
                bool_val = set_val;
                target->identify(this);
                set_val = bool();
            }

            modify( char set_val, PropertyBase* target)
            {
                char_val = set_val;
                target->identify(this);
                set_val = char();
            }

            modify( int set_val, PropertyBase* target)
            {
                int_val = set_val;
                target->identify(this);
                set_val = int();
            }

            modify( const std::string& set_val, PropertyBase* target)
            {
                string_val = set_val;
                target->identify(this);
                set_val = std::string();
            }

            modify( double set_val, PropertyBase* target)
            {
                double_val = set_val;
                target->identify(this);
                set_val = double();
            }

            modify( const PropertyBag& set_val, PropertyBase* target)
            {
                bag_val = set_val;
                target->identify(this);
                set_val = bag();
            }

			virtual void introspect(const Property<bool> &v)
            {
                v.value() = bool_val;
            }
			
			virtual void introspect(const Property<char> &v)
            {
                v.value() = char_val;
            }
			
			virtual void introspect(const Property<int> &v)
            {
                v.value() = int_val;
            }
			
			virtual void introspect(const Property<double> &v)
            {
                v.value() = double_val;
            }
			
			virtual void introspect(const Property<std::string> &v)
            {
                v.value() = string_val;
            }
			
            virtual void introspect(const Property<PropertyBag> &v)
            {
                v.value() = bag_val;
            }

            template< class T >
            void introspect( const Property<T> &v )
            {
                decomposeProperty(this, v);
            }
	};

                










}
#endif
