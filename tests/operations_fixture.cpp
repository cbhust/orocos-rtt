#include "operations_fixture.hpp"
#include <iostream>
#include <TaskContext.hpp>
#include <Method.hpp>
#include <Operation.hpp>
#include <interface/ServiceProvider.hpp>

using namespace std;
using namespace boost;
using namespace RTT;
using namespace RTT::detail;

OperationsFixture::OperationsFixture()
{
    tc = new TaskContext("root");
    tc->addService(this->createMethodFactory());
    tc->addAlias("ret", ret );
    caller = new TaskContext("caller");
    caller->start();
    tc->start();
}

OperationsFixture::~OperationsFixture()
{
    tc->stop();
    delete tc;
    delete caller;
}

ServiceProvider* OperationsFixture::createMethodFactory()
{
    ServiceProvider* to = new ServiceProvider("methods");

    to->addOperation("assert", &OperationsFixture::assertBool, this).doc("assert").arg("b", "bd");

    // ClientThread
    to->addOperation("m0r", &OperationsFixture::m0r, this).doc("M0r");
    to->addOperation("m0cr", &OperationsFixture::m0cr, this).doc("M0cr");
    to->addOperation("m1r", &OperationsFixture::m1r, this).doc("M1r");
    to->addOperation("m1cr", &OperationsFixture::m1cr, this).doc("M1cr");

    to->addOperation("m0", &OperationsFixture::m0, this).doc("M0");
    to->addOperation("m1", &OperationsFixture::m1, this).doc("M1").arg("a", "ad");
    to->addOperation("m2", &OperationsFixture::m2, this).doc("M2").arg("a", "ad").arg("a", "ad");
    to->addOperation("m3", &OperationsFixture::m3, this).doc("M3").arg("a", "ad").arg("a", "ad").arg("a", "ad");
    to->addOperation("m4", &OperationsFixture::m4, this).doc("M4").arg("a", "ad").arg("a", "ad").arg("a", "ad").arg("a", "ad");

    // OwnThread
    to->addOperation("o0r", &OperationsFixture::m0r, this, OwnThread).doc("M0r");
    to->addOperation("o0cr", &OperationsFixture::m0cr, this, OwnThread).doc("M0cr");
    to->addOperation("o1r", &OperationsFixture::m1r, this, OwnThread).doc("M1r");
    to->addOperation("o1cr", &OperationsFixture::m1cr, this, OwnThread).doc("M1cr");

    to->addOperation("o0", &OperationsFixture::m0, this, OwnThread).doc("M0");
    to->addOperation("o1", &OperationsFixture::m1, this, OwnThread).doc("M1").arg("a", "ad");
    to->addOperation("o2", &OperationsFixture::m2, this, OwnThread).doc("M2").arg("a", "ad").arg("a", "ad");
    to->addOperation("o3", &OperationsFixture::m3, this, OwnThread).doc("M3").arg("a", "ad").arg("a", "ad").arg("a", "ad");
    to->addOperation("o4", &OperationsFixture::m4, this, OwnThread).doc("M4").arg("a", "ad").arg("a", "ad").arg("a", "ad").arg("a", "ad");
    return to;
}