// CopyRight 2014. LeoHawke. All rights reserved.
//ϵͳʱ��,��ǰʱ��,ת��Ϊ���ⵥλ,����������ȥʱ��
//��Ϸʱ��,��ͣ,ת��Ϊ���ⵥλ,��Ϸ��ȥʱ��`
//ʱ����,���شӹ���ʱ���ȥ��ʱ��
#ifndef IndePlatform_Clock_Hpp
#define IndePlatform_Clock_Hpp


#include <chrono>
#include <thread>

#include "Singleton.hpp"
#include "BaseMacro.h"

namespace leo
{
	namespace clock{
		using time_point = std::chrono::steady_clock::time_point;
		using duration = time_point::duration;

		inline time_point now(){
			return std::chrono::steady_clock::now();
		}

		template<typename VARG = float, typename TIMEUNIT = std::chrono::seconds>
		//�����κ���ֵ���͵�ֵ����׼ʱ��,Ĭ��float(Ĭ��ʱ�䵥λ:��)
		static inline duration to_duration(const VARG& time)
		{
			std::chrono::duration<VARG,typename TIMEUNIT::period> result(time);
			return std::chrono::duration_cast<duration>(result);
		}

		template<typename RETURN = float, typename TIMEUNIT = std::chrono::seconds>
		//ע��:ϵͳʱ�ӵ�λ���Ǹ�����,����뷵�ظ�����,�������ģ�����ָ������������,Ĭ��float(Ĭ��ʱ�䵥λ:��)
		static inline RETURN duration_to(const duration& dura)
		{
			std::chrono::duration<RETURN,typename TIMEUNIT::period> result =
				std::chrono::duration_cast<decltype(result)>(dura);
			return result.count();
		}

		class ProgramClock : public Singleton<ProgramClock> {
			time_point mStart = now();
		public:
			template<typename RETURN = float, typename TIMEUNIT = std::chrono::seconds>
			//Ĭ�ϸ���,��λΪ��
			static RETURN GetElapse() {
				return duration_to<RETURN,TIMEUNIT>(now() -GetInstance().mStart);
			}

			static void Reset(){
				GetInstance().mStart = now();
			}
		private:
			static ProgramClock& GetInstance(){
				static ProgramClock c;
				return c;
			}
		};

		class GameClock : public Singleton<GameClock>{
		private:
			bool	mPaused = false;
			float	mScale = 1.f;
			duration mElapse;

			static GameClock& GetInstance(){
				static GameClock c;
				return c;
			}
		public:

			template<typename VARG = float, typename TIMEUNIT = std::chrono::seconds>
			static void Reset(const VARG& starttime = {}){
				GetInstance().mPaused = false;
				GetInstance().mScale = 1.f;
				GetInstance().mElapse = to_duration<VARG, TIMEUNIT>(starttime);
			}

			template<typename RETURN = float, typename TIMEUNIT = std::chrono::seconds>
			//��λ,��
			static RETURN Now() lnothrow
			{
				std::chrono::duration<RETURN, typename TIMEUNIT::period> result =
				std::chrono::duration_cast<decltype(result)>(GetInstance().mElapse);
				return result.count();
			}


#if 0
			template<typename RETURN = float, typename TIMEUNIT = std::chrono::seconds>
			RETURN CalcDeltas(const GameClock& other)
			{
				return duration_to<RETURN, TIMEUNIT>(this->m_timecycles - other.m_timecycles);
			}
#endif

			template<typename VARG = float, typename TIMEUNIT = std::chrono::seconds>
			static void Update(const VARG& dt)
			{
				if (GetInstance().mPaused)
				{
					auto dtScaleCycles = to_duration<VARG, TIMEUNIT>(GetInstance().mScale*dt);
					GetInstance().mElapse += dtScaleCycles;
				}
			}

			static void Pause()
			{
				GetInstance().mPaused = false;
			}

			static bool GetIsPause() lnothrow
			{
				return GetInstance().mPaused;
			}

			static bool Continue(){
				GetInstance().mPaused = true;
			}

			static void TimeScale(float scales)
			{
				GetInstance().mScale = scales;
			}

			static float TimeScale() lnothrow
			{
				return GetInstance().mScale;
			}
		};

		class Clock{
			time_point mStart = now();
		public:
			template<typename RETURN = float, typename TIMEUNIT = std::chrono::seconds>
			//Ĭ�ϸ���,��λΪ��
			RETURN GetElapse() {
				return duration_to<RETURN, TIMEUNIT>(now() - mStart);
			}
		};
	}

}
#endif