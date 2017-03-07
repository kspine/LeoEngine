/*! \file Engine\Render\D3D12\Convert.h
\ingroup Engine
\brief ���ϲ�ת����D3D12�㡣
*/

#ifndef LE_RENDER_D3D12_Convert_h
#define LE_RENDER_D3D12_Convert_h 1

#include "../IFormat.hpp"
#include "../PipleState.h"
#include "../InputLayout.hpp"
#include <dxgiformat.h>
#include "d3d12_dxgi.h"

namespace platform_ex {
	namespace Windows {
		namespace D3D12 {
			DXGI_FORMAT Convert(platform::Render::EFormat format);
			platform::Render::EFormat Convert(DXGI_FORMAT format);

			D3D12_SAMPLER_DESC Convert(platform::Render::SamplerDesc desc);

			D3D12_TEXTURE_ADDRESS_MODE Convert(platform::Render::TexAddressingMode mode);
			D3D12_FILTER Convert(platform::Render::TexFilterOp op);
			D3D12_COMPARISON_FUNC Convert(platform::Render::CompareOp op);

			std::vector<D3D12_INPUT_ELEMENT_DESC> Convert(const platform::Render::Vertex::Stream& stream);
		}
	}
}

#endif