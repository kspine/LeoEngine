// ����ļ����ִ����� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��

#include "NTHandle.h"
#include <LBase/Debug.h>

#include <Windows.h>
#include <winternl.h>

namespace platform_ex {
	namespace Windows {
		template class UniqueHandle<Kernel::NtHandleCloser>;

		namespace Kernel {
			void NtHandleCloser::operator()(Handle hObject) const noexcept {
				const auto lStatus = ::NtClose(hObject);
				LAssert(NT_SUCCESS(lStatus), L"NtClose() ʧ�ܡ�");
			}
		}
	}
}