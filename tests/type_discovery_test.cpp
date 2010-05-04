#include "unit.hpp"

#include <boost/serialization/vector.hpp>
#include <boost/array.hpp>

#include <rtt-fwd.hpp>
#include <internal/DataSources.hpp>
#include <types/type_discovery.hpp>
#include <os/fosi.h>
#include <boost/lambda/lambda.hpp>

#include "datasource_fixture.hpp"
#include "types/TemplateStructInfo.hpp"
#include "types/TemplateCArrayInfo.hpp"

using namespace boost::lambda;
using namespace boost::archive;
using namespace boost::serialization;

class TypeArchiveTest
{
public:
    TypeArchiveTest() {  }
    ~TypeArchiveTest() {  }
};

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, AType & g, const unsigned int version)
{
    ar & make_nvp("a", g.a);
    ar & make_nvp("b", g.b);
    ar & make_nvp("c", g.c );
    ar & make_nvp("ai", g.ai );
    ar & make_nvp("vd", g.vd );
}

template<class Archive>
void serialize(Archive & ar, BType & g, const unsigned int version)
{
    ar & make_nvp("a", g.a);
    ar & make_nvp("b", g.b);
    ar & make_nvp("c", make_array(g.c,10) );
    ar & make_nvp("ai", make_array(g.ai,5) );
    ar & make_nvp("vd", make_array(g.vd,10) );

    //ar & make_nvp("vvi", make_array( g.vvi, 4));
}

} // namespace serialization
} // namespace boost

// Registers the fixture into the 'registry'
BOOST_FIXTURE_TEST_SUITE(  TypeArchiveTestSuite,  TypeArchiveTest )

// Test writing an AType data sample into a type data archive
BOOST_AUTO_TEST_CASE( testATypeDiscovery )
{
    AssignableDataSource<AType>::shared_ptr atype = new ValueDataSource<AType>( AType() );

    // decompose a complex type
    type_discovery out( atype );
    out.discover( atype->set() );

    BOOST_CHECK_EQUAL( out.mnames.size(), 5 );
    BOOST_CHECK_EQUAL( out.mparts.size(), 5 );
    BOOST_CHECK_EQUAL( out.mparent, atype);

    for_each( out.mnames.begin(), out.mnames.end(), cout << lambda::_1 <<", " );
    cout <<endl;

    BOOST_REQUIRE_EQUAL( out.mparts.size(), 5);

    AssignableDataSource<int>::shared_ptr a = AssignableDataSource<int>::narrow( out.mparts[0].get() );
    AssignableDataSource<double>::shared_ptr b = AssignableDataSource<double>::narrow( out.mparts[1].get() );
    AssignableDataSource<string>::shared_ptr c = AssignableDataSource<string>::narrow( out.mparts[2].get());
    AssignableDataSource<boost::array<int,5> >::shared_ptr ai = AssignableDataSource<boost::array<int,5> >::narrow( out.mparts[3].get());
    AssignableDataSource<vector<double> >::shared_ptr vd = AssignableDataSource<vector<double> >::narrow( out.mparts[4].get());

    BOOST_REQUIRE( a );
    BOOST_REQUIRE( b );
    BOOST_REQUIRE( c );
    BOOST_REQUIRE( ai );
    BOOST_REQUIRE( vd );

    // Check reading parts (must equal parent)
    BOOST_CHECK_EQUAL( a->get(), atype->get().a );
    BOOST_CHECK_EQUAL( b->get(), atype->get().b );
    BOOST_CHECK_EQUAL( c->get(), atype->get().c );
    BOOST_CHECK_EQUAL( ai->get()[3], atype->get().ai[3] );
    BOOST_CHECK_EQUAL( vd->get()[3], atype->get().vd[3] );

    // Check writing a part (must change in parent too).
    a->set(10);
    BOOST_CHECK_EQUAL( a->get(), 10 );
    BOOST_CHECK_EQUAL( a->get(), atype->get().a );

    // Check getPart function.
    BOOST_CHECK_EQUAL( out.getPart("a"), out.mparts[0] );
    BOOST_CHECK_EQUAL( out.getPart("b"), out.mparts[1] );
    BOOST_CHECK_EQUAL( out.getPart("c"), out.mparts[2] );
    BOOST_CHECK_EQUAL( out.getPart("ai"), out.mparts[3] );
    BOOST_CHECK_EQUAL( out.getPart("vd"), out.mparts[4] );
}

// Test writing a BType data sample into a type data archive
BOOST_AUTO_TEST_CASE( testBTypeDiscovery )
{
    AssignableDataSource<BType>::shared_ptr atype = new ValueDataSource<BType>( BType() );

    // decompose a complex type
    type_discovery out( atype );
    out.discover( atype->set() );

    BOOST_CHECK_EQUAL( out.mnames.size(), 5 );
    BOOST_CHECK_EQUAL( out.mparts.size(), 5 );
    BOOST_CHECK_EQUAL( out.mparent, atype);

    for_each( out.mnames.begin(), out.mnames.end(), cout << lambda::_1 <<", " );
    cout <<endl;

    BOOST_REQUIRE_EQUAL( out.mparts.size(), 5);

    AssignableDataSource<int>::shared_ptr a = AssignableDataSource<int>::narrow( out.mparts[0].get() );
    AssignableDataSource<double>::shared_ptr b = AssignableDataSource<double>::narrow( out.mparts[1].get() );
    AssignableDataSource< carray<char> >::shared_ptr c = AssignableDataSource< carray<char> >::narrow( out.mparts[2].get());
    AssignableDataSource< carray<int> >::shared_ptr ai = AssignableDataSource< carray<int> >::narrow( out.mparts[3].get());
    AssignableDataSource< carray<double> >::shared_ptr vd = AssignableDataSource< carray<double> >::narrow( out.mparts[4].get());

    BOOST_REQUIRE( a );
    BOOST_REQUIRE( b );
    BOOST_REQUIRE( c );
    BOOST_REQUIRE( ai );
    BOOST_REQUIRE( vd );

    BOOST_CHECK( !out.getPart("zort") );


    // Check reading parts (must equal parent)
    BOOST_CHECK_EQUAL( a->get(), atype->get().a );
    BOOST_CHECK_EQUAL( b->get(), atype->get().b );
    BOOST_CHECK_EQUAL( c->get().address(), atype->set().c );
    BOOST_CHECK_EQUAL( ai->get().address()[3], atype->set().ai[3] );
    BOOST_CHECK_EQUAL( vd->get().address()[3], atype->set().vd[3] );

    // Check writing a part (must change in parent too).
    a->set(10);
    BOOST_CHECK_EQUAL( a->get(), 10 );
    BOOST_CHECK_EQUAL( a->get(), atype->get().a );

    c->set().address()[3] = 'L';
    BOOST_CHECK_EQUAL( c->get().address(), "helLo" );
    BOOST_CHECK_EQUAL( c->get().address(), atype->set().c );

    // Check getPart function.
    BOOST_CHECK_EQUAL( out.getPart("a"), out.mparts[0] );
    BOOST_CHECK_EQUAL( out.getPart("b"), out.mparts[1] );
    BOOST_CHECK_EQUAL( out.getPart("c"), out.mparts[2] );
    BOOST_CHECK_EQUAL( out.getPart("ai"), out.mparts[3] );
    BOOST_CHECK_EQUAL( out.getPart("vd"), out.mparts[4] );
}

// Test the TemplateStructInfo for AType
// Similar as the above tests, but now through the TypeInfo system.
BOOST_AUTO_TEST_CASE( testATypeStruct )
{
    Types()->addType( new TemplateStructInfo<AType>("AType") );

    AssignableDataSource<AType>::shared_ptr atype = new ValueDataSource<AType>( AType() );

    BOOST_REQUIRE( Types()->type("AType") );

    // check the part names lookup:
    vector<string> names = atype->getPartNames();
    BOOST_CHECK_EQUAL( atype->getPartNames().size(), 5 );

    for_each( names.begin(), names.end(), cout << lambda::_1 <<", " );
    cout <<endl;

    BOOST_REQUIRE_EQUAL( names.size(), 5);
    BOOST_REQUIRE( atype->getPart("a") );

    // Check individual part lookup by name:
    AssignableDataSource<int>::shared_ptr a = AssignableDataSource<int>::narrow( atype->getPart("a").get() );
    AssignableDataSource<double>::shared_ptr b = AssignableDataSource<double>::narrow( atype->getPart("b").get() );
    AssignableDataSource<string>::shared_ptr c = AssignableDataSource<string>::narrow( atype->getPart("c").get());
    AssignableDataSource<boost::array<int,5> >::shared_ptr ai = AssignableDataSource<boost::array<int,5> >::narrow( atype->getPart("ai").get());
    AssignableDataSource<vector<double> >::shared_ptr vd = AssignableDataSource<vector<double> >::narrow( atype->getPart("vd").get());

    BOOST_REQUIRE( a );
    BOOST_REQUIRE( b );
    BOOST_REQUIRE( c );
    BOOST_REQUIRE( ai );
    BOOST_REQUIRE( vd );

    BOOST_CHECK( !atype->getPart("zort") );

    // Check reading parts (must equal parent)
    BOOST_CHECK_EQUAL( a->get(), atype->get().a );
    BOOST_CHECK_EQUAL( b->get(), atype->get().b );
    BOOST_CHECK_EQUAL( c->get(), atype->get().c );
    BOOST_CHECK_EQUAL( ai->get()[3], atype->get().ai[3] );
    BOOST_CHECK_EQUAL( vd->get()[3], atype->get().vd[3] );

    // Check writing a part (must change in parent too).
    a->set(10);
    BOOST_CHECK_EQUAL( a->get(), 10 );
    BOOST_CHECK_EQUAL( a->get(), atype->get().a );
}


// Test the TemplateCArrayInfo for ints
BOOST_AUTO_TEST_CASE( testCTypeArray )
{
    Types()->addType( new TemplateCArrayInfo< carray<int> >("cints") );
    int tester[3] = { 3, 2, 1 };

    AssignableDataSource< carray<int> >::shared_ptr atype = new ValueDataSource< carray<int> >( carray<int>(tester, 5) );

    BOOST_REQUIRE( Types()->type("cints") );

    // check the part names lookup:
    vector<string> names = atype->getPartNames();
    BOOST_CHECK_EQUAL( atype->getPartNames().size(), 2 ); // capacity,size

    for_each( names.begin(), names.end(), cout << lambda::_1 <<", " );
    cout <<endl;

    BOOST_REQUIRE_EQUAL( names.size(), 2);
    BOOST_REQUIRE( atype->getPart("0") );

    // Check individual part lookup by index:
    AssignableDataSource<int>::shared_ptr a0 = AssignableDataSource<int>::narrow( atype->getPart("0").get() );
    AssignableDataSource<int>::shared_ptr a1 = AssignableDataSource<int>::narrow( atype->getPart("1").get() );
    AssignableDataSource<int>::shared_ptr a2 = AssignableDataSource<int>::narrow( atype->getPart("2").get() );

    BOOST_REQUIRE( a0 );
    BOOST_REQUIRE( a1 );
    BOOST_REQUIRE( a2 );

    BOOST_CHECK( !atype->getPart("zort") );

    // Check reading parts (must equal parent)
    BOOST_CHECK_EQUAL( a0->get(), tester[0] );
    BOOST_CHECK_EQUAL( a1->get(), tester[1] );
    BOOST_CHECK_EQUAL( a2->get(), tester[2] );

    // Check writing a part (must change in parent too).
    a0->set(30);
    a1->set(20);
    a2->set(10);
    BOOST_CHECK_EQUAL( a0->get(), tester[0] );
    BOOST_CHECK_EQUAL( a1->get(), tester[1] );
    BOOST_CHECK_EQUAL( a2->get(), tester[2] );
}


BOOST_AUTO_TEST_SUITE_END()
