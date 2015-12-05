#ifndef HUD_Control_H
#define HUD_Control_H

#include "WidgetEvent.h"
#include <type_traits>

LEO_BEGIN

HUD_BEGIN

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

	TryRet(DoEvent<HandlerType>(wgt, _vID, std::move(e)))
		CatchIgnore(BadEvent&)
		return 0;
}

HUD_END

LEO_END

#endif
