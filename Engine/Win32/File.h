/*! \file Engine\WIN32\File.h
\ingroup Engine
\brief Win32 File �ӿڡ�
*/
#ifndef LE_WIN32_File_H
#define LE_WIN32_File_H 1

#include <LBase/experimental/string_view.hpp> //TODO:replace it
#include "NTHandle.h"
#include <LBase/linttype.hpp>

namespace platform_ex {
	namespace Windows {
		using namespace leo::inttype;

		// ������벿���� MCF ��һ���֡�
		// �йؾ�����Ȩ˵��������� MCFLicense.txt��
		class File {
		public:
			enum : uint32 {
				// Ȩ�޿��ơ�
				kToRead = 0x00000001,
				kToWrite = 0x00000002,

				// ������Ϊ���ơ����û��ָ�� TO_WRITE����Щѡ���Ч��
				kDontCreate = 0x00000004, // �ļ���������ʧ�ܡ���ָ����ѡ���� kFailIfExists ��Ч��
				kFailIfExists = 0x00000008, // �ļ��Ѵ�����ʧ�ܡ�

				// �������Ȩ�ޡ�
				kSharedRead = 0x00000100, // �����Ȩ�ޡ����ڲ��� kToWrite �򿪵��ļ����ǿ����ġ�
				kSharedWrite = 0x00000200, // ����дȨ�ޡ�
				kSharedDelete = 0x00000400, // ����ɾ��Ȩ�ޡ�

				// ���
				kNoBuffering = 0x00010000,
				kWriteThrough = 0x00020000,
				kDeleteOnClose = 0x00040000,
				kDontTruncate = 0x00080000, // Ĭ�������ʹ�� kToWrite ���ļ�������������ݡ�
			};
		private:
			static UniqueNtHandle CreateFile(const std::experimental::wstring_view& path, uint32 flags);
		};
	}
}


#endif
