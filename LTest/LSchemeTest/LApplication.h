/*!	\file LApplication.h
\ingroup LTest
\brief ϵͳ��Դ��Ӧ�ó���ʵ������
*/

#include <LBase/sutility.h>
#include <LBase/lmacro.h>
#include <memory>

#ifndef LTEST_LScheme_LApplication_h_
#define LTEST_LScheme_LApplication_h_ 1

namespace leo {

	namespace Shells{
	//! \brief ��ǳ���ʵ�������ڿ�����ӳ�����塣
	class Shell : private noncopyable, public std::enable_shared_from_this<Shell>
	{
	public:
		/*!
		\brief �޲������졣
		*/
		DefDeCtor(Shell)
			/*!
			\brief ������
			*/
			virtual
			~Shell();
	};
	}

	class Application :public Shells::Shell
	{
	public:
		//! \brief �޲������죺Ĭ�Ϲ��졣
		Application();

		//! \brief �������ͷ� Shell ����Ȩ��������Դ��
		~Application() override;
	};
}


#endif