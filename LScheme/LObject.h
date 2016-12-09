/*! \file LObject.h
\ingroup LScheme
\brief ��������

*/
#ifndef LScheme_LObject_H
#define LScheme_LObject_H 1

#include <LBase/LCoreUtilities.h>
#include <LBase/any.h>
#include <LBase/examiner.hpp>
#include <LBase/operators.hpp>

#ifdef LB_IMPL_MSCPP
#include <fstream>
#include <sstream>
#endif

#include "sdef.h"

namespace leo
{
	/*!
	\brief ָ���Բ���ָ�����͵ĳ�Ա��������Ȩ�ı�ǩ��
	*/
	template<typename = void>
	struct OwnershipTag
	{};

	struct MoveTag
	{};

	struct PointerTag
	{};

	template<class _tOwner, typename _type>
	struct HasOwnershipOf : std::is_base_of<OwnershipTag<_type>, _tOwner>
	{};

	DeclDerivedI(LS_API, IValueHolder, leo::any_ops::holder)

		DeclIEntry(bool operator==(const IValueHolder&) const)

		/*!
		\brief �������á�

		�������ó��ж���
		����ʵ��Ӧ��֤���ص�ֵ���ж�Ӧ�� lref<T> ���͵�ֵ���õ�ǰ���е� T ���͵�ֵ��
		*/
		DeclIEntry(leo::any Refer() const)
		EndDecl




		template<typename _type1, typename _type2>
	struct HeldEqual : private examiners::equal_examiner

	{
		using examiners::equal_examiner::are_equal;
	};

#ifdef LB_IMPL_MSCPP
	template<>
	struct HeldEqual<std::ifstream, std::ifstream>
	{
		static bool are_equal(const std::ifstream&, const std::ifstream&)
		{
			return true;
		}
	};

	template<>
	struct HeldEqual<std::istringstream, std::istringstream>
	{
		static bool are_equal(const std::istringstream&, const std::istringstream&)
		{
			return true;
		}
	};
#endif

	template<typename _type1, typename _type2>
	struct HeldEqual<weak_ptr<_type1>, weak_ptr<_type2>>
	{
		static bool
			are_equal(const weak_ptr<_type1>& x, const weak_ptr<_type2>& y)
		{
			return x == y;
		}
	};



	template<typename _type1, typename _type2, typename _type3, typename _type4>
	struct HeldEqual<pair<_type1, _type2>, pair<_type3, _type4>>

	{
		static lconstfn bool
			are_equal(const pair<_type1, _type2>& x, const pair<_type3, _type4>& y)
		{
			return x.first == y.first && x.second == y.second;
		}
	};

	template<typename _type1, typename _type2>
	lconstfn bool
		AreEqualHeld(const _type1& x, const _type2& y)
	{
		return HeldEqual<_type1, _type2>::are_equal(x, y);
	}

	template<typename _type>
	class ValueHolder
		: protected boxed_value<_type>, implements IValueHolder
	{
		static_assert(std::is_object<_type>(), "Non-object type found.");
		static_assert(!is_cv<_type>(), "Cv-qualified type found.");

	public:
		using value_type = _type;

		//@{
		DefDeCtor(ValueHolder)
			template<typename _tParam,
			limpl(typename = exclude_self_t<ValueHolder, _tParam>)>
			ValueHolder(_tParam&& arg)
			lnoexcept(std::is_nothrow_constructible<_type, _tParam&&>::value)
			: boxed_value<_type>(lforward(arg))
		{}
		using boxed_value<_type>::boxed_value;
		//@}
		DefDeCopyMoveCtorAssignment(ValueHolder)

			PDefHOp(bool, == , const IValueHolder& obj) const ImplI(IValueHolder)
			ImplRet(type() == obj.type() && AreEqualHeld(this->value,
				Deref(static_cast<value_type*>(obj.get()))))

			leo::any
			Refer() const ImplI(IValueHolder);

			PDefH(ValueHolder*, clone, ) const ImplI(IValueHolder)
			ImplRet(try_new<ValueHolder>(*this))

			PDefH(void*, get, ) const ImplI(IValueHolder)
			ImplRet(leo::addressof(this->value))

			PDefH(const type_info&, type, ) const lnothrow ImplI(IValueHolder)
			ImplRet(type_id<_type>())
	};

	template<typename _type, class _tPointer = std::unique_ptr<_type>>
	class PointerHolder : implements IValueHolder
	{
		static_assert(std::is_object<_type>(), "Invalid type found.");

	public:
		using value_type = _type;
		using holder_pointer = _tPointer;
		using pointer = typename holder_pointer::pointer;

	protected:
		holder_pointer p_held;

	public:
		//! \brief ȡ������Ȩ��
		PointerHolder(pointer value)
			: p_held(value)
		{}
		//@{
		PointerHolder(const PointerHolder& h)
			: PointerHolder(leo::clone_monomorphic_ptr(h.p_held))
		{}
		DefDeMoveCtor(PointerHolder)
			//@}

			DefDeCopyAssignment(PointerHolder)
			DefDeMoveAssignment(PointerHolder)

			PDefHOp(bool, == , const IValueHolder& obj) const ImplI(IValueHolder)
			ImplRet(type() == obj.type() && AreEqualHeld(*p_held,
				Deref(static_cast<value_type*>(obj.get()))))

			leo::any
			Refer() const ImplI(IValueHolder);

			DefClone(const ImplI(IValueHolder), PointerHolder)

			PDefH(void*, get, ) const ImplI(IValueHolder)
			ImplRet(p_held.get())

			PDefH(const type_info&, type, ) const lnothrow ImplI(IValueHolder)
			ImplRet(p_held ? type_id<_type>() : type_id<void>())
	};

	/*!
	\brief �����ڽӿڵ����ö�̬���ͳ����ߡ�
	\tparam _type ���еı����õ�ֵ���͡�
	\note ���Գ���ֵ��������Ȩ��
	\sa ValueHolder
	*/
	template<typename _type>
	class RefHolder : implements IValueHolder
	{
		static_assert(std::is_object<_type>(), "Invalid type found.");

	public:
		using value_type
			= leo::remove_reference_t<leo::unwrap_reference_t<_type>>;

	private:
		ValueHolder<lref<value_type>> base;

	public:
		//! \brief ��ȡ������Ȩ��
		RefHolder(_type& r)
			: base(r)
		{}
		DefDeCopyMoveCtorAssignment(RefHolder)

			PDefHOp(bool, == , const IValueHolder& obj) const ImplI(IValueHolder)
			ImplRet(type() == obj.type() && AreEqualHeld(Deref(static_cast<
				value_type*>(get())), Deref(static_cast<value_type*>(obj.get()))))

			PDefH(leo::any, Refer, ) const ImplI(IValueHolder)
			ImplRet(leo::any(leo::any_ops::use_holder,
				leo::in_place<RefHolder>, *this))

			DefClone(const ImplI(IValueHolder), RefHolder)

			PDefH(void*, get, ) const ImplI(IValueHolder)
			ImplRet(leo::pvoid(std::addressof(
				Deref(static_cast<lref<value_type>*>(base.get())).get())))

			PDefH(const type_info&, type, ) const lnothrow ImplI(IValueHolder)
			ImplRet(leo::type_id<value_type>())
	};


	template<typename _type>
	leo::any
		ValueHolder<_type>::Refer() const
	{
		return leo::any(leo::any_ops::use_holder, leo::in_place<RefHolder<
			_type>>, leo::ref(this->value));
	}

	template<typename _type, class _tPointer>
	leo::any
		PointerHolder<_type, _tPointer>::Refer() const
	{
		if (const auto& p = p_held.get())
			return leo::any(leo::any_ops::use_holder,
				leo::in_place<RefHolder<_type>>, leo::ref(*p));
		leo::throw_invalid_construction();
	}

	class LS_API ValueObject : private equality_comparable<ValueObject>
	{
	public:
		/*!
		\brief ��������ݡ�
		*/
		using Content = leo::any;

	private:
		struct holder_refer_tag
		{};

		Content content;

	public:
		/*!
		\brief �޲������졣
		\note �õ���ʵ����
		*/
		DefDeCtor(ValueObject)
			/*!
			\brief ���죺ʹ�ö������á�
			\pre obj ����Ϊת�ƹ��������
			*/
			template<typename _type,
			limpl(typename = leo::exclude_self_t<ValueObject, _type>)>
			ValueObject(_type&& obj)
			: content(leo::any_ops::use_holder,
				leo::in_place<ValueHolder<leo::decay_t<_type>>>, lforward(obj))
		{}
		/*!
		\brief ���죺ʹ�ö����ʼ��������
		\tparam _type Ŀ�����͡�
		\tparam _tParams Ŀ�����ͳ�ʼ���������͡�
		\pre _type �ɱ� _tParams ������ʼ����
		*/
		template<typename _type, typename... _tParams>
		ValueObject(leo::in_place_type_t<_type>, _tParams&&... args)
			: content(leo::any_ops::use_holder,
				leo::in_place<ValueHolder<_type>>, lforward(args)...)
		{}
	private:
		/*!
		\brief ���죺ʹ�ó����ߡ�
		*/
		ValueObject(const IValueHolder& holder, holder_refer_tag)
			: content(holder.Refer())
		{}
	public:
		template<typename _type>
		ValueObject(_type& obj, OwnershipTag<>)
			: content(leo::any_ops::use_holder,
				leo::in_place<RefHolder<_type>>, leo::ref(obj))
		{}

		/*!
		\brief ���죺ʹ�ö���ָ�롣
		\note �õ�����ָ��ָ���ָ�������ʵ�������������Ȩ��
		\note ʹ�� PointerHolder ������Դ��Ĭ��ʹ�� delete �ͷ���Դ����
		*/
		template<typename _type>
		ValueObject(_type* p, PointerTag)
			: content(any_ops::use_holder,
				in_place<PointerHolder<_type>>, p)
		{}
		/*!
		\brief ���죺ʹ�ö��� unique_ptr ָ�롣
		\note �õ�����ָ��ָ���ָ�������ʵ�������������Ȩ��
		\note ʹ�� PointerHolder ������Դ��Ĭ��ʹ�� delete �ͷ���Դ����
		*/
		template<typename _type>
		ValueObject(unique_ptr<_type>&& p, PointerTag)
			: ValueObject(p.get(), PointerTag())
		{
			p.release();
		}
		/*!
		\brief ���ƹ��죺Ĭ��ʵ�֡�
		*/
		DefDeCopyCtor(ValueObject)
			/*!
			\brief ת�ƹ��죺Ĭ��ʵ�֡�
			*/
			DefDeMoveCtor(ValueObject)
			/*!
			\brief ������Ĭ��ʵ�֡�
			*/
			DefDeDtor(ValueObject)

			//@{
			DefDeCopyAssignment(ValueObject)
			DefDeMoveAssignment(ValueObject)
			//@}

			/*!
			\brief �ж��Ƿ�Ϊ�ջ�ǿա�
			*/
			DefBoolNeg(explicit, content.get_holder())

			//@{
			//! \brief �Ƚ���ȣ�������Ϊ�ջ򶼷ǿ��Ҵ洢�Ķ�����ȡ�
			LS_API friend bool
			operator==(const ValueObject&, const ValueObject&);


		/*!
		\brief ȡ��������ݡ�
		*/
		DefGetter(const lnothrow, const Content&, Content, content)

		/*!
		\brief ȡָ�����͵Ķ���
		\pre ��Ӷ��ԣ��洢�������ͺͷ��ʵ�����һ�¡�
		*/
		//@{
		template<typename _type>
		inline _type&
			GetObject() lnothrowv
		{
			return Deref(unchecked_any_cast<_type>(&content));
		}
		template<typename _type>
		inline const _type&
			GetObject() const lnothrowv
		{
			return Deref(unchecked_any_cast<const _type>(&content));
		}
		//@}
		//@}
		DefGetter(const lnothrow, const type_info&, Type, content.type())

			/*!
			\brief ����ָ�����Ͷ���
			\exception std::bad_cast ��ʵ�������ͼ��ʧ�� ��
			*/
			//@{
			template<typename _type>
		inline _type&
			Access()
		{
			return any_cast<_type&>(content);
		}
		template<typename _type>
		inline const _type&
			Access() const
		{
			return any_cast<const _type&>(content);
		}
		//@}

		/*!
		\brief ����ָ�����Ͷ���ָ�롣
		*/
		//@{
		template<typename _type>
		inline observer_ptr<_type>
			AccessPtr() lnothrow
		{
			return make_observer(any_cast<_type>(&content));
		}
		template<typename _type>
		inline observer_ptr<const _type>
			AccessPtr() const lnothrow
		{
			return make_observer(any_cast<const _type>(&content));
		}
		//@}

		/*!
		\brief �����
		\post <tt>*this == ValueObject()</tt> ��
		*/
		PDefH(void, Clear, ) lnothrow
			ImplExpr(content.reset())

			//@{
			template<typename _type, typename... _tParams>
		void
			Emplace(_tParams&&... args)
		{
			using Holder = ValueHolder<decay_t<_type>>;

			content.emplace<Holder>(leo::any_ops::use_holder,
				Holder(lforward(args)...));
		}
		template<typename _type>
		void
			Emplace(_type* p, PointerTag)
		{
			using Holder = PointerHolder<decay_t<_type>>;

			content.emplace<Holder>(leo::any_ops::use_holder, Holder(p));
		}

		//@}

			/*!
			\brief ȡ������õ�ֵ����
			*/
			ValueObject
			MakeIndirect() const;

			/*!
			\brief ������
			*/
			friend PDefH(void, swap, ValueObject& x, ValueObject& y) lnothrow
			ImplExpr(x.content.swap(y.content))
	};

	/*!
	\relates ValueObject
	*/
	//@{
	template<typename _func, typename... _tParams>
	void
		EmplaceFromCall(ValueObject&, leo::identity<void>, _func&& f,
			_tParams&&... args)
	{
		lforward(f)(lforward(args)...);
	}
	template<typename _type, typename _func, typename... _tParams>
	void
		EmplaceFromCall(ValueObject& vo, leo::identity<_type>, _func&& f,
			_tParams&&... args)
	{
		vo.Emplace<_type>(lforward(f)(lforward(args)...));
	}
	template<typename _func, typename... _tParams>
	void
		EmplaceFromCall(ValueObject& vo, _func&& f, _tParams&&... args)
	{
		leo::EmplaceFromCall(vo, leo::identity<leo::result_of_t<
			_func && (_tParams&&...)>>(), lforward(f), lforward(args)...);
	}

	template<typename _fCallable, typename... _tParams>
	void
		EmplaceFromInvoke(ValueObject&, leo::identity<void>, _fCallable&& f,
			_tParams&&... args)
	{
		leo::invoke(lforward(f), lforward(args)...);
	}
	template<typename _type, typename _fCallable, typename... _tParams>
	void
		EmplaceFromInvoke(ValueObject& vo, leo::identity<_type>, _fCallable&& f,
			_tParams&&... args)
	{
		vo.Emplace<_type>(leo::invoke(lforward(f), lforward(args)...));
	}
	template<typename _fCallable, typename... _tParams>
	void
		EmplaceFromInvoke(ValueObject& vo, _fCallable&& f, _tParams&&... args)
	{
		leo::EmplaceFromInvoke(leo::identity<decltype(leo::invoke(
			lforward(f), lforward(args)...))>(), lforward(f), lforward(args)...);
	}

	template<typename _type, typename... _tParams>
	_type&
		EmplaceIfEmpty(ValueObject& vo, _tParams&&... args)
	{
		if (!vo)
		{
			vo.Emplace<_type>(lforward(args)...);
			return vo.GetObject<_type>();
		}
		return vo.Access<_type>();
	}

	//! \brief �ж��Ƿ������ͬ����
	inline PDefH(bool, HoldSame, const ValueObject& x, const ValueObject& y)
		ImplRet(leo::hold_same(x.GetContent(), y.GetContent()))

	/*!
	\brief ������ģ�塣
	\tparam _type �������Ķ������ͣ����ܱ��޲������졣
	\tparam _tOwnerPointer ����������ָ�����͡�
	\warning ����������ָ����Ҫʵ������Ȩ���壬
	��������޷��ͷ���Դ�����ڴ�й©��������Ԥ����Ϊ��
	\todo �߳�ģ�ͼ���ȫ�ԡ�

	���ڱ�������Ĭ�϶��󣬿�ͨ��дʱ���Ʋ��Դ����¶��󣻿���Ϊ�ա�
	*/
	template<typename _type, class _tOwnerPointer = shared_ptr<_type>>
	class GDependency
	{
	public:
		using DependentType = _type;
		using PointerType = _tOwnerPointer;
		using ConstReferenceType = identity_t<decltype(*(PointerType()))>;
		using ReferentType = remove_const_t<remove_reference_t<
			ConstReferenceType>>;
		using ReferenceType = ReferentType&;

	private:
		PointerType ptr;

	public:
		inline
			GDependency(PointerType p = PointerType())
			: ptr(p)
		{
			GetCopyOnWritePtr();
		}

		DefDeCopyAssignment(GDependency)
			DefDeMoveAssignment(GDependency)

			DefCvt(const lnothrow, ConstReferenceType, *ptr)
			DefCvt(lnothrow, ReferenceType, *ptr)
			DefCvt(const lnothrow, bool, bool(ptr))

			DefGetter(const lnothrow, ConstReferenceType, Ref,
				operator ConstReferenceType())
			DefGetter(lnothrow, ReferenceType, Ref, operator ReferenceType())
			DefGetter(lnothrow, ReferenceType, NewRef, *GetCopyOnWritePtr())

			//! \post ����ֵ�ǿա�
			PointerType
			GetCopyOnWritePtr()
		{
			if (!ptr)
				ptr = PointerType(new DependentType());
			else if (!ptr.unique())
				ptr = PointerType(leo::clone_monomorphic(Deref(ptr)));
			return Nonnull(ptr);
		}

		inline
			void Reset()
		{
			reset(ptr);
		}
	};

	template<typename _type>
	class GMRange
	{
	public:
		using ValueType = _type;

	protected:
		ValueType max_value; //!< ���ȡֵ��
		ValueType value; //!< ֵ��

		/*!
		\brief ���죺ʹ��ָ�����ȡֵ��ֵ��
		*/
		GMRange(ValueType m, ValueType v)
			: max_value(m), value(v)
		{}

	public:
		DefGetter(const lnothrow, ValueType, MaxValue, max_value)
			DefGetter(const lnothrow, ValueType, Value, value)
	};

}

#endif
