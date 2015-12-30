#ifndef IndePlatform_cstdio_h
#define IndePlatform_cstdio_h

#include "Lassert.h" // for ../ldef.h, <cstdio> and lconstraint;
#include <cstdarg> // for std::va_list;
#include <memory> // for std::unique_ptr;
#include <ios> // for std::ios_base::openmode;
#include "iterator_op.hpp" // for iterator_operators_t, is_undereferenceable;
#include "base.h" // for noncopyable;

namespace stdex
{
	/*!
	\brief ���� ISO C ��׼�����ֻ����������
	*/
	class LB_API ifile_iterator : public std::iterator<std::input_iterator_tag,
		byte, ptrdiff_t, const byte*, const byte&>, public leo::iterator_operators_t<
		ifile_iterator, std::iterator_traits<limpl(std::iterator<
			std::input_iterator_tag, byte, ptrdiff_t, const byte*, const byte&>) >>
	{
	protected:
		using traits_type = std::iterator<std::input_iterator_tag, byte, ptrdiff_t,
			const byte*, const byte&>;

	public:
		using char_type = byte;
		using int_type = int;

	private:
		/*!
		*/
		std::FILE* stream{};
		char_type value;

	public:
		/*!
		\brief �޲������졣
		\post <tt>!stream</tt> ��

		���������������
		*/
		lconstfn
			ifile_iterator()
			: value()
		{}
		/*!
		\brief ���죺ʹ����ָ�롣
		\pre ���ԣ� <tt>ptr</tt> ��
		\post <tt>stream == ptr</tt> ��
		*/
		explicit
			ifile_iterator(std::FILE* ptr)
			: stream(ptr)
		{
			lconstraint(ptr);
			++*this;
		}
		/*!
		\brief ���ƹ��죺Ĭ��ʵ�֡�
		*/
		lconstfn
			ifile_iterator(const ifile_iterator&) = default;
		~ifile_iterator() = default;

		ifile_iterator&
			operator=(const ifile_iterator&) = default;

		lconstfn reference
			operator*() const lnothrow
		{
			return value;
		}

		/*
		\brief ǰ��������
		\pre ���ԣ���ָ��ǿա�
		\return �������á�
		\note ������ EOF ʱ����ָ��Ϊ��ָ�롣

		ʹ�� std::fgetc ���ַ���
		*/
		ifile_iterator&
			operator++();

		friend bool
			operator==(const ifile_iterator& x, const ifile_iterator& y)
		{
			return x.stream == y.stream;
		}

		lconstfn std::FILE*
			get_stream() const
		{
			return stream;
		}

		/*!
		\brief ������д���ַ���
		*/
		//@{
		//! \pre ���ԣ� <tt>!stream</tt> ��
		int_type
			sputbackc(char_type c)
		{
			lconstraint(stream);
			return std::ungetc(c, stream);
		}
		//! \pre ���ԣ� <tt>!stream || steram == s</tt> ��
		int_type
			sputbackc(char_type c, std::FILE* s)
		{
			lconstraint(!stream || stream == s);
			stream = s;
			return sputbackc(c);
		}

		//! \pre ��Ӷ��ԣ� <tt>!stream</tt> ��
		int_type
			sungetc()
		{
			return sputbackc(value);
		}
		//! \pre ��Ӷ��ԣ� <tt>!stream || steram == s</tt> ��
		int_type
			sungetc(std::FILE* s)
		{
			return sputbackc(value, s);
		}
		//@}
	};


	/*!
	\ingroup is_undereferenceable
	\brief �ж� ifile_iterator ʵ���Ƿ�ȷ��Ϊ���ɽ����á�
	*/
	inline bool
		is_undereferenceable(const ifile_iterator& i) lnothrow
	{
		return !i.get_stream();
	}
}


#endif