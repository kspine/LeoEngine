// ����ļ��� MCF ��һ���֡�
// �йؾ�����Ȩ˵��������� MCFLicense.txt��
// Copyleft 2014. LH_Mouse. All wrongs reserved.

#ifndef __MCF_CRT_OFFSET_OF_H__
#define __MCF_CRT_OFFSET_OF_H__

#include "../../env/_crtdef.h"

#define OFFSET_OF(s, m)	\
	((__MCF_STD size_t)((__MCF_STD uintptr_t)&(((const s *)(__MCF_STD uintptr_t)1)->m) - (__MCF_STD uintptr_t)1))

#endif
