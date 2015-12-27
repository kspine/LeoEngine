#ifndef IndePlatform_cstring_H
#define IndePlatform_cstring_H

#include "type_op.hpp" // for or_, is_same;
#include <cstring> // for std::strlen, std::strcpy, std::memchr, std::strncpy;
#include <string> // for std::char_traits;
#include "lassert.h" // for yconstraint;
#include "cctype.h" // for ystdex::tolower;
#include <cwchar> // for std::wmemchr, std::wcscpy, std::wcsncpy;

namespace stdex {

	/*!
	\ingroup unary_type_traits
	\brief �ж��ַ������Ƿ� ISO C++ ָ���ṩ <tt>std::char_traits</tt> ���ػ���
	*/
	template<typename _tChar>
	struct is_char_specialized_in_std : or_<is_same<_tChar, char>,
		is_same<_tChar, wchar_t>, is_same<_tChar, char16_t>,
		is_same<_tChar, char32_t >>
	{};

	/*!
	\brief ʹ�� <tt>std::char_traits::eq</tt> �ж��Ƿ�Ϊ���ַ���
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

			while (!stdex::is_null(*p))
				++p;
			return size_t(p - s);
		}

	} // namespace details;


	  /*!	\defgroup NTCTSUtil null-terminated character string utilities
	  \brief �� NTCTS ������
	  \note NTCTS(null-terminated character string) �����ַ���ǽ������ַ�����
	  ���˽����ַ���û�п��ַ���
	  \note ��ָ������ NTMBS(null-terminated mutibyte string) �����ȿ��ַ����ǡ�
	  \see ISO C++03 (17.1.12, 17.3.2.1.3.2) ��
	  */
	  //@{
	  /*!
	  \brief ����� NTCTS ���ȡ�
	  \pre ���ԣ� <tt>s</tt> ��
	  \note ����ͬ std::char_traits<_tChar>::length ��
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
	\brief ���㲻����ָ�����ȵļ� NTCTS ���ȡ�
	\pre ���ԣ� <tt>s</tt> ��
	\note ����ͬ std::char_traits<_tChar>::length ����������ָ��ֵ��
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