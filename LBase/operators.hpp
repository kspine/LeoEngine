/*!	\file operators.hpp
\ingroup LBase
\brief ���ز�������
\see http://www.boost.org/doc/libs/1_60_0/boost/operators.hpp ��
\see https://github.com/taocpp/operators ��

�÷�ͬ Boost.Operators ���������������ֺ�׺�Ľӿڡ�
*/

#ifndef LBase_Operators_hpp
#define LBase_Operators_hpp 1

#include "LBase/addressof.hpp"// for "type_traits.hpp", _t, true_type, empty_base,
//	leo::constfn_addressof;
#include "LBase/integer_sequence.hpp "// for index_sequence, vseq::defer_apply_t,
//	vseq::_a, vseq::fold_t;

namespace leo
{

	/*!
	\brief �������Ʋ��Ҳ�������Ϊ ADL �ı߽硣
	\note ����ģ�岻ָ��Ϊ��ģ������ģ�塣
	\since build 1.4
	*/
	namespace dep_ops
	{

		//! \since build 1.4
		using no_constfn = limpl(false_type);
		//! \since build 1.4
		using use_constfn = limpl(true_type);

} // namespace dep_ops;

#define LB_Impl_Operators_DeOpt class _tOpt = limpl(use_constfn)
#define LB_Impl_Operators_H_n(_args) template<class _type, _args>
#define LB_Impl_Operators_H1 \
	LB_Impl_Operators_H_n(LB_Impl_Operators_DeOpt)
#define LB_Impl_Operators_H2 \
	LB_Impl_Operators_H_n(typename _type2 LPP_Comma LB_Impl_Operators_DeOpt)
#define LB_Impl_Operators_H2_de \
	LB_Impl_Operators_H_n(typename _type2 = _type LPP_Comma \
		LB_Impl_Operators_DeOpt)
#define LB_Impl_Operators_H2_Alias(_name, ...) \
	LB_Impl_Operators_H2 \
	limpl(using) _name = __VA_ARGS__;
#define LB_Impl_Operators_H2_Alias_de(_name, ...) \
	LB_Impl_Operators_H2_de \
	limpl(using) _name = __VA_ARGS__;
#define LB_Impl_Operators_H3 \
	LB_Impl_Operators_H_n(typename _type2 LPP_Comma typename _type3 \
		LPP_Comma LB_Impl_Operators_DeOpt)

	namespace details
	{

		//! \since build 1.4
		//@{
		template<typename _type = void>
		struct op_idt
		{
			using type = _type;
		};

		//! \since build 1.4
		template<>
		struct op_idt<void>;

		template<typename _type = void>
		using op_idt_t = _t<op_idt<_type>>;
		//@}


		//! \since build 1.4
		//@{
		template<class... _types>
		struct ebases : _types...
		{};

		template<size_t _vN, class _type, typename _type2, class _tOpt> \
		struct bin_ops;

		template<class, typename, class, class>
		struct ops_seq;

		template<class _type, typename _type2, class _tOpt, size_t... _vSeq>
		struct ops_seq<_type, _type2, _tOpt, index_sequence<_vSeq...>>
		{
			using type = ebases<bin_ops<_vSeq, _type, _type2, _tOpt>...>;
		};
		//@}


#define LB_Impl_Operators_f(_c, _op, _tRet, _expr, ...) \
	friend _c LB_ATTR(always_inline) _tRet \
	operator _op(__VA_ARGS__) lnoexcept(noexcept(_expr)) \
	{ \
		return (_expr); \
	}
#if LB_IMPL_GNUCPP && LB_IMPL_GNUCPP < 50000
#	define LB_Impl_Operators_bin_ts(_n, _f, _c, _opt, ...) \
	LB_Impl_Operators_H_n(typename _type2) \
	struct bin_ops<_n, _type, _type2, dep_ops::_opt> \
	{ \
		_f(_c, __VA_ARGS__) \
	};
#else
#	define LB_Impl_Operators_bin_ts(_n, _f, _c, _opt, ...) \
	LB_Impl_Operators_H_n(typename _type2) \
	struct bin_ops<_n, _type, _type2, dep_ops::_opt> \
	{ \
		template<limpl(typename = void)> \
		_f(_c, __VA_ARGS__) \
	};
	//MSCPP has __VA_ARGS__ expack bug
#ifdef LB_IMPL_MSCPP
#	define LB_Impl_Operators_bin_ts_four(_n, _f, _c, _opt, _op, _expr, _ptp, _ptp2) \
	LB_Impl_Operators_H_n(typename _type2) \
	struct bin_ops<_n, _type, _type2, dep_ops::_opt> \
	{ \
		template<limpl(typename = void)> \
		_f(_c,_op, _expr, _ptp, _ptp2) \
	};
#endif
#endif
#define LB_Impl_Operators_bin_spec(_n, _f, ...) \
	LB_Impl_Operators_bin_ts(_n, _f, inline, no_constfn, __VA_ARGS__) \
	LB_Impl_Operators_bin_ts(_n, _f, lconstfn, use_constfn, __VA_ARGS__)
		// NOTE: The trunk libstdc++ std::experimental::string_view comparison should
		//	depend on the same technique.
		// TODO: See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52072. It is strange
		//	to still have this bug. Not fully tested for G++ 5. See also https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67426.
#if true // LB_IMPL_GNUCPP
#	define LB_Impl_Operators_cmpf(_c, _op, _expr, _ptp, _ptp2) \
	LB_Impl_Operators_f(_c, _op, bool, _expr, const _ptp& x, const _ptp2& y)
#else
#	define LB_Impl_Operators_cmpf(_c, _op, _expr, _ptp, _ptp2) \
	LB_Impl_Operators_f(_c, _op, bool, _expr, const _ptp& x, const _ptp2& y) \
	LB_Impl_Operators_f(_c, _op, bool, _expr, const op_idt_t<_ptp>& x, \
		const _ptp2& y) \
	LB_Impl_Operators_f(_c, _op, bool, _expr, const _ptp& x, \
		const op_idt_t<_ptp2>& y)
#endif

		//! \since build 1.4
		//@{
		//MSVC Workaround
#ifdef LB_IMPL_MSCPP
#define LB_Impl_Operators_cmp(_n, _op, _expr, _ptp, _ptp2) \
LB_Impl_Operators_bin_ts_four(_n,LB_Impl_Operators_cmpf, inline, no_constfn, _op, _expr, _ptp, _ptp2) \
	LB_Impl_Operators_bin_ts_four(_n,LB_Impl_Operators_cmpf,lconstfn, use_constfn, _op, _expr, _ptp, _ptp2)
#else
#define LB_Impl_Operators_cmp(_n, _op, _expr, _ptp, _ptp2) \
	LB_Impl_Operators_bin_spec(_n, LB_Impl_Operators_cmpf, _op, _expr, \
		_ptp, _ptp2)
#endif

		// TODO: Add strictly partial order comparison support to reduce duplicate code
		//	between 'less_than_comparable' and 'partially_ordered'.

		// NOTE: Range 0-5 is reserved for %less_than_comparable. Range 2-3 is also
		//	used by %partially_ordered.
		LB_Impl_Operators_cmp(0, <= , !bool(x > y), _type, _type2)
			LB_Impl_Operators_cmp(1, >= , !bool(x < y), _type, _type2)
			LB_Impl_Operators_cmp(2, >, y < x, _type2, _type)
			LB_Impl_Operators_cmp(3, <, y > x, _type2, _type)
			LB_Impl_Operators_cmp(4, <= , !bool(y < x), _type2, _type)
			LB_Impl_Operators_cmp(5, >= , !bool(y > x), _type2, _type)

			// NOTE: Range 6-8 is reserved for %equality_comparable.
			LB_Impl_Operators_cmp(6, == , y == x, _type2, _type)
			LB_Impl_Operators_cmp(7, != , !bool(x == y), _type2, _type)
			LB_Impl_Operators_cmp(8, != , !bool(x == y), _type, _type2)

			// NOTE: Range 9-10 is reserved for %equivalent.
			LB_Impl_Operators_cmp(9, != , !bool(x < y) && !bool(x > y), _type, _type2)
			LB_Impl_Operators_cmp(10, != , !bool(x < y) && !bool(y < x), _type, _type2)

			// NOTE: Range 11-14 is reserved for %partially_ordered.
			LB_Impl_Operators_cmp(11, <= , bool(x < y) || bool(x == y), _type, _type2)
			LB_Impl_Operators_cmp(12, >= , bool(x > y) || bool(x == y), _type, _type2)
			LB_Impl_Operators_cmp(13, <= , bool(y > x) || bool(y == x), _type2, _type)
			LB_Impl_Operators_cmp(14, >= , bool(y < x) || bool(y == x), _type2, _type)

#undef LB_Impl_Operators_cmp
#undef LB_Impl_Operators_cmpf
#undef LB_Impl_Operators_bin_ts_four

			// NOTE: Offset range 0-1 is reserved for general binary operations. Range 2-3
			//	is for commutative operations. Range 4-5 is for non commutative "left"
			//	operations.
			lconstfn size_t
			ops_bin_id(size_t id, size_t off) lnothrow
		{
			return (id + 1) * 16 + off;
		}

		template<size_t _vID, size_t... _vSeq>
		using ops_bin_id_seq = index_sequence<ops_bin_id(_vID, _vSeq)...>;

#ifdef LB_IMPL_MSCPP
#	define LB_Impl_Operators_bin_ts_five(_n, _f, _c, _opt, _op, _type, arg1, arg2, arg3) \
	LB_Impl_Operators_H_n(typename _type2) \
	struct bin_ops<_n, _type, _type2, dep_ops::_opt> \
	{ \
		template<limpl(typename = void)> \
		_f(_c,_op, _type, arg1, arg2, arg3) \
	};
#define LB_Impl_Operators_bin_tmpl(_id, _off, _op,arg1,arg2,arg3) \
	LB_Impl_Operators_bin_ts_five(ops_bin_id(_id LPP_Comma _off), LB_Impl_Operators_f, inline, no_constfn, _op, _type, arg1, arg2, arg3) \
	LB_Impl_Operators_bin_ts_five(ops_bin_id(_id LPP_Comma _off), LB_Impl_Operators_f, lconstfn, use_constfn, _op, _type, arg1, arg2, arg3)
#else
#define LB_Impl_Operators_bin_tmpl(_id, _off, _op, ...) \
	LB_Impl_Operators_bin_spec(ops_bin_id(_id LPP_Comma _off), \
		LB_Impl_Operators_f, _op, _type, __VA_ARGS__)
#endif
#define LB_Impl_Operators_bin(_id, _op) \
	LB_Impl_Operators_bin_tmpl(_id, 0, _op, x _op##= y, _type x, \
		const _type2& y) \
	LB_Impl_Operators_bin_tmpl(_id, 1, _op, x _op##= std::move(y), _type x, \
		_type2&& y)
#define LB_Impl_Operators_con(_id, _op) \
	LB_Impl_Operators_bin(_id, _op) \
	LB_Impl_Operators_bin_tmpl(_id, 2, _op, y _op##= x, const _type2& x, \
		_type y) \
	LB_Impl_Operators_bin_tmpl(_id, 3, _op, y _op##= std::move(x), _type2&& x, \
		_type y)
#define LB_Impl_Operators_left(_id, _op) \
	LB_Impl_Operators_bin(_id, _op) \
	LB_Impl_Operators_bin_tmpl(_id, 4, _op, _type(x) _op##= y, \
		const _type2& x, const _type& y) \
	LB_Impl_Operators_bin_tmpl(_id, 5, _op, \
		_type(x) _op##= std::move(y), const _type2& x, _type&& y)

		LB_Impl_Operators_con(0, +)
			LB_Impl_Operators_left(1, -)
			LB_Impl_Operators_con(2, *)
			LB_Impl_Operators_left(3, / )
			LB_Impl_Operators_left(4, %)
			LB_Impl_Operators_con(5, ^)
			LB_Impl_Operators_con(6, &)
			LB_Impl_Operators_con(7, | )
			LB_Impl_Operators_bin(8, << )
			LB_Impl_Operators_bin(9, >> )

#undef LB_Impl_Operators_left
#undef LB_Impl_Operators_con
#undef LB_Impl_Operators_bin
#undef LB_Impl_Operators_bin_tmpl
#undef LB_Impl_Operators_bin_ts_five

#undef LB_Impl_Operators_bin_spec
#undef LB_Impl_Operators_bin_ts
#undef LB_Impl_Operators_f

			template<class _type, typename _type2, class _tOpt,
			template<typename...> class... _gOps>
		using flat_ops = vseq::defer_t<ebases, vseq::fold_t<vseq::_a<vseq::concat_t>,
			empty_base<>,
			empty_base<vseq::defer_t<empty_base, _gOps<_type, _type2, _tOpt>>... >> >;
		//@}

	} // namespace details;


	namespace dep_ops
	{

		//! \since build 1.4
		//@{
#define LB_Impl_Operators_Compare(_name, _bseq, _bseq_s) \
	LB_Impl_Operators_H2_Alias_de(_name, _t<details::ops_seq<_type, _type2, \
		_tOpt, cond_t<is_same<_type, _type2>, index_sequence<_bseq_s>, \
		index_sequence<_bseq>>>>)

		LB_Impl_Operators_Compare(less_than_comparable, 0 LPP_Comma 1 LPP_Comma 2
			LPP_Comma 3 LPP_Comma 4 LPP_Comma 5, 2 LPP_Comma 4 LPP_Comma 1)


			LB_Impl_Operators_Compare(equality_comparable, 6 LPP_Comma 7 LPP_Comma 8,
				7)


			LB_Impl_Operators_Compare(equivalent, 9, 10)


			LB_Impl_Operators_Compare(partially_ordered, 11 LPP_Comma 12 LPP_Comma 2
				LPP_Comma 3 LPP_Comma 13 LPP_Comma 14, 2 LPP_Comma 11 LPP_Comma 14)

#undef LB_Impl_Operators_Compare


#define LB_Impl_Operators_Binary_Tmpl(_id, _name, _bseq) \
	LB_Impl_Operators_H2_Alias_de(_name, _t<details::ops_seq<_type, _type2, \
		_tOpt, details::ops_bin_id_seq<_id, _bseq>>>)
#define LB_Impl_Operators_Binary(_id, _name) \
	LB_Impl_Operators_Binary_Tmpl(_id, _name, 0 LPP_Comma 1)
#define LB_Impl_Operators_Commutative(_id, _name) \
	LB_Impl_Operators_H2_Alias_de(_name, _t< \
		details::ops_seq<_type, _type2, _tOpt, cond_t<is_same<_type, _type2>, \
		details::ops_bin_id_seq<_id, 0 LPP_Comma 1>, \
		details::ops_bin_id_seq<_id, 0 LPP_Comma 1 LPP_Comma 2 LPP_Comma 3>>>>)
#define LB_Impl_Operators_Left(_id, _name) \
	LB_Impl_Operators_H2_Alias_de(_name, _t<details::ops_seq<_type, _type2, \
		_tOpt, details::ops_bin_id_seq<_id, 0 LPP_Comma 1>>>) \
	LB_Impl_Operators_H2_Alias_de(_name##_left, _t<details::ops_seq<_type, \
		_type2, _tOpt, details::ops_bin_id_seq<_id, 4 LPP_Comma 5>>>)

			LB_Impl_Operators_Commutative(0, addable)


			LB_Impl_Operators_Left(1, subtractable)


			LB_Impl_Operators_Commutative(2, multipliable)


			LB_Impl_Operators_Left(3, dividable)


			LB_Impl_Operators_Left(4, modable)


			LB_Impl_Operators_Binary(5, left_shiftable)


			LB_Impl_Operators_Binary(6, right_shiftable)


			LB_Impl_Operators_Commutative(7, xorable)


			LB_Impl_Operators_Commutative(8, andable)


			LB_Impl_Operators_Commutative(9, orable)

#undef LB_Impl_Operators_Binary_Tmpl
#undef LB_Impl_Operators_Left
#undef LB_Impl_Operators_Commutative
#undef LB_Impl_Operators_Binary


			LB_Impl_Operators_H1
			limpl(struct) incrementable
		{
			//! \since build 1.4
			friend _type
				operator++(_type& x, int) lnoexcept(noexcept(_type(x)) && noexcept(++x)
					&& noexcept(_type(std::declval<_type>())))
			{
				_type t(x);

				++x;
				return t;
			}
		};

		LB_Impl_Operators_H1
			limpl(struct) decrementable
		{
			//! \since build 1.4
			friend _type
				operator--(_type& x, int) lnoexcept(noexcept(_type(x)) && noexcept(--x)
					&& noexcept(_type(std::declval<_type>())))
			{
				_type t(x);

				++x;
				return t;
			}
		};

		LB_Impl_Operators_H2
			limpl(struct) dereferenceable
		{
			// TODO: Add non-const overloaded version? SFINAE?
			// TODO: Use '_tOpt'.
			lconstfn decltype(leo::constfn_addressof(std::declval<const _type2&>()))
				operator->() const lnoexcept_spec(*std::declval<const _type&>())
			{
				return leo::constfn_addressof(*static_cast<const _type&>(*this));
			}
		};

		LB_Impl_Operators_H3
			limpl(struct) indexable
		{
			// TODO: Add non-const overloaded version? SFINAE?
			// TODO: Use '_tOpt'.
			lconstfn _type3
				operator[](_type2 n) const
				lnoexcept_spec(*(std::declval<const _type&>() + n))
			{
				return *(static_cast<const _type&>(*this) + n);
			}
		};


#define LB_Impl_Operators_FlatAlias2_de(_name, ...) \
	LB_Impl_Operators_H2_Alias_de(_name, details::flat_ops<_type, _type2, \
		_tOpt, __VA_ARGS__>)

		LB_Impl_Operators_FlatAlias2_de(totally_ordered, less_than_comparable,
			equality_comparable)


			LB_Impl_Operators_FlatAlias2_de(additive, addable, subtractable)


			LB_Impl_Operators_FlatAlias2_de(multiplicative, multipliable, dividable)


			LB_Impl_Operators_FlatAlias2_de(integer_multiplicative, multiplicative, modable)


			LB_Impl_Operators_FlatAlias2_de(arithmetic, additive, multiplicative)


			LB_Impl_Operators_FlatAlias2_de(integer_arithmetic, additive,
				integer_multiplicative)


			LB_Impl_Operators_FlatAlias2_de(bitwise, xorable, andable, orable)


			LB_Impl_Operators_H1
			limpl(using) unit_steppable
			= details::ebases<incrementable<_type>, decrementable<_type>>;


		LB_Impl_Operators_FlatAlias2_de(shiftable, left_shiftable, right_shiftable)


			LB_Impl_Operators_FlatAlias2_de(ring_operators, additive, subtractable_left,
				multipliable)


			LB_Impl_Operators_FlatAlias2_de(ordered_ring_operators, ring_operators,
				totally_ordered)


			LB_Impl_Operators_FlatAlias2_de(field_operators, ring_operators, dividable,
				dividable_left)


			LB_Impl_Operators_FlatAlias2_de(ordered_field_operators, field_operators,
				totally_ordered)


			LB_Impl_Operators_FlatAlias2_de(euclidean_ring_operators, ring_operators,
				dividable, dividable_left, modable, modable_left)


			LB_Impl_Operators_FlatAlias2_de(ordered_euclidean_ring_operators,
				totally_ordered, euclidean_ring_operators)


			LB_Impl_Operators_H2_Alias(input_iteratable, details::ebases<
				equality_comparable<_type, _type, _tOpt>, incrementable<_type>,
				dereferenceable<_type, _type2, _tOpt >> )


			LB_Impl_Operators_H2_Alias(output_iteratable,
				details::ebases<incrementable<_type>>)


			LB_Impl_Operators_H2_Alias(forward_iteratable,
				details::ebases<input_iteratable<_type, _type2, _tOpt>>)


			LB_Impl_Operators_H2_Alias(bidirectional_iteratable, details::ebases<
				forward_iteratable<_type, _type2, _tOpt>, decrementable<_type >> )


			LB_Impl_Operators_H3
			limpl(using) random_access_iteratable
			= details::ebases<bidirectional_iteratable<_type, _type3, _tOpt>,
			less_than_comparable<_type, _type, _tOpt>,
			additive<_type, _type2, _tOpt>, indexable<_type, _type2, _type, _tOpt >> ;

		/*!
		\note ����ģ������������ɵĲ�������������
		\note ��ǰ��Ԫ����Ԫ��������ʹ�� lconstfn ��
		\todo ʵ�ֵ���ѡ�
		*/
		LB_Impl_Operators_FlatAlias2_de(operators, totally_ordered, integer_arithmetic,
			bitwise)

#undef LB_Impl_Operators_FlatAlias2_de
			//@}

	} // namespace dep_ops;

	  //! \since build 1.4
	using namespace dep_ops;

#undef LB_Impl_Operators_H3
#undef LB_Impl_Operators_H2_Alias_de
#undef LB_Impl_Operators_H2_Alias
#undef LB_Impl_Operators_H2
#undef LB_Impl_Operators_H1
#undef LB_Impl_Operators_H_n
#undef LB_Impl_Operators_DeOpt

} // namespace leo;

#endif