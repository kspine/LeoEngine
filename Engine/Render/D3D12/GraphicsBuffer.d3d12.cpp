#include "GraphicsBuffer.hpp"
#include "Context.h"
#include "Convert.h"

namespace platform_ex::Windows::D3D12 {
	using namespace platform::Render::Buffer;
	GraphicsBuffer::GraphicsBuffer(platform::Render::Buffer::Usage usage,
		uint32_t access_hint, uint32_t size_in_byte,
		platform::Render::EFormat fmt)
		:platform::Render::GraphicsBuffer(usage, access_hint, size_in_byte),format(fmt)
	{
	}

	GraphicsBuffer::~GraphicsBuffer() {
		Context::Instance().SyncCPUGPU();
	}

	void GraphicsBuffer::CopyToBuffer(platform::Render::GraphicsBuffer & rhs_)
	{
		auto& rhs = static_cast<GraphicsBuffer&>(rhs_);

		D3D12_RESOURCE_BARRIER src_barrier_before, dst_barrier_before;
		src_barrier_before.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		src_barrier_before.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		D3D12_HEAP_TYPE src_heap_type;

		if (access == EAccessHint::EA_CPURead) {
			src_barrier_before.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			src_heap_type = D3D12_HEAP_TYPE_READBACK;
		}
		else if ((rhs.GetAccess() & EAccessHint::EA_CPURead) || (rhs.GetAccess() & EAccessHint::EA_CPUWrite)) {
			src_barrier_before.Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
			src_heap_type = D3D12_HEAP_TYPE_UPLOAD;
		}
		else {
			src_barrier_before.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
			src_heap_type = D3D12_HEAP_TYPE_DEFAULT;
		}
		src_barrier_before.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
		src_barrier_before.Transition.pResource = buffer.Get();
		src_barrier_before.Transition.Subresource = 0;

		dst_barrier_before.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		dst_barrier_before.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		D3D12_HEAP_TYPE dst_heap_type;
		if (rhs.GetAccess() == EAccessHint::EA_CPURead) {
			src_barrier_before.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			src_heap_type = D3D12_HEAP_TYPE_READBACK;
		}
		else if ((rhs.GetAccess() & EAccessHint::EA_CPURead) || (rhs.GetAccess() & EAccessHint::EA_CPUWrite)) {
			dst_barrier_before.Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
			dst_heap_type = D3D12_HEAP_TYPE_UPLOAD;
		}
		else {
			dst_barrier_before.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
			dst_heap_type = D3D12_HEAP_TYPE_DEFAULT;
		}
		dst_barrier_before.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
		dst_barrier_before.Transition.pResource = rhs.buffer.Get();
		dst_barrier_before.Transition.Subresource = 0;

		int n = 0;
		D3D12_RESOURCE_BARRIER barrier_before[2];
		if (src_heap_type != dst_heap_type) {
			if (D3D12_HEAP_TYPE_DEFAULT == src_heap_type)
				barrier_before[n++] = src_barrier_before;
			if (D3D12_HEAP_TYPE_DEFAULT == dst_heap_type)
				barrier_before[n++] = dst_barrier_before;
		}

		auto cmd_list = Context::Instance().GetCommandList(Device::Command_Resource);
		if (n > 0)
			cmd_list->ResourceBarrier(n,barrier_before);

		cmd_list->CopyBufferRegion(rhs.buffer.Get(), 0, buffer.Get(), 0, size_in_byte);

		D3D12_RESOURCE_BARRIER barrier_after[2] = { barrier_before[0],barrier_before[1] };
		std::swap(barrier_after[0].Transition.StateBefore, barrier_after[0].Transition.StateAfter);
		std::swap(barrier_after[1].Transition.StateBefore, barrier_after[1].Transition.StateAfter);
		
		if (n > 0)
			cmd_list->ResourceBarrier(n, barrier_after);
	}

	void GraphicsBuffer::HWResourceCreate(void const * init_data)
	{
		auto& device = Context::Instance().GetDevice();

		D3D12_RESOURCE_STATES init_state;
		D3D12_HEAP_PROPERTIES heap_prop;
		if (EAccessHint::EA_CPURead == access) {
			init_state = D3D12_RESOURCE_STATE_COPY_DEST;
			heap_prop.Type = D3D12_HEAP_TYPE_READBACK;
		}
		else if ((0 == access) || (access & EAccessHint::EA_CPURead) || (access & EAccessHint::EA_CPUWrite))
		{
			init_state = D3D12_RESOURCE_STATE_GENERIC_READ;
			heap_prop.Type = D3D12_HEAP_TYPE_UPLOAD;
		}
		else
		{
			init_state = D3D12_RESOURCE_STATE_COMMON;
			heap_prop.Type = D3D12_HEAP_TYPE_DEFAULT;
		}

		heap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap_prop.CreationNodeMask = 0;
		heap_prop.VisibleNodeMask = 0;

		uint32_t total_size = size_in_byte;
		if ((access &EAccessHint::EA_GPUWrite)
			&& !(
			/*(access & EAccessHint::EA_GPUStructured) || */
			(access & EAccessHint::EA_GPUUnordered)))
		{
			total_size = ((size_in_byte + 4 - 1) & ~(4 - 1)) + sizeof(uint64_t);
		}
		else if ((access & EAccessHint::EA_GPUUnordered) && (format != EF_Unknown)
			&&
#if 0
			((access & EAccessHint::EA_Append) || (access & EAccessHint::EA_Counter))
#else 
			false
#endif
			)
		{
			total_size = ((size_in_byte + D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - 1) & ~(D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - 1))
				+ sizeof(uint64_t);
		}

		D3D12_RESOURCE_DESC res_desc;
		res_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		res_desc.Alignment = 0;
		res_desc.Width = total_size;
		res_desc.Height = 1;
		res_desc.DepthOrArraySize = 1;
		res_desc.MipLevels = 1;
		res_desc.Format = DXGI_FORMAT_UNKNOWN;
		res_desc.SampleDesc.Count = 1;
		res_desc.SampleDesc.Quality = 0;
		res_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		res_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		if (access & EAccessHint::EA_GPUUnordered)
			res_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		CheckHResult(device->CreateCommittedResource(&heap_prop, D3D12_HEAP_FLAG_NONE,
			&res_desc, init_state, nullptr,
			COMPtr_RefParam(buffer, IID_ID3D12Resource)));

		if (init_data != nullptr) {
			auto const & cmd_list = Context::Instance().GetCommandList(Device::Command_Resource);
			std::lock_guard<std::mutex> lock(Context::Instance().GetCommandListMutex(Device::Command_Resource));

			heap_prop.Type = D3D12_HEAP_TYPE_UPLOAD;
			res_desc.Flags &= ~D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

			COMPtr<ID3D12Resource> buffer_upload;
			CheckHResult(device->CreateCommittedResource(&heap_prop, D3D12_HEAP_FLAG_NONE,
				&res_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
				COMPtr_RefParam(buffer_upload, IID_ID3D12Resource)));
			D3D::Debug(buffer_upload, "GraphicsBuffer::buffer_upload[TEMP]");

			void* p;
			buffer_upload->Map(0, nullptr, &p);
			memcpy(p, init_data, size_in_byte);
			buffer_upload->Unmap(0, nullptr);

			cmd_list->CopyResource(buffer.Get(), buffer_upload.Get());

			Context::Instance().CommitCommandList(Device::Command_Resource);
		}

		auto const structure_byte_stride = NumFormatBytes(format);

		if ((access & EAccessHint::EA_GPURead) && (format != EF_Unknown))
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC desc;
			desc.Format = /*(access & EAccessHint::EA_GPUStructured) ? DXGI_FORMAT_UNKNOWN :*/ Convert(format);
			desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			desc.Buffer.FirstElement = 0;
			desc.Buffer.NumElements = size_in_byte / structure_byte_stride;
			desc.Buffer.StructureByteStride = /*(access & EAccessHint::EA_GPUStructured) ? structure_byte_stride :*/ 0;
			desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

			srv = std::make_unique<ViewSimulation>(buffer, desc);
		}

		if ((access & EAccessHint::EA_GPUWrite)
			&& !(/*(access & EAH_GPU_Structured) ||*/ (access & EAccessHint::EA_GPUUnordered)))
		{
			counter_offset = (size_in_byte + 4 - 1) & ~(4 - 1);
		}
		else if ((access & EAccessHint::EA_GPUUnordered) && (format != EF_Unknown))
		{
			if (/*(access & EAccessHint::EA_Append) || (access & EAccessHint::EA_Counter)*/ false)
			{
				counter_offset = (size_in_byte + D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - 1)
					& ~(D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - 1);

				heap_prop.Type = D3D12_HEAP_TYPE_UPLOAD;
				res_desc.Width = sizeof(uint64_t);
				res_desc.Flags &= ~D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
				CheckHResult(device->CreateCommittedResource(&heap_prop, D3D12_HEAP_FLAG_NONE,
					&res_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
					COMPtr_RefParam(buffer_counter_upload, IID_ID3D12Resource)));
			}
			else
			{
				counter_offset = 0;
			}

			D3D12_UNORDERED_ACCESS_VIEW_DESC desc;
			if (/*access & EAccessHint::EA_Raw*/ false)
			{
				desc.Format = DXGI_FORMAT_R32_TYPELESS;
				desc.Buffer.StructureByteStride = 0;
			}
			else if (/*access & EAccessHint::EAH_GPUStructured*/ false)
			{
				desc.Format = DXGI_FORMAT_UNKNOWN;
				desc.Buffer.StructureByteStride = structure_byte_stride;
			}
			else
			{
				desc.Format = Convert(format);
				desc.Buffer.StructureByteStride = 0;
			}
			desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
			desc.Buffer.FirstElement = 0;
			desc.Buffer.NumElements = size_in_byte / structure_byte_stride;
			desc.Buffer.CounterOffsetInBytes = counter_offset;
			if (/*access & EAccessHint::EA_Raw*/false)
			{
				desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
			}
			else
			{
				desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
			}

			uav = std::make_unique<ViewSimulation>(buffer, desc);
		}
	}

	void GraphicsBuffer::HWResourceDelete()
	{
		srv.reset();
		uav.reset();

		counter_offset = 0;
		Reset(buffer_counter_upload);
		Reset(buffer);
	}
	void GraphicsBuffer::UpdateSubresource(leo::uint32 offset, leo::uint32 size, void const * data)
	{
		auto p = static_cast<stdex::byte*>(Map(platform::Render::Buffer::Write_Only));
		memcpy(p + offset, data, size);
		Unmap();
	}
	ID3D12Resource * GraphicsBuffer::Resource() const
	{
		return buffer.Get();
	}
	ID3D12Resource * GraphicsBuffer::UploadResource() const
	{
		return buffer_counter_upload.Get();
	}
	ViewSimulation * GraphicsBuffer::RetriveShaderResourceView()
	{
		return srv.get();
	}
	ViewSimulation * GraphicsBuffer::RetriveUnorderedAccessView()
	{
		return uav.get();
	}
	void * GraphicsBuffer::Map(platform::Render::Buffer::Access ba)
	{
		//TODO Cached Rule
		switch (ba)
		{
		case platform::Render::Buffer::Read_Only:
		case platform::Render::Buffer::Write_Only:
		case platform::Render::Buffer::Read_Write:
			Context::Instance().SyncCPUGPU();
			break;
		case platform::Render::Buffer::Write_No_Overwrite:
			break;
		default:
			break;
		}

		void* p = nullptr;
		buffer->Map(0, nullptr, &p);
		return p;
	}
	void GraphicsBuffer::Unmap()
	{
		buffer->Unmap(0, nullptr);
	}
}
