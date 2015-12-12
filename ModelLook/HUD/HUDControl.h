#ifndef HUD_Control_H
#define HUD_Control_H

#include <type_traits>
#include <bitset>
#include "WidgetEvent.h"
#include "Widget.h"

LEO_BEGIN

HUD_BEGIN




class LB_API Controller : public AController
{
public:
	/*!
	\brief �¼�ӳ���
	*/
	mutable EventMapping::MapType EventMap;

private:
	/*!
	\brief ָ���Ƿ����õ����롣
	*/
	std::bitset<static_cast<std::size_t>(VisualEvent::MaxEvent)> event_mask;

public:
	explicit
		Controller(bool b)
		: AController(b), EventMap()
	{}
	//! \since build 368
	template<typename... _tParams>
	Controller(bool b, _tParams&&... args)
		: AController(b), EventMap(lforward(args)...)
	{}

	//! \since build 581
	//@{
	PDefH(bool, IsEventEnabled, VisualEvent id) const ImplI(AController)
		ImplRet(AController::IsEnabled() && !event_mask[static_cast<std::size_t>(id)])

		PDefH(EventMapping::ItemType&, GetItem, VisualEvent id) const
		ImplI(AController)
		ImplRet(EventMap.at(id))
		EventMapping::ItemType&
		GetItemRef(VisualEvent, EventMapping::MappedType(&)()) const override;
	//@}
	//! \brief ȡ�¼�ӳ���
	DefGetter(const lnothrow, EventMapping::MapType&, EventMap, EventMap)

		PDefH(void, SetEventEnabled, VisualEvent id, bool b) ImplI(AController)
		ImplExpr(event_mask[static_cast<std::size_t>(id)] = !b)

		//! \since build 409
		DefClone(const ImplI(AController), Controller)
};


/*!
\ingroup helper_functions
\brief ȡ�����¼���
\tparam _vID ָ���¼����͡�
\param wgt ָ��������
\exception BadEvent �쳣�������ɿ������׳���
\note ��Ҫȷ�� EventTypeMapping ���ж�Ӧ�� EventType �������޷�ƥ��˺���ģ�塣
\note ���ؼ��¼����������Զ���ӿ��¼���
*/
template<VisualEvent _vID>
inline GEvent<typename EventTypeMapping<_vID>::HandlerType::FuncType>&
FetchEvent(IWidget& wgt)
{
	return FetchEvent<_vID>(wgt.GetController());
}

/*!
\brief ���ò����¼��������� BadEvent �쳣��
\pre �¼��������ת��Ϊ EventTypeMapping �� EventType ��
\note ���ؼ��¼�����������ԡ�
*/
template<VisualEvent _vID, typename _tEventArgs>
inline size_t
CallEvent(IWidget& wgt, _tEventArgs&& e)
{
	using HandlerType = typename EventTypeMapping<_vID>::HandlerType;
	static_assert(std::is_convertible<std::remove_reference_t<_tEventArgs>,
		std::remove_reference_t<typename EventArgsHead<typename
		HandlerType::TupleType>::type >>::value,
		"Invalid event argument type found @ CallEvent;");

	TryRet(DoEvent<HandlerType>(wgt.GetController(), _vID, std::move(e)))
		CatchIgnore(BadEvent&)
		return 0;
}

/*!
\brief ������������
\since build 236
*/
class LB_API WidgetController : public AController
{
public:
	//! \since build 581
	mutable GEventWrapper<GEvent<void(PaintEventArgs&&)>, UIEventArgs&&> Paint;

	/*!
	\brief ���죺ʹ��ָ�������ԡ�
	*/
	explicit
		WidgetController(bool = {});

	EventMapping::ItemType&
		GetItem(VisualEvent) const ImplI(AController);

	DefClone(const ImplI(AController), WidgetController)
};

class LB_API Control :public Widget
{
protected:
	DefExtendEventMap(LB_API ControlEventMap,VisualEventMap)
public:
	explicit
		Control(const Rect& = {});
	/*!
	\brief ���죺ʹ��ָ���߽�ͱ�����ˢ��
	\sa Control::Control
	*/
	explicit
		Control(const Rect&, HBrush);
	/*!
	\brief ���ƹ��죺������Ϊ������ơ�
	*/
	Control(const Control&);
	DefDeMoveCtor(Control)
	/*!
	\brief ���������ඨ����Ĭ��ʵ�֡�
	*/
	~Control();
};

HUD_END

LEO_END

#endif
