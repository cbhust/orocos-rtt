/***************************************************************************
  tag: Peter Soetens  Mon Jan 19 14:11:20 CET 2004  XMLRPCDemarshaller.hpp 

                        XMLRPCDemarshaller.hpp -  description
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
 
#ifndef PI_PROPERTIES_XMLRPCDESERIALIZER
#define PI_PROPERTIES_XMLRPCDESERIALIZER

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <iostream>
//#include <rtstl/rtstreams.hpp>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <corelib/Property.hpp>
#include "StreamProcessor.hpp"
#include <corelib/Marshaller.hpp>

namespace ORO_CoreLib
{
    //using namespace rt_std; // Should be removed soon
    using std::cerr;
    using std::endl;
    
#ifdef XERCES_CPP_NAMESPACE
    using namespace XERCES_CPP_NAMESPACE;
#endif

    class SAX2XMLRPCHandler : public DefaultHandler
    {

            /**
             * Stores the results of the parsing.
             */
            PropertyBag &bag;
            enum State { STATE_NAME, STATE_BOOLEAN, STATE_CHAR, STATE_INT, STATE_DOUBLE, STATE_STRING, STATE_PROPERTIES};
			std::stack<State> state_stack;

        public:

            SAX2XMLRPCHandler( PropertyBag &b ) : bag( b )
            {}

            void endElement( const XMLCh* const uri,
                             const XMLCh* const localname,
                             const XMLCh* const qname )
            {
                char *ln = XMLString::transcode( localname );
                if ( !strcmp( ln, "xmlrpc" ) )
                {
                    state_stack.pop();
                }
                else
                    if ( !strcmp( ln, "boolean" ) )
                    {
						//bool v;	
						std::stringstream buffer;
						buffer << value.sv;

                        bag.add( new Property<bool>( name, "", true ) );
								
                        state_stack.pop();
                    }
                    else
                        if ( !strcmp( ln, "char" ) )
                        {
                                    bag.add( new Property<char>( name, "", 'Z' ) );
                                    state_stack.pop();
                        }
                        else
                            if ( !strcmp( ln, "int" ) )
                            {
                                        bag.add( new Property<int>( name, "", 1234 ) );
                                        state_stack.pop();
                            }
                            else
                                if ( !strcmp( ln, "double" ) )
                                {
                                            bag.add( new Property<double>( name, "", 6.789 ) );
                                            state_stack.pop();
                                }
                                else
                                    if ( !strcmp( ln, "string" ) )
                                    {
                                                bag.add( new Property<std::string>( name, "", value.sv ) );
                                                state_stack.pop();
                                    }
                                    else
                                        if ( !strcmp( ln, "name" ) )
                                        {
                                            state_stack.pop();
                                        }
            }

            void startElement( const XMLCh* const uri,
                               const XMLCh* const localname,
                               const XMLCh* const qname,
                               const Attributes& attributes )
            {
                char *ln = XMLString::transcode( localname );
                if ( !strcmp( ln, "boolean" ) )
                    state_stack.push( STATE_BOOLEAN );
                else
                    if ( !strcmp( ln, "char" ) )
                        state_stack.push( STATE_CHAR );
                    else
                        if ( !strcmp( ln, "int" ) )
                            state_stack.push( STATE_INT );
                        else
                            if ( !strcmp( ln, "double" ) )
                                state_stack.push( STATE_DOUBLE );
                            else
                                if ( !strcmp( ln, "string" ) )
                                    state_stack.push( STATE_STRING );
                                else
                                    if ( !strcmp( ln, "xmlrpc" ) )
                                        state_stack.push( STATE_PROPERTIES );
                                    else
                                        if ( !strcmp( ln, "name" ) )
                                            state_stack.push( STATE_NAME );
            }
#if 0
            void warning( const SAXParseException& exception )
            {
                cerr << "\nWarning\n";
            }
            void error( const SAXParseException& exception )
            {
                cerr << "\nError\n";
            }
            void fatalError( const SAXParseException& exception )
            {
                cerr << "\nFatal error\n";
            }
#endif
            void characters( const XMLCh* const chars, const unsigned int length )
            {
				char* string_value;
                switch ( state_stack.top() )
                {
                    case STATE_NAME:
                        name = XMLString::transcode( chars );
                        break;

                    case STATE_STRING:
                        value.sv = XMLString::transcode( chars );
                        break;
                        
					case STATE_BOOLEAN:
                        string_value = XMLString::transcode( chars );
							
                            break;
                    }
            }


            /**
             * The name of the property.
             */
            char* name;

            /**
             * The value of the property.
             */
            union {
                int iv;
                char *sv;
            } value;

    };


    template <typename input_stream>
    class XMLRPCDemarshaller : public Demarshaller, public StreamProcessor<input_stream>
    {
        public:
            XMLRPCDemarshaller( input_stream &is ) :
                    StreamProcessor<input_stream>( is )
            {}

            virtual bool deserialize( PropertyBag &v )
            {
                try
                {
                    XMLPlatformUtils::Initialize();
                }
                catch ( const XMLException & toCatch )
                {
                    cerr << "Error during initialization! :\n" /*<< StrX(toCatch.getMessage())*/ << endl;
                }
                catch ( ... )
                {
                    cerr << "other exception" << endl;
                }

                SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
                //char *xmlFile = "test.xml";
                int errorCount = 0;
                try
                {
                    SAX2XMLRPCHandler handler( v );
                    parser->setContentHandler( &handler );
                    parser->setErrorHandler( &handler );
                    //        parser->setFeature( XMLUni::fgXercesSchemaFullChecking, false );
                    //      parser->setFeature( XMLUni::fgSAX2CoreValidation, false );
                    //    parser->setFeature( XMLUni::fgXercesDynamic, false );
                    //      parser->setFeature( XMLUni::fgSAX2CoreNameSpaces, false );
                    //    parser->setFeature( XMLUni::fgSAX2CoreNameSpacePrefixes, false );
                    //  parser->setFeature( XMLUni::fgXercesSchema, false );

                    //parser->parse( xmlFile );
                    parser->parse( *(this->s) );
                    errorCount = parser->getErrorCount();
                }
                catch ( const XMLException & toCatch )
                {
                    cerr << "\nAn XML parsing error occurred\n  Error: " << endl;
                    XMLPlatformUtils::Terminate();
                    return 4;
                }
                catch ( ... )
                {
                    cerr << "General error" << endl;
                    XMLPlatformUtils::Terminate();
                    return false;
                }
                return true;
            }

    };
}
#endif
