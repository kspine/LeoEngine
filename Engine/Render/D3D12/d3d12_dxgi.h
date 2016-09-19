/*! \file Engine\Render\d3d12_dxgi.h
\ingroup Engine
\brief ��װ��Ҫ��̬����ĺ�����������ؽӿڡ�
*/
#ifndef LE_RENDER_D3D12_d3d12_dxgi_h
#define LE_RENDER_D3D12_d3d12_dxgi_h 1

#include "../../Win32/COM.h"

#include <UniversalDXSDK/d3d12.h>
#include <UniversalDXSDK/dxgi1_5.h>

extern "C"
{
	typedef interface ID3D12DescriptorHeapWrap ID3D12DescriptorHeapWrap;

	typedef struct ID3D12DescriptorHeapVtbl
	{
		BEGIN_INTERFACE

			HRESULT(STDMETHODCALLTYPE *QueryInterface)(
				ID3D12DescriptorHeapWrap * This,
				REFIID riid,
				_COM_Outptr_  void **ppvObject);

		ULONG(STDMETHODCALLTYPE *AddRef)(
			ID3D12DescriptorHeapWrap * This);

		ULONG(STDMETHODCALLTYPE *Release)(
			ID3D12DescriptorHeapWrap * This);

		HRESULT(STDMETHODCALLTYPE *GetPrivateData)(
			ID3D12DescriptorHeapWrap * This,
			_In_  REFGUID guid,
			_Inout_  UINT *pDataSize,
			_Out_writes_bytes_opt_(*pDataSize)  void *pData);

		HRESULT(STDMETHODCALLTYPE *SetPrivateData)(
			ID3D12DescriptorHeapWrap * This,
			_In_  REFGUID guid,
			_In_  UINT DataSize,
			_In_reads_bytes_opt_(DataSize)  const void *pData);

		HRESULT(STDMETHODCALLTYPE *SetPrivateDataInterface)(
			ID3D12DescriptorHeapWrap * This,
			_In_  REFGUID guid,
			_In_opt_  const IUnknown *pData);

		HRESULT(STDMETHODCALLTYPE *SetName)(
			ID3D12DescriptorHeapWrap * This,
			_In_z_  LPCWSTR Name);

		HRESULT(STDMETHODCALLTYPE *GetDevice)(
			ID3D12DescriptorHeapWrap * This,
			REFIID riid,
			_COM_Outptr_opt_  void **ppvDevice);

		D3D12_DESCRIPTOR_HEAP_DESC(STDMETHODCALLTYPE *GetDesc)(
			ID3D12DescriptorHeapWrap * This);

		void(STDMETHODCALLTYPE *GetCPUDescriptorHandleForHeapStart)(
			ID3D12DescriptorHeapWrap * This, D3D12_CPU_DESCRIPTOR_HANDLE *pOut);

		D3D12_GPU_DESCRIPTOR_HANDLE(STDMETHODCALLTYPE *GetGPUDescriptorHandleForHeapStart)(
			ID3D12DescriptorHeapWrap * This);

		END_INTERFACE
	} ID3D12DescriptorHeapVtbl;

	interface ID3D12DescriptorHeapWrap
	{
		CONST_VTBL struct ID3D12DescriptorHeapVtbl *lpVtbl;
	};
}


namespace platform_ex {
	namespace Windows {
		/*
		\note ����ƽ̨��Щ������ֱ��ͨ��LoadProc��ʵ��
		\warning ���������Щ��������Ҳ����Ƶ������,����LoadProc�Ŀ���
		\warning ����Ӧģ��δ�������� �׳�Win32Exception
		\todo UWP֧��
		*/
		namespace DXGI {
			using namespace leo;

			HRESULT CreateFactory1(REFIID riid, void** ppFactory);
		}
		namespace D3D12 {
			using namespace leo;

			HRESULT CreateDevice(IUnknown* pAdapter,
				D3D_FEATURE_LEVEL MinimumFeatureLevel, REFIID riid,
				void** ppDevice);
			HRESULT GetDebugInterface(REFIID riid, void** ppvDebug);
			HRESULT SerializeRootSignature(D3D12_ROOT_SIGNATURE_DESC const * pRootSignature,
				D3D_ROOT_SIGNATURE_VERSION Version, ID3D10Blob** ppBlob, ID3D10Blob** ppErrorBlob);

			
			inline D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart(ID3D12DescriptorHeap * This) {
				auto ThisWrap = reinterpret_cast<ID3D12DescriptorHeapWrap*>(This);
				D3D12_CPU_DESCRIPTOR_HANDLE handle = {};
				ThisWrap->lpVtbl->GetCPUDescriptorHandleForHeapStart(ThisWrap, &handle);
				return handle;
			}
		}
	}
}


#endif