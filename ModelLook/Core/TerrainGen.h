//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   Core/TerrainGen.h
//  Version:     v1.00
//  Created:     03/12/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2013
//  Description: ��������<GPU�������-GPUGem3,��д��Ϣ>
// -------------------------------------------------------------------------
//  History:
//				
//
////////////////////////////////////////////////////////////////////////////

#ifndef Core_TerrainGen_hpp
#define Core_TerrainGen_hpp

#include <leoint.hpp>
#include "file.hpp"
#include "RenderSystem/ShaderMgr.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace leo {

	struct TerrainFileHeaderEx {
		float mChunkSize;
		uint32 mHorChunkNum;
		uint32 mVerChunkNum;
		wchar_t mHeightMap[leo::win::file::max_path];
		uint64 Reserved;//���ֵһ������һ��Offset,��ȡ������Ϣ
	};

	struct TerrainFileHeader
	{
		float mChunkSize;
		std::uint32_t mHorChunkNum;
		std::uint32_t mVerChunkNum;
		wchar_t mHeightMap[leo::win::file::max_path];
	};

	bool GenTerrainEx(ID3D11Device* device, ID3D11DeviceContext* context,const TerrainFileHeader& header,const uint8& maxedage,const std::wstring& dst_terrain, const std::wstring& height_map_path);
	bool ReBuildTerrain(ID3D11Device* device, ID3D11DeviceContext* context,const std::wstring& src_terrain, const uint8& maxedage,const std::wstring& dst_terrain,const std::wstring& height_map_path);

}



#endif
