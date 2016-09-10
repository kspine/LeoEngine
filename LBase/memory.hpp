/*! \file memory.hpp
\ingroup LBase
\brief 存储和智能指针特性。

扩展标准库头 <memory> ，提供对智能指针类型的操作及判断迭代器不可解引用的模板。
*/

#ifndef LBase_memory_Hpp
#define LBase_memory_Hpp 1

#include "LBase/addressof.hpp" // for "addressof.hpp", and_,
//	is_copy_constructible, is_class_type, cond_t, vdefer, detected_t,
//	conditional, ::constfn_addressof, indirect_element_t,
//	remove_reference_t, detected_or_t, is_void, remove_pointer_t,
//	is_pointer, enable_if_t, is_array, extent, remove_extent_t;
#include "LBase/type_op.hpp" // for is_class_type, is_nonconst_object;
#include <iterator> // for std::iterator_traits;
#include "LBase/cassert.h" // for lonstraint;
#include "LBase/deref_op.hpp" // for is_undereferenceable;
#include "LBase/operators.hpp" // for totally_ordered, equality_comparable;
#include <new> // for placement ::operator new from standard library;
#include <memory>

#if LB_IMPL_MSCPP >= 1800
/*!
\brief \<memory\> 特性测试宏。
\see WG21 P0096R1 3.5 。
*/
#	ifndef __cpp_lib_make_unique
#		define __cpp_lib_make_unique 201304
#	endif
#endif

namespace leo
{
	//@{
	namespace details
	{

		template<typename _type>
		using is_copy_constructible_class
			= and_<is_copy_constructible<_type>, is_class_type<_type>>;


		//! \since build 1.4
		template<class _tAlloc, typename _type>
		using rebind_t = typename _tAlloc::template rebind<_type>::other;

		// XXX: Details member across headers?
		//! \since build 1.4
		template<typename _type>
		using check_allocator = and_<cond_t<has_mem_value_type<_type>,
			is_detected<rebind_t, _type, vdefer<details::mem_value_type_t, _type>>>,
			is_copy_constructible_class<_type>>;

		//! \since build 1.4
		template<typename _type>
		using nested_allocator_t = typename _type::allocator_type;


		//! \since build 1.4
		//@{
		template<typename _type>
		struct pack_obj_impl
		{
			template<typename... _tParams>
			static _type
				pack(_tParams&&... args)
			{
				return _type(lforward(args)...);
			}
		};

		template<typename _type, class _tDeleter>
		struct pack_obj_impl<std::unique_ptr<_type, _tDeleter>>
		{
			template<typename... _tParams>
			static std::unique_ptr<_type>
				pack(_tParams&&... args)
			{
				return std::unique_ptr<_type>(lforward(args)...);
			}
		};

		template<typename _type>
		struct pack_obj_impl<std::shared_ptr<_type>>
		{
			template<typename... _tParams>
			static std::shared_ptr<_type>
				pack(_tParams&&... args)
			{
				return std::shared_ptr<_type>(lforward(args)...);
			}
		};
		//@}

	} // namespace details;
	//@}

	/*!
	\ingroup unary_type_traits
	\brief 判断类型是否符合分配器要求的目标类型。
	\since build 1.4
	*/
	template<typename _type>
	struct is_allocatable : is_nonconst_object<_type>
	{};


	/*!
	\ingroup unary_type_traits
	\brief 判断类型具有嵌套的成员 allocator_type 指称一个可复制构造的类类型。
	*/
	template<typename _type>
	struct has_nested_allocator
		: details::check_allocator<detected_t<details::nested_allocator_t, _type>>
	{};


	/*!
	\ingroup metafunctions
	\brief 取嵌套成员分配器类型，若不存在则使用第二模板参数指定的默认类型。
	*/
	template<typename _type, class _tDefault = std::allocator<_type>>
	struct nested_allocator
		: conditional<has_nested_allocator<_type>::value,
		detected_t<details::nested_allocator_t, _type>, _tDefault>
	{};
	//@}


	/*!
	\brief 原地构造。
	\tparam _tParams 用于构造对象的参数包类型。
	\param args 用于构造对象的参数包。
	\since build 1.4
	*/
	template<typename _type, typename... _tParams>
	inline void
		construct_in(_type& obj, _tParams&&... args)
	{
		::new(static_cast<void*>(static_cast<_type*>(
			constfn_addressof(obj)))) _type(lforward(args)...);
	}


	//! \since build 1.4
	//@{
	//! \tparam _tIter 迭代器类型。
	//@{
	/*!
	\tparam _tParams 用于构造对象的参数包类型。
	\param args 用于构造对象的参数包。
	\pre 断言：指定范围末尾以外的迭代器满足 <tt>!is_undereferenceable</tt> 。
	*/
	//@{
	/*!
	\brief 使用指定参数在迭代器指定的位置构造对象。
	\param i 迭代器。
	\note 显式转换为 void* 指针以实现标准库算法 uninitialized_* 实现类似的语义。
	\see libstdc++ 5 和 Microsoft VC++ 2013 标准库在命名空间 std 内对指针类型的实现：
	_Construct 模板。
	*/
	template<typename _tIter, typename... _tParams>
	void
		construct(_tIter i, _tParams&&... args)
	{
		using value_type = typename std::iterator_traits<_tIter>::value_type;

		lconstraint(!is_undereferenceable(i));
		construct_in<value_type>(*i, lforward(args)...);
	}

	/*!
	\brief 使用指定的参数重复构造迭代器范围内的对象序列。
	\note 参数被传递的次数和构造的对象数相同。
	*/
	template<typename _tIter, typename... _tParams>
	void
		construct_range(_tIter first, _tIter last, _tParams&&... args)
	{
		for (; first != last; ++first)
			construct(first, lforward(args)...);
	}
	//@}

	/*!
	\brief 原地析构。
	\tparam _tParams 用于构造对象的参数包类型。
	\param args 用于构造对象的参数包。
	\since build 1.4
	*/
	template<typename _type>
	inline void
		destruct_in(_type& obj)
	{
		obj.~_type();
	}


	/*!
	\brief 析构迭代器指向的对象。
	\param i 迭代器。
	\pre 断言：<tt>!is_undereferenceable(i)</tt> 。
	\see libstdc++ 5 和 Microsoft VC++ 2013 标准库在命名空间 std 内对指针类型的实现：
	_Destroy 模板。
	*/
	template<typename _tIter>
	void
		destruct(_tIter i)
	{
		using value_type = typename std::iterator_traits<_tIter>::value_type;

		lconstraint(!is_undereferenceable(i));
		destruct_in<value_type>(*i);
	}

	/*!
	\brief 析构迭代器范围内的对象序列。
	\note 保证顺序析构。
	\see libstdc++ 5 标准库在命名空间 std 内的实现： _Destroy 模板。
	*/
	template<typename _tIter>
	void
		destruct_range(_tIter first, _tIter last)
	{
		for (; first != last; ++first)
			destruct(first);
	}
	//@}


	/*!
	\brief 在迭代器指定的未初始化的范围上构造对象。
	\tparam _tFwd 输出范围前向迭代器类型。
	\tparam _tParams 用于构造对象的参数包类型。
	\param first 输出范围起始迭代器。
	\param args 用于构造对象的参数包。
	\note 参数被传递的次数和构造的对象数相同。
	\note 接口不保证失败时的析构顺序。
	*/
	//@{
	/*!
	\param last 输出范围终止迭代器。
	\note 和 std::unitialized_fill 类似，但允许指定多个初始化参数。
	\see WG21 N4431 20.7.12.3[uninitialized.fill] 。
	*/
	template<typename _tFwd, typename... _tParams>
	void
		uninitialized_construct(_tFwd first, _tFwd last, _tParams&&... args)
	{
		auto i = first;

		try
		{
			for (; i != last; ++i)
				construct(i, lforward(args)...);
		}
		catch (...)
		{
			// NOTE: The order is unspecified.
			destruct_range(first, i);
			throw;
		}
	}

	/*!
	\tparam _tSize 范围大小类型。
	\param n 范围大小。
	\note 和 std::unitialized_fill_n 类似，但允许指定多个初始化参数。
	\see WG21 N4431 20.7.12.4[uninitialized.fill.n] 。
	*/
	template<typename _tFwd, typename _tSize, typename... _tParams>
	void
		uninitialized_construct_n(_tFwd first, _tSize n, _tParams&&... args)
	{
		auto i = first;

		try
		{
			// NOTE: This form is by specification (WG21 N4431) of
			//	'std::unitialized_fill' literally.
			for (; n--; ++i)
				construct(i, lforward(args)...);
		}
		catch (...)
		{
			// NOTE: The order is unspecified.
			destruct_range(first, i);
			throw;
		}
	}
	//@}
	//@}

	/*!
	\brief 使用显式析构函数调用和 std::free 的删除器。
	\note 数组类型的特化无定义。
	\since build 1.4

	除使用 std::free 代替 \c ::operator delete，和 std::default_deleter
	的非数组类型元相同。注意和直接使用 std::free 不同，会调用析构函数且不适用于数组。
	*/
	//@{
	template<typename _type>
	struct free_delete
	{
		lconstfn free_delete() lnothrow = default;
		template<typename _type2,
			limpl(typename = enable_if_convertible_t<_type2*, _type*>)>
			free_delete(const free_delete<_type2>&) lnothrow
		{}

		//! \since build 1.4
		void
			operator()(_type* p) const lnothrowv
		{
			p->~_type();
			std::free(p);
		}
	};

	template<typename _type>
	struct free_delete<_type[]>
	{
		//! \since build 1.4
		//@{
		static_assert(is_trivially_destructible<_type>::value, "Invalid type found");

		lconstfn free_delete() lnothrow = default;
		template<typename _type2,
			limpl(typename = enable_if_convertible_t<_type2(*)[], _type(*)[]>)>
			free_delete(const free_delete<_type2[]>&) lnothrow
		{}

		template<typename _type2,
			limpl(typename = enable_if_convertible_t<_type2(*)[], _type(*)[]>)>
			void
			operator()(_type2* p) const lnothrowv
		{
			std::free(p);
		}
		//@}
	};
	//@}


	/*!
	\brief 使用显式 std::return_temporary_buffer 的删除器。
	\note 非数组类型的特化无定义。
	\since build 1.4
	*/
	//@{
	template<typename>
	struct temporary_buffer_delete;

	template<typename _type>
	struct temporary_buffer_delete<_type[]>
	{
		lconstfn temporary_buffer_delete() lnothrow = default;

		void
			operator()(_type* p) const lnothrowv
		{
			std::return_temporary_buffer(p);
		}
	};
	//@}


	/*!
	\brief 临时缓冲区。
	\since build 1.4
	*/
	//@{
	template<typename _type>
	class temporary_buffer
	{
	public:
		using array_type = _type[];
		using element_type = _type;
		using deleter = temporary_buffer_delete<array_type>;
		using pointer = std::unique_ptr<_type, deleter>;

	private:
		std::pair<pointer, size_t> buf;

	public:
		//! \throw std::bad_cast 取临时存储失败。
		temporary_buffer(size_t n)
			: buf([n] {
			// NOTE: See http://wg21.cmeerw.net/lwg/issue2072.
			const auto pr(std::get_temporary_buffer<_type>(ptrdiff_t(n)));

			if (pr.first)
				return std::pair<pointer, size_t>(pointer(pr.first),
					size_t(pr.second));
			throw std::bad_cast();
		}())
		{}
		temporary_buffer(temporary_buffer&&) = default;
		temporary_buffer&
			operator=(temporary_buffer&&) = default;

		//! \since build 1.4
		lconstfn const pointer&
			get() const lnothrow
		{
			return buf.first;
		}

		pointer&
			get_pointer_ref() lnothrow
		{
			return buf.first;
		}

		size_t
			size() const lnothrow
		{
			return buf.second;
		}
	};
	//@}


	/*!
	\brief 释放分配器的删除器。
	\since build 1.4
	*/
	template<class _tAlloc>
	class allocator_delete
	{
	private:
		using traits = std::allocator_traits<_tAlloc>;

	public:
		using pointer = typename traits::pointer;
		using size_type = typename traits::size_type;

	private:
		_tAlloc& alloc_ref;
		size_type size;

	public:
		allocator_delete(_tAlloc& alloc, size_type s)
			: alloc_ref(alloc), size(s)
		{}

		void
			operator()(pointer p) const lnothrowv
		{
			traits::deallocate(alloc_ref, p, size);
		}
	};


	//! \since build 1.4
	//@{
	//! \brief 使用分配器复制指定指针指向的对象。
	template<typename _type, class _tAlloc
		= std::allocator<indirect_element_t<_type>>>
		auto
		clone_monomorphic(const _type& p, _tAlloc&& a = _tAlloc())
		-> decltype(std::addressof(*p))
	{
		using traits = std::allocator_traits<_tAlloc>;
		using value_type = typename traits::value_type;

		auto p_allocated(traits::allocate(a, sizeof(value_type)));
		const auto p_storage(std::addressof(*p_allocated));

		traits::construct(a, p_storage, *p);
		return p_storage;
	}

	/*!
	\brief 使用 \c clone 成员函数复制指定指针指向的多态类类型对象。
	\pre 断言： <tt>is_polymorphic<indirect_element_t<decltype(p)>>()</tt> 。
	*/
	template<class _type>
	auto
		clone_polymorphic(const _type& p) -> decltype(std::addressof(*p))
	{
		static_assert(is_polymorphic<indirect_element_t<decltype(p)>>(),
			"Non-polymorphic class type found.");

		return p->clone();
	}
	//@}


	/*!
	\ingroup metafunctions
	\since build 1.4
	*/
	//@{
	//! \brief 取删除器的 \c pointer 成员类型。
	template<class _tDeleter>
	using deleter_member_pointer_t
		= typename remove_reference_t<_tDeleter>::pointer;

	//! \brief 取 unique_ptr 包装的指针类型。
	template<typename _type, class _tDeleter>
	using unique_ptr_pointer
		= detected_or_t<_type*, deleter_member_pointer_t, _tDeleter>;

	/*!
	\brief 取指针对应的元素类型。

	当指针为空指针时为 \c void ，否则间接操作的元素类型。
	*/
	template<typename _tPointer, typename _tDefault = void>
	using defer_element = cond_t<is_void<remove_pointer_t<_tPointer>>, _tDefault,
		vdefer<indirect_element_t, _tPointer>>;
	//@}

	template<typename _type>
	lconstfn _type*
		//取内建指针
		get_raw(_type* const& p) lnothrow
	{
		return p;
	}

	template<typename _type>
	lconstfn auto
		get_raw(const std::unique_ptr<_type>& p) lnothrow -> decltype(p.get())
	{
		return p.get();
	}
	template<typename _type>
	lconstfn _type*
		get_raw(const std::shared_ptr<_type>& p) lnothrow
	{
		return p.get();
	}
	template<typename _type>
	lconstfn _type*
		get_raw(const std::weak_ptr<_type>& p) lnothrow
	{
		return p.lock().get();
	}

	template<typename _type>
	inline bool
		reset(std::unique_ptr<_type>& p) lnothrow
	{
		if (p.get())
		{
			p.reset();
			return true;
		}
		return false;
	}
	template<typename _type>
	inline bool
		reset(std::shared_ptr<_type>& p) lnothrow
	{
		if (p.get())
		{
			p.reset();
			return true;
		}
		return false;
	}

	template<typename _type, typename _pSrc>
	lconstfn std::unique_ptr<_type>
		//_pSrc是内建指针
		unique_raw(_pSrc* p) lnothrow
	{
		return std::unique_ptr<_type>(p);
	}
	template<typename _type>
	lconstfn std::unique_ptr<_type>
		unique_raw(_type* p) lnothrow
	{
		return std::unique_ptr<_type>(p);
	}
	template<typename _type, typename _tDeleter, typename _pSrc>
	lconstfn std::unique_ptr<_type, _tDeleter>
		unique_raw(_pSrc* p, _tDeleter&& d) lnothrow
	{
		return std::unique_ptr<_type, _tDeleter>(p, lforward(d));
	}

	template<typename _type, typename _tDeleter>
	lconstfn std::unique_ptr<_type, _tDeleter>
		unique_raw(_type* p, _tDeleter&& d) lnothrow
	{
		return std::unique_ptr<_type, _tDeleter>(p, lforward(d));
	}

	template<typename _type>
	lconstfn std::unique_ptr<_type>
		unique_raw(nullptr_t) lnothrow
	{
		return std::unique_ptr<_type>();
	}


	template<typename _type, typename... _tParams>
	lconstfn std::shared_ptr<_type>
		share_raw(_type* p, _tParams&&... args)
	{
		return std::shared_ptr<_type>(p, lforward(args)...);
	}

	template<typename _type, typename _pSrc, typename... _tParams>
	lconstfn std::shared_ptr<_type>
		share_raw(_pSrc&& p, _tParams&&... args)
	{
		static_assert(is_pointer<remove_reference_t<_pSrc>>(),
			"Invalid type found.");

		return std::shared_ptr<_type>(p, lforward(args)...);
	}
	template<typename _type>
	lconstfn std::shared_ptr<_type>
		share_raw(nullptr_t) lnothrow
	{
		return std::shared_ptr<_type>();
	}
	/*!
	\note 使用空指针和其它参数构造空对象。
	\pre 参数复制构造不抛出异常。
	*/
	template<typename _type, class... _tParams>
	lconstfn limpl(enable_if_t) < sizeof...(_tParams) != 0, std::shared_ptr<_type> >
		share_raw(nullptr_t, _tParams&&... args) lnothrow
	{
		return std::shared_ptr<_type>(nullptr, lforward(args)...);
	}
	inline namespace cpp2014
	{

#if __cpp_lib_make_unique >= 201304 || __cplusplus > 201103L
		//! \since build 617
		using std::make_unique;
#else
		/*!
		\ingroup helper_functions
		\brief 使用 new 和指定参数构造指定类型的 std::unique_ptr 对象。
		\tparam _type 被指向类型。
		*/
		//@{
		/*!
		\note 使用值初始化。
		\see http://herbsutter.com/gotw/_102/ 。
		\see WG21 N3656 。
		\see WG21 N3797 20.7.2[memory.syn] 。
		*/
		//@{
		template<typename _type, typename... _tParams>
		lconstfn limpl(enable_if_t<!is_array<_type>::value, std::unique_ptr<_type>>)
			make_unique(_tParams&&... args)
		{
			return std::unique_ptr<_type>(new _type(lforward(args)...));
		}
		template<typename _type, typename... _tParams>
		lconstfn limpl(enable_if_t<is_array<_type>::value && extent<_type>::value == 0,
			std::unique_ptr<_type>>)
			make_unique(size_t size)
		{
			return std::unique_ptr<_type>(new remove_extent_t<_type>[size]());
		}
		template<typename _type, typename... _tParams>
		limpl(enable_if_t<extent<_type>::value != 0>)
			make_unique(_tParams&&...) = delete;
		//@}
#endif

	} // inline namespace cpp2014;


	/*!
	\note 使用默认初始化。
	\see WG21 N3588 A4 。
	\since build 1.4
	*/
	//@{
	template<typename _type, typename... _tParams>
	lconstfn limpl(enable_if_t<!is_array<_type>::value, std::unique_ptr<_type>>)
		make_unique_default_init()
	{
		return std::unique_ptr<_type>(new _type);
	}
	template<typename _type, typename... _tParams>
	lconstfn limpl(enable_if_t<is_array<_type>::value && extent<_type>::value == 0,
		std::unique_ptr<_type>>)
		make_unique_default_init(size_t size)
	{
		return std::unique_ptr<_type>(new remove_extent_t<_type>[size]);
	}
	template<typename _type, typename... _tParams>
	limpl(enable_if_t<extent<_type>::value != 0>)
		make_unique_default_init(_tParams&&...) = delete;
	//@}
	/*!
	\ingroup helper_functions
	\brief 使用指定类型的初始化列表构造指定类型的 std::unique_ptr 对象。
	\tparam _type 被指向类型。
	\tparam _tValue 初始化列表的元素类型。
	\since build 1.4
	*/
	template<typename _type, typename _tValue>
	lconstfn std::unique_ptr<_type>
		make_unique(std::initializer_list<_tValue> il)
	{
		return leo::make_unique<_type>(il);
	}

	/*!
	\note 使用删除器。
	\since build 1.4
	*/
	//@{
	template<typename _type, typename _tDeleter, typename... _tParams>
	lconstfn limpl(enable_if_t<!is_array<_type>::value, std::unique_ptr<_type>>)
		make_unique_with(_tDeleter&& d)
	{
		return std::unique_ptr<_type, _tDeleter>(new _type, lforward(d));
	}
	template<typename _type, typename _tDeleter, typename... _tParams>
	lconstfn limpl(enable_if_t<is_array<_type>::value && extent<_type>::value == 0,
		std::unique_ptr<_type>>)
		make_unique_with(_tDeleter&& d, size_t size)
	{
		return std::unique_ptr<_type, _tDeleter>(new remove_extent_t<_type>[size],
			lforward(d));
	}
	template<typename _type, typename _tDeleter, typename... _tParams>
	limpl(enable_if_t<extent<_type>::value != 0>)
		make_unique_with(_tDeleter&&, _tParams&&...) = delete;
	//@}
	//@}

	/*!
	\ingroup helper_functions
	\brief 使用指定类型的初始化列表构造指定类型的 std::shared_ptr 对象。
	\tparam _type 被指向类型。
	\tparam _tValue 初始化列表的元素类型。
	\since build 1.4
	*/
	template<typename _type, typename _tValue>
	lconstfn std::shared_ptr<_type>
		make_shared(std::initializer_list<_tValue> il)
	{
		return std::make_shared<_type>(il);
	}


	/*!
	\brief 构造分配器守护。
	\since build 1.4
	*/
	template<typename _tAlloc>
	std::unique_ptr<_tAlloc, allocator_delete<_tAlloc>>
		make_allocator_guard(_tAlloc& alloc,
			typename std::allocator_traits<_tAlloc>::size_type n = 1)
	{
		using del_t = allocator_delete<_tAlloc>;

		return std::unique_ptr<_tAlloc, del_t>(alloc.allocate(n), del_t(alloc, n));
	}

	/*!
	\brief 智能指针转换。
	\since build 1.4
	*/
	//@{
	template<typename _tDst, typename _type>
	std::unique_ptr<_tDst>
		static_pointer_cast(std::unique_ptr<_type> p) lnothrow
	{
		return std::unique_ptr<_tDst>(static_cast<_tDst*>(p.release()));
	}
	template<typename _tDst, typename _type, typename _tDeleter>
	std::unique_ptr<_tDst, _tDeleter>
		static_pointer_cast(std::unique_ptr<_type, _tDeleter> p) lnothrow
	{
		return std::unique_ptr<_tDst, _tDeleter>(static_cast<_tDst*>(p.release()),
			std::move(p.get_deleter()));
	}

	template<typename _tDst, typename _type>
	std::unique_ptr<_tDst>
		dynamic_pointer_cast(std::unique_ptr<_type>& p) lnothrow
	{
		if (auto p_res = dynamic_cast<_tDst*>(p.get()))
		{
			p.release();
			return std::unique_ptr<_tDst>(p_res);
		}
		return std::unique_ptr<_tDst>();
	}
	template<typename _tDst, typename _type>
	std::unique_ptr<_tDst>
		dynamic_pointer_cast(std::unique_ptr<_type>&& p) lnothrow
	{
		return leo::dynamic_pointer_cast<_tDst, _type>(p);
	}
	template<typename _tDst, typename _type, typename _tDeleter>
	std::unique_ptr<_tDst, _tDeleter>
		dynamic_pointer_cast(std::unique_ptr<_type, _tDeleter>& p) lnothrow
	{
		if (auto p_res = dynamic_cast<_tDst*>(p.get()))
		{
			p.release();
			return std::unique_ptr<_tDst, _tDeleter>(p_res);
		}
		return std::unique_ptr<_tDst, _tDeleter>(nullptr);
	}
	template<typename _tDst, typename _type, typename _tDeleter>
	std::unique_ptr<_tDst, _tDeleter>
		dynamic_pointer_cast(std::unique_ptr<_type, _tDeleter>&& p) lnothrow
	{
		return leo::dynamic_pointer_cast<_tDst, _type, _tDeleter>(p);
	}

	template<typename _tDst, typename _type>
	std::unique_ptr<_tDst>
		const_pointer_cast(std::unique_ptr<_type> p) lnothrow
	{
		return std::unique_ptr<_tDst>(const_cast<_tDst*>(p.release()));
	}
	template<typename _tDst, typename _type, typename _tDeleter>
	std::unique_ptr<_tDst, _tDeleter>
		const_pointer_cast(std::unique_ptr<_type, _tDeleter> p) lnothrow
	{
		return std::unique_ptr<_tDst, _tDeleter>(const_cast<_tDst*>(p.release()),
			std::move(p.get_deleter()));
	}
	//@}


	//! \since build 1.4
	//@{
	/*!
	\brief 观察者指针：无所有权的智能指针。
	\see WG21 N4529 8.12[memory.observer.ptr] 。
	*/
	template<typename _type>
	class observer_ptr : private totally_ordered<observer_ptr<_type>>,
		private equality_comparable<observer_ptr<_type>, nullptr_t>
	{
	public:
		using element_type = _type;
		using pointer = limpl(add_pointer_t<_type>);
		using reference = limpl(add_lvalue_reference_t<_type>);

	private:
		_type* ptr{};

	public:
		//! \post <tt>get() == nullptr</tt> 。
		//@{
		lconstfn
			observer_ptr() lnothrow limpl(= default);
		lconstfn
			observer_ptr(nullptr_t) lnothrow
			: ptr()
		{}
		//@}
		explicit lconstfn
			observer_ptr(pointer p) lnothrow
			: ptr(p)
		{}
		template<typename _tOther>
		lconstfn
			observer_ptr(observer_ptr<_tOther> other) lnothrow
			: ptr(other.get())
		{}

		//! \pre 断言： <tt>get() != nullptr</tt> 。
		lconstfn reference
			operator*() const lnothrowv
		{
			return lconstraint(get() != nullptr), *ptr;
		}

		lconstfn pointer
			operator->() const lnothrow
		{
			return ptr;
		}

		//! \since build 1.4
		friend lconstfn bool
			operator==(observer_ptr p, nullptr_t) lnothrow
		{
			return !p.ptr;
		}

		explicit lconstfn
			operator bool() const lnothrow
		{
			return ptr;
		}

		explicit lconstfn
			operator pointer() const lnothrow
		{
			return ptr;
		}

		lconstfn pointer
			get() const lnothrow
		{
			return ptr;
		}

		lconstfn_relaxed pointer
			release() lnothrow
		{
			const auto res(ptr);

			reset();
			return res;
		}

		lconstfn_relaxed void
			reset(pointer p = {}) lnothrow
		{
			ptr = p;
		}

		lconstfn_relaxed void
			swap(observer_ptr& other) lnothrow
		{
			std::swap(ptr, other.ptr);
		}
	};

	//! \relates observer_ptr
	//@{
	//! \since build 1.4
	//@{
	template<typename _type1, typename _type2>
	lconstfn bool
		operator==(observer_ptr<_type1> p1, observer_ptr<_type2> p2) lnothrowv
	{
		return p1.get() == p2.get();
	}

	template<typename _type1, typename _type2>
	lconstfn bool
		operator!=(observer_ptr<_type1> p1, observer_ptr<_type2> p2) lnothrowv
	{
		return !(p1 == p2);
	}

	template<typename _type1, typename _type2>
	lconstfn bool
		operator<(observer_ptr<_type1> p1, observer_ptr<_type2> p2) lnothrowv
	{
		return std::less<common_type_t<_type1, _type2>>(p1.get(), p2.get());
	}
	//@}

	template<typename _type>
	inline void
		swap(observer_ptr<_type>& p1, observer_ptr<_type>& p2) lnothrow
	{
		p1.swap(p2);
	}
	template<typename _type>
	inline observer_ptr<_type>
		make_observer(_type* p) lnothrow
	{
		return observer_ptr<_type>(p);
	}
	//@}
	//@}


	//! \since build 1.4
	//@{
	/*!
	\brief 打包对象：通过指定参数构造对象。
	\since build 

	对 std::unique_ptr 和 std::shared_ptr 的实例，使用 make_unique
	和 std::make_shared 构造，否则直接使用参数构造。
	*/
	template<typename _type, typename... _tParams>
	auto
		pack_object(_tParams&&... args)
		-> decltype(limpl(details::pack_obj_impl<_type>::pack(lforward(args)...)))
	{
		return details::pack_obj_impl<_type>::pack(lforward(args)...);
	}


	//! \brief 打包的对象：使用 pack_object 得到的对象包装。
	template<typename _type, typename _tPack = std::unique_ptr<_type>>
	struct object_pack
	{
	public:
		using value_type = _type;
		using pack_type = _tPack;

	private:
		pack_type pack;

	public:
		//! \note 使用 ADL pack_object 。
		template<typename... _tParams>
		object_pack(_tParams&&... args)
			: pack(pack_object<_tPack>(lforward(args)...))
		{}

		operator pack_type&() lnothrow
		{
			return pack;
		}
		operator const pack_type&() const lnothrow
		{
			return pack;
		}
	};
	//@}
}

namespace std
{

	/*!
	\brief leo::observer_ptr 散列支持。
	\see ISO WG21 N4529 8.12.7[memory.observer.ptr.hash] 。
	\since build 1.4
	*/
	//@{
	template<typename>
	struct hash;

	template<typename _type>
	struct hash<leo::observer_ptr<_type>>
	{
		size_t
			operator()(const leo::observer_ptr<_type>& p) const limpl(lnothrow)
		{
			return hash<_type*>(p.get());
		}
	};
	//@}

} // namespace std;


#endif