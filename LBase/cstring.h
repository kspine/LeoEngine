#ifndef LBase_cstring_h
#define LBase_cstring_h 1

#include "LBase/type_pun.hpp" // for or_, is_same;
#include "LBase/cassert.h" // for lconstraint;
#include "LBase/cctype.h" // for stdex::tolower;

#include <cstring> // for std::strlen, std::strcpy, std::memchr, std::strncpy;
#include <string> // for std::char_traits;
#include <cwchar> // for std::wmemchr, std::wcscpy, std::wcsncpy;
#include <algorithm> // for std::min, std::lexicographical_compare;

namespace leo {

	/*!
	\ingroup unary_type_traits
	\brief 判断字符类型是否被 ISO C++ 指定提供 <tt>std::char_traits</tt> 的特化。
	*/
	template<typename _tChar>
	struct is_char_specialized_in_std : or_<is_same<_tChar, char>,
		is_same<_tChar, wchar_t>, is_same<_tChar, char16_t>,
		is_same<_tChar, char32_t >>
	{};

	/*!
	\brief 指定和 \c wchar_t 互相替换存储的非 \c char 内建字符类型。
	\warning 不同类型的非空字符的值是否可以替换取决于实现定义。
	\note 若存在这样的类型，为 \c char16_t 或 \c char32_t 之一，否则为 \c void 。
	*/
	using uchar_t = cond_t<is_trivially_replaceable<wchar_t, char16_t>, char16_t,
		cond_t<is_trivially_replaceable<wchar_t, char32_t>, char32_t, void>>;

	/*!
	\brief 使用 <tt>std::char_traits::eq</tt> 判断是否为空字符。
	*/
	template<typename _tChar>
	lconstfn  bool
		is_null(_tChar c)
	{
		return std::char_traits<_tChar>::eq(c, _tChar());
	}


	namespace details
	{

		template<typename _tChar>
		inline LB_PURE size_t
			ntctslen_raw(const _tChar* s, std::true_type)
		{
			return std::char_traits<_tChar>::length(s);
		}
		template<typename _tChar>
		LB_PURE size_t
			ntctslen_raw(const _tChar* s, std::false_type)
		{
			const _tChar* p(s);

			while (!leo::is_null(*p))
				++p;
			return size_t(p - s);
		}

	} // namespace details;


	  /*!	\defgroup NTCTSUtil null-terminated character string utilities
	  \brief 简单 NTCTS 操作。
	  \note NTCTS(null-terminated character string) 即空字符标记结束的字符串，
	  除了结束字符外没有空字符。
	  \note 简单指不包括 NTMBS(null-terminated mutibyte string) ，按等宽字符考虑。
	  \see ISO C++03 (17.1.12, 17.3.2.1.3.2) 。
	  */
	  //@{
	  /*!
	  \brief 计算简单 NTCTS 长度。
	  \pre 断言： <tt>s</tt> 。
	  \note 语义同 std::char_traits<_tChar>::length 。
	  */
	template<typename _tChar>
	inline LB_PURE size_t
		ntctslen(const _tChar* s)
	{
		lconstraint(s);

		return details::ntctslen_raw(s,
			typename is_char_specialized_in_std<_tChar>::type());
	}

	/*!
	\brief 计算不超过指定长度的简单 NTCTS 长度。
	\pre 断言： <tt>s</tt> 。
	\note 语义同 std::char_traits<_tChar>::length ，但不超过指定值。
	\since build 604
	*/
	//@{
	template<typename _tChar>
	LB_PURE size_t
		ntctsnlen(const _tChar* s, size_t n)
	{
		lconstraint(s);

		const auto str(s);

		while (n-- != 0 && *s)
			++s;

		return s - str;
	}
	inline LB_PURE size_t
		ntctsnlen(const char* s, size_t n)
	{
		lconstraint(s);

		const auto p(static_cast<const char*>(std::memchr(s, char(), n)));

		return p ? size_t(p - s) : n;
	}
	inline LB_PURE size_t
		ntctsnlen(const wchar_t* s, size_t n)
	{
		lconstraint(s);

		const auto p(static_cast<const wchar_t*>(std::wmemchr(s, char(), n)));

		return p ? size_t(p - s) : n;
	}
	//@}

}

#endif