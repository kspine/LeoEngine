//  File name:   HUD/Label.hpp
//  Version:     v1.00
//  Created:     12/1/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: ��׼�����¼�����

#ifndef HUD_WIDGETEVENT_HPP
#define HUD_WIDGETEVENT_HPP

#include "HUDGraphics.h"
#include "HUDEvent.hpp"

LEO_BEGIN

HUD_BEGIN
	/*!
	\brief �û�����������ȼ���
	*/
	//@{
	lconstexpr const EventPriority BackgroundPriority(0xC0);
	lconstexpr const EventPriority BoundaryPriority(0x60);
	lconstexpr const EventPriority ForegroundPriority(0x40);
	//@}

	/*!
	\brief �û������¼��������ࡣ
	*/
	struct LB_API UIEventArgs
	{
	private:
		IWidget* pSender;

	public:
		explicit
			UIEventArgs(IWidget& wgt)
			: pSender(&wgt)
		{}
		/*!
		\brief ���ƹ��죺Ĭ��ʵ�֡�
		\since build 295
		*/
		DefDeCopyCtor(UIEventArgs)
			/*!
			\brief ���������ඨ����Ĭ��ʵ�֡�
			\since build 423
			*/
			virtual ~UIEventArgs();

		/*!
		\brief ���Ƹ�ֵ��Ĭ��ʵ�֡�
		\since build 295
		*/
		DefDeCopyAssignment(UIEventArgs)

			DefGetter(const lnothrow, IWidget&, Sender, *pSender)
			PDefH(void, SetSender, IWidget& wgt)
			ImplExpr(pSender = &wgt)
	};

	/*!
	\brief �������Ʋ�����
	*/
	struct LB_API PaintEventArgs : public UIEventArgs,public PaintContext
	{
		PaintEventArgs(IWidget&);
		PaintEventArgs(IWidget&, const PaintContext&);
		DefDeCopyCtor(PaintEventArgs)
			/*!
			\brief ���������ඨ����Ĭ��ʵ�֡�
			*/
			~PaintEventArgs() override;
	};

	using HPaintEvent = GHEvent<void(PaintEventArgs&&)>;

#define DefEventTypeMapping(_name, _tEventHandler) \
	template<> \
	struct EventTypeMapping<_name> \
	{ \
		using HandlerType = _tEventHandler; \
	};

	/*!
	\brief ��׼�ؼ��¼��ռ䡣
	*/
	enum class VisualEvent:std::size_t {
		Move,
		ReSize,
		Paint,

		MaxEvent,
	};

	template<VisualEvent>
	struct EventTypeMapping
	{
		//���� HandlerType ��Ĭ��ֵ���ܻᵼ�������� dynamic_cast ʧ�ܡ�
		//	using HandlerType = HEvent;
	};

	DefEventTypeMapping(VisualEvent::Paint, HPaintEvent)

	/*!
	\brief �¼�ӳ�������ռ䡣
	*/
	namespace EventMapping
	{
		using MappedType = GEventPointerWrapper<UIEventArgs&&>; //!< ӳ�������͡�
		using ItemType = GIHEvent<UIEventArgs&&>;
		using PairType = std::pair<VisualEvent, MappedType>;
		using MapType = std::map<VisualEvent, MappedType>; //!< ӳ������͡�
		using SearchResult = std::pair<typename MapType::iterator, bool>; //!< �����������͡�
	}

	using VisualEventMap = EventMapping::MapType;

	/*!
	\ingroup exception_types
	\brief ����򲻴��ڵĲ����¼��쳣��
	*/
	class LB_API BadEvent : public logged_event
	{
	public:
		using logged_event::logged_event;

		//@{
		DefDeCtor(BadEvent)
			DefDeCopyCtor(BadEvent)
			//! \brief ���������ඨ����Ĭ��ʵ�֡�
			~BadEvent() override;
		//@}
	};


	using HBrush = std::function<void(PaintEventArgs&&)>;
	

	/*!
	\brief ����ָ��ָ��� VisualEvent ָ�����¼�����
	*/
	template<VisualEvent _vID>
	EventMapping::MappedType
		NewEvent()
	{
		return EventMapping::MappedType(new GEventWrapper<GEvent<typename
			EventTypeMapping<_vID>::HandlerType::FuncType>, UIEventArgs&&>());
	}

	template<VisualEvent _vID>
	GEvent<typename EventTypeMapping<_vID>::HandlerType::FuncType>&
		FetchEvent(AController& controller)
	{
		return dynamic_cast<GEvent<typename EventTypeMapping<_vID>::HandlerType
			::FuncType>&>(controller.GetItemRef(_vID, NewEvent<_vID>));
	}


	/*!
	\brief �����������
	\since build 243
	*/
	class LB_API AController : public cloneable
	{
	private:
		bool enabled; //!< �ؼ������ԡ�

	public:
		/*!
		\brief ���죺ʹ��ָ�������ԡ�
		*/
		AController(bool b = true)
			: enabled(b)
		{}
		//! \since build 586
		DefDeCopyCtor(AController)
			/*!
			\brief ���������ඨ����Ĭ��ʵ�֡�
			\since build 295
			*/
			~AController() override;

		DefPred(const lnothrow, Enabled, enabled)
			/*!
			\brief �ж�ָ���¼��Ƿ����á�
			\note Ĭ��ʵ�֣������� Paint �¼���
			\since build 581
			*/
			virtual PDefH(bool, IsEventEnabled, VisualEvent id) const
			ImplRet(id == VisualEvent::Paint)

			/*!
			\brief ȡ�¼��
			\since build 581
			*/
			DeclIEntry(EventMapping::ItemType& GetItem(VisualEvent) const)
			/*!
			\brief ȡ�¼��������������ָ������ָ����ӡ�
			\note �������ʵ�ֿ����׳��쳣�����Լ����κ��¼��
			\since build 581
			*/
			virtual PDefH(EventMapping::ItemType&, GetItemRef, VisualEvent id,
				EventMapping::MappedType(&)()) const
			ImplRet(GetItem(id))

			DefSetter(bool, Enabled, enabled)
			/*!
			\brief ����ָ���¼��Ƿ����á�
			\throw ystdex::unsupported ��֧�������¼����ò�����
			\note Ĭ��ʵ�֣������׳��쳣��
			\since build 581
			*/
			virtual PDefH(void, SetEventEnabled, VisualEvent, bool)
			ImplThrow(unsupported("AController::SetEventEnabled"))

			/*
			\brief ����ʵ����
			\since build 409
			*/
			DeclIEntry(AController* clone() const ImplI(cloneable))
	};



template<class _tEventHandler>
size_t
DoEvent(AController& controller, VisualEvent id,
	typename EventArgsHead<typename _tEventHandler::TupleType>::type&& e)
{
	TryRet(dynamic_cast<GEvent<typename _tEventHandler::FuncType>&>(
		controller.GetItem(id))(std::move(e)))
		CatchIgnore(std::out_of_range&)
		CatchIgnore(std::bad_cast&)
		return 0;
}

/*!
\brief ���¼�ӳ�����ȡָ�� id ��Ӧ���¼���
*/
LB_API EventMapping::ItemType&
GetEvent(EventMapping::MapType&, VisualEvent, EventMapping::MappedType(&)());

HUD_END
LEO_END

#endif
