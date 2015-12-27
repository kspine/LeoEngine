//����freetype��������Ⱦʵ��
#ifndef UI_Font_h
#define UI_Font_h

#include "UI.h"
#include <cstddef>
#include <set>
#include <leoint.hpp>
#include <id.hpp>
#include <cache.hpp>
#include <ref.hpp>
#include <exception.hpp>
#include <container.hpp>
#include <string.hpp>
#include <functional.hpp>

struct FT_SizeRec_;
using FT_Size = ::FT_SizeRec_*;
using FT_SizeRec = ::FT_SizeRec_;
struct FT_FaceRec_;
using FT_FaceRec = ::FT_FaceRec_;
struct FT_GlyphSlotRec_;
using FT_GlyphSlot = ::FT_GlyphSlotRec_*;
struct FT_LibraryRec_;
using FT_Library = ::FT_LibraryRec_*;
struct FT_Size_Metrics_;
using FT_Size_Metrics = FT_Size_Metrics_;

LEO_DRAW_BEGIN

class Font;
class FontCache;
class FontFamily;
class Typeface;

using FontSize = std::uint8_t;
using FontPath = std::string;
using FamilyName = std::string;
using StyleName = std::string;

enum class FontStyle : std::uint8_t
{
	Regular = 0, //!< �������塣
	Bold = 1, //!< ���塣
	Italic = 2, //!< б�塣
	Underline = 4, //!< �»��ߡ�
	Strikeout = 8 //!< ɾ���ߡ�
};

DefBitmaskEnum(FontStyle)

lconstfn PDefH(const char*, FetchName, FontStyle style) lnothrow
ImplRet(style == FontStyle::Bold ? "Bold" : (style == FontStyle::Italic
	? "Italic" : (style == FontStyle::Underline ? "Underline" : (style
		== FontStyle::Strikeout ? "Strikeout" : "Regular"))))

/*!
\brief �����쳣��
*/
class LB_API FontException : public logged_event
{
public:
	//! \note �� \c ::FT_Error һ�¡�
	using FontError = int;

private:
	FontError err;

public:
	FontException(FontError e, const std::string& msg = {})
		: logged_event(msg),
		err(e)
	{}
	DefDeCopyCtor(FontException)
	/*!
	\brief ���������ඨ����Ĭ��ʵ�֡�
	*/
	~FontException() override;

	DefGetter(const lnothrow, FontError, ErrorCode, err)
};

/*!
\brief ���������С��
*/
class LB_API NativeFontSize final : private noncopyable
{
private:
	::FT_Size size;

public:
	NativeFontSize(::FT_FaceRec&, FontSize);
	NativeFontSize(NativeFontSize&&) lnothrow;
	~NativeFontSize();

	::FT_SizeRec&
		GetSizeRec() const;

	/*!
	\brief ���ǰ��С��
	\note ��� \c ::FT_Activate_Size ��
	*/
	void
		Activate() const;
};

/*!
\brief ���ͼ��� (Typeface Family) ��ʶ��
*/
class LB_API FontFamily final : private noncopyable
{
public:
	using FaceMap = std::map<const StyleName, Typeface*>; //!< �������������͡�

	FontCache& Cache;

private:
	FamilyName family_name;

protected:
	FaceMap mFaces; //!< �������������͡�

public:
	/*!
	\brief ʹ�����建�����ú����ƹ������ͼ��塣
	*/
	FontFamily(FontCache&, const FamilyName&);

	/*!
	\brief �������������������������Ͷ���
	*/
	void
		operator+=(Typeface&);
	/*!
	\brief ����������������������Ƴ�ָ�����Ͷ���
	\since build 572
	*/
	bool
		operator-=(Typeface&) lnothrow;

	DefGetter(const lnothrow, const FamilyName&, FamilyName, family_name)
		/*!
		\brief ȡָ����ʽ������ָ�롣
		\note ���� Regular ��ʽʧ������ȡ Regular ��ʽ������ָ�롣
		*/
		Typeface*
		GetTypefacePtr(FontStyle) const;
	/*!
	\brief ȡָ����ʽ���Ƶ�����ָ�롣
	*/
	Typeface*
		GetTypefacePtr(const StyleName&) const;
	Typeface&
		GetTypefaceRef(FontStyle) const;
	Typeface&
		GetTypefaceRef(const StyleName&) const;
};


/*!
\brief ���ͱ�ʶ��
*/
class LB_API Typeface final : private noncopyable, private nonmovable
{
	friend class Font;
	friend class CharBitmap;

public:
	const FontPath Path;

private:
	//@{
	struct BitmapKey
	{
		unsigned Flags;
		unsigned GlyphIndex;
		FontSize Size;
		FontStyle Style;

		PDefHOp(bool, == , const BitmapKey& key) const lnothrow
			ImplRet(Flags == key.Flags && GlyphIndex == key.GlyphIndex
				&& Size == key.Size && Style == key.Style)
	};

	struct BitmapKeyHash
	{
		PDefHOp(size_t, (), const BitmapKey& key) const lnothrow
			ImplRet(hash_combine_seq(size_t(key.Style), key.Size,
				key.GlyphIndex, key.Flags))
	};

	class SmallBitmapData
	{
		friend class CharBitmap;

	private:
		/*!
		\sa ::FTC_SBitRec_
		*/
		//@{
		byte width = 255, height = 0;
		signed char left = 0, top = 0;
		byte format = 0, max_grays = 0;
		short pitch = 0;
		signed char xadvance = 0, yadvance = 0;
		byte* buffer = {};
		//@}

	public:
		//! \since build 421
		SmallBitmapData(::FT_GlyphSlot, FontStyle);
		SmallBitmapData(SmallBitmapData&&);
		~SmallBitmapData();
	};
	//@}

	long face_index;
	int cmap_index;
	StyleName style_name;
	std::pair<lref<FontFamily>, lref<::FT_FaceRec_>> ref;
	mutable used_list_cache<BitmapKey, SmallBitmapData, BitmapKeyHash>
		bitmap_cache;
	mutable std::unordered_map<ucs4_t, unsigned> glyph_index_cache;
	mutable std::unordered_map<FontSize, NativeFontSize> size_cache;

public:
	/*!
	\brief ʹ�����建��������ָ�������ļ�·����ȡָ�����������Ͳ��������
	\post ���ԣ� \c cmap_index �� face ���ܵķ�Χ�ڡ�
	*/
	Typeface(FontCache&, const FontPath&, std::uint32_t = 0);
	~Typeface();

	/*!
	\brief �Ƚϣ���ȹ�ϵ��
	*/
	bool
		operator==(const Typeface&) const;
	/*!
	\brief �Ƚϣ��ϸ����ƫ���ϵ��
	*/
	bool
		operator<(const Typeface&) const;

	DefGetterMem(const lnothrow, FamilyName, FamilyName, GetFontFamily())
		/*!
		\brief ȡ���ͼ��塣
		*/
		DefGetter(const lnothrow, const FontFamily&, FontFamily, ref.first)
		DefGetter(const lnothrow, const StyleName&, StyleName, style_name)
		/*!
		\brief ȡ�ַ�ӳ�������š�
		*/
		DefGetter(const lnothrow, int, CMapIndex, cmap_index)

private:
	//@{
	SmallBitmapData&
		LookupBitmap(const BitmapKey&) const;

	unsigned
		LookupGlyphIndex(ucs4_t) const;

	NativeFontSize&
		LookupSize(FontSize) const;

public:
	PDefH(void, ClearBitmapCache,)
		ImplExpr(bitmap_cache.clear())

		PDefH(void, ClearGlyphIndexCache,)
		ImplExpr(glyph_index_cache.clear())
		//@}

		PDefH(void, ClearSizeCache,)
		ImplExpr(size_cache.clear())
};


/*!
\brief ȡĬ���������á�
\throw logged_event ���أ��쳣�¼���
*/
LB_API const Typeface&
FetchDefaultTypeface();


/*!
\brief �ַ�λͼ��
\warning ��Ϊ��ʱ���ó�Ա����ʱ��Ϊδ���塣
*/
class LB_API CharBitmap final
{
public:
	//! \note �� \c ::FT_Byte* һ�¡�
	using BufferType = byte*;
	/*!
	\note ֵ������ \c ::FT_Pixel_Mode ��
	*/
	enum FormatType
	{
		None = 0,
		Mono,
		Gray,
		Gray2,
		Gray4,
		LCD,
		LCD_V
	};
	//! \brief �������Ͷ���ָ�����͡�
	using NativeType = Typeface::SmallBitmapData*;
	/*!
	\note �� \c ::FT_Short һ�¡�
	*/
	using PitchType = short;
	//! \note �� \c ::FT_Byte һ�¡�
	using ScaleType = byte;
	//! \note �� \c ::FT_Char һ�¡�
	using SignedScaleType = signed char;

private:
	NativeType bitmap = {};

public:
	//@{
	//! \brief ���죺��λͼ��
	DefDeCtor(CharBitmap)
		//! \brief ���죺ʹ�ñ������Ͷ���
		lconstfn
		CharBitmap(NativeType b)
		: bitmap(b)
	{}
	//@}

	lconstfn DefCvt(const lnothrow, NativeType, bitmap)

		/*!
		\pre ��Ӷ��ԣ�λͼ�������Ͷ���ǿա�
		*/
		//@{
		lconstfn DefGetter(const lnothrowv, BufferType, Buffer,
			Deref(bitmap).buffer)
		lconstfn DefGetter(const lnothrowv, FormatType, Format,
			FormatType(Deref(bitmap).format))
		lconstfn DefGetter(const lnothrowv, ScaleType, GrayLevel,
			Deref(bitmap).max_grays)
		lconstfn DefGetter(const lnothrowv, ScaleType, Height, Deref(bitmap).height)
		lconstfn DefGetter(const lnothrowv, SignedScaleType, Left,
			Deref(bitmap).left)
		lconstfn DefGetter(const lnothrowv, PitchType, Pitch, Deref(bitmap).pitch)
		lconstfn DefGetter(const lnothrowv, SignedScaleType, Top, Deref(bitmap).top)
		lconstfn DefGetter(const lnothrowv, ScaleType, Width, Deref(bitmap).width)
		lconstfn DefGetter(const lnothrowv, SignedScaleType, XAdvance,
			Deref(bitmap).xadvance)
		lconstfn DefGetter(const lnothrowv, SignedScaleType, YAdvance,
			Deref(bitmap).yadvance)
		//@}
};


/*!
\brief ���建�档
*/
class LB_API FontCache final : private noncopyable,
	private OwnershipTag<Typeface>, private OwnershipTag<FontFamily>
{
	friend class Typeface;
/*!
\brief ��Ԫ�ࣺ���� scaler �ȶ���
*/
friend class Font;

public:
	using FaceSet = std::set<Typeface*,deref_comp<const Typeface>>; \
		//!< ���������͡�
		//! \brief ���ͼ������������͡�
		using FamilyMap = std::unordered_map<FamilyName, std::unique_ptr<FontFamily>>;

	/*!
	\brief ���λ�������С��
	\note ��λΪ�ֽڡ�
	*/
	static lconstexpr const std::size_t DefaultGlyphCacheSize = 128U << 10;

private:
	::FT_Library library; //!< ��ʵ����

protected:
	FaceSet sFaces; //!< �����顣
	FamilyMap mFamilies; //!< ���ͼ�����������

	Typeface* pDefaultFace; //!< Ĭ������ָ�롣

public:
	/*!
	\brief ���죺����ָ����С�����λ���ռ䡣
	\note ��ǰ��ʱ���Բ�����
	*/
	explicit
		FontCache(std::size_t = DefaultGlyphCacheSize);
	/*!
	\brief �������ͷſռ䡣
	*/
	~FontCache();

public:
	/*!
	\brief ȡ�����顣
	*/
	DefGetter(const lnothrow, const FaceSet&, Faces, sFaces)
		DefGetter(const lnothrow, const FamilyMap&, FamilyIndices, mFamilies) \
		//!< ȡ���ͼ�����������
		//	Font*
		//	GetFontPtr() const;
		/*!
		\brief ȡָ�����Ƶ����ͼ���ָ�롣
		*/
		const FontFamily*
		GetFontFamilyPtr(const FamilyName&) const;
	/*!
	\brief ȡĬ������ָ�롣
	\throw logged_event ��¼�쳣�¼���
	*/
	const Typeface*
		GetDefaultTypefacePtr() const;
	//	Typeface*
	//	GetTypefacePtr(u16) const; //!< ȡ�����鴢���ָ������������ָ�롣
	/*!
	\brief ȡָ�����Ƶ�����ָ�롣
	*/
	const Typeface*
		GetTypefacePtr(const FamilyName&, const StyleName&) const;

private:
	/*!
	\brief �����ͼ�����������ͼ��塣
	*/
	void
		operator+=(std::unique_ptr<FontFamily>);
	/*!
	\brief ��������������Ͷ���
	*/
	void
		operator+=(Typeface&);

	/*!
	\brief �����ͼ��������Ƴ�ָ�����Ͷ���
	*/
	bool
		operator-=(FontFamily&) lnothrow;
	/*!
	\brief �����������Ƴ�ָ�����Ͷ���
	*/
	bool
		operator-=(Typeface&) lnothrow;

	/*!
	\brief ���������
	*/
	void
		ClearContainers() lnothrow;

public:
	/*!
	\brief �������ļ���������������Ϣ��
	\return �ɹ��������������
	*/
	std::size_t
		LoadTypefaces(const FontPath&);

public:
	/*!
	\brief ��ʼ��Ĭ�����͡�
	*/
	void
		InitializeDefaultTypeface();
};

/*!
\brief ���壺��ģ��������ʽ�ʹ�С��
\todo ����
*/
class LB_API Font final{
public:
	static lconstexpr const FontSize DefaultSize = 12,
		MinimalSize = 4, MaximalSize = 96;

private:
private:
	lref<Typeface> typeface;

	FontSize font_size;
	/*!
	\brief ������ʽ��
	*/
	FontStyle style;

public:
	Font()
		:Font(FetchDefaultTypeface().GetFontFamily())
	{}

	Font(const FontFamily&, FontSize = DefaultSize,
		FontStyle = FontStyle::Regular);

	DefPred(const lnothrow, Bold, bool(style & FontStyle::Bold))
	DefPred(const lnothrow, Italic, bool(style & FontStyle::Italic))
	DefPred(const lnothrow, Underline, bool(style & FontStyle::Underline))
	DefPred(const lnothrow, Strikeout, bool(style & FontStyle::Strikeout))

		/*!
		\brief ȡ��ࡣ
		*/
		std::int8_t
		GetAdvance(ucs4_t, CharBitmap = {}) const;

	/*!
	\brief ȡ������
	*/
	std::int8_t
		GetAscender() const;
	DefGetter(const lnothrow, FontCache&, Cache, GetFontFamily().Cache)
	/*!
	\brief ȡ������
	*/
	std::int8_t
		GetDescender() const;

	DefGetterMem(const lnothrow, const FamilyName&, FamilyName,
		GetFontFamily())
		DefGetterMem(const lnothrow, const FontFamily&, FontFamily, GetTypeface())
		DefGetter(const lnothrow, FontSize, Size, font_size)
		DefGetter(const lnothrow, FontStyle, Style, style)

		
		/*!
		\brief ȡ��ǰ���ͺʹ�С��Ⱦ��ָ���ַ������Ρ�
		\param c ָ����Ҫ����Ⱦ���ַ���
		\param flags FreeType ��Ⱦ��ʶ��
		\note Ĭ�ϲ���Ϊ FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL ��
		\warning ���ص�λͼ����һ�ε��� FontCache ������ײ� FreeType ����ʱ����֤��Ч��
		\warning flags ���ܱ��Ƴ���Ӧ�������ڲ�ʵ�֡�
		*/
		CharBitmap
		GetGlyph(ucs4_t c, unsigned flags = 4UL) const;
	/*!
	\brief ȡ�����Ӧ���ַ��߶ȡ�
	*/
	FontSize
		GetHeight() const lnothrow;
	DefGetter(const lnothrow, StyleName, StyleName, FetchName(style))
private:
	/*!
	\brief ȡ�ڲ���Ϣ��
	*/
	::FT_Size_Metrics
		GetInternalInfo() const;

public:
	/*!
	\brief ȡ�������á�
	\since build 280
	*/
	DefGetter(const lnothrow, Typeface&, Typeface, typeface)

	/*!
	\brief ���������С��
	*/
	void
		SetSize(FontSize = DefaultSize);
	/*!
	\brief ������ʽ��
	\note ������������ʱ������ʽ��
	*/
	bool
		SetStyle(FontStyle);
};

/*!
\brief ȡĬ�����建�档
\exception FatalError ���建���ʼ��ʧ�ܡ�
\todo Initialization.h
*/
LB_API Drawing::FontCache&
FetchDefaultFontCache();


LEO_DRAW_END

#endif
