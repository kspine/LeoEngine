////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   Core/BilaterFilter.hpp
//  Version:     v1.3
//  Created:     4/20/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: �ṩBilaterFilter��Shader����
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef Core_BilaterFilter_hpp
#define Core_BilaterFilter_hpp

#include <leoint.hpp>
#include <utility>
namespace leo {
	void CompilerBilaterCS(unsigned int radius, const wchar_t* savename);

	void CompilerBilaterCS(unsigned int radius, std::pair<uint16,uint16> size , const wchar_t* savevername /*��ֱ*/, const wchar_t* savehorname/*ˮƽ*/);

}

#endif
