/*!	\file string.hpp
\ingroup LBase
\brief ISO C++ ��׼�ַ�����չ��
*/

#ifndef LBase_string_hpp
#define LBase_string_hpp 1

#include "LBase/memory.hpp" // for decay_t, remove_rcv_t, std::declval,
//	nested_allocator, is_enum, is_class;
#include "LBase/container.hpp" // for "container.hpp", make_index_sequence,
//	index_sequence, begin, end, sort_unique, size, underlying;
#include "LBase/cstdio.h" // for lconstraint, vfmtlen;
#include "LBase/cstring.h" // for ntctslen;
#include "LBase/array.hpp" // for std::bidirectional_iterator_tag, to_array;
#include <istream> // for std::basic_istream;
#include "ios.hpp" // for rethrow_badstate;
#include <ostream> // for std::basic_ostream;
#include <sstream> // for std::ostringstream, std::wostring_stream;
#include <cstdarg> // for std::va_list;

namespace leo
{
	template<typename _tString>
	//�ַ�������
	struct string_traits
	{
		using string_type = decay_t < _tString >;
		using value_type = remove_rcv_t < decltype(std::declval<string_type>()[0]) >;
		using traits_type = typename std::char_traits < value_type >;
		//! \since build 1.4
		//@{
		using allocator_type = _t<nested_allocator<string_type>>;
		using size_type = typename std::allocator_traits<allocator_type>::size_type;
		using difference_type
			= typename std::allocator_traits<allocator_type>::difference_type;
		using reference = value_type&;
		using const_reference = const value_type&;
		//@}
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using initializer = std::initializer_list < value_type >;
	};


	//! \since build 1.3
	namespace details
	{

		//! \since build 1.4
		//@{
		template<typename _type, typename = void>
		struct is_string_like : false_type
		{};

		template<typename _type>
		struct is_string_like<_type, enable_if_t<
			is_object<decay_t<decltype(std::declval<_type>()[0])>>::value>> : true_type
		{};
		//@}


		//! \todo ֧�� std::forward_iterator_tag ���ء�
		template<typename _tFwd1, typename _tFwd2, typename _fPred>
		bool
			ends_with_iter_dispatch(_tFwd1 b, _tFwd1 e, _tFwd2 bt, _tFwd2 et,
				_fPred comp, std::bidirectional_iterator_tag)
		{
			auto i(e);
			auto it(et);

			while (i != b && it != bt)
				if (!comp(*--i, *--it))
					return{};
			return it == bt;
		}


		//! \since build 1.4
		//@{
		template<size_t>
		struct str_algos;

		template<>
		struct str_algos<0>
		{
			//! \since build 1.4
			template<class _tString,
				typename _tSize = typename string_traits<_tString>::size_type>
				static lconstfn auto
				erase_left(_tString& s, _tSize n) lnothrowv
				->limpl(decltype(s.remove_prefix(n), s))
			{
				return lconstraint(n < s.size() || n == _tSize(-1)),
					s.remove_prefix(n != _tSize(-1) ? n : _tSize(s.size())), s;
			}

			//! \since build 1.4
			template<class _tString,
				typename _tSize = typename string_traits<_tString>::size_type>
				static lconstfn auto
				erase_right(_tString& s, _tSize n) lnothrowv
				->limpl(decltype(s.remove_suffix(n), s))
			{
				return lconstraint(n < s.size() || n == _tSize(-1)),
					s.remove_suffix(s.size() - n - 1), s;
			}
		};

		template<>
		struct str_algos<1>
		{
			//! \since build 1.4
			template<class _tString,
				typename _tSize = typename string_traits<_tString>::size_type>
				static lconstfn auto
				erase_left(_tString& s, _tSize n) lnothrowv
				->limpl(decltype(s.erase(0, n)))
			{
				return lconstraint(n < s.size() || n == _tSize(-1)), s.erase(0, n);
			}

			//! \since build 1.4
			template<class _tString,
				typename _tSize = typename string_traits<_tString>::size_type>
				static lconstfn auto
				erase_right(_tString& s, _tSize n) lnothrowv
				->limpl(decltype(s.erase(n + 1)))
			{
				return lconstraint(n < s.size() || n == _tSize(-1)), s.erase(n + 1);
			}
		};


		template<limpl(typename = make_index_sequence<2>)>
		struct str_algo;

		template<>
		struct str_algo<index_sequence<>>
		{
			void
				erase_left() = delete;

			void
				erase_right() = delete;
		};

		template<size_t _vIdx, size_t... _vSeq>
		struct str_algo<index_sequence<_vIdx, _vSeq...>>
			: str_algos<_vIdx>, str_algo<index_sequence<_vSeq...>>
		{
			using str_algos<_vIdx>::erase_left;
			using str_algo<index_sequence<_vSeq...>>::erase_left;
			using str_algos<_vIdx>::erase_right;
			using str_algo<index_sequence<_vSeq...>>::erase_right;
		};
		//@}

	} // unnamed namespace;


	  //! \ingroup unary_type_traits
	  //@{
	  /*!
	  \brief �ж�ָ�������Ƿ�Ϊ���ַ������͡�
	  \since build 1.4
	  */
	template<typename _type>
	struct is_string_like : details::is_string_like<_type>
	{};

	/*!
	\brief �ж�ָ�������Ƿ�Ϊ�ַ��������͡�
	\since build 1.4
	*/
	template<typename _type>
	struct is_string_class : and_<is_class<_type>, is_string_like<_type>>
	{};
	//@}

	/*!
	\ingroup metafunctions
	\brief ѡ���ַ��������͵��ض����ر�����������ַ������ͳ�ͻ��
	\sa enable_if_t
	\since build 1.4
	*/
	template<typename _tParam, typename _type = void>
	using enable_for_string_class_t
		= enable_if_t<is_string_class<decay_t<_tParam>>::value, _type>;

	/*!
	\note ʹ�� ADL �����ַ�����Χ��
	\note ͬ std::begin �� std::end �����ַ�������⡣
	\note �˴� string_end ����� boost::end ��ͬ�������������Ͳ�ͬ�� std::end ��
	\bug decltype ָ���ķ������Ͳ���ʹ�� ADL ��
	\see WG21 N3936 20.4.7[iterator.range] ��
	\since build 1.4
	*/
	//@{
	template<class _tRange>
	lconstfn auto
		string_begin(_tRange& c) -> decltype(c.begin())
	{
		return begin(c);
	}
	template<class _tRange>
	lconstfn auto
		string_begin(const _tRange& c) -> decltype(c.begin())
	{
		return begin(c);
	}
	//! \since build 1.4
	//@{
	template<typename _tChar>
	lconstfn _tChar*
		string_begin(_tChar* str) lnothrow
	{
		return lconstraint(str), str;
	}
#if __cplusplus <= 201402L
	//! \see http://wg21.cmeerw.net/cwg/issue1591 ��
	template<typename _tElem>
	lconstfn auto
		string_begin(std::initializer_list<_tElem> il) -> decltype(il.begin())
	{
		return il.begin();
	}
#endif
	//@}

	template<class _tRange>
	lconstfn auto
		string_end(_tRange& c) -> decltype(c.end())
	{
		return end(c);
	}
	template<class _tRange>
	lconstfn auto
		string_end(const _tRange& c) -> decltype(c.end())
	{
		return end(c);
	}
	//! \since build 1.4
	//@{
	template<typename _tChar>
	lconstfn _tChar*
		string_end(_tChar* str) lnothrow
	{
		return str + ntctslen(str);
	}
#if __cplusplus <= 201402L
	//! \see http://wg21.cmeerw.net/cwg/issue1591 ��
	template<typename _tElem>
	lconstfn auto
		string_end(std::initializer_list<_tElem> il) -> decltype(il.end())
	{
		return il.end();
	}
#endif
	//@}
	//@}

	/*!	\defgroup string_algorithms String Algorithms
	\addtogroup algorithms
	\brief �ַ����㷨��
	\note ģ���βιؼ��� \c class ��ʾ��֧�������Ͷ����ַ�����
	*/
	//@{
	/*!
	\breif �����ַ������ȡ�
	*/
	//@{
	template<typename _type>
	inline size_t
		string_length(const _type* str) lnothrow
	{
		return std::char_traits<_type>::length(str);
	}
	template<class _type>
	lconstfn auto
		//�ַ�������
		string_length(const _type& str)->decltype(size(str))
	{
		return size(str);
	}
#if __cplusplus <= 201402L
	//! \see http://wg21.cmeerw.net/cwg/issue1591 ��
	template<typename _tElem>
	lconstfn size_t
		string_length(std::initializer_list<_tElem> il)
	{
		return il.size();
	}
#endif
	//@}


	/*!
	\note ʹ�� ADL string_begin �� string_end ָ����Χ��������
	\note �� ADL ��ӿ�ͬ Boost.StringAlgo ��
	\sa leo::string_begin, leo::string_end
	\since build 1.4
	*/
	//@{
	//! \brief �жϵ�һ������ָ���Ĵ��Ƿ��Եڶ���������ʼ��
	//@{
	//! \since build 519
	template<typename _tRange1, typename _tRange2, typename _fPred>
	bool
		begins_with(const _tRange1& input, const _tRange2& test, _fPred comp)
	{
		return leo::string_length(test) <= leo::string_length(input)
			&& std::equal(string_begin(test), string_end(test),
				string_begin(input), comp);
	}
	//! \since build 1.4
	template<typename _tRange1, typename _tRange2>
	inline bool
		begins_with(const _tRange1& input, const _tRange2& test)
	{
		return leo::begins_with(input, test, is_equal());
	}
	//@}

	//! \brief �жϵ�һ������ָ���Ĵ��Ƿ��Եڶ�����������
	//@{
	template<typename _tRange1, typename _tRange2, typename _fPred>
	inline bool
		ends_with(const _tRange1& input, const _tRange2& test, _fPred comp)
	{
		// NOTE: See $2014-07 @ %Documentation::Workflow::Annual2014.
		return details::ends_with_iter_dispatch(string_begin(input),
			string_end(input), string_begin(test), string_end(test), comp, typename
			std::iterator_traits<remove_reference_t<decltype(string_begin(input))>>
			::iterator_category());
	}
	template<typename _tRange1, typename _tRange2>
	inline bool
		ends_with(const _tRange1& input, const _tRange2& test)
	{
		return leo::ends_with(input, test, is_equal());
	}
	//@}
	//@}

	/*!
	\brief �ж��Ƿ�����Ӵ���
	\since build 1.4
	\todo �������������Ӵ����ذ汾���Ż������⹹���Ӵ�����
	*/
	//@{
	template<class _tString, typename _type>
	inline bool
		exists_substr(const _tString& str, const _type& sub)
	{
		return str.find(sub) != _tString::npos;
	}
	template<class _tString, typename _type>
	inline bool
		exists_substr(const _tString& str, const typename _tString::value_type* p_sub)
	{
		lconstraint(p_sub);

		return str.find(p_sub) != _tString::npos;
	}
	//@}


	//! \ingroup NTCTSUtil
	//@{
	/*!
	\brief NTCTS ���滯���������ַ�֮ǰ���ַ���
	\post <tt>str.length() == leo::ntctslen(str.c_str())</tt> ��
	\since build 606
	*/
	template<class _tString>
	auto
		normalize(_tString& str) -> decltype(str.resize(leo::ntctslen(str.c_str())))
	{
		return str.resize(leo::ntctslen(str.c_str()));
	}

	/*!
	\brief ���Ʋ�����ָ�����ȵ� NTCTS ��
	\note Ŀ���ַ�������ָ�����ȵĲ��ֿ��ܱ������ַ���
	\warning Դ�ַ�����ָ��������û�п��ַ���Ŀ���ַ�������֤�Կ��ַ���β��
	\since build 604
	*/
	//@{
	//! \pre ���ԣ�ָ������ǿա�
	template<class _tString,
		limpl(typename = leo::enable_for_string_class_t<_tString>)>
		inline _tString&&
		ntctsncpy(_tString&& str, const typename string_traits<_tString>::const_pointer
			s, const typename string_traits<_tString>::size_type n)
	{
		lconstraint(s);
		return static_cast<_tString&&>(str = decay_t<_tString>(s, n));
	}
	template<class _tString,
		limpl(typename = leo::enable_for_string_class_t<_tString>)>
		inline _tString&&
		ntctsncpy(_tString&& str, _tString&& s,
			const typename string_traits<_tString>::size_type n)
	{
		auto sub(lforward(s));

		sub.resize(n);
		return static_cast<_tString&&>(str = std::move(sub));
	}
	//@}
	//@}


	/*!
	\brief ���Ƴ��Ȳ�����ָ��������
	\since build 1.4
	*/
	template<class _tString>
	inline _tString&&
		restrict_length(_tString&& str,
			const typename string_traits<_tString>::size_type n)
	{
		if (n < str.length())
			str.resize(n);
		return static_cast<_tString&&>(str);
	}

	/*!
	\brief ȡ��ĸ����������ַ����Ĳ��ظ�����
	*/
	template<class _tString>
	_tString
		alph(_tString& str)
	{
		_tString res(str);

		leo::sort_unique(res);
		return res;
	}

	/*!
	\brief �ظ����ӡ�
	\param str �����ӵ��ַ��������á�
	\param n ���ӽ������ԭ�ַ������ظ�������
	\pre ���ԣ� <tt>1 < n</tt> ��
	\todo ��� reserve ��
	*/
	template<class _tString>
	void
		concat(_tString& str, size_t n)
	{
		lconstraint(n != 0);
		if (1 < n)
		{
			const auto len(str.length());

			//����˹ũ���㷨
			leo::concat(str, n / 2);
			str.append(str.data(), str.length());
			if (n % 2 != 0)
				str.append(str.data(), len);
		}
	}

	//!\pre �ַ����������� std::basic_string �� basic_string_view �Ĳ������쳣�淶��
	//@{
	//! \since build 1.4
	//@{
	//! \brief ɾ���ַ�����ָ���������ַ������ַ�����
	//@{
	template<class _tString>
	inline limpl(enable_if_t) < is_class<decay_t<_tString>>::value, _tString&& >
		erase_left(typename string_traits<_tString>::size_type pos, _tString&& str)
	{
		return static_cast<_tString&&>(pos != decay_t<_tString>::npos
			? details::str_algo<>::erase_left(str, pos) : str);
	}
	template<class _tString>
	inline limpl(enable_if_t) < is_class<decay_t<_tString>>::value, _tString&& >
		erase_left(_tString&& str, typename string_traits<_tString>::value_type c)
	{
		return static_cast<_tString&&>(leo::erase_left(str.find_last_of(c), str));
	}
	template<class _tString>
	inline limpl(enable_if_t) < is_class<decay_t<_tString>>::value, _tString&& >
		erase_left(_tString&& str, const remove_reference_t<_tString>& t)
	{
		return static_cast<_tString&&>(leo::erase_left(str.find_last_of(t), str));
	}
	/*!
	\pre ���ԣ�ָ������ǿա�
	\since build 1.4
	*/
	template<class _tString>
	inline limpl(enable_if_t) < is_class<decay_t<_tString>>::value, _tString&& >
		erase_left(_tString&& str, typename string_traits<_tString>::const_pointer t
			= &to_array<typename string_traits<_tString>::value_type>(" \f\n\r\t\v")[0])
	{
		lconstraint(t);
		return static_cast<_tString&&>(leo::erase_left(str.find_last_of(t), str));
	}
	//@}

	//! \brief ɾ���ַ�����ָ���������ַ��Ҳ���ַ�����
	//@{
	template<class _tString>
	inline limpl(enable_if_t) < is_class<decay_t<_tString>>::value, _tString&& >
		erase_right(typename string_traits<_tString>::size_type pos, _tString&& str)
	{
		return static_cast<_tString&&>(pos != decay_t<_tString>::npos
			? details::str_algo<>::erase_right(str, pos) : str);
	}
	template<class _tString>
	inline limpl(enable_if_t) < is_class<decay_t<_tString>>::value, _tString&& >
		erase_right(_tString&& str, typename string_traits<_tString>::value_type c)
	{
		return static_cast<_tString&&>(leo::erase_right(str.find_last_of(c),
			str));
	}
	//! \since build 1.4
	template<class _tString>
	inline limpl(enable_if_t) < is_class<decay_t<_tString>>::value, _tString&& >
		erase_right(_tString&& str, const remove_reference_t<_tString>& t)
	{
		return static_cast<_tString&&>(leo::erase_right(str.find_last_of(t),
			str));
	}
	/*!
	\pre ���ԣ�ָ������ǿա�
	\since build 1.4
	*/
	template<class _tString>
	inline limpl(enable_if_t) < is_class<decay_t<_tString>>::value, _tString&& >
		erase_right(_tString&& str, typename string_traits<_tString>::const_pointer t
			= &to_array<typename string_traits<_tString>::value_type>(" \f\n\r\t\v")[0])
	{
		lconstraint(t);
		return static_cast<_tString&&>(leo::erase_right(str.find_last_of(t),
			str));
	}
	//@}
	//@}

	//! \since build 1.3
	//@{
	//! \brief ɾ���ַ�����ָ��������ǰ׺�ַ���
	//@{
	template<class _tString>
	lconstfn _tString&&
		ltrim(_tString&& str, typename string_traits<_tString>::value_type c)
	{
		return static_cast<_tString&&>(
			details::str_algo<>::erase_left(str, str.find_first_not_of(c)));
	}
	//! \since build 1.4
	template<class _tString>
	inline _tString&&
		ltrim(_tString&& str, const _tString& t)
	{
		return static_cast<_tString&&>(
			details::str_algo<>::erase_left(str, str.find_first_not_of(t)));
	}
	/*!
	\pre ���ԣ�ָ������ǿա�
	\since build 1.4
	*/
	template<class _tString>
	lconstfn _tString&&
		ltrim(_tString&& str, typename string_traits<_tString>::const_pointer t
			= &to_array<typename string_traits<_tString>::value_type>(" \f\n\r\t\v")[0])
	{
		return lconstraint(t), static_cast<_tString&&>(
			details::str_algo<>::erase_left(str, str.find_first_not_of(t)));
	}
	//@}

	//! \brief ɾ���ַ�����ָ����������׺�ַ���
	//@{
	template<class _tString>
	lconstfn _tString&&
		rtrim(_tString&& str, typename string_traits<_tString>::value_type c)
	{
		return static_cast<_tString&&>(
			details::str_algo<>::erase_right(str, str.find_last_not_of(c)));
	}
	//! \since build 1.4
	template<class _tString>
	lconstfn _tString&&
		rtrim(_tString&& str, const remove_reference_t<_tString>& t) lnothrowv
	{
		return static_cast<_tString&&>(
			details::str_algo<>::erase_right(str, str.find_last_not_of(t)));
	}
	/*!
	\pre ���ԣ�ָ������ǿա�
	\since build 1.4
	*/
	template<class _tString>
	lconstfn _tString&&
		rtrim(_tString&& str, typename string_traits<_tString>::const_pointer t
			= &to_array<typename string_traits<_tString>::value_type>(" \f\n\r\t\v")[0])
	{
		return lconstraint(t), static_cast<_tString&&>(
			details::str_algo<>::erase_right(str, str.find_last_not_of(t)));
	}
	//@}

	//! \brief ɾ���ַ�����ָ��������ǰ׺���׺�ַ���
	//@{
	template<class _tString>
	lconstfn _tString&&
		trim(_tString&& str, typename string_traits<_tString>::value_type c)
	{
		return lforward(leo::ltrim(lforward(leo::rtrim(lforward(str), c)), c));
	}
	//! \since build 1.4
	template<class _tString>
	lconstfn _tString&&
		trim(_tString&& str, const _tString& t) lnothrowv
	{
		return lforward(leo::ltrim(lforward(leo::rtrim(lforward(str), t)), t));
	}
	/*!
	\pre ���ԣ�ָ������ǿա�
	\since build 1.4
	*/
	template<class _tString>
	lconstfn _tString&&
		trim(_tString&& str, typename string_traits<_tString>::const_pointer t
			= &to_array<typename string_traits<_tString>::value_type>(" \f\n\r\t\v")[0])
	{
		return lconstraint(t),
			lforward(leo::ltrim(lforward(leo::rtrim(lforward(str), t)), t));
	}
	//@}
	//@}
	//@}


	//! \since build 1.4
	//@{
	//! \brief ȡǰ׺���ַ�����
	//@{
	template<typename _tString, typename... _tParams>
	inline _tString
		find_prefix(const _tString& str, _tParams&&... args)
	{
		if (!str.empty())
		{
			const auto pos(str.find(lforward(args)...));

			if (pos != _tString::npos)
				return str.substr(0, pos);
		}
		return{};
	}
	//@}

	//! \brief ȡ��׺���ַ�����
	//@{
	template<typename _tString, typename... _tParams>
	inline _tString
		find_suffix(const _tString& str, _tParams&&... args)
	{
		if (!str.empty())
		{
			const auto pos(str.rfind(lforward(args)...));

			if (pos != _tString::npos)
				return str.substr(pos + 1);
		}
		return{};
	}
	//@}
	//@}

	/*!
	\brief ȡ��ͬ��ǰ׺�ͺ�׺Ԫ�ء�
	\pre ȡǰ׺�ͺ�׺�����޸����á�
	\pre �ַ����ǿա�
	\since build 1.4
	\todo ���ǳ�Ա \c front �� back \c ��
	\todo ֧��ǰ׺�ͺ�׺�ַ�����
	\todo ��չ��һ��������
	*/
	//@{
	template<typename _tString>
	lconstfn typename string_traits<_tString>::value_type
		get_quote_mark_nonstrict(const _tString& str)
	{
		return str.front() == str.back() ? str.front()
			: typename string_traits<_tString>::value_type();
	}
	//@}

	/*!
	\brief ȡ����ǰ׺�ͺ�׺���ַ�����
	\pre ���ԣ�ɾ�����ַ��������ڴ�����
	\since build 1.4
	*/
	//@{
	template<typename _tString>
	inline _tString
		quote(const _tString& str, typename string_traits<_tString>::value_type c
			= typename string_traits<_tString>::value_type('"'))
	{
		return c + str + c;
	}
	template<typename _tString, typename _tString2>
	inline limpl(enable_if_t)<!is_convertible<_tString2,
		typename string_traits<_tString>::value_type>::value, _tString>
		quote(const _tString& str, const _tString2& s)
	{
		return s + str + s;
	}
	template<typename _tString, typename _tPrefix, typename _tSuffix>
	inline _tString
		quote(const _tString& str, const _tPrefix& pfx, const _tSuffix& sfx)
	{
		return pfx + str + sfx;
	}
	//@}

	/*!
	\brief ȡɾ��ǰ׺�ͺ�׺�����ַ�����
	\pre ���ԣ�ɾ�����ַ��������ڴ�����
	*/
	//@{
	template<typename _tString>
	inline _tString
		get_mid(const _tString& str, typename _tString::size_type l = 1)
	{
		lassume(!(str.size() < l * 2));
		return str.substr(l, str.size() - l * 2);
	}
	template<typename _tString>
	inline _tString
		get_mid(const _tString& str, typename _tString::size_type l,
			typename _tString::size_type r)
	{
		lassume(!(str.size() < l + r));
		return str.substr(l, str.size() - l - r);
	}
	//@}
	//@}


	/*!
	\brief ����������ȡ�ַ�����
	\since build 1.4
	*/
	//@{
	//! \note ͬ std::getline �����жϷָ����������ĸ������ɲ����ĺ������������
	template<typename _tChar, class _tTraits, class _tAlloc, typename _func>
	std::basic_istream<_tChar, _tTraits>&
		extract(std::basic_istream<_tChar, _tTraits>& is,
			std::basic_string<_tChar, _tTraits, _tAlloc>& str, _func f)
	{
		std::string::size_type n(0);
		auto st(std::ios_base::goodbit);

		if (const auto k
			= typename std::basic_istream<_tChar, _tTraits>::sentry(is, true))
		{
			const auto msize(str.max_size());
			const auto p_buf(is.rdbuf());

			lassume(p_buf);
			str.erase();
			try
			{
				const auto eof(_tTraits::eof());

				for (auto c(p_buf->sgetc()); ; c = p_buf->snextc())
				{
					if (_tTraits::eq_int_type(c, eof))
					{
						st |= std::ios_base::eofbit;
						break;
					}
					if (f(c, *p_buf))
					{
						++n;
						break;
					}
					if (!(str.length() < msize))
					{
						st |= std::ios_base::failbit;
						break;
					}
					str.append(1, _tTraits::to_char_type(c));
					++n;
				}
			}
			catch (...)
			{
				// NOTE: See http://wg21.cmeerw.net/lwg/issue91.
				rethrow_badstate(is, std::ios_base::badbit);
			}
		}
		if (n == 0)
			st |= std::ios_base::failbit;
		if (st)
			is.setstate(st);
		return is;
	}

	//! \note ͬ std::getline �����ַ�����β�����ָ�����
	//@{
	template<typename _tChar, class _tTraits, class _tAlloc>
	std::basic_istream<_tChar, _tTraits>&
		extract_line(std::basic_istream<_tChar, _tTraits>& is,
			std::basic_string<_tChar, _tTraits, _tAlloc>& str, _tChar delim)
	{
		const auto d(_tTraits::to_int_type(delim));

		return leo::extract(is, str,
			[d, &str](typename std::basic_istream<_tChar, _tTraits>::int_type c,
				std::basic_streambuf<_tChar, _tTraits>& sb) -> bool {
			if (_tTraits::eq_int_type(c, d))
			{
				sb.sbumpc();
				// NOTE: If not appended here, this function template shall be
				//	equivalent with %std::getline.
				str.append(1, d);
				return true;
			}
			return{};
		});
	}
	template<typename _tChar, class _tTraits, class _tAlloc>
	inline std::basic_istream<_tChar, _tTraits>&
		extract_line(std::basic_istream<_tChar, _tTraits>& is,
			std::basic_string<_tChar, _tTraits, _tAlloc>& str)
	{
		return leo::extract_line(is, str, is.widen('\n'));
	}

	/*!
	\brief ͬ \c leo::extract_line ���������ָ����������ӵ�ǰ׺�ַ���
	\note Ĭ�� \c LF ��Ϊ��׼�ָ�����ǰ׺Ϊ \c CR �������� \c LF �� <tt>CR+LF</tt> ��
	\note һ����϶����Ʒ�ʽ�򿪵���ʹ�ã��Ա��ⲻ��Ҫ�ķָ���ת����
	*/
	template<typename _tChar, class _tTraits, class _tAlloc>
	std::basic_istream<_tChar, _tTraits>&
		extract_line_cr(std::basic_istream<_tChar, _tTraits>& is,
			std::basic_string<_tChar, _tTraits, _tAlloc>& str, _tChar delim_cr = '\r',
			_tChar delim = '\n')
	{
		const auto cr(_tTraits::to_int_type(delim_cr));
		const auto d(_tTraits::to_int_type(delim));

		return leo::extract(is, str,
			[cr, d, &str](typename std::basic_istream<_tChar, _tTraits>::int_type c,
				std::basic_streambuf<_tChar, _tTraits>& sb) -> bool {
			if (_tTraits::eq_int_type(c, d))
				str.append(1, d);
			else if (_tTraits::eq_int_type(c, cr)
				&& _tTraits::eq_int_type(sb.sgetc(), d))
			{
				sb.sbumpc();
				str.append(1, cr);
				str.append(1, d);
			}
			else
				return{};
			sb.sbumpc();
			return true;
		});
	}
	//@}
	//@}

	/*!
	\brief �Ǹ�ʽ�����
	\since build 1.4
	*/
	//@{
	template<typename _tChar, class _tString>
	std::basic_ostream<_tChar, typename _tString::traits_type>&
		write(std::basic_ostream<_tChar, typename _tString::traits_type>& os,
			const _tString& str, typename _tString::size_type pos = 0,
			typename _tString::size_type n = _tString::npos)
	{
		const auto len(str.length());

		if (pos < len)
			os.write(&str[pos], std::streamsize(std::min(n, len - pos)));
		return os;
	}
	//! \since build 1.4
	template<typename _tChar, class _tTraits, size_t _vN>
	std::basic_ostream<_tChar, _tTraits>&
		write(std::basic_ostream<_tChar, _tTraits>& os, const _tChar(&s)[_vN])
	{
		return os.write(std::addressof(s[0]), std::streamsize(size(s)));
	}

	/*!
	\note ����������Ϊ�ַ�����������
	\since build 1.4
	*/
	template<typename _tChar, class _tTraits, size_t _vN>
	std::basic_ostream<_tChar, _tTraits>&
		write_literal(std::basic_ostream<_tChar, _tTraits>& os, const _tChar(&s)[_vN])
	{
		static_assert(0 < _vN, "Empty string literal found.");

		return
			os.write(std::addressof(s[0]), std::streamsize(size(s) - 1));
	}
	//@}

	/*!
	\brief ת��Ϊ�ַ����� std::basic_string ��ʵ������
	\note �����׼���ͬ�����������Ա���ĳЩ����ת�����棬�� G++ �� [-Wsign-promo] ��
	*/
	//@{
	//! \since build 1.3
	//@{
	inline std::string
		to_string(unsigned char val)
	{
		return std::to_string(unsigned(val));
	}	
	inline std::string
		to_string(unsigned short val)
	{
		return std::to_string(unsigned(val));
	}
	template<typename _type>
	inline limpl(enable_if_t)<is_enum<_type>::value,std::string>
		to_string(_type val)
	{
		using std::to_string;
		using leo::to_string;

		return to_string(underlying(val));
	}
	template<typename _type, class _tStream = std::ostringstream>
	limpl(enable_if_t)<is_class<_type>::value,
		decltype(std::declval<_tStream&>().str())>
		to_string(const _type& x)
	{
		_tStream oss;

		oss << x;
		return oss.str();
	}
	inline std::string
		to_string(const std::string& str)
	{
		return str;
	}
	//@}
	

	//! \since build 1.4
	//@{
	inline std::wstring
		to_wstring(unsigned char val)
	{
		return std::to_wstring(unsigned(val));
	}
	inline std::wstring
		to_wstring(unsigned short val)
	{
		return std::to_wstring(unsigned(val));
	}
	//! \since build 1.4
	template<typename _type>
	inline limpl(enable_if_t)<is_enum<_type>::value, std::wstring>
		to_wstring(_type val)
	{
		using std::to_wstring;
		using leo::to_wstring;

		return to_wstring(leo::underlying(val));
	}
	template<typename _type, class _tStream = std::wostringstream>
	limpl(enable_if_t)<is_class<_type>::value,
		decltype(std::declval<_tStream&>().str())>
		to_wstring(const _type& x)
	{
		_tStream oss;

		oss << x;
		return oss.str();
	}
	inline std::wstring
		to_wstring(const std::wstring& str)
	{
		return str;
	}
	//@}
	//@}

	namespace details
	{

		template<typename _tString, typename _type>
		struct ston_dispatcher;

#define LB_Impl_String_ston_begin(_tString, _type, _n, ...) \
	template<> \
	struct ston_dispatcher<_tString, _type> \
	{ \
		static _type \
		cast(const _tString& str, __VA_ARGS__) \
		{ \
			return _n(str
#define LB_Impl_String_ston_end \
			); \
		} \
	};

#define LB_Impl_String_ston_i(_tString, _type, _n) \
	LB_Impl_String_ston_begin(_tString, _type, _n, size_t* idx = {}, \
		int base = 10), idx, base \
	LB_Impl_String_ston_end
#define LB_Impl_String_ston_i_std(_type, _n) \
	LB_Impl_String_ston_i(std::string, _type, std::_n)
		LB_Impl_String_ston_i_std(int, stoi)
			LB_Impl_String_ston_i_std(long, stol)
			LB_Impl_String_ston_i_std(unsigned long, stoul)
			LB_Impl_String_ston_i_std(long long, stoll)
			LB_Impl_String_ston_i_std(unsigned long long, stoull)
#undef LB_Impl_String_ston_i_std
#undef LB_Impl_String_ston_i

#define LB_Impl_String_ston_f(_tString, _type, _n) \
	LB_Impl_String_ston_begin(_tString, _type, _n, size_t* idx = {}), idx \
	LB_Impl_String_ston_end
#define LB_Impl_String_ston_f_std(_type, _n) \
	LB_Impl_String_ston_f(std::string, _type, std::_n)
#	ifndef __BIONIC__
			LB_Impl_String_ston_f_std(float, stof)
			LB_Impl_String_ston_f_std(double, stod)
			LB_Impl_String_ston_f_std(long double, stold)
#	endif
#undef LB_Impl_String_ston_f_std
#undef LB_Impl_String_ston_f

#undef LB_Impl_String_ston_end
#undef LB_Impl_String_ston_begin

	} // namespace details;


	/*!
	\brief ת����ʾ�����ַ�����
	\since build 1.4
	\todo ֧�� std::string ���������ַ�����
	\todo ֧�ֱ�׼�������ת����
	*/
	template<typename _type, typename _tString, typename... _tParams>
	inline _type
		ston(const _tString& str, _tParams&&... args)
	{
		return details::ston_dispatcher<decay_t<_tString>, _type>::cast(str,
			lforward(args)...);
	}


	/*!
	\pre ��Ӷ��ԣ���һ�����ǿա�
	\since build 1.4
	\bug \c char �����ģ���������ȷʵ�֡�
	*/
	//@{
	/*!
	\brief �� C ��׼�����ʽ����� std::basic_string ʵ���Ķ���
	\throw std::runtime_error ��ʽ���ַ������ʧ�ܡ�
	\note �� _tString �����쳣������
	*/
	template<typename _tChar, class _tString = std::basic_string<_tChar>>
	LB_NONNULL(1) _tString
		vsfmt(const _tChar* fmt, std::va_list args)
	{
		std::va_list ap;

		va_copy(ap, args);

		const auto l(leo::vfmtlen(fmt, ap));

		va_end(ap);
		if (l == size_t(-1))
			throw std::runtime_error("Failed to write formatted string.");

		_tString str(l, _tChar());

		if (l != 0)
		{
			lassume(str.length() > 0 && str[0] == _tChar());
			std::vsprintf(&str[0], fmt, args);
		}
		return str;
	}
	
	/*!
	\brief �� C ��׼�����ʽ����� std::basic_string ʵ���Ķ���
	\note ʹ�� ADL ���ʿɱ������
	\note Clang++ ��ģ������ attribute ֱ����ʾ��ʽ�ַ������ʹ���
	*/
	template<typename _tChar>
	LB_NONNULL(1) auto
		sfmt(const _tChar* fmt, ...)
		-> decltype(vsfmt(fmt, std::declval<std::va_list>()))
	{
		std::va_list args;

		va_start(args, fmt);
		try
		{
			auto str(vsfmt(fmt, args));

			va_end(args);
			return str;
		}
		catch (...)
		{
			va_end(args);
			throw;
		}
	}
	//@}

#if LB_IMPL_MSCPP
#else
	template LB_ATTR(format(printf, 1, 2)) std::string
		sfmt<char>(const char*, ...);
#endif

	/*!
	\ingroup string_algorithms
	\brief ����ǰ׺������ǰ׺ʱ�����Ƴ�ǰ׺����Ӵ���
	\since build 1.4
	*/
	template<typename _type, typename _tString, typename _func>
	bool
		filter_prefix(const _tString& str, const _type& prefix, _func f)
	{
		if (leo::begins_with(str, prefix))
		{
			auto&& sub(str.substr(string_length(prefix)));

			if (!sub.empty())
				f(std::move(sub));
			return true;
		}
		return{};
	}

	/*!
	\brief ѡ���Թ���ǰ׺��
	\return ָ��ǰ׺����ʱΪȥ��ǰ׺�Ĳ��֣�����Ϊ����ָ�������ֵ��
	\since build 1.4
	*/
	template<typename _type, typename _tString>
	_tString
		cond_prefix(const _tString& str, const _type& prefix, _tString&& val = {})
	{
		leo::filter_prefix(str, prefix, [&](_tString&& s)
			lnoexcept(is_nothrow_move_assignable<decay_t<_tString>>::value) {
			val = std::move(s);
		});
		return std::move(val);
	}
}
#endif