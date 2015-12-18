#pragma once

#include "Widget.h"
#include "HUDGraphics.h"
#include "RenderSystem/TextureX.hpp"

namespace leo {
	namespace HUD {
		namespace details {
			class GraphicsWrap :public Graphics {
			public:
				GraphicsWrap(Texture& tex, uint8 array_index, uint8 level,Texture::MapAccess tma,
					uint16 x_offset, uint16 y_offset,
					uint16 width, uint16 height)
					:delegate(tex,array_index,level,tma,x_offset,y_offset,width,height)
				{
					pBuffer = reinterpret_cast<Pixel<>*>(delegate.pSysMem);
					sGraphics.Height = height;
					sGraphics.Width = width;
				}
			private:
				Texture::Mapper delegate;
			};


			class hud_tex_wrapper : implements IImage {
			public:
				hud_tex_wrapper(const Size& s= {}) {
					tex = leo::X::MakeTexture2D(static_cast<uint16>(s.Width), static_cast<uint16>(s.Height)
						, 1, 1, 
						EFormat::EF_ARGB8, SampleDesc(), 
						EAccess::EA_C_W | EAccess::EA_G_R, ElementInitData());
				}

				~hud_tex_wrapper() {}

				std::unique_ptr<Graphics> GetContext() const lnothrow override {
					return std::make_unique<GraphicsWrap>(*tex,
						0,0,//array_index,level
						Texture::MapAccess::MA_WO,0,0,//tma,x_offset,y_offset
						tex->Width(0),tex->Height(0));
				}

				void SetSize(const Size& s) override {
					//need to impl
				}

				hud_tex_wrapper* clone() const override {
					//error impl
					//need copy data
					return new hud_tex_wrapper(Size(tex->Width(0), tex->Height(0)));
				}
			private:
				TexturePtr tex;
			};
		}
	}


	namespace HUD {
		namespace details {
			void DrawText(const std::string& Text, const PaintContext& pc, const Point& offset);
		}
	}
}
