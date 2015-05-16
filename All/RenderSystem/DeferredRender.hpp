////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2015.
// -------------------------------------------------------------------------
//  File name:   ShaderSystem/DeferredRender.h
//  Version:     v1.00
//  Created:     05/16/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: �ӳ���Ⱦ
// -------------------------------------------------------------------------
//  History:
////////////////////////////////////////////////////////////////////////////


/*
R8G8B8A8 NORMAL_SPECULAR <float3->float2->float3>
R10G10B10A2 DIFFUSE
R10G10B10A2 LIGHT
*/


#ifndef ShaderSystem_DeferredRender_Hpp
#define ShaderSystem_DeferredRender_Hpp

#include <memory>

namespace leo {
	class DeferredRender {
	private:
		class DeferredImpl;
		std::unique_ptr<DeferredImpl> pImpl;
	public:
		/*
		ID3D11RenderTargetView** GetMRTs() const;
		ID3D11ShaderResourceView** GetSRVs() const;

		//GBuffer���������׼������
		void OMSet() noexcept;
		//GBuffer���������׼������
		//note please clear RenderTarget
		void IASet() noexcept;

		void UnIASet() noexcept;

		void ReSize(const size_type& size) noexcept;
		*/
	};
}

#endif
