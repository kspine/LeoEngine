/*! \file Engine\Asset\COM.h
\ingroup Engine
\brief Texture IO/Infomation ...
*/
#ifndef LE_ASSET_TEXTURE_X_H
#define LE_ASSET_TEXTURE_X_Hs 1

#include "../Render/ITexture.hpp"
#include "../Core/LFile.h"

namespace platform {
	namespace X {
		void GetImageInfo(File const & file, Render::TextureType& type,
			uint16& width, uint16& height, uint16& depth, uint8& num_mipmaps, uint8& array_size,
			Render::EFormat& format, uint32& row_pitch, uint32& slice_pitch);
	}

#if ENGINE_TOOL
	namespace T {
		//���ڴ��ƽ̨����ʹ������ʱ���봦�����ǽ���Դת�����߼�ƥ�����Դ��ʽ
		void ImageLE2BE(File const & file) {

		}
	}
#endif
}
#endif