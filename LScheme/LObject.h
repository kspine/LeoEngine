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

#include "sdef.h"

namespace leo
{
	/*!
	\brief ָ���Բ���ָ�����͵ĳ�Ա��������Ȩ�ı�ǩ��
	*/
	template<typename>
	struct OwnershipTag
	{};

	struct MoveTag
	{};

	struct PointerTag
	{};

	template<class _tOwner, typename _type>
	struct HasOwnershipOf : std::is_base_of<OwnershipTag<_type>, _tOwner>
	{};

	DeclDerivedI(LS_API, IValueHolder, any_ops::holder)

		DeclIEntry(bool operator==(const IValueHolder&) const)

		EndDecl


		template<typename _type1, typename _type2>
	struct HeldEqual : private examiners::equal_examiner

	{
		using examiners::equal_examiner::are_equal;
	};



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
		DefDeCopyCtor(ValueHolder)
			DefDeMoveCtor(ValueHolder)

			DefDeCopyAssignment(ValueHolder)
			DefDeMoveAssignment(ValueHolder)

			PDefHOp(bool, == , const IValueHolder& obj) const ImplI(IValueHolder)
			ImplRet(type() == obj.type() && AreEqualHeld(this->value,
				static_cast<const ValueHolder&>(obj).value))

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
			: PointerHolder(h.p_held ? new _type(*h.p_held) : nullptr)
		{}
		DefDeMoveCtor(PointerHolder)
			//@}

			DefDeCopyAssignment(PointerHolder)
			DefDeMoveAssignment(PointerHolder)

			PDefHOp(bool, == , const IValueHolder& obj) const ImplI(IValueHolder)
			ImplRet(type() == obj.type() && AreEqualHeld(*p_held,
				Deref(static_cast<const PointerHolder&>(obj).p_held)))

			DefClone(const ImplI(IValueHolder), PointerHolder)

			PDefH(void*, get, ) const ImplI(IValueHolder)
			ImplRet(p_held.get())

			PDefH(const type_info&, type, ) const lnothrow ImplI(IValueHolder)
			ImplRet(p_held ? type_id<_type>() : type_id<void>())
	};

	class LS_API ValueObject : private equality_comparable<ValueObject>
	{
	private:
		any content;

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
			limpl(typename = exclude_self_t<ValueObject, _type>)>
			ValueObject(_type&& obj)
			: content(any_ops::use_holder,
				in_place<ValueHolder<decay_t<_type>>>, lforward(obj))
		{}
		/*!
		\brief ���죺ʹ�ö����ʼ��������
		\tparam _type Ŀ�����͡�
		\tparam _tParams Ŀ�����ͳ�ʼ���������͡�
		\pre _type �ɱ� _tParams ������ʼ����
		*/
		template<typename _type, typename... _tParams>
		ValueObject(in_place_type_t<_type>, _tParams&&... args)
			: content(any_ops::use_holder,
				in_place<ValueHolder<_type>>, lforward(args)...)
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

		//@{
		template<typename _type, typename... _tParams>
		void
			Emplace(_tParams&&... args)
		{
			using Holder = ValueHolder<decay_t<_type>>;

			content.emplace<Holder>(any_ops::use_holder,
				Holder(lforward(args)...));
		}
		template<typename _type>
		void
			Emplace(_type* p, PointerTag)
		{
			using Holder = PointerHolder<decay_t<_type>>;

			content.emplace<Holder>(any_ops::use_holder, Holder(p));
		}

		//@{
		template<typename _func, typename... _tParams>
		void
			EmplaceFromCall(identity<void>, _func&& f,
				_tParams&&... args)
		{
			lforward(f)(lforward(args)...);
		}
		template<typename _type, typename _func, typename... _tParams>
		void
			EmplaceFromCall(identity<_type>, _func&& f,
				_tParams&&... args)
		{
			Emplace<_type>(lforward(f)(lforward(args)...));
		}
		template<typename _func, typename... _tParams>
		void
			EmplaceFromCall(_func&& f, _tParams&&... args)
		{
			EmplaceFromCall(identity<result_of_t<
				_func && (_tParams&&...)>>(), lforward(f), lforward(args)...);
		}

		template<typename _fCallable, typename... _tParams>
		void
			EmplaceFromInvoke(identity<void>, _fCallable&& f,
				_tParams&&... args)
		{
			invoke(lforward(f), lforward(args)...);
		}
		template<typename _type, typename _fCallable, typename... _tParams>
		void
			EmplaceFromInvoke(identity<_type>, _fCallable&& f,
				_tParams&&... args)
		{
			Emplace<_type>(invoke(lforward(f), lforward(args)...));
		}
		template<typename _fCallable, typename... _tParams>
		void
			EmplaceFromInvoke(_fCallable&& f, _tParams&&... args)
		{
			EmplaceFromInvoke(identity<decltype(invoke(lforward(f),
				lforward(args)...))>(), lforward(f), lforward(args)...);
		}
		//@}

		template<typename _type, typename... _tParams>
		_type&
			EmplaceIfEmpty(_tParams&&... args)
		{
			if (!*this)
			{
				Emplace<_type>(lforward(args)...);
				return GetObject<_type>();
			}
			return Access<_type>();
		}
		//@}

		/*!
		\brief �����
		\post <tt>*this == ValueObject()</tt> ��
		*/
		PDefH(void, Clear, ) lnothrow
			ImplExpr(content.reset())

			/*!
			\brief ������
			*/
			friend PDefH(void, swap, ValueObject& x, ValueObject& y) lnothrow
			ImplExpr(x.content.swap(y.content))
	};

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
				ptr = PointerType(clone_monomorphic(ptr));
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
