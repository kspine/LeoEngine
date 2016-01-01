#ifndef UI_Blit_H
#define UI_Blit_H

#include "GUI.h"

#pragma warning(push)
#pragma warning(disable:4804)

namespace leo
{
	namespace Drawing
	{

		/*!
		\brief �鴫��߽��������
		\return �Ƿ���ں��ʱ߽硣

		��ָ����Ŀ������λ�á�Դ����λ�á�Ŀ��߽��С��Դ�߽��С�Ϳ鴫�������С�������
		�ձ߽���ص�ֵ��ǰ���� SDst �������Ǹ�ֵΪԴ����ϵ�ı߽�������Сֵ�����޺��ʱ�
		��ʱ������ SDst ���������޸ģ�����ֵΪ���տ鴫������Ŀ�͸ߡ�
		*/
		LB_API bool
			BlitBounds(const Point&, const Point&, const Size&, const Size&, const Size&,
				SDst&, SDst&, SDst&, SDst&);


		/*!
		\brief �鴫��ƫ�Ʒ�����������
		*/
		template<bool _bDec>
		lconstfn size_t
			BlitScaleComponent(SPos d, SPos s, SDst delta)
		{
			return size_t(std::max<SPos>(0, s < 0 ? d - s : d))
				+ size_t(_bDec ? delta - 1 : 0);
		}

		/*!
		\brief �鴫��ɨ�躯��ģ�塣
		\tparam _bDec ָ���Ƿ�תˮƽ����ֱ����֮һ��
		\tparam _tScalar ������С�Ĵ������͡�
		\tparam _tDiff ����ƫ�����Ĳ�ֵ���͡�
		\tparam _tOut ������������͡�
		\tparam _tIn ������������͡�
		\tparam _fBlitLoop ѭ���������͡�
		\param loop ѭ��������
		\param d_width Ŀ�껺�����Ŀ�
		\param s_width Դ�������Ŀ�
		\param delta_x ʵ�ʲ����Ŀ�
		\param delta_y ʵ�ʲ����ĸߡ�
		\param dst Ŀ���������
		\param src Դ��������
		\warning loop ���ܵ������������Ӧ��֤�����޷����������͡�

		��һ������������ָ����������ɨ�������
		*/
		template<bool _bDec, typename _tScalar, typename _tDiff, typename _tOut,
			typename _tIn, typename _fBlitLoop>
			void
			BlitScan(_fBlitLoop loop, _tOut dst, _tIn src, _tScalar d_width,
				_tScalar s_width, _tDiff delta_x, _tDiff delta_y)
		{
			loop(dst, src, delta_x, delta_y, _tDiff(_bDec ? -1 : 1) * _tDiff(d_width)
				- delta_x, _tDiff(s_width) - delta_x);
		}


		/*!
		\brief �鴫�亯��ģ�塣
		\tparam _bSwapLR ˮƽ��ת���񣨹���ˮƽ����Գƣ���
		\tparam _bSwapUD ��ֱ��ת���񣨹�����ֱ����Գƣ���
		\tparam _tOut ������������ͣ���Ҫ֧�� + ������һ��Ӧ���������������
		\tparam _tIn ������������ͣ���Ҫ֧�� + ������һ��Ӧ���������������
		\tparam _fBlitLoop ѭ���������͡�
		\param loop ѭ��������
		\param dst Ŀ���������
		\param src Դ��������
		\param ds Ŀ����������ڻ�������С��
		\param ss Դ���������ڻ�������С��
		\param dp Ŀ���������ʼ�����ڻ�����ƫ�ơ�
		\param sp Դ��������ʼ�����ڻ�����ƫ�ơ�
		\param sc Դ��������Ҫ���Ƶ������С��
		\note �����������Խ�硣Խ�Ͻ�ʱ�� BlitBounds ���ˣ�Խ�½�ʱ����Ϊ 0 ��
		\sa BlitBounds
		\sa BlitScaleComponent
		\sa Drawing::BlitScan

		��һ���������������˳������������
		*/
		template<bool _bSwapLR, bool _bSwapUD, typename _tOut, typename _tIn,
			typename _fBlitLoop>
			void
			Blit(_fBlitLoop loop, _tOut dst, _tIn src, const Size& ds, const Size& ss,
				const Point& dp, const Point& sp, const Size& sc)
		{
			SDst min_x, min_y, delta_x, delta_y;

			// XXX: Conversion to 'difference_type' might be implementation-defined.
			if (BlitBounds(dp, sp, ds, ss, sc, min_x, min_y, delta_x, delta_y))
				BlitScan<_bSwapLR != _bSwapUD>(loop, dst + typename
					std::iterator_traits<_tOut>::difference_type(BlitScaleComponent<
						_bSwapUD>(dp.Y, sp.Y, delta_y) * ds.Width + BlitScaleComponent<
						_bSwapLR>(dp.X, sp.X, delta_x)), src + typename
					std::iterator_traits<_tIn>::difference_type((_bSwapUD ? ss.Height
						- delta_y - min_y : min_y) * ss.Width + (_bSwapLR ? ss.Width
							- delta_x - min_x : min_x)), ds.Width, ss.Width, delta_x, delta_y);
		}


		/*!
		\ingroup BlitLoop
		\brief �鴫��ɨ����ѭ��������
		\sa BlitScan
		*/
		template<bool _bDec>
		struct BlitScannerLoop
		{
			template<typename _tOut, typename _tIn, typename _fBlitScanner>
			void
				operator()(_fBlitScanner scanner, _tOut dst_iter, _tIn src_iter,
					SDst delta_x, SDst delta_y, SPos dst_inc, SPos src_inc) const
			{
				while (delta_y-- > 0)
				{
					scanner(dst_iter, src_iter, delta_x);
					// NOTE: See $2015-02 @ %Documentation::Workflow::Annual2015.
					if (LB_LIKELY(delta_y != 0))
						lunseq(src_iter += src_inc,
							delta_assign<_bDec>(dst_iter, dst_inc));
				}
			}
		};


		/*!
		\brief ɨ���߿鴫�亯��ģ�塣
		\tparam _bSwapLR ˮƽ��ת���񣨹���ˮƽ����Գƣ���
		\tparam _bSwapUD ��ֱ��ת���񣨹�����ֱ����Գƣ���
		\tparam _tOut ������������ͣ���Ҫ֧�� + ������һ��Ӧ���������������
		\tparam _tIn ������������ͣ���Ҫ֧�� + ������һ��Ӧ���������������
		\tparam _fBlitScanner ɨ���߲������͡�
		\param scanner ɨ���߲�����
		\param dst Ŀ���������
		\param src Դ��������
		\param ds Ŀ����������ڻ�������С��
		\param ss Դ���������ڻ�������С��
		\param dp Ŀ���������ʼ�����ڻ�����ƫ�ơ�
		\param sp Դ��������ʼ�����ڻ�����ƫ�ơ�
		\param sc Դ��������Ҫ���Ƶ������С��
		\sa Drawing::Blit
		\sa BlitScannerLoop

		��һ�����������ˮƽ��ɨ��������������
		*/
		template<bool _bSwapLR, bool _bSwapUD, typename _tOut, typename _tIn,
			typename _fBlitScanner>
			inline void
			BlitLines(_fBlitScanner scanner, _tOut dst, _tIn src, const Size& ds,
				const Size& ss, const Point& dp, const Point& sp, const Size& sc)
		{
			using namespace std::placeholders;

			Blit<_bSwapLR, _bSwapUD, _tOut, _tIn>(std::bind(BlitScannerLoop<!_bSwapLR>(
				), scanner, _1, _2, _3, _4, _5, _6), dst, src, ds, ss, dp, sp, sc);
		}

		/*!
		\ingroup BlitLineScanner
		\brief �鴫��ɨ���ѭ��������
		\tparam _bPositiveScan ����ɨ�衣
		\warning ������������Ч�ԡ�
		*/
		template<bool _bDec>
		struct BlitLineLoop
		{
			template<typename _tOut, typename _tIn, typename _fPixelShader>
			void
				operator()(_fPixelShader shader, _tOut& dst_iter, _tIn& src_iter,
					SDst delta_x)
			{
				for (SDst x(0); x < delta_x; ++x)
				{
					shader(dst_iter, src_iter);
					++src_iter;
					xcrease<_bDec>(dst_iter);
				}
			}
		};


		/*!
		\brief ���ؿ鴫�亯��ģ�塣
		\tparam _bSwapLR ˮƽ��ת���񣨹���ˮƽ����Գƣ���
		\tparam _bSwapUD ��ֱ��ת���񣨹�����ֱ����Գƣ���
		\tparam _tOut ������������ͣ���Ҫ֧�� + ������һ��Ӧ���������������
		\tparam _tIn ������������ͣ���Ҫ֧�� + ������һ��Ӧ���������������
		\tparam _fPixelShader ������ɫ�����͡�
		\param shader ������ɫ����
		\param dst Ŀ���������
		\param src Դ��������
		\param ds Ŀ����������ڻ�������С��
		\param ss Դ���������ڻ�������С��
		\param dp Ŀ���������ʼ�����ڻ�����ƫ�ơ�
		\param sp Դ��������ʼ�����ڻ�����ƫ�ơ�
		\param sc Դ��������Ҫ���Ƶ������С��
		\sa Drawing::Blit
		\sa BlitScannerLoop
		\sa BlitLineLoop

		��һ�����������ˮƽ��ɨ��������������
		*/
		template<bool _bSwapLR, bool _bSwapUD, typename _tOut, typename _tIn,
			typename _fPixelShader>
			inline void
			BlitPixels(_fPixelShader shader, _tOut dst, _tIn src, const Size& ds,
				const Size& ss, const Point& dp, const Point& sp, const Size& sc)
		{
			BlitLines<_bSwapLR, _bSwapUD, _tOut, _tIn>(
				[shader](_tOut& dst_iter, _tIn& src_iter, SDst delta_x) {
				BlitLineLoop<!_bSwapLR>()(shader, dst_iter, src_iter, delta_x);
			}, dst, src, ds, ss, dp, sp, sc);
		}

		/*!
		\tparam _tOut ������������ͣ���Ҫ֧�� + ������һ��Ӧ���������������
		\tparam _tIn ������������ͣ���Ҫ֧�� + ������һ��Ӧ���������������
		*/
		//@{
		/*!
		\brief ���ο鴫�亯��ģ�塣
		\tparam _func �������������͡�
		\param f ��������
		\param dst Ŀ���������
		\param src Դ��������

		��һ���������ָ��������������в�����
		*/
		//! \param ds Ŀ����������ڻ�������С��
		//@{
		//@{
		/*!
		\param dp Ŀ���������ʼ�����ڻ�����ƫ�ơ�
		\param sc Դ��������Ҫ���Ƶ������С��
		*/
		//@{
		/*!
		\param ss Դ���������ڻ�������С��
		\param sp Դ��������ʼ�����ڻ�����ƫ�ơ�
		*/
		template<typename _func, typename _tOut, typename _tIn>
		inline void
			BlitRect(_func f, _tOut dst, _tIn src, const Size& ds, const Size& ss,
				const Point& dp, const Point& sp, const Size& sc)
		{
			f(dst, src, ds, ss, dp, sp, sc);
		}
		template<typename _func, typename _tOut, typename _tIn>
		inline void
			BlitRect(_func f, _tOut dst, _tIn src, const Size& ds, const Point& sp,
				const Size& sc)
		{
			Drawing::BlitRect(f, dst, src, ds, ds, sp, sp, sc);
		}
		//@}
		//! \param r Դ���������ڻ������߽硣
		template<typename _func, typename _tOut, typename _tIn>
		inline void
			BlitRect(_func f, _tOut dst, _tIn src, const Size& ds, const Rect& r)
		{
			Drawing::BlitRect(f, dst, src, ds, r.GetPoint(), r.GetSize());
		}
		//@}
		/*!
		\param dw Ŀ����������ڻ������Ŀ�
		\param dh Ŀ����������ڻ������ĸߡ�
		\param x Դ��������ʼ�����ڻ�����ˮƽƫ�ơ�
		\param y Դ��������ʼ�����ڻ�������ֱƫ�ơ�
		\param w Դ��������Ҫ���Ƶ�����Ŀ�
		\param h Դ��������Ҫ���Ƶ�����ĸߡ�
		*/
		template<typename _func, typename _tOut, typename _tIn>
		inline void
			BlitRect(_func f, _tOut dst, _tIn src, SDst dw, SDst dh, SPos x, SPos y, SDst w,
				SDst h)
		{
			Drawing::BlitRect(f, dst, src, { dw, dh }, { x, y }, { w, h });
		}
		//@}


		/*!
		\brief ����ָ�������������鴫��ɨ������������ɫ������������Ծ������ؿ��������������
		\note ʹ�� ADL ���� BlitRect ���в�����
		\sa BlitRect
		*/
		//@{
		//! \sa Drawing::BlitLines
		template<typename _tOut, typename _tIn, typename _fBlitScanner,
			typename... _tParams>
			inline void
			BlitRectLines(_fBlitScanner scanner, _tOut dst, _tIn src, _tParams&&... args)
		{
			using namespace std::placeholders;

			BlitRect(std::bind(Drawing::BlitLines<false, false, _tOut, _tIn,
				_fBlitScanner>, scanner, _1, _2, _3, _4, _5, _6, _7), dst, src,
				lforward(args)...);
		}


		//! \sa Drawing::BlitPixels
		template<typename _tOut, typename _tIn, typename _fPixelShader,
			typename... _tParams>
			inline void
			BlitRectPixels(_fPixelShader shader, _tOut dst, _tIn src, _tParams&&... args)
		{
			using namespace std::placeholders;

			BlitRect(std::bind(Drawing::BlitPixels<false, false, _tOut, _tIn,
				_fPixelShader>, shader, _1, _2, _3, _4, _5, _6, _7), dst, src,
				lforward(args)...);
		}
		//@}
		//@}

		/*!
		\ingroup BlitScanner
		\brief ɨ���ߣ���ָ��ɨ��˳����һ�����ء�
		\warning ������������Ч�ԡ�
		*/
		//@{
		template<bool _bDec>
		struct CopyLine
		{
			/*!
			\brief ���Ƶ�����ָ����һ�����ء�
			\tparam _tOut ������������ͣ���Ҫ֧�� + ������һ��Ӧ���������������
			\tparam _tIn ������������͡�
			\pre ���ԣ��Է��������ʼ�����������ж�Ϊ���ɽ����á�
			*/
			//@{
			template<typename _tOut, typename _tIn>
			void
				operator()(_tOut& dst_iter, _tIn& src_iter, SDst delta_x) const
			{
				LAssert(delta_x == 0 || !is_undereferenceable(dst_iter),
					"Invalid output iterator found."),
					LAssert(delta_x == 0 || !is_undereferenceable(src_iter),
						"Invalid input iterator found.");
				std::copy_n(src_iter, delta_x, dst_iter);
				// NOTE: Possible undefined behavior. See $2015-02
				//	@ %Documentation::Workflow::Annual2015.
				lunseq(src_iter += delta_x, dst_iter += delta_x);
			}
			template<typename _tOut, typename _tPixel>
			void
				operator()(_tOut& dst_iter,pseudo_iterator<_tPixel> src_iter,
					SDst delta_x) const
			{
				LAssert(delta_x == 0 || !is_undereferenceable(dst_iter),
					"Invalid output iterator found."),
					std::fill_n(dst_iter, delta_x, Deref(src_iter));
				// NOTE: Possible undefined behavior. See $2015-02
				//	@ %Documentation::Workflow::Annual2015.
				dst_iter += delta_x;
			}
			//@}
		};

		template<>
		struct CopyLine<false>
		{
			/*!
			\todo ���ӶԲ�֧��ǰ�� -- �����ĵ�������֧�֡�
			\todo ƽ�����������Ż���
			*/
			template<typename _tOut, typename _tIn>
			void
				operator()(_tOut& dst_iter, _tIn& src_iter, SDst delta_x) const
			{
				while (delta_x-- > 0)
					*dst_iter-- = *src_iter++;
			}
		};
		//@}

		/*
		\brief ��ʾ������������/�Դ�ɫ������䡣
		\tparam _tOut ������������ͣ���Ҫ֧�� += ������һ��Ӧ���������������
		*/
		//@{
		/*!
		\brief ���ָ��λ�õ� n ���������ء�
		\tparam _tOut ������������͡�
		\sa ClearSequence
		*/
		template<typename _tOut>
		inline _tOut
			ClearPixel(_tOut dst, size_t n) lnothrow
		{
			ClearSequence(dst, n);
			return dst;
		}

		/*!
		\brief ʹ�� n ��ָ�������������ָ��λ�á�
		*/
		template<typename _tPixel, typename _tOut>
		inline void
			FillPixel(_tOut dst_iter, size_t n, _tPixel c)
		{
			CopyLine<true>()(dst_iter, pseudo_iterator<_tPixel>(c), SDst(n));
		}

		/*!
		\brief ʹ��ָ���������ָ���ı�׼��������
		*/
		template<typename _tPixel, typename _tOut, typename... _tParams>
		inline void
			FillRectRaw(_tOut dst, _tPixel c, _tParams&&... args)
		{
			Drawing::BlitRectLines(CopyLine<true>(), dst,
				pseudo_iterator<_tPixel>(c), lforward(args)...);
		}


		/*!
		\brief ���ͼ�νӿ������Ļ�������
		*/
		LB_API void
			ClearImage(const Graphics&);

		/*!
		\brief ʹ��ָ����ɫ���ͼ�νӿ������Ļ�������
		*/
		LB_API void
			Fill(const Graphics&, Color);
	}
}

#pragma warning(pop)

#endif
