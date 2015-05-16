////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   ShaderSystem/Deferred.h
//  Version:     v1.00
//  Created:     03/16/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: �ӳ���Ⱦ
// -------------------------------------------------------------------------
//  History:
//			Todo : DepthStencil�����ӳ���Դ
////////////////////////////////////////////////////////////////////////////


/*
R8G8B8A8 NORMAL_SPECULAR <float3->float2->float3>
R10G10B10A2 DIFFUSE
R10G10B10A2 LIGHT
*/


#ifndef ShaderSystem_Deferred_H
#define ShaderSystem_Deferred_H

#include "utility.hpp"
#include "Singleton.hpp"

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

namespace leo {

	class CameraFrustum;

	class  DeferredResources :public Singleton<DeferredResources>  {
	protected:
		DeferredResources() noexcept;
	public:
		using size_type = std::pair<leo::uint16, leo::uint16>;

		
		~DeferredResources();

		//todo:����ʹ���޲�ʵ����,���Ǵ���Device����
		static DeferredResources& GetInstance() noexcept {
			static DeferredResources obj;
			return obj;
		}

		ID3D11RenderTargetView** GetMRTs() const;
		ID3D11ShaderResourceView** GetSRVs() const;

		ID3D11RenderTargetView* GetSSAORTV() const;
		ID3D11ShaderResourceView* GetSSAOSRV() const;

		//GBuffer���������׼������
		void OMSet() noexcept;
		//GBuffer���������׼������
		//note please clear RenderTarget
		void IASet() noexcept;

		void UnIASet() noexcept;

		//before call this ,please call IASet
		void ComputerSSAO() noexcept;

		void ReSize(const size_type& size) noexcept;

		void SetFrustum(const CameraFrustum& frustum) noexcept;
	};

}

#endif