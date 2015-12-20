#ifndef HUD_Graphics_H
#define HUD_Graphics_H

#include "HUD.h"

LEO_BEGIN


//2D�����װ
//warning��һ�㲻��ֱ��ʹ�ø�����,using std::share_ptr<texture2d_wrapper>
//warning, HUD�ĸ�ʽһ����R8G8B8A8(not A8B8...)��DYNAMIC,NOMIP,ARRAYSIZE = 1
//warning, R8G8B8A8,ĳЩ�Կ�Ч�����ܲ���
class texture2d_wrapper;

HUD_BEGIN

using BitmapPtr = Drawing::Pixel<>*;
using ConstBitmapPtr = const Drawing::Pixel<>*;

template<typename _tOut, typename _tIn>
void
CopyBitmapBuffer(_tOut p_dst, _tIn p_src, const Size& s)
{
	std::copy_n(Nonnull(p_src), GetAreaOf(s), Nonnull(p_dst));
}

/*!
\brief ��άͼ�νӿ�������ģ�塣
\warning Ϊ����YSLIB����
*/
template<typename _tPointer, class _tSize = Size>
class GGraphics
{
	static_assert(std::is_nothrow_move_constructible<_tPointer>::value,
		"Invalid pointer type found.");
	static_assert(std::is_nothrow_copy_constructible<_tSize>::value,
		"Invalid size type found.");

public:
	using PointerType = _tPointer;
	using SizeType = _tSize;
	using PixelType = std::decay_t<decltype(PointerType()[0])>;

protected:
	/*!
	\brief ��ʾ������ָ�롣
	\warning ���� PointerType ����Ϊ��������Ȩ������ָ�룬��Ӧ��Ϊ��������Ȩ��
	*/
	PointerType pBuffer{};
	//! \brief ͼ�������С��
	SizeType sGraphics{};

public:
	virtual ~GGraphics()
	{}

	//! \brief Ĭ�Ϲ��죺ʹ�ÿ�ָ��ʹ�С��
	DefDeCtor(GGraphics)
		//! \brief ���죺ʹ��ָ��λͼָ��ʹ�С��
		explicit lconstfn
		GGraphics(PointerType p_buf, const SizeType& s = {}) lnothrow
		: pBuffer(std::move(p_buf)), sGraphics(s)
	{}
	//! \brief ���죺ʹ���������͵�ָ��λͼָ��ʹ�С��
	template<typename _tPointer2, class _tSize2>
	explicit lconstfn
		GGraphics(_tPointer2 p_buf, const _tSize2& s = {}) lnothrow
		: GGraphics(static_cast<PointerType>(std::move(p_buf)),
			static_cast<SizeType>(s))
	{}
	//! \brief ���죺ʹ���������͵�ָ��λͼָ��ʹ�С���͵Ķ�άͼ�νӿ������ġ�
	template<typename _tPointer2, class _tSize2>
	lconstfn
		GGraphics(const GGraphics<_tPointer2, _tSize2>& g) lnothrow
		: GGraphics(std::move(g.GetBufferPtr()), g.GetSize())
	{}
	//! \brief ���ƹ��죺ǳ���ơ�
	DefDeCopyCtor(GGraphics)
		DefDeMoveCtor(GGraphics)

		DefDeCopyAssignment(GGraphics)
		DefDeMoveAssignment(GGraphics)

		/*!
		\brief �ж���Ч����Ч�ԡ�
		\since build 319
		*/
		DefBoolNeg(explicit,
			bool(pBuffer) && sGraphics.Width != 0 && sGraphics.Height != 0)

		/*!
		\brief ȡָ������Ԫ��ָ�롣
		\pre ���ԣ�������Խ�硣
		\pre ��Ӷ��ԣ�������ָ��ǿա�
		*/
		PointerType
		operator[](size_t r) const lnothrow
	{
		LAssert(r < sGraphics.Height, "Access out of range.");
		return Nonnull(pBuffer) + r * sGraphics.Width;
	}

	//! \since build 566
	DefGetter(const lnothrow, const PointerType&, BufferPtr, pBuffer)
		DefGetter(const lnothrow, const SizeType&, Size, sGraphics)
		//! \since build 196
		DefGetter(const lnothrow, platform::unitlength_type, Width, sGraphics.Width)
		//! \since build 196
		DefGetter(const lnothrow, platform::unitlength_type, Height, sGraphics.Height)
		//! \since build 177
		DefGetter(const lnothrow, size_t, SizeOfBuffer,
			sizeof(PixelType) * size_t(GetAreaOf(sGraphics))) //!< ȡ������ռ�ÿռ䡣

															  /*!
															  \brief ȡָ������Ԫ��ָ�롣
															  \throw GeneralEvent ������ָ��Ϊ�ա�
															  \throw std::out_of_range ����Խ�硣
															  */
		PointerType
		at(size_t r) const
	{
		if (LB_UNLIKELY(!pBuffer))
			throw general_event("Null pointer found.");
		if (LB_UNLIKELY(r >= sGraphics.Height))
			throw std::out_of_range("Access out of range.");
		return pBuffer + r * sGraphics.Width;
	}
};

using ConstGraphics = GGraphics<ConstBitmapPtr>;
using Graphics = GGraphics<BitmapPtr>;

/*!
\brief ͼ��ӿڡ�
\warning ����ײ�ʵ��һ����һ��leo::HUD::details::hud_tex_wrapper����
*/
DeclDerivedI(LB_API, IImage,cloneable)
DeclIEntry(std::unique_ptr<Graphics> GetContext() const lnothrow)
DeclIEntry(void SetSize(const Size&))

DeclIEntry(IImage* clone() const ImplI(cloneable))
EndDecl


/*
\brief ���������ġ�
\warning ����������
*/
struct LB_API PaintContext
{
	//todo:modify it
	//maybe GDI impl
	Graphics Target; //!< ��ȾĿ�꣺ͼ�νӿ������ġ�
	/*!
	\brief �ο�λ�á�

	ָ����ȾĿ������Ĳο����λ�õ�ƫ�����ꡣ
	��������Լ����ѡȡ��ȾĿ�����Ͻ�Ϊԭ�����Ļ����ϵ��
	*/
	Point Location;
	/*!
	\brief ��������

	���ͼ�νӿ������ĵı�׼���Σ�ָ����Ҫ��֤��ˢ�µı߽�����
	��������Լ�������������λ������ѡȡ��ȾĿ�����Ͻ�Ϊԭ�����Ļ����ϵ��
	*/
	Rect ClipArea;
};


/*!
\brief ����ָ���߾��Դ�Ĵ�С�Ż����������ĵļ�������
\return ���߾���������Ա���Ⱦ��Ϊ Point() ������ΪԴ����ʼƫ��λ�á�
\note ������Ҫ����ʱ�����޸�ƫ�����ꡣ

���߾���������Ҫ�������Ƶ����򣬽�����������������ĵĳ���ȾĿ�����������Ա��
*/
LB_API Point
ClipMargin(PaintContext&, const Drawing::Padding&, const Size&);

HUD_END

LEO_END

#endif
