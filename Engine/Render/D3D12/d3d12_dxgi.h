/*! \file Engine\Render\d3d12_dxgi.h
\ingroup Engine
\brief ��װ��Ҫ��̬����ĺ�����������ؽӿڡ�
*/
#ifndef LE_RENDER_D3D12_d3d12_dxgi_h
#define LE_RENDER_D3D12_d3d12_dxgi_h 1

#include "../../Win32/COM.h"

#include <UniversalDXSDK/d3d12.h>
#include <UniversalDXSDK/dxgi1_5.h>

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
		}
	}
}

#endif