#ifndef IndePlatform_utility_hpp
#define IndePlatform_utility_hpp

#include "type_op.hpp" //leo::qualified_decay,assert
#include "Abstract.hpp"
#include <stdexcept> //std::logic_error

namespace leo
{
//������˶��Ե���Ϊ�ǽӿ���ȷ��δ����ģ���Ϊ����Ԥ��
#define lconstraint assert
//����ʱ���Ļ�������Լ�����ԡ�������ȷ�ط� yconstraint ���õ�����
#define lassume assert

	class LB_API unsupported : public std::logic_error
	{
	public:
		unsupported()
			:logic_error("Unsupported operation found.")
		{}
		template<typename type>
		unsupported(type && arg)
			: logic_error(lforward(arg))
		{}
	};

	class LB_API unimplemented : public unsupported
	{
	public:
		unimplemented()
			: unsupported("Unimplemented operation found.")
		{}
		template<typename _type>
		unimplemented(_type&& arg)
			: unsupported(lforward(arg))
		{}
	};

	class noncopyable
	{
	protected:
		lconstfn
			noncopyable() = default;
		~noncopyable() = default;

	public:
		lconstfn
			noncopyable(const noncopyable&) = delete;

		noncopyable&
			operator=(const noncopyable&) = delete;
	};

	class nonmovable
	{
	protected:
		lconstfn
			nonmovable() = default;
		~nonmovable() = default;

	public:
		lconstfn
			nonmovable(const nonmovable&) = delete;

		nonmovable&
			operator=(const nonmovable&) = delete;
	};


	class LB_API cloneable
	{
	public:
#if LB_IMPL_MSCPP
		cloneable() = default;
		cloneable(const cloneable&) = default;
		cloneable(cloneable&&)
		{}
#endif
		virtual cloneable*
			clone() const = 0;

		virtual
			~cloneable()
		{}
	};


	/*
	\see ISO WG21/N3797 20.2.3[utility.exchange] ��
	\see http://www.open-std.org/JTC1/sc22/WG21/docs/papers/2013/n3668.html ��
	*/
	template<typename _type, typename _type2 = _type>
	_type
		exchange(_type& obj, _type2&& new_val)
	{
		_type old_val = std::move(obj);

		obj = std::forward<_type2>(new_val);
		return old_val;
	}


	/*!
	\see ISO C++11 30.2.6[thread.decaycopy] ��
	\see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2011/n3255.html ��
	*/
	template<typename _type>
	decay_t<_type>
		decay_copy(_type&& arg)
	{
		return std::forward<_type>(arg);
	}

	/*!
	\brief �˻����ݡ�
	\note ���� decay_copy �������Ժ��������鼰�����������˻�������������ֵ��
	*/
	template<typename _type>
	typename qualified_decay<_type>::type
		decay_forward(_type&& arg)
	{
		return std::forward<_type>(arg);
	}


	/*!
	\brief ����ָ���������Ͷ���ĳ��ȡ�
	*/
	template<typename _type, size_t _vN>
	lconstfn size_t
		arrlen(_type(&)[_vN])
	{
		return _vN;
	}
	template<typename _type, size_t _vN>
	lconstfn size_t
		arrlen(_type(&&)[_vN])
	{
		return _vN;
	}


	/*!
	\brief ��װ�����͵�ֵ�Ķ���
	\warning ����������
	*/
	template<typename _type>
	struct boxed_value
	{
		_type value;

		template<typename... _tParams>
		lconstfn
			boxed_value(_tParams&&... args)
			: value(yforward(args)...)
		{}

		operator _type&() lnothrow
		{
			return value;
		}

		operator const _type&() const lnothrow
		{
			return value;
		}
	};


	/*!
	\brief ��װ��������Ϊ�����͡�
	*/
	template<typename _type>
	using classify_value_t = conditional_t<std::is_class<_type>::value, _type,
		boxed_value<_type >> ;


	/*!
	\brief ����ʶ���ú�������֤����һ�Ρ�
	\note ���� std::call_once ��������֤�̰߳�ȫ�ԡ�
	\note ISO C++11���� N3691 �� 30.4 synopsis �����������ڴ���
	\see https://github.com/cplusplus/draft/issues/151 ��

	����ʶΪ true ʱ�������ã�������ú�����
	*/
	template<typename _fCallable, typename... _tParams>
	inline void
		call_once(bool& b, _fCallable&& f, _tParams&&... args)
	{
		if (!b)
		{
			f(lforward(args)...);
			b = true;
		}
	}


	/*!
	\brief ���Ͳ�������̬����
	\warning �������롣
	\warning ���̰߳�ȫ��
	\since build 303
	*/
	template<typename _type, typename, typename...>
	inline _type&
		parameterize_static_object()
	{
		static _type obj;

		return obj;
	}
	/*!
	\brief �����Ͳ�������̬����
	\warning �������롣
	\warning ���̰߳�ȫ��
	\since build 301
	*/
	template<typename _type, size_t...>
	inline _type&
		parameterize_static_object()
	{
		static _type obj;

		return obj;
	}


	/*!
	\brief ȡ���ͱ�ʶ�ͳ�ʼ������ָ���Ķ���
	\tparam _tKey ��ʼ���Ͳ�������ʶ��
	\tparam _tKeys ����ʼ���Ͳ�������ʶ��
	\tparam _fInit ��ʼ���������͡�
	\tparam _tParams ��ʼ���������͡�
	\return ��ʼ����Ķ������ֵ���á�
	\since build 327
	*/
	template<typename _tKey, typename... _tKeys, typename _fInit,
		typename... _tParams>
		inline auto
		get_init(_fInit&& f, _tParams&&... args) -> decltype(f(lforward(args)...))&
	{
		using obj_type = decltype(f(lforward(args)...));

		auto& p(leo::parameterize_static_object<obj_type*, _tKey, _tKeys...>());

		if (!p)
			p = new obj_type(f(lforward(args)...));
		return *p;
	}
	/*!
	\brief ȡ�����ͱ�ʶ�ͳ�ʼ������ָ���Ķ���
	\tparam _vKeys �����Ͳ�������ʶ��
	\tparam _fInit ��ʼ���������͡�
	\tparam _tParams ��ʼ���������͡�
	\return ��ʼ����Ķ������ֵ���á�
	\since build 327
	*/
	template<size_t... _vKeys, typename _fInit, typename... _tParams>
	inline auto
		get_init(_fInit&& f, _tParams&&... args) -> decltype(f(lforward(args)...))&
	{
		using obj_type = decltype(f(lforward(args)...));

		auto& p(leo::parameterize_static_object<obj_type*, _vKeys...>());

		if (!p)
			p = new obj_type(f(lforward(args)...));
		return *p;
	}


	/*!	\defgroup init_mgr Initialization Managers
	\brief ��ʼ����������
	\since build 328

	ʵ�ֱ����ʼ���ͷ���ʼ����״̬�Ķ��󡣲�ֱ�ӳ�ʼ�����󣬿�����ͷ�ļ���ֱ�Ӷ��塣
	��֤��ʼ�������ض�������
	*/

	/*!
	\ingroup init_mgr
	\brief ʹ�����ü����ľ�̬��ʼ����������
	\pre _type ���� Destructible ��
	\note ��ʵ��֧�־�̬ TLS ʱΪÿ�̵߳���������Ϊȫ�־�̬������
	\warning ���ڲ�֧�� TLS ��ʵ�ַ��̰߳�ȫ��
	\sa ythread
	\see http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Nifty_Counter ��
	\since build 425

	��̬��ʼ����ͨ�����ü�����֤�����ڶ��屾���͵Ķ�������о�̬���󱻳�ʼ����
	�����з��뵥Ԫ�ı����Ͷ����������Զ�����ʼ����
	*/
	template<class _type>
	class nifty_counter
	{
	public:
		using object_type = _type;

		template<typename... _tParams>
		nifty_counter(_tParams&&... args)
		{
			if (get_count()++ == 0)
				get_object_ptr() = new _type(lforward(args)...);
		}
		//! \since build 425
		//@{
		//! \since build 461
		~nifty_counter()
		{
			if (--get_count() == 0)
				delete get_object_ptr();
		}

		static object_type&
			get() lnothrow
		{
			lassume(get_object_ptr());
			return *get_object_ptr();
		}

	private:
		static size_t&
			get_count() lnothrow
		{
			lthread size_t count;

			return count;
		}
			static object_type*&
			get_object_ptr() lnothrow
		{
			lthread object_type* ptr;

			return ptr;
		}

	public:
		static size_t
			use_count() lnothrow
		{
			return get_count();
		}
	};


	/*!
	\ingroup init_mgr
	\brief ʹ�� call_once �ľ�̬��ʼ����������
	\tparam _tOnceFlag ��ʼ�����ñ�ʶ��
	\note �̰߳�ȫȡ���� call_once �� _tOnceFlag ��֧�֡�
	������֧�� <tt><mutex></tt> ��ʵ�֣�ʹ�� std::once_flag ��
	��Ӧ std::call_once �������̰߳�ȫ�ģ�
	��ʹ�� bool ����Ӧ leo::call_once ������֤�̰߳�ȫ��
	�������Ϳ�ʹ���û����ж��� call_once ��
	\since build 328
	\todo ʹ��֧�� lambda pack չ����ʵ�ֹ���ģ�塣
	\todo ֧�ַ�������

	��̬��ʼ����ʹ�� _tOnceFlag ���͵ľ�̬�����ʾ��ʼ���ͷ���ʼ��״̬��
	��֤�����ڶ��屾���͵Ķ�������о�̬���󱻳�ʼ����
	�����з��뵥Ԫ�ı����Ͷ����������Զ�����ʼ����
	��ʼ���ͷ���ʼ������û���޶������ε� call_once ��ʼ���ͷ���ʼ����
	�û������Զ��� _tOnceFlag ʵ�ʲ�����Ӧ�� call_once ��������
	Ӧ�� std::call_once �� leo::call_once ��ʽһ�¡�
	*/
	template<typename _type, typename _tOnceFlag>
	class call_once_init
	{
	public:
		using object_type = _type;
		using flag_type = _tOnceFlag;

		template<typename... _tParams>
		call_once_init(_tParams&&... args)
		{
			call_once(get_init_flag(), init<_tParams...>, lforward(args)...);
		}
		~call_once_init()
		{
			call_once(get_uninit_flag(), uninit);
		}

		static object_type&
			get()
		{
			lassume(get_object_ptr());
			return *get_object_ptr();
		}

	private:
		static flag_type&
			get_init_flag()
		{
			static flag_type flag;

			return flag;
		}

		static object_type*&
			get_object_ptr()
		{
			static object_type* ptr;

			return ptr;
		}

		static flag_type&
			get_uninit_flag()
		{
			static flag_type flag;

			return flag;
		}

		template<typename... _tParams>
		static void
			init(_tParams&&... args)
		{
			get_object_ptr() = new object_type(lforward(args)...);
		}

		static void
			uninit()
		{
			delete get_object_ptr();
		}
	};
}

#if !LB_HAS_BUILTIN_NULLPTR
using leo::stdex::nullptr;
#endif

#endif