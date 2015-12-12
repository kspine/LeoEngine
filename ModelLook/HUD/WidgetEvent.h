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
HUD_END
LEO_END

#include "HUDControl.h"

LEO_BEGIN
HUD_BEGIN
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
HUD_END
LEO_END

#endif
