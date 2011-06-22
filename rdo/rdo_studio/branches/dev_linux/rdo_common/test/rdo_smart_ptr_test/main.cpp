/*
 * copyright: (c) RDO-Team, 2009
 * filename : main.cpp
 * author   : ������ ������
 * date     : 14.07.2009
 * bref     : ���� ����� ����������
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

protected:
	MyClass()
		: m_i(2)
	{
		int i = 1;
	}

	~MyClass()
	{
		int i = 1;
	}
};

OBJECT_INTERFACE(IMyClass21)
{
	virtual void ifun21() = 0;

	IMyClass21()
	{}
	virtual ~IMyClass21()
	{}
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

	rbool operator== (CREF(MyClass2) obj) const
	{
		return m_k == obj.m_k;
	}

protected:
	MyClass2()
		: m_k(20)
	{
		int i = 1;
	}

	virtual ~MyClass2()
	{
		int i = 1;
	}
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

	MyClass3()
		: m_i(3)
	{
		int i = 1;
	}

	~MyClass3()
	{
		int i = 1;
	}

	LPMyClass3 fun()
	{
		return this;
	}
};

CLASS(MyClass4): INSTANCE_OF(MyClass2)
{
DECLARE_FACTORY(MyClass4)
public:
	ruint m_a;

private:
	MyClass4()
		: m_a(24)
	{
		int i = 1;
	}

	virtual ~MyClass4()
	{
		int i = 1;
	}
};
DECLARE_POINTER(MyClass4)

void main()
{
	ruint size1 = sizeof(MyClass);
	ruint size2 = sizeof(LPMyClass);

	{ //! �������� ����� ������� � ���� ����������� �����

		LPMyClass pMyClass = rdo::Factory<MyClass>::create();
		{
			LPMyClass pMyClass2 = pMyClass;
		}
		pMyClass->m_i = 3;
	}

	{ //! ����������� ��-���������, ���������� ��������� � rbool, �������� �����������, ����� ���������

		LPMyClass pMyClass1;
		ASSERT(!pMyClass1);
		LPMyClass pMyClass2 = rdo::Factory<MyClass>::create();
		ASSERT(pMyClass2);
		pMyClass1 = pMyClass2;
		ASSERT(pMyClass1);

		LPMyClass pMyClass3 = rdo::Factory<MyClass>::create();
		pMyClass2 = NULL;
		pMyClass1 = pMyClass3;
	}

	{ //! �������� ��������� �� ���� (�� ������ �� �������), �������� ��������� �� this
		PTR(MyClass3) pMyClass3 = new MyClass3();
		LPMyClass3    pMyClass4(pMyClass3);
		LPMyClass3    pMyClass5 = pMyClass4->fun();
	}

	{ //! ������� � ������

		LPMyClass2 pMyClass2 = rdo::Factory<MyClass2>::create();
		pMyClass2->m_i = 10;
		pMyClass2->m_k = 22;
		LPMyClass  pMyClass = pMyClass2.object_static_cast<MyClass>();
		pMyClass->m_i = 11;
	}

	{ //! ������� � ����������

		LPMyClass2 pMyClass2 = rdo::Factory<MyClass2>::create();
		LPIMyClass21 pMyClass21 = pMyClass2.interface_cast<IMyClass21>();
		pMyClass2 = NULL;
		pMyClass21 = LPIMyClass21();
	}

	{ //! ������� � ���������� ����� �������, ���� ��������� ������� � ������

		LPMyClass4 pMyClass4 = rdo::Factory<MyClass4>::create();
		LPIMyClass21 pMyClass21 = pMyClass4.interface_cast<IMyClass21>();
		pMyClass4 = NULL;
		pMyClass21 = LPIMyClass21();
	}

	{ //! ��������� ����������

		LPMyClass2 pMyClass1 = rdo::Factory<MyClass2>::create();
		LPMyClass  pMyClass2 = pMyClass1.object_static_cast<MyClass>();
		ASSERT(pMyClass1 == pMyClass2);
		ASSERT(!(pMyClass1 != pMyClass2));

		LPMyClass3 pMyClass3 = rdo::Factory<MyClass3>::create();
		ASSERT(pMyClass1 != pMyClass3);
		ASSERT(!(pMyClass1 == pMyClass3));
	}

	{ //! ��������� ��������

		LPMyClass2 pMyClass1 = rdo::Factory<MyClass2>::create();
		LPMyClass2 pMyClass2 = rdo::Factory<MyClass2>::create();
		ASSERT(pMyClass1.compare(pMyClass2));
		pMyClass2->m_k++;
		ASSERT(!pMyClass1.compare(pMyClass2));
	}

	int i = 1;
}
