/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_fuzzy.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     Нечеткая логика
  \indent    4T
*/

#ifndef _LIB_RUNTIME_FUZZY_H_
#define _LIB_RUNTIME_FUZZY_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <boost/icl/interval.hpp>
#include <boost/icl/interval_set.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(FuzzySet);
PREDECLARE_POINTER(DefineArea);
//! Область определения

OBJECT(DefineArea)
{
DECLARE_FACTORY(DefineArea);
public:
	typedef  boost::icl::interval<RDOValue> DomainPart;
	typedef  boost::icl::interval<RDOValue>::type IntervalType;
	typedef  boost::icl::interval_set<RDOValue> Domain;
	
	DefineArea (); // типа бесконечность
	virtual ~DefineArea();
private:
	DefineArea(CREF(RDOValue));// 1 element
	DefineArea(CREF(RDOValue) leftBorder, CREF(RDOValue) rightBorder); //;

	Domain m_domain;
};

//! Нечеткое множество
OBJECT(FuzzySet)
{
DECLARE_FACTORY(FuzzySet);
public:
	typedef  std::pair<RDOValue, double>                              FuzzyItem;
	typedef  std::map<FuzzyItem::first_type, FuzzyItem::second_type>  FuzzySetDefinition;

	LPFuzzySet                                append     (CREF(RDOValue) rdovalue, double appertain); // вставка с проверкой значения, если такое же есть - не вставит
	LPFuzzySet                                operator() (CREF(RDOValue) rdovalue, double appertain);
	REF(double)                               operator[] (CREF(RDOValue) rdovalue); // вставка без проверки значения
	FuzzySetDefinition::const_iterator        find       (CREF(RDOValue) rdovalue) const;
	FuzzyItem                                 findValue  (CREF(RDOValue) rdovalue) const;
	FuzzySetDefinition::const_iterator        begin      () const;
	FuzzySetDefinition::const_iterator        end        () const;
	FuzzySetDefinition::iterator              begin      ();
	FuzzySetDefinition::iterator              end        ();
	LPFuzzySet                                clone      () const;
	rbool                                     empty      () const;
	rbool                                     inRange    (CREF(RDOValue) rdovalue);
	void                                      setValues  (CREF(FuzzySetDefinition) values);

	            LPFuzzySet operator&& (CREF(LPFuzzySet) pFuzzyValue) const;
	            LPFuzzySet operator|| (CREF(LPFuzzySet) pFuzzyValue) const;
	/* 3.102*/  LPFuzzySet operator+  (CREF(LPFuzzySet) pFuzzyValue) const;
	/* 3.104*/  LPFuzzySet operator-  (CREF(LPFuzzySet) pFuzzyValue) const;
	/* 3.106*/  LPFuzzySet operator*  (CREF(LPFuzzySet) pFuzzyValue) const;
	/* 3.108*/  LPFuzzySet operator/  (CREF(LPFuzzySet) pFuzzyValue) const;


	tstring getAsString() const;
private:
	FuzzySet();
	FuzzySet(CREF(LPDefineArea) pDefineArea);
	FuzzySet(CREF(LPFuzzySet) pSet); // KK

	virtual ~FuzzySet();

	FuzzySetDefinition  m_fuzzySet;
	LPDefineArea        m_defineArea;
};

PREDECLARE_POINTER(RDOValue);
PREDECLARE_POINTER(RDOLingvoVariable);

//! Свойства НМ:
class MemberFunctionProperties
{
public:
	typedef RDOValue (*ExtUnaryFun )(CREF(RDOValue) rdovalue);
	typedef RDOValue (*ExtUnaryFunP)(CREF(RDOValue) rdovalue, PTR(void) pParam);
	typedef RDOValue (*ExtBinaryFun)(CREF(RDOValue) rdovalue1, CREF(RDOValue) rdovalue2);

	/* 3.116*/  static LPFuzzySet u_minus(CREF(LPFuzzySet) pSet);
	/* 3.117*/  static LPFuzzySet u_obr  (CREF(LPFuzzySet) pSet);
	/* 3.118*/  static LPFuzzySet u_scale(CREF(LPFuzzySet) pSet, double scale);
	/* 3.119*/  static LPFuzzySet u_log  (CREF(LPFuzzySet) pSet);

	/* 3.39 */  static LPFuzzySet a_mult    (CREF(LPFuzzySet) pSet1, CREF(LPFuzzySet) pSet2); // не работает пока
	/* 3.48 */  static LPFuzzySet alpha     (CREF(LPFuzzySet) pSet,  double appertain);
	/* 3.62 */  static LPFuzzySet supplement(CREF(LPFuzzySet) pSet);
	/* 3.78 */  static LPFuzzySet a_con     (CREF(LPFuzzySet) pSet);
	/* 3.79 */  static LPFuzzySet a_dil     (CREF(LPFuzzySet) pSet);

				static LPRDOLingvoVariable fuzzyfication  (CREF(RDOValue), CREF(LPRDOLingvoVariable));
	/* 3.272*/  static RDOValue            defuzzyfication(CREF(LPFuzzySet) pSet);

	            static LPFuzzySet a_pow     (LPFuzzySet pSet, double power);
	/* 3.114*/  static LPFuzzySet ext_unary (ExtUnaryFun  fun, CREF(LPFuzzySet) pSet);
	/* 3.114*/  static LPFuzzySet ext_unary (ExtUnaryFunP fun, PTR(void) pParam, CREF(LPFuzzySet) pSet);
	/* 3.83 */  static LPFuzzySet ext_binary(ExtBinaryFun fun, CREF(LPFuzzySet) pSet1, CREF(LPFuzzySet) pSet2);

};
// ! Терм

OBJECT (RDOFuzzyTerm)
{
DECLARE_FACTORY (RDOFuzzyTerm)
public:
	typedef tstring  termName;
	typedef std::pair<termName,LPFuzzySet> Term;

	CREF(LPFuzzySet)  getFuzzySetDefinition() const;
	termName          getName              () const;

private:
	RDOFuzzyTerm(CREF(termName), CREF(LPFuzzySet));
	virtual ~RDOFuzzyTerm();

	Term m_term;
};

// !Лингвистическая переменная

OBJECT (RDOLingvoVariable)
{
DECLARE_FACTORY(RDOLingvoVariable)
public:
	typedef std::map<RDOFuzzyTerm::Term::first_type, RDOFuzzyTerm::Term::second_type> TermSet;
	typedef tstring nameOfVariable;

	TermSet::const_iterator            begin () const;
	TermSet::const_iterator            end   () const;

	REF(LPFuzzySet)                    operator[] (tstring name);
	
	void                               setName      (nameOfVariable);
	nameOfVariable                     getName      () {return m_name;};
	void                               append       (tstring name,CREF(LPFuzzySet) fuzzySet);

private:
	TermSet        m_set;
	nameOfVariable m_name;

	RDOLingvoVariable(CREF(LPRDOFuzzyTerm) term, nameOfVariable nameOfVariable);
	RDOLingvoVariable(CREF(RDOValue) pDefineAreaValue, CREF(LPRDOLingvoVariable) variable);
	RDOLingvoVariable(CREF(RDOLingvoVariable)); // KK
	virtual ~RDOLingvoVariable();
};

OBJECT (Statement)
{
DECLARE_FACTORY(Statement)
public:
	void setTerm(LPRDOFuzzyTerm term);
	void setVariable(LPRDOLingvoVariable variable);

private:
	LPRDOLingvoVariable m_variable;
	LPRDOFuzzyTerm      m_term;
	Statement();
	~Statement();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_fuzzy.inl"

#endif // _LIB_RUNTIME_FUZZY_H_
