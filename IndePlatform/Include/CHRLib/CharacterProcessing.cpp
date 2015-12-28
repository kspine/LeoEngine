/*!	\file CharacterProcessing.cpp
\ingroup CHRLib
\brief �ַ����봦��
\version r1634
\author FrankHB <frankhb1989@gmail.com>
\par ����ʱ��:
2015-11-28 17:53:21 +0800
\par �޸�ʱ��:

\par �ı�����:
UTF-8
\par ģ������:
CHRLib::CharacterProcessing
*/

#include "CharacterProcessing.h"
#include <cctype>
#include <cstdlib>
#include <cwchar>

namespace CHRLib
{

	using std::malloc;
	using std::size_t;
	using std::tolower;
	using stdex::is_null;
	using stdex::ntctslen;
	using std::make_unique;

	ConversionResult
		MBCToUC(char16_t& uc, const char*& c, Encoding enc, ConversionState&& st)
	{
		if (const auto pfun = FetchMapperPtr<ConversionResult, char16_t&, const char*&,
			ConversionState&&>(enc))
			return pfun(uc, c, std::move(st));
		return ConversionResult::Unhandled;
	}
	ConversionResult
		MBCToUC(char16_t& uc, const char*& c, const char* e, Encoding enc,
			ConversionState&& st)
	{
		lconstraint(c <= e);
		if (const auto pfun = FetchMapperPtr<ConversionResult, char16_t&,
			GuardPair<const char*>&&, ConversionState&&>(enc))
			return pfun(uc, { c, e }, std::move(st));
		return ConversionResult::Unhandled;
	}
	ConversionResult
		MBCToUC(char16_t& uc, std::FILE* fp, Encoding enc, ConversionState&& st)
	{
		lconstraint(fp);
		if (const auto pfun = FetchMapperPtr<ConversionResult, char16_t&,
			ystdex::ifile_iterator&, ConversionState&&>(enc))
		{
			ystdex::ifile_iterator i(fp);
			const auto r(pfun(uc, i, std::move(st)));

			i.sungetc(fp);
			return r;
		}
		return ConversionResult::Unhandled;
	}
	ConversionResult
		MBCToUC(const char*& c, Encoding enc, ConversionState&& st)
	{
		if (const auto pfun = FetchMapperPtr<ConversionResult,
			ystdex::pseudo_output&&, const char*&, ConversionState&&>(enc))
			return pfun(ystdex::pseudo_output(), c, std::move(st));
		return ConversionResult::Unhandled;
	}
	ConversionResult
		MBCToUC(const char*& c, const char* e, Encoding enc, ConversionState&& st)
	{
		lconstraint(c <= e);
		if (const auto pfun = FetchMapperPtr<ConversionResult,
			ystdex::pseudo_output&&, GuardPair<const char*>&&,
			ConversionState&&>(enc))
			return pfun(ystdex::pseudo_output(), { c, e }, std::move(st));
		return ConversionResult::Unhandled;
	}
	ConversionResult
		MBCToUC(std::FILE* fp, Encoding enc, ConversionState&& st)
	{
		lconstraint(fp);
		if (const auto pfun = FetchMapperPtr<ConversionResult,
			ystdex::pseudo_output&&, ystdex::ifile_iterator&,
			ConversionState&&>(enc))
		{
			ystdex::ifile_iterator i(fp);
			const auto r(pfun(ystdex::pseudo_output(), i, std::move(st)));

			i.sungetc(fp);
			return r;
		}
		return ConversionResult::Unhandled;
	}

	size_t
		UCToMBC(char* d, const char16_t& s, Encoding enc)
	{
		lconstraint(d);

		size_t l(0);

		if (const auto pfun = FetchMapperPtr<size_t, char*, char32_t>(enc))
			l = pfun(d, s);
		return l;
	}


	size_t
		MBCSToUCS2(char16_t* d, const char* s)
	{
		return MBCSToUCS2(FetchMapper_Default<ConversionResult, char16_t&,
			const char*&, ConversionState&&>(), d, s);
	}
	size_t
		MBCSToUCS2(char16_t* d, const char* s, Encoding enc)
	{
		if (const auto pfun = FetchMapperPtr<ConversionResult, char16_t&,
			const char*&, ConversionState&&>(enc))
			return MBCSToUCS2(pfun, d, s);
		else
			lconstraint(d && s);
		return 0;
	}
	size_t
		MBCSToUCS2(char16_t* d, const char* s, const char* e)
	{
		return MBCSToUCS2(FetchMapper_Default<ConversionResult, char16_t&,
			GuardPair<const char*>&&, ConversionState&&>(), d, s, e);
	}
	size_t
		MBCSToUCS2(char16_t* d, const char* s, const char* e, Encoding enc)
	{
		if (const auto pfun = FetchMapperPtr<ConversionResult, char16_t&,
			GuardPair<const char*>&&, ConversionState&&>(enc))
			return MBCSToUCS2(pfun, d, s, e);
		else
			lconstraint(d && s && e && s <= e);
		return 0;
	}

	size_t
		MBCSToUCS4(char32_t* d, const char* s)
	{
		// TODO: Use UCS-4 internal conversion directly?
		return MBCSToUCS4(FetchMapper_Default<ConversionResult, char16_t&,
			const char*&, ConversionState&&>(), d, s);
	}
	size_t
		MBCSToUCS4(char32_t* d, const char* s, Encoding enc)
	{
		// TODO: Use UCS-4 internal conversion directly?
		if (const auto pfun = FetchMapperPtr<ConversionResult, char16_t&,
			const char*&, ConversionState&&>(enc))
			return MBCSToUCS4(pfun, d, s);
		else
			lconstraint(d && s);
		return 0;
	}
	size_t
		MBCSToUCS4(char32_t* d, const char* s, const char* e)
	{
		// TODO: Use UCS-4 internal conversion directly?
		return MBCSToUCS4(FetchMapper_Default<ConversionResult, char16_t&,
			GuardPair<const char*>&&, ConversionState&&>(), d, s, e);
	}
	size_t
		MBCSToUCS4(char32_t* d, const char* s, const char* e, Encoding enc)
	{
		// TODO: Use UCS-4 internal conversion directly?
		if (const auto pfun = FetchMapperPtr<ConversionResult, char16_t&,
			GuardPair<const char*>&&, ConversionState&&>(enc))
			return MBCSToUCS4(pfun, d, s, e);
		else
			lconstraint(d && s && e && s <= e);
		return 0;
	}

	size_t
		UCS2ToMBCS(char* d, const char16_t* s)
	{
		return UCS2ToMBCS(FetchMapper_Default<size_t, char*, char32_t>(), d, s);
	}
	size_t
		UCS2ToMBCS(char* d, const char16_t* s, Encoding enc)
	{
		if (const auto pfun = FetchMapperPtr<size_t, char*, char32_t>(enc))
			return UCS2ToMBCS(pfun, d, s);
		else
			lconstraint(d && s);
		return 0;
	}
	size_t
		UCS2ToMBCS(char* d, const char16_t* s, const char16_t* e)
	{
		// TODO: Deferred. Use guard for encoding.
		return UCS2ToMBCS(FetchMapper_Default<size_t, char*, char32_t>(), d, s, e);
	}
	size_t
		UCS2ToMBCS(char* d, const char16_t* s, const char16_t* e, Encoding enc)
	{
		// TODO: Deferred. Use guard for encoding.
		if (const auto pfun = FetchMapperPtr<size_t, char*, char32_t>(enc))
			return UCS2ToMBCS(pfun, d, s, e);
		else
			lconstraint(d && s && e && s <= e);
		return 0;
	}

	size_t
		UCS2ToUCS4(char32_t* d, const char16_t* s)
	{
		const auto p(ystdex::copy_when(s, d, [](char16_t c) lnothrow{
			return !is_null(c);
		}));

		*p = char32_t();
		return size_t(p - d);
	}
	size_t
		UCS2ToUCS4(char32_t* d, const char16_t* s, const char16_t* e)
	{
		lconstraint(d),
			lconstraint(s),
			lconstraint(s <= e);

		return size_t(std::copy(s, e, d) - d);
	}

	size_t
		UCS4ToMBCS(char* d, const char32_t* s)
	{
		return UCS4ToMBCS(FetchMapper_Default<size_t, char*, char32_t>(), d, s);
	}
	size_t
		UCS4ToMBCS(char* d, const char32_t* s, Encoding enc)
	{
		if (const auto pfun = FetchMapperPtr<size_t, char*, char32_t>(enc))
			return UCS4ToMBCS(pfun, d, s);
		else
			lconstraint(d && s);
		return 0;
	}
	size_t
		UCS4ToMBCS(char* d, const char32_t* s, const char32_t* e)
	{
		// TODO: Deferred. Use guard for encoding.
		return UCS4ToMBCS(FetchMapper_Default<size_t, char*, char32_t>(), d, s, e);
	}
	size_t
		UCS4ToMBCS(char* d, const char32_t* s, const char32_t* e, Encoding enc)
	{
		// TODO: Deferred. Use guard for encoding.
		if (const auto pfun = FetchMapperPtr<size_t, char*, char32_t>(enc))
			return UCS4ToMBCS(pfun, d, s, e);
		else
			lconstraint(d && s && e && s <= e);
		return 0;
	}

	size_t
		UCS4ToUCS2(char16_t* d, const char32_t* s)
	{
		lconstraint(d),
			lconstraint(s);

		const auto p(ystdex::transform_when(s, d, [](char32_t c) lnothrow{
			return !is_null(c);
		}, [](char32_t c) lnothrow{
			return char16_t(c);
		}));

		*p = char16_t();
		return size_t(p - d);
	}
	size_t
		UCS4ToUCS2(char16_t* d, const char32_t* s, const char32_t* e)
	{
		lconstraint(d),
			lconstraint(s),
			lconstraint(s <= e);

		return size_t(std::transform(s, e, d, [](char32_t c) lnothrow{
			return char16_t(c);
		}) - d);
	}

} // namespace CHRLib;