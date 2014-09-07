#ifndef IndePlatform_Singleton_hpp
#define IndePlatform_Singleton_hpp

#include "memory.hpp"
#include <functional>
namespace leo
{
	namespace details
	{
		void SingletonRegister(const std::function<void()>& f,const std::type_info&);
	}
	template<typename Single>
	//����ģʽ����
	//0.�̳�Singleton ->class Sample : public Singleton<Sample>
	//1.������������Ϊpublic,�������Դ�ͷ�
	class Singleton
	{
	protected:
		Singleton()
		{
			auto f = [&]()
			{
				this->~Singleton();
			};
			details::SingletonRegister(f, typeid(Single));
		}
	public:
		virtual ~Singleton()
		{
			//assert(0);
		}
	};

	class SingletonManger : public Singleton<SingletonManger>
	{
	protected:
		SingletonManger() = default;
	public:
		//do nothing
		~SingletonManger() = default;
	public:
		void UnInstallAllSingleton();

#ifdef DEBUG
		void PrintAllSingletonInfo();
#endif

		static const std::unique_ptr<SingletonManger>& GetInstance()
		{
			static auto mInstance = unique_raw(new SingletonManger());
			return mInstance;
		}
	};
}



#endif