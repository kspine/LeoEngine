//  File name:   HUD/Label.hpp
//  Version:     v1.00
//  Created:     12/1/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: HUD�¼�
#ifndef HUD_EVENT_HPP
#define HUD_EVENT_HPP

#include <ldef.h>
#include <BaseMacro.h>
#include <functional.hpp>
#include <ref.hpp>
#include <map>
LEO_BEGIN

namespace HUD
{
	/*!
	\brief �¼��������ӿ�ģ�塣
	*/
	template<typename... _tParams>
	DeclDerivedI(,GIHEvent,cloneable)
		DeclIEntry(size_t operator()(_tParams...) const)
		DeclIEntry(GIHEvent* clone() const ImplI(cloneable))
	EndDecl

		template<typename... _tParams>
	GIHEvent<_tParams...>::DefDeDtor(GIHEvent)


	/*!
	\brief ��׼�¼�������ģ�塣
	*/
	//@{
	template<typename>
	class GHEvent;

	template<typename _tRet, typename... _tParams>
	class GHEvent<_tRet(_tParams...)>
		: protected std::function<_tRet(_tParams...)>
	{
	public:
		using TupleType = std::tuple<_tParams...>;
		using FuncType = _tRet(_tParams...);
		using BaseType = std::function<FuncType>;

	private:
		//! \brief �ȽϺ������͡�
		using Comparer = bool(*)(const GHEvent&, const GHEvent&);
		template<class _tFunctor>
		struct GEquality
		{
			//! \since build v1.3
			//@{
			using decayed_type = std::decay_t<_tFunctor>;

			static bool
				AreEqual(const GHEvent& x, const GHEvent& y)
				lnoexcept_spec(std::declval<const decayed_type>()
					== std::declval<const decayed_type>())
			{
				const auto p(x.template target<decayed_type>());

				if (const auto q = y.template target<decayed_type>())
					return p == q || *p == *q;
				else
					return !p;
				return{};
			}
			//@}
		};

		Comparer comp_eq; //!< �ȽϺ�������ȹ�ϵ��

	public:
		/*!
		\brief ���죺ʹ�ú���ָ�롣
		*/
		lconstfn
			GHEvent(FuncType* f = {})
			: BaseType(f), comp_eq(GEquality<FuncType>::AreEqual)
		{}
		/*!
		\brief ʹ�ú�������
		*/
		template<class _fCallable>
		lconstfn
			GHEvent(_fCallable f, std::enable_if_t<
				std::is_constructible<BaseType, _fCallable>::value, int> = 0)
			: BaseType(f), comp_eq(GetComparer(f, f))
		{}
		/*!
		\brief ʹ����չ��������
		\todo �ƶϱȽ���Ȳ�����
		*/
		template<class _fCallable>
		lconstfn
			GHEvent(_fCallable&& f, std::enable_if_t<
				!std::is_constructible<BaseType, _fCallable>::value, int> = 0)
			: BaseType(make_expanded<_tRet(_tParams...)>(lforward(f))),
			comp_eq(GHEvent::AreAlwaysEqual)
		{}
		/*!
		\brief ���죺ʹ�ö������úͳ�Ա����ָ�롣
		\warning ʹ�ÿճ�Աָ�빹��ĺ��������������δ������Ϊ��
		*/
		template<class _type>
		lconstfn
			GHEvent(_type& obj, _tRet(_type::*pm)(_tParams...))
			: GHEvent([&, pm](_tParams... args) lnoexcept(
				noexcept((obj.*pm)(lforward(args)...))
				&& std::is_nothrow_copy_constructible<_tRet>::value) {
			return (obj.*pm)(lforward(args)...);
		})
		{}
		DefDeCopyMoveCtorAssignment(GHEvent)

			//! \since build 520
			lconstfn friend bool
			operator==(const GHEvent& x, const GHEvent& y)
		{
			return
#if defined(LB_DLL) || defined(LB_BUILD_DLL)
				x.BaseType::target_type() == y.BaseType::target_type()
#else
				x.comp_eq == y.comp_eq
#endif
				&& (x.comp_eq(x, y));
		}

		/*!
		\brief ���á�
		*/
		using BaseType::operator();

		//! \since build 516
		using BaseType::operator bool;

	private:
		//! \since build 319
		//@{
		template<typename _type>
		static lconstfn Comparer
			GetComparer(_type& x, _type& y, decltype(x == y) = {}) lnothrow
		{
			return GEquality<_type>::AreEqual;
		}
		template<typename _type, typename _tUnused>
		static lconstfn Comparer
			GetComparer(_type&, _tUnused&) lnothrow
		{
			return GHEvent::AreAlwaysEqual;
		}

		static lconstfn bool
			AreAlwaysEqual(const GHEvent&, const GHEvent&) lnothrow
		{
			return true;
		}
		//@}
	};
	//@}

	/*!
	\brief �¼����ȼ���
	*/
	using EventPriority = std::uint8_t;


	/*!
	\brief Ĭ���¼����ȼ���
	*/
	lconstexpr const EventPriority DefaultEventPriority(0x80);


	/*!
	\brief �¼�ģ�塣
	\note ֧��˳��ಥ��
	*/
	//@{
	template<typename>
	class GEvent;

	template<typename _tRet, typename... _tParams>
	class GEvent<_tRet(_tParams...)>
	{
	public:
		using HandlerType = GHEvent<_tRet(_tParams...)>;
		using TupleType = typename HandlerType::TupleType;
		using FuncType = typename HandlerType::FuncType;
		/*!
		\brief �������͡�
		\since build 294
		*/
		using ContainerType
			= std::multimap<EventPriority, HandlerType, std::greater<EventPriority>>;
		//! \since build 573
		//@{
		using const_iterator = typename ContainerType::const_iterator;
		using const_reference = typename ContainerType::const_reference;
		using const_reverse_iterator
			= typename ContainerType::const_reverse_iterator;
		using iterator = typename ContainerType::iterator;
		using reference = typename ContainerType::reference;
		using reverse_iterator = typename ContainerType::reverse_iterator;
		using size_type = typename ContainerType::size_type;
		using value_type = typename ContainerType::value_type;
		//@}

	private:
		/*!
		\brief ��Ӧ�б�
		\since build 572
		*/
		ContainerType handlers;

	public:
		/*!
		\brief �޲������죺Ĭ��ʵ�֡�
		\note �õ���ʵ����
		*/
		lconstfn DefDeCtor(GEvent)
			/*!
			\brief ���죺����¼���������
			\since build 448
			*/
			template<typename _tHandler,
			limpl(typename = exclude_self_ctor_t<GEvent, _tHandler>)>
			GEvent(_tHandler&& h)
			: handlers()
		{
			Add(lforward(h));
		}
		DefDeCopyMoveCtorAssignment(GEvent)

			/*!
			\brief ��ֵ�������¼���Ӧ��ʹ�õ�һ�������ָ����ָ���¼���������
			\since build 448
			*/
			template<typename _tHandler,
			limpl(typename = exclude_self_ctor_t<GEvent, _tHandler>)>
			inline GEvent&
			operator=(_tHandler&& _arg)
		{
			return *this = GEvent(forward(_arg));
		}

		/*!
		\brief ����¼���Ӧ��ʹ�� const �¼������������ȼ���
		\note ������Ƿ��Ѿ����б��С�
		*/
		inline PDefHOp(GEvent&, +=, const HandlerType& h)
			ImplRet(Add(h))
			/*!
			\brief ����¼���Ӧ��ʹ���¼���������
			\note ������Ƿ��Ѿ����б��С�
			*/
			inline PDefHOp(GEvent&, +=, HandlerType&& h)
			ImplRet(Add(std::move(h)))
			/*!
			\brief ����¼���Ӧ��Ŀ��Ϊ��һ�������ָ����ָ���¼���������
			\note ������Ƿ��Ѿ����б��С�
			*/
			template<typename _type>
		inline GEvent&
			operator+=(_type&& _arg)
		{
			return Add(HandlerType(lforward(_arg)));
		}

		/*!
		\brief �Ƴ��¼���Ӧ��ָ�� const �¼���������
		*/
		GEvent&
			operator-=(const HandlerType& h)
		{
			ystdex::erase_all_if<ContainerType>(handlers, handlers.cbegin(),
				handlers.cend(), [&](decltype(*handlers.cbegin()) pr) {
				return pr.second == h;
			});
			return *this;
		}
		/*!
		\brief �Ƴ��¼���Ӧ��ָ���� const �¼���������
		\note ��ֹģ�� <tt>operator-=</tt> �ݹ顣
		*/
		inline PDefHOp(GEvent&, -=, HandlerType&& h)
			ImplRet(*this -= static_cast<const HandlerType&>(h))
			/*!
			\brief �Ƴ��¼���Ӧ��Ŀ��Ϊ��һ�������ָ����ָ���¼���������
			\since build 293
			*/
			template<typename _type>
		inline GEvent&
			operator-=(_type&& _arg)
		{
			return *this -= HandlerType(forward(_arg));
		}

		/*!
		\brief �����¼���Ӧ��
		\note ������Ƿ��Ѿ����б��С�
		\sa Insert
		*/
		//@{
		/*!
		\note ʹ�� const �¼������������ȼ���
		\since build 294
		*/
		inline PDefH(GEvent&, Add, const HandlerType& h,
			EventPriority prior = DefaultEventPriority)
			ImplRet(Insert(h, prior), *this)
			/*!
			\note ʹ�÷� const �¼������������ȼ���
			\since build 294
			*/
			inline PDefH(GEvent&, Add, HandlerType&& h,
				EventPriority prior = DefaultEventPriority)
			ImplRet(Insert(std::move(h), prior), *this)
			/*!
			\note ʹ�õ�һ�������ָ�����¼������������ȼ���
			\since build 294
			*/
			template<typename _type>
		inline GEvent&
			Add(_type&& _arg, EventPriority prior = DefaultEventPriority)
		{
			return Add(HandlerType(lforward(_arg)), prior);
		}
		/*!
		\note ʹ�ö������á���Ա����ָ������ȼ���
		\since build 413
		*/
		template<class _tObj, class _type>
		inline GEvent&
			Add(_tObj& obj, _tRet(_type::*pm)(_tParams...),
				EventPriority prior = DefaultEventPriority)
		{
			return Add(HandlerType(static_cast<_type&>(obj), std::move(pm)), prior);
		}
		//@}

		/*!
		\brief �����¼���Ӧ��
		\note ������Ƿ��Ѿ����б��С�
		\since build 572
		*/
		//@{
		//! \note ʹ�� const �¼������������ȼ���
		inline PDefH(typename ContainerType::iterator, Insert, const HandlerType& h,
			EventPriority prior = DefaultEventPriority)
			ImplRet(handlers.emplace(prior, h))
			//! \note ʹ�÷� const �¼������������ȼ���
			inline PDefH(typename ContainerType::iterator, Insert, HandlerType&& h,
				EventPriority prior = DefaultEventPriority)
			ImplRet(handlers.emplace(prior, std::move(h)))
			//! \note ʹ�õ�һ�������ָ�����¼������������ȼ���
			template<typename _type>
		inline typename ContainerType::iterator
			Insert(_type&& _arg, EventPriority prior = DefaultEventPriority)
		{
			return Insert(HandlerType(forward(_arg)), prior);
		}
		//! \note ʹ�ö������á���Ա����ָ������ȼ���
		template<class _tObj, class _type>
		inline typename ContainerType::iterator
			Insert(_tObj& obj, _tRet(_type::*pm)(_tParams...),
				EventPriority prior = DefaultEventPriority)
		{
			return
				Insert(HandlerType(static_cast<_type&>(obj), std::move(pm)), prior);
		}
		//@}

		/*!
		\brief �Ƴ��¼���Ӧ��Ŀ��Ϊָ���������úͳ�Ա����ָ�롣
		\since build 413
		*/
		template<class _tObj, class _type>
		inline GEvent&
			Remove(_tObj& obj, _tRet(_type::*pm)(_tParams...))
		{
			return *this -= HandlerType(static_cast<_type&>(obj), std::move(pm));
		}

		/*!
		\brief �ж��Ƿ����ָ���¼���Ӧ��
		*/
		bool
			Contains(const HandlerType& h) const
		{
			using get_value;

			return std::count(handlers.cbegin() | get_value,
				handlers.cend() | get_value, h) != 0;
		}
		/*!
		\brief �ж��Ƿ������һ�������ָ�����¼���Ӧ��
		\since build 293
		*/
		template<typename _type>
		inline bool
			Contains(_type&& _arg) const
		{
			return Contains(HandlerType(lforward(_arg)));
		}

		/*!
		\brief �����¼���������
		\return �ɹ����õ��¼�������������
		\exception std::bad_function_call �����쳣������
		\since build 573
		*/
		size_type
			operator()(_tParams... args) const
		{
			size_type n(0);

			for (const auto& pr : handlers)
			{
				TryExpr(pr.second(lforward(args)...))
					CatchIgnore(std::bad_function_call&)
					++n;
			}
			return n;
		}

		//! \since build 573
		PDefH(const_iterator, cbegin, ) const lnothrow
			ImplRet(handlers.cbegin())

			//! \since build 572
			//@{
			PDefH(iterator, begin, ) lnothrow
			ImplRet(handlers.begin())
			PDefH(iterator, begin, ) const lnothrow
			ImplRet(handlers.begin())

			//! \since build 573
			PDefH(const_iterator, cend, ) const lnothrow
			ImplRet(handlers.cend())

			//! \brief ������Ƴ������¼���Ӧ��
			PDefH(void, clear, ) lnothrow
			ImplRet(handlers.clear())

			//! \since build 573
			//@{
			PDefH(size_type, count, EventPriority prior) const lnothrow
			ImplRet(handlers.count(prior))

			PDefH(const_reverse_iterator, crbegin, ) const lnothrow
			ImplRet(handlers.crbegin())

			PDefH(const_reverse_iterator, crend, ) const lnothrow
			ImplRet(handlers.crend())
			//@}

			PDefH(bool, empty, ) const lnothrow
			ImplRet(handlers.empty())

			PDefH(iterator, end, ) lnothrow
			ImplRet(handlers.end())
			PDefH(iterator, end, ) const lnothrow
			ImplRet(handlers.end())
			//@}

			//! \since build 573
			//@{
			PDefH(reverse_iterator, rbegin, ) lnothrow
			ImplRet(handlers.rbegin())

			PDefH(reverse_iterator, rend, ) lnothrow
			ImplRet(handlers.rend())

			//! \brief ȡ�б��е���Ӧ����
			PDefH(size_type, size, ) const lnothrow
			ImplRet(handlers.size())
			//@}

			/*
			\brief ������
			\since build 409
			*/
			PDefH(void, swap, GEvent& e) lnothrow
			ImplRet(handlers.swap(e))
	};
	//@}


	/*!
	\brief ��ӵ�һ�¼���Ӧ��ɾ������ӡ�
	*/
	//@{
	template<typename _tRet, typename... _tParams>
	inline GEvent<_tRet(_tParams...)>&
		AddUnique(GEvent<_tRet(_tParams...)>& evt,
			const typename GEvent<_tRet(_tParams...)>::HandlerType& h,
			EventPriority prior = DefaultEventPriority)
	{
		return (evt -= h).Add(h, prior);
	}
	template<typename _tRet, typename... _tParams>
	inline GEvent<_tRet(_tParams...)>&
		AddUnique(GEvent<_tRet(_tParams...)>& evt, typename GEvent<_tRet(_tParams...)>
			::HandlerType&& h, EventPriority prior = DefaultEventPriority)
	{
		return (evt -= h).Add(std::move(h), prior);
	}
	template<typename _type, typename _tRet, typename... _tParams>
	inline GEvent<_tRet(_tParams...)>&
		AddUnique(GEvent<_tRet(_tParams...)>& evt, _type&& arg,
			EventPriority prior = DefaultEventPriority)
	{
		return AddUnique(evt, HandlerType(yforward(arg)), prior);
	}
	template<class _type, typename _tRet, typename... _tParams>
	inline GEvent<_tRet(_tParams...)>&
		AddUnique(GEvent<_tRet(_tParams...)>& evt, _type& obj,
			_tRet(_type::*pm)(_tParams...), EventPriority prior = DefaultEventPriority)
	{
		return AddUnique(evt, HandlerType(static_cast<_type&>(obj), std::move(pm)),
			prior);
	}
	//@}

	/*!
	\relates GEvent
	*/
	template<typename _tRet, typename... _tParams>
	inline DefSwap(lnothrow, GEvent<_tRet(_tParams...)>)


		/*!
		\brief ʹ�� RAII ������¼������ࡣ
		\warning ����������
		*/
	template<typename... _tEventArgs>
	class GEventGuard
	{
	public:
		using EventType = GEvent<_tEventArgs...>;
		using HandlerType = GHEvent<_tEventArgs...>;
		//! \since build 554
		lref<EventType> Event;
		HandlerType Handler;

		template<typename _type>
		GEventGuard(EventType& evt, _type&& handler,
			EventPriority prior = DefaultEventPriority)
			: Event(evt), Handler(yforward(handler))
		{
			Event.get().Add(Handler, prior);
		}
		~GEventGuard()
		{
			Event.get() -= Handler;
		}
	};

	template<typename... _tParams>
	struct EventArgsHead
	{
		using type = std::conditional_t<sizeof...(_tParams) == 0, void,
			std::tuple_element_t<0, std::tuple<_tParams...>>>;
	};


	template<typename... _tParams>
	struct EventArgsHead<std::tuple<_tParams...>> : EventArgsHead<_tParams...>
	{};


	/*!
	\brief �¼���װ��ģ�塣
	*/
	template<class _tEvent, typename _tBaseArgs>
	class GEventWrapper : public _tEvent, implements GIHEvent<_tBaseArgs>
	{
	public:
		using EventType = _tEvent;
		using BaseArgsType = _tBaseArgs;
		using EventArgsType
			= typename EventArgsHead<typename _tEvent::TupleType>::type;

		/*!
		\brief ί�е��á�
		\warning ��Ҫȷ�� BaseArgsType ���õĶ����ܹ�ת���� EventArgsType ��
		*/
		size_t
			operator()(BaseArgsType e) const ImplI(GIHEvent<_tBaseArgs>)
		{
			return EventType::operator()(EventArgsType(lforward(e)));
		}

		DefClone(const ImplI(GIHEvent<_tBaseArgs>), GEventWrapper)
	};

	/*!
	\brief �¼������͡�
	\warning ����������
	*/
	template<typename _tBaseArgs>
	class GEventPointerWrapper
	{
	public:
		using ItemType = GIHEvent<_tBaseArgs>;
		using PointerType = std::unique_ptr<ItemType>;

	private:
		PointerType ptr;

	public:
		template<typename _type, limpl(
			typename = exclude_self_ctor_t<GEventPointerWrapper, _type>)>
			inline
			GEventPointerWrapper(_type&& p)
			lnoexcept(std::is_nothrow_constructible<PointerType, _type>::value)
			: ptr(Nonnull(p))
		{}
		/*!
		\brief ���ƹ��죺��ơ�
		*/
		GEventPointerWrapper(const GEventPointerWrapper& item)
			: ptr(ClonePolymorphic(item.ptr))
		{}
		DefDeMoveCtor(GEventPointerWrapper)

		lconstfn DefCvt(const lnothrow, const ItemType&, *ptr)
		lconstfn DefCvt(const lnothrow, ItemType&, *ptr)
	};
}

LEO_END

#endif
