#define _DEBUG
#include "Core\MeshLoad.hpp"
#include "file.hpp"
#include "leomath.hpp"
#include "Core\Camera.hpp"
#include <Windows.h>
#include "Input.h"
#include "Singleton.hpp"

namespace leo {
	
}

namespace xp
{

	struct register_value_type
	{
		register_value_type(const std::function<void()>& _f)
			:f(_f) {
		}

		std::function<void()> f;
	};

	std::list<register_value_type> SingletonUnInstallFunctionContainer;


	void SingletonRegister(const std::function<void()>& f)
	{
		SingletonUnInstallFunctionContainer.emplace_back(f);
	}

	template<typename Single, bool Manged = true>
	//����ģʽ����
	//0.�̳�Singleton ->class Sample : public Singleton<Sample>
	//1.������������Ϊpublic,�������Դ�ͷ�
	class Singleton
	{
	protected:
		Singleton()
		{
			auto f = [this]()
			{
				this->~Singleton();
			};
			leo::details::SingletonRegister(f);
		}
	public:
		virtual ~Singleton()
		{
			//assert(0);
		}
	};

	class X :Singleton<X> {
	};

	X a;
	X b;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	return 0;
}