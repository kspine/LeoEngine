/*! \file Engine\Render\D3D12\GraphicsBuffer.hpp
\ingroup Engine
\brief D3D12 Buffer�ӿ��ࡣ
*/
#ifndef LE_RENDER_D3D12_GraphicsBuffer_hpp
#define LE_RENDER_D3D12_GraphicsBuffer_hpp 1

#include "../IFormat.hpp"
#include "../IGraphicsBuffer.hpp"
#include "RenderView.h"
#include "d3d12_dxgi.h"
#include "../../Win32/COM.h"

namespace platform_ex::Windows::D3D12 {

	class GraphicsBuffer : public platform::Render::GraphicsBuffer {
	public:
		GraphicsBuffer(platform::Render::Buffer::Usage usage, uint32_t access_hint,
			uint32_t size_in_byte,platform::Render::EFormat fmt);
		virtual ~GraphicsBuffer();

		void CopyToBuffer(platform::Render::GraphicsBuffer& rhs) override;

		void HWResourceCreate(void const * init_data) override;
		void HWResourceDelete() override;

		void UpdateSubresource(leo::uint32 offset, leo::uint32 size, void const * data) override;

		ID3D12Resource* Resource() const;
		ID3D12Resource* UploadResource() const;

		ViewSimulation* RetriveShaderResourceView();
		ViewSimulation* RetriveUnorderedAccessView();
	private:
		void* Map(platform::Render::Buffer::Access ba) override;
		void Unmap() override;

		friend class Device;
		friend class Context;

		std::unique_ptr<ViewSimulation> srv;
		std::unique_ptr<ViewSimulation> uav;
		
		COMPtr<ID3D12Resource> buffer;

		COMPtr<ID3D12Resource> buffer_counter_upload;
		leo::uint32 counter_offset;

		platform::Render::EFormat format;
	};
}

#endif