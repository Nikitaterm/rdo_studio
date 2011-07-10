/*
 * copyright: (c) RDO-Team, 2011
 * filename : main.cpp
 * author   : Proydakov Evgeny
 * email	: lord.tiran@gmail.com
 * date     : 16.06.2011
 * bref     : Test rdo_common smart pointer
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <vector>
#include <list>
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_common/smart_ptr/interface_ptr.h"
#include "rdo_common/rdodebug.h"
// ===============================================================================
 
OBJECT(MyClass)
{
	DECLARE_FACTORY(MyClass)
public:
	ruint m_i;

	enum { DAFAUL_VALUE = 2 };

protected:
	MyClass() : m_i(DAFAUL_VALUE) { }

	~MyClass() { }
};

OBJECT_INTERFACE(IMyClass21)
{
	virtual void ifun21() = 0;

	IMyClass21() {}
	virtual ~IMyClass21() {}
};

#define DECLARE_IMyClass21 \
public:                    \
	void ifun21();

CLASS(MyClass2):
IMPLEMENTATION_OF(IMyClass21)
AND INSTANCE_OF      (MyClass   )
{
	DECLARE_FACTORY(MyClass2)
public:
	ruint m_k;

	enum { DAFAUL_VALUE = 20 };

	rbool operator== (CREF(MyClass2) obj) const
	{
		return m_k == obj.m_k;
	}

protected:
	MyClass2() : m_k(DAFAUL_VALUE) { }

	virtual ~MyClass2() { }
	DECLARE_IMyClass21;
};
DECLARE_POINTER(MyClass2)

void MyClass2::ifun21()
{
	int i = 1;
}

OBJECT(MyClass3)
{
public:
	ruint m_i;

	enum { DAFAUL_VALUE = 3 };

	MyClass3() : m_i(DAFAUL_VALUE) { }

	~MyClass3() { }

	LPMyClass3 returnThis()
	{
		return this;
	}
};

CLASS(MyClass4): INSTANCE_OF(MyClass2)
{
	DECLARE_FACTORY(MyClass4)
public:
	ruint m_a;

	enum { DAFAUL_VALUE = 24 };

private:
	MyClass4() : m_a(DAFAUL_VALUE) { }

	virtual ~MyClass4() { }
};
DECLARE_POINTER(MyClass4)

#define BOOST_TEST_MODULE test_rdo_smart_prt
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_rdo_smart_prt)

BOOST_AUTO_TEST_CASE(creating_a_factory)
{
	BOOST_CHECK(rdo::Factory<MyClass>::create());
}

BOOST_AUTO_TEST_CASE(default_constructor)
{
	LPMyClass pMyClass = rdo::Factory<MyClass>::create();
	BOOST_CHECK(pMyClass->m_i == MyClass::DAFAUL_VALUE);
}

BOOST_AUTO_TEST_CASE(test_copy_operator)
{
	LPMyClass pMyClass_1;
	LPMyClass pMyClass_2 = rdo::Factory<MyClass>::create();

	pMyClass_1 = pMyClass_2;
	BOOST_CHECK(pMyClass_1 == pMyClass_2);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
	LPMyClass pMyClass_1 = rdo::Factory<MyClass>::create();
	LPMyClass pMyClass_2 = pMyClass_1;
	LPMyClass pMyClass_3 = rdo::Factory<MyClass>::create();

	pMyClass_1->m_i = 2 * MyClass::DAFAUL_VALUE;
	pMyClass_3->m_i = 3 * MyClass::DAFAUL_VALUE;

	BOOST_CHECK(pMyClass_1->m_i == pMyClass_2->m_i);
	BOOST_CHECK(pMyClass_1->m_i != pMyClass_3->m_i);
}

BOOST_AUTO_TEST_CASE(cast_pointer_to_rbool)
{
	LPMyClass pMyClass2 = rdo::Factory<MyClass>::create();
	BOOST_CHECK((rbool)pMyClass2);
}

BOOST_AUTO_TEST_CASE(reset_the_pointer)
{
	LPMyClass pMyClass_1;
	LPMyClass pMyClass_2 = rdo::Factory<MyClass>::create();
	LPMyClass pMyClass_3 = rdo::Factory<MyClass>::create();
	pMyClass_2 = NULL;
	BOOST_CHECK(pMyClass_2 == NULL);
	pMyClass_1 = pMyClass_3;
	BOOST_CHECK(pMyClass_1 == pMyClass_3 && pMyClass_1 != NULL);
}

BOOST_AUTO_TEST_CASE(creating_a_pointer_type_not_just_from_the_factory)
{
	PTR(MyClass3) pMyClass3_1 = new MyClass3();
	BOOST_CHECK(pMyClass3_1);
	LPMyClass3 pMyClass3_2(pMyClass3_1);
	pMyClass3_1->m_i = 3 * MyClass3::DAFAUL_VALUE;
	BOOST_CHECK(pMyClass3_1->m_i == pMyClass3_2->m_i);
}

BOOST_AUTO_TEST_CASE(create_a_pointer_to_this)
{
	PTR(MyClass3) pMyClass3_1 = new MyClass3();
	LPMyClass3 pMyClass3_2 = pMyClass3_1->returnThis();
	pMyClass3_1->m_i = 2 * MyClass3::DAFAUL_VALUE;
	BOOST_CHECK(pMyClass3_1->m_i == pMyClass3_2->m_i);
}

BOOST_AUTO_TEST_CASE(casting_to_an_ancestor)
{
	// TODO
	LPMyClass2 pMyClass_1 = rdo::Factory<MyClass2>::create();
	pMyClass_1->m_i = 10;
	pMyClass_1->m_k = 22;
	LPMyClass  pMyClass_2 = pMyClass_1.object_static_cast<MyClass>();
	pMyClass_1->m_i = 11;
}

BOOST_AUTO_TEST_CASE(casting_to_the_interface)
{
	// TODO
	LPMyClass2 pMyClass_2 = rdo::Factory<MyClass2>::create();
	LPIMyClass21 pMyClass_21 = pMyClass_2.interface_cast<IMyClass21>();
	pMyClass_2 = NULL;
	pMyClass_21 = LPIMyClass21();
}

BOOST_AUTO_TEST_CASE(Casting_to_the_interface_through_the_child)
{
	// TODO
	LPMyClass4 pMyClass_4 = rdo::Factory<MyClass4>::create();
	LPIMyClass21 pMyClass_21 = pMyClass_4.interface_cast<IMyClass21>();
	pMyClass_4 = NULL;
	pMyClass_21 = LPIMyClass21();
}

BOOST_AUTO_TEST_CASE(comparing_pointers)
{
	LPMyClass2 pMyClass_1 = rdo::Factory<MyClass2>::create();
	LPMyClass  pMyClass_2 = pMyClass_1.object_static_cast<MyClass>();
	BOOST_CHECK(pMyClass_1 == pMyClass_2);
	BOOST_CHECK(!(pMyClass_1 != pMyClass_2));

	LPMyClass3 pMyClass_3 = rdo::Factory<MyClass3>::create();
	BOOST_CHECK(pMyClass_1 != pMyClass_3);
	BOOST_CHECK(!(pMyClass_1 == pMyClass_3));
}

BOOST_AUTO_TEST_CASE(comparison_of_the_values)
{
	LPMyClass2 pMyClass_1 = rdo::Factory<MyClass2>::create();
	LPMyClass2 pMyClass_2 = rdo::Factory<MyClass2>::create();
	BOOST_CHECK(pMyClass_1.compare(pMyClass_2));
	pMyClass_2->m_k++;
	BOOST_CHECK(!pMyClass_1.compare(pMyClass_2));
}

BOOST_AUTO_TEST_SUITE_END()