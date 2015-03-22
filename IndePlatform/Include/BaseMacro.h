#ifndef IndePlatform_BaseMacro_h
#define IndePlatform_BaseMacro_h

#include "ldef.h"

/*
������������д�ĺ��壺
Ctor constructor
Cvt converter
De default
Decl declare
Def define
Del deleted
Dtor destructor
Expr expression
Fn function
Fwd forward
H head
I interface
Impl implement
Mem member
Op operator
P partially
PDecl pre-declare
Pred predicate
Ret returning
S statically
Tmpl template

���º������д�ĺ��壺
_a argument
_alist arguments list
_attr attributes
_b base
_e expression
_i interface
_m member
_n name
_op operator
_p parameter
_plist parameters list
_q qualifier(s)
_sig signature
_t type
*/

//ͨ��ͷ���塣
#define PDefH(_t, _n, ...) \
	_t \
	_n(__VA_ARGS__)
#define PDefHOp(_t, _op, ...) \
	PDefH(_t, operator _op, __VA_ARGS__)

#define PDefCvt(_t) \
	operator _t()


//��ͨ�ú���ʵ�֡�
//prefix "Impl" = Implementation;
#define ImplExpr(...) \
		{ \
		(__VA_ARGS__), void(); \
		}
#define ImplRet(...) \
		{ \
		return __VA_ARGS__; \
		}
#define ImplThrow(...) \
		{ \
		throw __VA_ARGS__; \
		}
// NOTE: GCC complains about 'void(lunseq(__VA_ARGS__))'.
#define ImplUnseq(...) \
		{ \
		lunused(lunseq(__VA_ARGS__)); \
		}

//����ͬ������ӳ��ͳ�Աͬ������ӳ��ʵ�֡�
//prefix "Impl" = Implement;
#define ImplBodyBase(_b, _n, ...) \
	ImplRet(_b::_n(__VA_ARGS__))
#define ImplBodyMem(_m, _n, ...) \
	ImplRet((_m)._n(__VA_ARGS__))


//��ͨ�ó�Ա�������塣
//prefix "Def" = Define;
/*!
\def DefDeDtor
\brief ����Ĭ������������
\note CWG defect 906 �Ľ����ֹ��ʽĬ���麯������ CWG defect 1135
�Ľ����������һ���ơ� ISO C++11 ����û�д����ơ�
\note ISO C++11 ����Ҫ��ʽʹ���쳣�淶�����Զ��Ƶ����μ� ISO C++11 12.4/3 ����
��ʽ�쳣�淶����ʹ����ʽ�̳���Ҫ��֤��Ա�������쳣�淶���ơ�
*/

#define DefDeCtor(_t) \
	_t() = default;
#define DefDelCtor(_t) \
	_t() = delete;

#define DefDeCopyCtor(_t) \
	_t(const _t&) = default;
#define DefDelCopyCtor(_t) \
	_t(const _t&) = delete;

#define DefDeMoveCtor(_t) \
	_t(_t&&) = default;
#define DefDelMoveCtor(_t) \
	_t(_t&&) = delete;

#define DefDeDtor(_t) \
	~_t() = default;
#define DefDelDtor(_t) \
	~_t() = delete;

#define ImplEmptyDtor(_t) \
	inline _t::DefDeDtor(_t)

#define DefDeCopyAssignment(_t) \
	_t& operator=(const _t&) = default;
#define DefDelCopyAssignment(_t) \
	_t& operator=(const _t&) = delete;

#define DefDeMoveAssignment(_t) \
	_t& operator=(_t&&) = default;
#define DefDelMoveAssignment(_t) \
	_t& operator=(_t&&) = delete;

#define DefCvt(_q, _t, ...) \
	operator _t() _q \
	ImplRet(__VA_ARGS__)
#define DefCvtBase(_q, _t, _b) \
	DefCvt(_q, _t, _b::operator _t())
#define DefCvtMem(_q, _t, _m) \
	DefCvt(_q, _t, (_m).operator _t())

#define DefPred(_q, _n, ...) \
	bool LPP_Concat(Is, _n)() _q \
	ImplRet(__VA_ARGS__)
#define DefPredBase(_q, _n, _b) \
	DefPred(_q, _n, _b::LPP_Concat(Is, _n)())
#define DefPredMem(_q, _n, _m) \
	DefPred(_q, _n, (_m).LPP_Concat(Is, _n)())

#define DefGetter(_q, _t, _n, ...) \
	_t LPP_Concat(Get, _n)() _q \
	ImplRet(__VA_ARGS__)
#define DefGetterBase(_q, _t, _n, _b) \
	DefGetter(_q, _t, _n, _b::LPP_Concat(Get, _n)())
#define DefGetterMem(_q, _t, _n, _m) \
	DefGetter(_q, _t, _n, (_m).LPP_Concat(Get, _n)())

#define DefSetter(_t, _n, _m) \
	void LPP_Concat(Set, _n)(_t _tempArgName) \
	ImplExpr((_m) = _tempArgName)
#define DefSetterDe(_t, _n, _m, _defv) \
	void LPP_Concat(Set, _n)(_t _tempArgName = _defv) \
	ImplExpr((_m) = _tempArgName)
#define DefSetterBase(_t, _n, _b) \
	void LPP_Concat(Set, _n)(_t _tempArgName) \
	ImplExpr(_b::LPP_Concat(Set, _n)(_tempArgName))
#define DefSetterBaseDe(_t, _n, _b, _defv) \
	void LPP_Concat(Set, _n)(_t _tempArgName = _defv) \
	ImplExpr(_b::LPP_Concat(Set, _n)(_tempArgName))
#define DefSetterMem(_t, _n, _m) \
	void LPP_Concat(Set, _n)(_t _tempArgName) \
	ImplExpr((_m).LPP_Concat(Set, _n)(_tempArgName))
#define DefSetterMemDe(_t, _n, _m, _defv) \
	void LPP_Concat(Set, _n)(_t _tempArgName = _defv) \
	ImplExpr((_m).LPP_Concat(Set, _n)(_tempArgName))
#define DefSetterEx(_t, _n, _m, ...) \
	void LPP_Concat(Set, _n)(_t _tempArgName) \
	ImplExpr((_m) = (__VA_ARGS__))
#define DefSetterDeEx(_t, _n, _m, _defv, ...) \
	void LPP_Concat(Set, _n)(_t _tempArgName = _defv) \
	ImplExpr((_m) = (__VA_ARGS__))


/*!
\def DefClone
\brief ��̬���ơ�
\note ��Ҫ������ \c CopyConstructible ����Ķ����ڡ�
\note ����Ҫ��̬���ƣ���Ҫ��ʾǰ�� \c virtual ����� \c override ��ָʾ����
\since build 409
*/
#define DefClone(_q, _t) \
	PDefH(_t*, clone, ) _q \
		ImplRet(new _t(*this))


/*!
\def DefSwap
\brief ������Ա��
\note ��Ҫ��Ӧ���;��н���������ֵ���õ� swap ��Ա������
\since build 409
*/
#define DefSwap(_q, _t) \
	PDefH(void, swap, _t& _x, _t& _y) _q \
		ImplExpr(_x.swap(_y))


//��Ա������ģ��ӳ�䡣


/*!
\brief ���ݺ�����
\since build 266
*/
#define DefFwdFn(_q, _t, _n, ...) \
	inline _t \
	_n() _q \
		{ \
		return (__VA_ARGS__); \
		}

/*!
\brief ����ģ�塣
\since build 266
*/
#define DefFwdTmpl(_q, _t, _n, ...) \
	template<typename... _tParams> \
	inline _t \
	_n(_tParams&&... args) _q \
		{ \
		return (__VA_ARGS__); \
		}


/*!	\defgroup InterfaceTypeMacros Interface Type Macros
\brief �ӿ����ͺꡣ
\since build 161
*/
//@{

#define _lInterface struct

#define implements public

/*!
\def _lInterfaceHead
\brief ����ӿ�����ͷ����
\sa ImplEmptyDtor
*/
#define _lInterfaceHead(_n) { \
protected: \
	DefDeCtor(_n) \
\
public: \
	virtual DefDeDtor(_n)

#define FwdDeclI(_n) _lInterface _n;

/*!
\def DeclI
\brief ����ӿ����͡�
\since build 362
*/
#define DeclI(_attr, _n) \
	_lInterface _attr _n \
	_lInterfaceHead(_n)

/*
\def DeclDerivedI
\brief ���������ӿ����͡�
\note ���ڽӿڶ���Ϊ struct ���ͣ����ͨ��ֻ��ָ���Ƿ�Ϊ virtual �̳С�
\since build 362
*/
#define DeclDerivedI(_attr, _n, ...) \
	_lInterface _attr _n : __VA_ARGS__ \
	_lInterfaceHead(_n)

// ImplI = Implements Interface;
#define ImplI(...) virtual

//����ʵ�֣������ӿڹ�������ʵ�֣������ṩ�ӿں�����Ĭ��ʵ�֣���
// ImplA = Implements Abstractly;
#define ImplA(...)

#define DeclIEntry(_sig) virtual _sig = 0;

#define EndDecl };


/*!
\brief ��̬�ӿڡ�
\since build 266
*/
#define DeclSEntry(...)
/*!
\brief ��̬�ӿ�ʵ�֡�
\since build 266
*/
#define ImplS(...)
//@}


/*!
\brief ����ֱ�������ࡣ
\note �����캯�����������Ա�����������Ƭ��������Ա�洢й©���⡣
\since build 352
*/
#define DefExtendClass(_attr, _n, ...) \
	class _attr _n : __VA_ARGS__ \
		{ \
	public: \
		_n(); \
		};


/*!
\brief λ�������Ͳ�����
\note �μ� ISO C++11 17.5.2.1.3[bitmask.types] ��
\since build 270
*/
//@{
#define DefBitmaskAnd(_tBitmask, _tInt) \
	lconstfn _tBitmask operator&(_tBitmask _x, _tBitmask _y) \
		ImplRet(static_cast<_tBitmask>( \
			static_cast<_tInt>(_x) & static_cast<_tInt>(_y)))

#define DefBitmaskOr(_tBitmask, _tInt) \
	lconstfn _tBitmask operator|(_tBitmask _x, _tBitmask _y) \
		ImplRet(static_cast<_tBitmask>( \
			static_cast<_tInt>(_x) | static_cast<_tInt>(_y)))

#define DefBitmaskXor(_tBitmask, _tInt) \
	lconstfn _tBitmask operator^(_tBitmask _x, _tBitmask _y) \
		ImplRet(static_cast<_tBitmask>( \
			static_cast<_tInt>(_x) ^ static_cast<_tInt>(_y)))

#define DefBitmaskNot(_tBitmask, _tInt) \
	lconstfn _tBitmask operator~(_tBitmask _x) \
		ImplRet(static_cast<_tBitmask>(~static_cast<_tInt>(_x)))

#define DefBitmaskAndAssignment(_tBitmask, _tInt) \
	inline _tBitmask& operator&=(_tBitmask& _x, _tBitmask _y) \
		ImplRet(_x = _x & _y)

#define DefBitmaskOrAssignment(_tBitmask, _tInt) \
	inline _tBitmask& operator|=(_tBitmask& _x, _tBitmask _y) \
		ImplRet(_x = _x | _y)

#define DefBitmaskXorAssignment(_tBitmask, _tInt) \
	inline _tBitmask& operator^=(_tBitmask& _x, _tBitmask _y) \
		ImplRet(_x = _x ^ _y)

#define DefBitmaskOperations(_tBitmask, _tInt) \
	DefBitmaskAnd(_tBitmask, _tInt) \
	DefBitmaskOr(_tBitmask, _tInt) \
	DefBitmaskXor(_tBitmask, _tInt) \
	DefBitmaskNot(_tBitmask, _tInt) \
	DefBitmaskAndAssignment(_tBitmask, _tInt) \
	DefBitmaskOrAssignment(_tBitmask, _tInt) \
	DefBitmaskXorAssignment(_tBitmask, _tInt)

#define DefBitmaskEnum(_tEnum) \
	DefBitmaskOperations(_tEnum, typename std::underlying_type<_tEnum>::type)

#endif