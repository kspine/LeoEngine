/*! \file Core\LFile.h
\ingroup Engine
\brief �ļ���ȡ/д�� �ӿڡ�
\brief ����ϵͳ����
*/
#ifndef LE_Core_File_H
#define LE_Core_File_H 1

#include <LBase/Platform.h>

#ifdef LFL_Win32
#include "../Win32/File.h"

namespace platform {
	using platform_ex::Windows::File;
}

#endif




#endif