#ifndef IndePlatform_Singleton_hpp
#define IndePlatform_Singleton_hpp

#include "memory.hpp"
#include "utility.hpp"
#include <functional>
namespace leo
{
	namespace details
	{
		void SingletonRegister(const std::function<void()>& f,const std::type_info& t);
	}
	template<typename Single,bool Manged = true>
	//����ģʽ����
	//0.�̳�Singleton ->class Sample : public Singleton<Sample>
	//1.������������Ϊpublic,�������Դ�ͷ�
	class Singleton : noncopyable
	{
	protected:
		Singleton()
		{
			auto f = [this]()
			{
				this->~Singleton();
			};
			details::SingletonRegister(f,typeid(Single));
		}
	public:
		virtual ~Singleton()
		{
			//assert(0);
		}
	};

	template<typename Single>
	class Singleton < Single, false > : noncopyable
	{
	protected:
		Singleton()
		{
			details::SingletonRegister([]{}, typeid(Single));
		}
	public:
		virtual ~Singleton()
		{
			//assert(0);
		}
	};

	class SingletonManger : public Singleton<SingletonManger,false>
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

		static std::unique_ptr<SingletonManger>& GetInstance()
		{
			static auto mInstance = unique_raw(new SingletonManger());
			return mInstance;
		}
	};
}



#endif