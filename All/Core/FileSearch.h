//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   Core/FileSearch.h
//  Version:     v1.00
//  Created:     11/18/2014 by leo hawke.
//  Compilers:   Visual Studio.NET 2013
//  Description: �ṩ���������ļ������߼�
// -------------------------------------------------------------------------
//  History:
//				
//
////////////////////////////////////////////////////////////////////////////

#ifndef Core_FielSearch_H
#define Core_FielSearch_H

namespace leo{
	class FileSearch{
		static void PushSearchDir();
		static void ClearSearchDirs();
		static const std::vector<std::wstring>& SearchDirectors();
	};
}

#endif
