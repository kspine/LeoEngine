#include "ITexture.hpp"

using namespace leo::inttype;

platform::Render::Texture::Texture(Type type_, uint32 access, SampleDesc sample_info_)
	:type(type_), access_mode(access), sample_info(sample_info_)
{
}

platform::Render::Texture::~Texture()
{
}

uint8 platform::Render::Texture::GetNumMipMaps() const
{
	return mipmap_size;
}

platform::Render::EFormat platform::Render::Texture::GetFormat() const
{
	return format;
}

uint8 platform::Render::Texture::GetArraySize() const
{
	return array_size;
}

platform::Render::TextureType platform::Render::Texture::GetType() const
{
	return type;
}

platform::Render::SampleDesc platform::Render::Texture::GetSampleInfo() const
{
	return sample_info;
}

uint32 platform::Render::Texture::GetSampleCount() const
{
	return sample_info.Count;
}

uint32 platform::Render::Texture::GetSampleQuality() const
{
	return sample_info.Quality;
}

uint32 platform::Render::Texture::GetAccessMode() const
{
	return access_mode;
}

platform::Render::Texture1D::Texture1D(uint32 access, SampleDesc sample_info)
	:Texture(TextureType::T_1D, access, sample_info)
{
}

platform::Render::Texture1D::~Texture1D()
{
}

void platform::Render::Texture1D::Resize(Texture1D & target, uint32 dst_array_index, uint32 dst_level, uint32 dst_x_offset, uint32 dst_width, uint32 src_array_index, uint32 src_level, uint32 src_x_offset, uint32 src_width, bool linear)
{

}

void platform::Render::Texture1D::Resize(uint32 dst_array_index, uint32 dst_level, uint32 dst_x_offset, uint32 dst_width, uint32 src_array_index, uint32 src_level, uint32 src_x_offset, uint32 src_width, bool linear)
{
	Resize(*this, dst_array_index, dst_level, dst_x_offset, dst_width, src_array_index, src_level, src_x_offset, src_width, linear);
}

platform::Render::Texture2D::Texture2D(uint32 access, SampleDesc sample_info)
	:Texture(TextureType::T_2D, access, sample_info)
{
}

platform::Render::Texture2D::~Texture2D()
{
}

void platform::Render::Texture2D::Resize(Texture2D & target, uint32 dst_array_index, uint32 dst_level, uint32 dst_x_offset, uint32 dst_y_offset, uint32 dst_width, uint32 dst_height, 
	uint32 src_array_index, uint32 src_level, uint32 src_x_offset, uint32 src_y_offset, uint32 src_width, uint32 src_height, bool linear)
{
}

void platform::Render::Texture2D::Resize(uint32 dst_array_index, uint32 dst_level, uint32 dst_x_offset, uint32 dst_y_offset, uint32 dst_width, uint32 dst_height, 
	uint32 src_array_index, uint32 src_level, uint32 src_x_offset, uint32 src_y_offset, uint32 src_width, uint32 src_height, bool linear)
{
	Resize(*this, dst_array_index, dst_level, dst_x_offset, dst_y_offset, dst_width, dst_height,
		src_array_index, src_level, src_x_offset, src_y_offset, src_width, src_height, linear);
}


platform::Render::Texture3D::Texture3D(uint32 access, SampleDesc sample_info)
	:Texture(TextureType::T_3D,access,sample_info)
{
}

platform::Render::Texture3D::~Texture3D()
{
}

void platform::Render::Texture3D::Resize(Texture3D & target, uint32 dst_array_index, uint32 dst_level, uint32 dst_x_offset, uint32 dst_y_offset, uint32 dst_z_offset, uint32 dst_width, uint32 dst_height, uint32 dst_depth, uint32 src_array_index, uint32 src_level, uint32 src_x_offset, uint32 src_y_offset, uint32 src_z_offset, uint32 src_width, uint32 src_height, uint32 src_depth, bool linear)
{
}

void platform::Render::Texture3D::Resize(uint32 dst_array_index, uint32 dst_level, uint32 dst_x_offset, uint32 dst_y_offset, uint32 dst_z_offset, uint32 dst_width, uint32 dst_height, uint32 dst_depth, 
	uint32 src_array_index, uint32 src_level, uint32 src_x_offset, uint32 src_y_offset, uint32 src_z_offset, uint32 src_width, uint32 src_height, uint32 src_depth, bool linear)
{
	Resize(*this, dst_array_index, dst_level, dst_x_offset, dst_y_offset, dst_z_offset, dst_width, dst_height, dst_depth,
		src_array_index, src_level, src_x_offset, src_y_offset, src_z_offset, src_width, src_height, src_depth,
		linear);
}

platform::Render::TextureCube::TextureCube(uint32 access, SampleDesc sample_info)
	:Texture(TextureType::T_Cube,access,sample_info)
{
}

platform::Render::TextureCube::~TextureCube()
{
}

void platform::Render::TextureCube::Resize(TextureCube & target, uint32 dst_array_index, CubeFaces dst_face, uint32 dst_level, uint32 dst_x_offset, uint32 dst_y_offset, uint32 dst_width, uint32 dst_height, uint32 src_array_index, CubeFaces src_face, uint32 src_level, uint32 src_x_offset, uint32 src_y_offset, uint32 src_width, uint32 src_height, bool linear)
{
}

void platform::Render::TextureCube::Resize(uint32 dst_array_index, CubeFaces dst_face, uint32 dst_level, uint32 dst_x_offset, uint32 dst_y_offset, uint32 dst_width, uint32 dst_height, 
	uint32 src_array_index, CubeFaces src_face, uint32 src_level, uint32 src_x_offset, uint32 src_y_offset, uint32 src_width, uint32 src_height, bool linear)
{
	Resize(*this, dst_array_index, dst_face, dst_level, dst_x_offset, dst_y_offset, dst_width, dst_height,
		src_array_index, src_face, src_level, src_x_offset, src_y_offset, src_width, src_height,
		linear);
}
