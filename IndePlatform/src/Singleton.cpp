#include "Singleton.hpp"
#include <string>
#include <vector>
#include <list>
#include <typeindex>

namespace leo
{
	namespace details
	{

		struct register_value_type
		{
			std::function<void()> f;
#ifdef DEBUG
			//std::type_index t;
			register_value_type(const std::function<void()>& f)// const std::type_info& t)
				:f(f)//, t(t)
			{}
#else
			register_value_type(const std::function<void()>& f, const std::type_info& t)
				: f(f)
			{}
#endif
		};

		std::vector<register_value_type> SingletonUnInstallFunctionContainer;


		void SingletonRegister(const std::function<void()>& f)
		{
			SingletonUnInstallFunctionContainer.emplace_back(f);//, t);
		}

		void SingletonRegister(const std::function<void()>& f,const std::type_info& t)
		{
			SingletonUnInstallFunctionContainer.emplace_back(f);//, t);
		}
	}

	void SingletonManger::UnInstallAllSingleton()
	{
		for (auto rb = details::SingletonUnInstallFunctionContainer.rbegin(); rb != details::SingletonUnInstallFunctionContainer.rend(); ++rb)
			rb->f();
	}

#ifdef DEBUG
	void SingletonManger::PrintAllSingletonInfo()
	{
		DebugPrintf("�ѹ��� %u ������,��������:\n",details::SingletonUnInstallFunctionContainer.size());
		for (auto rb = details::SingletonUnInstallFunctionContainer.begin(); rb != details::SingletonUnInstallFunctionContainer.end(); ++rb)
			;//DebugPrintf("\t%s\n",rb->t.name());
	}
#endif
}