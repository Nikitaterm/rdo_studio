/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      ui/abstract/headers/memdc/memdc.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.07.2012
  \brief     Абстрактный DeviceContext, созданный в памяти
  \indent    4T
*/

#ifndef _UI_ABSTRACT_MEMDC_H_
#define _UI_ABSTRACT_MEMDC_H_

// ----------------------------------------------------------------------- INCLUDES
#include <memory>
#include <boost/config.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdotypes.h"
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace gui
	{
		class MemDCBase
		{
		public:
			 MemDCBase();
			~MemDCBase();

			ruint     width () const;
			ruint     height() const;

		protected:
			ruint m_width;
			ruint m_height;
		};

		template <class TDC, class TBMP>
		class MemDC: public MemDCBase
		{
		public:
			 MemDC();
			~MemDC();

			rbool valid () const;
			rbool create(ruint width, ruint height);
			rbool create(ruint width, ruint height, TDC& from);
			rbool resize(ruint width, ruint height);

			TDC&  dc    ();
			TBMP& buffer();

		private:
#ifdef BOOST_NO_CXX11_SMART_PTR
			std::auto_ptr<TDC>     m_pDC;
			std::auto_ptr<TBMP>    m_pBitmap;
#else
			std::unique_ptr<TDC>   m_pDC;
			std::unique_ptr<TBMP>  m_pBitmap;
#endif
			rbool onCreate();

			void clear();
		};

	} // namespace gui
} // namespace rdo

#include "ui/abstract/headers/memdc/memdc.inl"

#endif // _UI_ABSTRACT_MEMDC_H_
