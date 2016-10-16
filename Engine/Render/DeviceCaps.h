/*! \file Engine\Render\DeviceCaps.h
\ingroup Engine
\brief ���ƴ����ӿ��ࡣ
*/
#ifndef LE_RENDER_DeviceCaps_h
#define LE_RENDER_DeviceCaps_h 1

#include "IFormat.hpp"

#include <functional>

namespace platform{
	namespace Render {
		struct Caps {
			uint16 max_texture_depth;

			std::function<bool(EFormat)> TextureFormatSupport;
			std::function<bool(EFormat)> VertexFormatSupport;
			std::function<bool(EFormat, SampleDesc)>	RenderTargetMSAASupport;
		};
	}
}

#endif