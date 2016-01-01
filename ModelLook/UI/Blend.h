#ifndef UI_Blend_H
#define UI_Blend_H

#include "GUI.h"
#include "Blit.h"
#include <rational.hpp>

namespace leo
{
	namespace Drawing
	{

		namespace Shaders
		{

			/*!
			\brief �����������
			\tparam _vDstAlphaBits Ŀ�� Alpha λ��
			\tparam _vSrcAlphaBits Դ Alpha λ��
			\pre Դ��Ŀ�� Alpha �������ڣ�Ϊ��һ��ֵ�������Ǹ������򶨵�������
			\note ��� Alpha λ��Դ��Ŀ�� Alpha λ�е����ֵ������ΪĿ�� Alpha ���ͣ���
			\todo ֧���Ƶ��������͡�
			*/
			//@{
			template<size_t _vDstAlphaBits, size_t _vSrcAlphaBits>
			struct GPixelCompositor
			{
				/*!
				\brief Alpha ��� Alpha ������

				a := 1 - (1 - sa) * (1 - da)
				= 1 - (1 - sa - da + sa * da)
				= sa + da - sa * da
				= sa + da * (1 - sa)
				*/
				template<typename _tDstAlpha, typename _tSrcAlpha>
				static lconstfn conditional_t<(_vDstAlphaBits < _vSrcAlphaBits),
					_tSrcAlpha, _tDstAlpha>
					CompositeAlphaOver(_tDstAlpha da, _tSrcAlpha sa)
				{
					static_assert(is_normalizable<_tDstAlpha>(),
						"Non-normalizable destination alpha type found.");
					static_assert(is_normalizable<_tSrcAlpha>(),
						"Non-normalizable source alpha type found.");

					return sa + da * (_tSrcAlpha(1) - sa);
				}

				/*!
				\brief Alpha ��Ϸ� Alpha ������

				a != 0
				=> c := (sa * s + (1 - sa) * da * d) / a
				= (sa * s + (a - sa) * d) / a
				= (sa * s + a * d - sa * d) / a
				= (sa * (s - d) + a * d) / a
				= sa * (s - d) / a + d
				*/
				template<typename _tDst, typename _tSrc, typename _tSrcAlpha,
					typename _tAlpha>
					static lconstfn _tDst
					CompositeComponentOver(_tDst d, _tSrc s, _tSrcAlpha sa, _tAlpha a)
				{
					static_assert(is_normalizable<_tDst>(),
						"Non-normalizable destination type found.");
					static_assert(is_normalizable<_tSrc>(),
						"Non-normalizable source type found.");
					static_assert(is_normalizable<_tSrcAlpha>(),
						"Non-normalizable source alpha type found.");
					static_assert(is_normalizable<_tAlpha>(),
						"Non-normalizable alpha type found.");

					return a != _tAlpha(0) ? (s < d ? _tDst(d - sa * (d - s) / a)
						: _tDst(sa * (s - d) / a + d)) : _tDst(0);
				}
			};


			/*!
			\brief ���ط�����ϡ�
			\sa GPixelCompositor
			\todo ֧�ָ�������
			*/
			template<size_t _vSrcAlphaBits, typename _tDstInt, typename _tSrcInt,
				typename _tSrcAlphaInt>
				lconstfn _tDstInt
				BlendComponent(_tDstInt d, _tSrcInt s, _tSrcAlphaInt sa)
			{
				static_assert(integer_width<_tDstInt>::value == _vSrcAlphaBits,
					"Invalid integer destination type found.");
				static_assert(integer_width<_tSrcInt>::value == _vSrcAlphaBits,
					"Invalid integer source type found.");
				static_assert(integer_width<_tSrcAlphaInt>::value == _vSrcAlphaBits,
					"Invalid integer source alpha type found.");
				using pix = make_fixed_t<_vSrcAlphaBits>;

				return static_cast<_tDstInt>(GPixelCompositor<1, _vSrcAlphaBits>::CompositeComponentOver(
					pix(d, raw_tag()), pix(s, raw_tag()), pix(sa, raw_tag()), pix(1)).get());
			}


			/*!
			\brief Alpha ���ػ�ϣ�ʹ��ָ����Դ Alpha ͬʱ���͸������ Alpha ��
			\note ����Դ�����е� Alpha ��
			\sa Drawing::BlendComponent
			\sa Drawing::GPixelCompositor
			*/
			//@{
			//! \note ʹ�� ADL BlendComponent ָ��������ط�����
			template<size_t _vDstAlphaBits, size_t _vSrcAlphaBits, typename _tPixel,
				typename _tSrcAlphaInt>
				lconstfn _tPixel
				BlendAlpha(const _tPixel& d, const _tPixel& s, _tSrcAlphaInt sa)
			{
				static_assert(std::is_integral<_tSrcAlphaInt>(),
					"Invalid integer source alpha type found.");
				using pixd = make_fixed_t<_vDstAlphaBits>;
				using pix = make_fixed_t<_vSrcAlphaBits>;

				return Color(BlendComponent<_vSrcAlphaBits>(d.GetR(), s.GetR(), sa),
					BlendComponent<_vSrcAlphaBits>(d.GetG(), s.GetG(), sa), BlendComponent<
					_vSrcAlphaBits>(d.GetB(), s.GetB(), sa),
					static_cast<AlphaType>(GPixelCompositor<_vDstAlphaBits, _vSrcAlphaBits>::CompositeAlphaOver(
						pixd(d.GetA(), raw_tag()), pix(sa, raw_tag())).get()));
			}

			/*!
			\ingroup PixelShaders
			\brief ���ص�����͸��������
			\warning ������������Ч�ԡ�
			*/
			struct BlitTransparentPoint
			{
				//! \note ʹ��Դ��������Ӧ���صĵ� 15 λ��ʾ͸���ԡ�
				template<typename _tOut, typename _tIn>
				inline void
					operator()(_tOut dst_iter, _tIn src_iter)
				{
					if (FetchAlpha(*src_iter))
						*dst_iter = *src_iter;
				}
				/*!
				\note ʹ����������� Alpha ͨ����ʾ͸���ԡ�
				*/
				template<typename _tOut, typename _tInAlpha>
				inline void
					operator()(_tOut dst_iter,
						pair_iterator<ConstBitmapPtr, _tInAlpha> src_iter)
				{
					*dst_iter = *get<1>(src_iter.base()) & 0x80 ? FetchOpaque(*src_iter)
						: FetchOpaque(Pixel());
				}
			};


			/*!
			\ingroup PixelShaders
			\brief ���ؼ��㣺Alpha ��ϡ�
			*/
			struct BlitAlphaPoint
			{
			private:
				template<typename _type>
				using ABitTrait = typename decay_t<_type>::Trait;

			public:
				//! \since build 584
				template<typename _tOut, typename _tInPixel, typename _tInAlpha>
				inline void
					operator()(_tOut dst_iter,
						pair_iterator<_tInPixel, _tInAlpha> src_iter)
				{
					static_assert(std::is_convertible<remove_reference_t<
						decltype(*dst_iter)>, Pixel<>>(), "Wrong type found.");

					*dst_iter = Shaders::BlendAlpha<ABitTrait<decltype(*dst_iter)>::ABitsN,
						8>(*dst_iter, *src_iter, AlphaType(*get<1>(src_iter.base())));
				}
				//! \since build 448
				template<typename _tOut, typename _tIn>
				inline void
					operator()(_tOut dst_iter, _tIn src_iter)
				{
					static_assert(std::is_convertible<remove_reference_t<
						decltype(*dst_iter)>, Pixel<>>(), "Wrong type found.");

					*dst_iter = Shaders::Composite<ABitTrait<decltype(*dst_iter)>::ABitsN,
						ABitTrait<decltype(*src_iter)>::ABitsN>(*dst_iter, *src_iter);
				}
			};

			/*!
			\ingroup PixelShaders
			\brief ���ؼ��㣺Alpha ��ϡ�
			*/
			struct BlitBlendPoint
			{
			private:
				template<typename _type>
				using ABitTrait = typename decay_t<_type>::Trait;
			public:
				template<typename _tOut, typename _tIn>
				inline void
					operator()(_tOut dst_iter, _tIn src_iter)
				{
					static_assert(std::is_convertible<remove_reference_t<
						decltype(*dst_iter)>, Pixel< >> (), "Wrong type found.");

					*dst_iter = Shaders::BlendAlpha<ABitTrait<decltype(*dst_iter)>::ABitsN,
						8>(*dst_iter, *src_iter, AlphaType(FetchAlpha(*src_iter)));
				}
			};

		} // namespace Shaders;


		//@{
		//! \brief ʹ��ָ������ Alpha ���ָ���ı�׼��������
		//@{
		template<typename _tPixel, typename _tOut, typename... _tParams>
		inline void
			BlendRectRaw(_tOut dst, _tPixel c, _tParams&&... args)
		{
			using bl_it = pseudo_iterator<const Pixel<>>;

			Drawing::BlitRectPixels(Shaders::BlitBlendPoint(), dst,
				bl_it(c), lforward(args)...);
		}

		void
			LB_API BlendRect(const Graphics& g, const Rect& r, Color c);
	}
}

#endif
