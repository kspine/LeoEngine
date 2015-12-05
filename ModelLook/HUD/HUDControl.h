#ifndef HUD_Control_H
#define HUD_Control_H

#include <type_traits>

LEO_BEGIN

HUD_BEGIN

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
		ImplRet(id == Paint)

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

HUD_END

LEO_END

#endif
