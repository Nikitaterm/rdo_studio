/*!
  \copyright (c) RDO-Team, 2012
  \file      main.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      13.05.2012
  \brief     Тест точности вывода дробных чисел
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDORealFormatTest
#include "utils/src/common/warning_disable.h"
#include <iostream>
#include <boost/test/included/unit_test.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDORealFormatTest)

BOOST_AUTO_TEST_CASE(MantissaPrecisionSet)
{
#if defined(COMPILER_VISUAL_STUDIO) or defined(COMPILER_MINGW)
	_set_output_format(_TWO_DIGIT_EXPONENT);
#endif
}

BOOST_AUTO_TEST_CASE(MantissaPrecision2)
{
	double value = 10e+37;
	std::stringstream stream;

	stream << value;

	BOOST_CHECK(stream.str() == "1e+38");
}

BOOST_AUTO_TEST_CASE(MantissaPrecision3)
{
	double value = 10e+237;
	std::stringstream stream;

	stream << value;

	BOOST_CHECK(stream.str() == "1e+238");
}

BOOST_AUTO_TEST_SUITE_END() // RDORealFormatTest
