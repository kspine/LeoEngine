/*!	\file MinGW32.h
\ingroup Framework
\ingroup Win32
\brief Framework MinGW32 ƽ̨������չ��
*/

#ifndef FrameWork_Win32_Mingw32_h
#define FrameWork_Win32_Mingw32_h 1

#include <LBase/Host.h>
#include <LBase/NativeAPI.h>
#if !LFL_Win32
#	error "This file is only for Win32."
#endif
#include <LBase/Debug.h>
#include <LBase/enum.hpp>
#include <LBase/FReference.h> //for unique_ptr todo FileIO.h
#include <chrono>

namespace platform_ex {

	inline namespace Windows {
		using ErrorCode = unsigned long;

		/*!
		\ingroup exceptions
		\brief Win32 ��������������쳣��
		*/
		class LB_API Win32Exception : public Exception
		{
		public:
			/*!
			\pre �����벻���� 0 ��
			\warning ��ʼ������ʱ���ܻ�ı� ::GetLastError() �Ľ����
			*/
			//@{
			Win32Exception(ErrorCode, string_view = "Win32 exception",
				leo::RecordLevel = leo::Emergent);
			/*!
			\pre ���������ǿա�
			\note ����������ʾ������������ʹ�� \c __func__ ��
			*/
			LB_NONNULL(4)
				Win32Exception(ErrorCode, string_view, const char*,
					leo::RecordLevel = leo::Emergent);
			//@}
			DefDeCopyCtor(Win32Exception)
				/*!
				\brief ���������ඨ����Ĭ��ʵ�֡�
				*/
				~Win32Exception() override;

			DefGetter(const lnothrow, ErrorCode, ErrorCode, ErrorCode(code().value()))
				DefGetter(const lnothrow, std::string, Message,
					FormatMessage(GetErrorCode()))

				explicit DefCvt(const lnothrow, ErrorCode, GetErrorCode())

				/*!
				\brief ȡ�������
				\return std::error_category ������� const ���á�
				*/
				static const std::error_category&
				GetErrorCategory();

			/*!
			\brief ��ʽ��������Ϣ�ַ�����
			\return �������쳣����Ϊ�գ�����Ϊ����̶�Ϊ en-US ��ϵͳ��Ϣ�ַ�����
			*/
			static std::string
				FormatMessage(ErrorCode) lnothrow;
			//@}
		};


		//! \since build 592
		//@{
		//! \brief �� ::GetLastError �Ľ����ָ�������׳� Windows::Win32Exception ����
#	define LFL_Raise_Win32Exception(...) \
	{ \
		const auto err(::GetLastError()); \
	\
		throw platform_ex::Windows::Win32Exception(err, __VA_ARGS__); \
	}

		//! \brief ������ʽ��ֵ��ָ�������׳� Windows::Win32Exception ����
#	define LFL_Raise_Win32Exception_On_Failure(_expr, ...) \
	{ \
		const auto err(Windows::ErrorCode(_expr)); \
	\
		if(err != ERROR_SUCCESS) \
			throw platform_ex::Windows::Win32Exception(err, __VA_ARGS__); \
	}
		//@}

		/*!
		\brief ���� ::GetLastError ȡ�õĵ���״̬�����
		*/
#	define LFL_Trace_Win32Error(_lv, _fn, _msg) \
	TraceDe(_lv, "Error %lu: failed calling " #_fn " @ %s.", \
		::GetLastError(), _msg)

		/*!
		\brief ���� Win32 API ���������� ::GetLastError ȡ�õ���״̬�����̡�
		\note ����ʱֱ��ʹ��ʵ�ʲ�������ָ���Ǳ�ʶ���ı���ʽ������֤��ȫ�����ơ�
		*/
		//@{
		/*!
		\note ��ʧ���׳� Windows::Win32Exception ����
		*/
		//@{
#	define LFL_WrapCallWin32(_fn, ...) \
	[&](const char* msg) LB_NONNULL(1){ \
		const auto res(_fn(__VA_ARGS__)); \
	\
		if(LB_UNLIKELY(!res)) \
			LFL_Raise_Win32Exception(#_fn, msg); \
		return res; \
	}

#	define LFL_CallWin32(_fn, _msg, ...) \
	LFL_WrapCallWin32(_fn, __VA_ARGS__)(_msg)

#	define LFL_CallWin32F(_fn, ...) LFL_CallWin32(_fn, lfsig, __VA_ARGS__)
		//@}

		/*!
		\note ��ʧ�ܸ��� ::GetLastError �Ľ����
		\note ��ʽת��˵����������ǰ�Ա�������Ӱ������
		\sa LFL_Trace_Win32Error
		*/
		//@{
#	define LFL_WrapCallWin32_Trace(_fn, ...) \
	[&](const char* msg) LB_NONNULL(1){ \
		const auto res(_fn(__VA_ARGS__)); \
	\
		if(LB_UNLIKELY(!res)) \
			LFL_Trace_Win32Error(platform::Descriptions::Warning, _fn, msg); \
		return res; \
	}

#	define LFL_CallWin32_Trace(_fn, _msg, ...) \
	LFL_WrapCallWin32_Trace(_fn, __VA_ARGS__)(_msg)

#	define LFL_CallWin32F_Trace(_fn, ...) \
	LFL_CallWin32_Trace(_fn, lfsig, __VA_ARGS__)
		//@}
		//@}
	
		//! \since for Load D3D12
		//@{
		//! \brief ���ع��̵�ַ�õ��Ĺ������͡�
		using ModuleProc = std::remove_reference_t<decltype(*::GetProcAddress(::HMODULE(),{}))>;

		/*!
		\brief ��ģ�����ָ�����̵�ָ�롣
		\pre �����ǿա�
		*/
		//@{
		LB_API LB_ATTR_returns_nonnull LB_NONNULL(2) ModuleProc*
			LoadProc(::HMODULE, const char*);
		template<typename _func>
		inline LB_NONNULL(2) _func&
			LoadProc(::HMODULE h_module, const char* proc)
		{
			return  *platform::FwdIter(reinterpret_cast<_func*>(LoadProc(h_module, proc)));
			//return platform::Deref(reinterpret_cast<_func*>(LoadProc(h_module, proc))); cl bug
		}
		template<typename _func>
		LB_NONNULL(1, 2) _func&
			LoadProc(const wchar_t* module, const char* proc)
		{
			return LoadProc<_func>(LFL_CallWin32F(GetModuleHandleW, module), proc);
		}

		/*!
		\brief �ֲ��洢ɾ������
		*/
		struct LB_API LocalDelete
		{
			using pointer = ::HLOCAL;

			void
				operator()(pointer) const lnothrow;
		};

		/*!	\defgroup native_encoding_conv Native Encoding Conversion
		\brief �����ı�����ת����
		\exception leo::LoggedEvent ����Ϊ��������� int ��

		ת����һ�� \c unsigned ����ָ��������ַ���Ϊ�ڶ��� \c unsigned ����ָ���ı��롣
		*/
		//@{
		//! \pre ��Ӷ��ԣ��ַ���ָ������ǿա�
		LB_API LB_NONNULL(1) string
			MBCSToMBCS(const char*, unsigned = CP_UTF8, unsigned = CP_ACP);
		//! \pre ���Ȳ��������Ҳ����� \c int ʱ��Ӷ��ԣ��ַ���ָ������ǿա�
		LB_API string
			MBCSToMBCS(string_view, unsigned = CP_UTF8, unsigned = CP_ACP);

		//! \pre ��Ӷ��ԣ��ַ���ָ������ǿա�
		LB_API LB_NONNULL(1) wstring
			MBCSToWCS(const char*, unsigned = CP_ACP);
		//! \pre ���Ȳ��������Ҳ����� \c int ʱ��Ӷ��ԣ��ַ���ָ������ǿա�
		LB_API wstring
			MBCSToWCS(string_view, unsigned = CP_ACP);

		//! \pre ��Ӷ��ԣ��ַ���ָ������ǿա�
		LB_API LB_NONNULL(1) string
			WCSToMBCS(const wchar_t*, unsigned = CP_ACP);
		//! \pre ���Ȳ��������Ҳ����� \c int ʱ��Ӷ��ԣ��ַ���ָ������ǿա�
		LB_API string
			WCSToMBCS(wstring_view, unsigned = CP_ACP);

		//! \pre ��Ӷ��ԣ��ַ���ָ������ǿա�
		inline LB_NONNULL(1) PDefH(wstring, UTF8ToWCS, const char* str)
			ImplRet(MBCSToWCS(str, CP_UTF8))
			//! \pre ���Ȳ��������Ҳ����� \c int ʱ��Ӷ��ԣ��ַ���ָ������ǿա�
			inline PDefH(wstring, UTF8ToWCS, string_view sv)
			ImplRet(MBCSToWCS(sv, CP_UTF8))

			//! \pre ��Ӷ��ԣ��ַ���ָ������ǿա�
			inline LB_NONNULL(1) PDefH(string, WCSToUTF8, const wchar_t* str)
			ImplRet(WCSToMBCS(str, CP_UTF8))
			//! \pre ���Ȳ��������Ҳ����� \c int ʱ��Ӷ��ԣ��ַ���ָ������ǿա�
			inline PDefH(string, WCSToUTF8, wstring_view sv)
			ImplRet(WCSToMBCS(sv, CP_UTF8))
	}
}

#endif
