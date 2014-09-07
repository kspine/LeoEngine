// CopyRight 2014. LeoHawke. All rights reserved.

#ifndef IndePlatform_Clock_Hpp
#define IndePlatform_Clock_Hpp


#include <chrono>
#include <thread>
#include "BaseMacro.h"

namespace leo
{
	//������ͣ,��������,���ṩ���ؾ���ѡ��
	class Clock
	{
	public:
		using steady_clock = std::chrono::steady_clock;
		using time_point = steady_clock::time_point;
		using duration = time_point::duration;
	public:
		template<typename VARG = float, typename TIMEUNIT = std::chrono::seconds::period>
		//�����κ���ֵ���͵�ֵ����׼ʱ��,Ĭ��float(Ĭ��ʱ�䵥λ:��)
		static inline Clock::duration ToDuration(const VARG& time)
		{
			std::chrono::duration<VARG, TIMEUNIT> result(time);
			return std::chrono::duration_cast<Clock::duration>(result);
		}
		
		template<typename RETURN = float, typename TIMEUNIT = std::chrono::seconds::period>
		//ע��:ϵͳʱ�ӵ�λ���Ǹ�����,����뷵�ظ�����,�������ģ�����ָ������������,Ĭ��float(Ĭ��ʱ�䵥λ:��)
		static inline RETURN DurationTo(const Clock::duration& dura)
		{
			std::chrono::duration<RETURN, TIMEUNIT> result =
				std::chrono::duration_cast<decltype(result)>(dura);
			return result.count();
		}

		static inline time_point Now()
		{
			return std::chrono::steady_clock::now();
		}
	private:
		duration mElapse;
		duration mDelta;
		time_point mStart;
	public:
		Clock()
			:mStart(steady_clock::now()), mElapse(), mDelta()
		{
			//Todo:
		}
		virtual ~Clock()
		{}
		void Update()
		{
			duration newelapse = std::chrono::steady_clock::now() - mStart;
			mDelta = newelapse - mElapse;
			mElapse = newelapse;
		}
		
		DefGetter(const _NOEXCEPT,duration,Elapse,mElapse)
		DefGetter(const _NOEXCEPT,duration,Delta, mDelta)
	};

	class GameClock
	{
	private:
		bool	m_ispaused;
		float	m_timescale;
		Clock::duration m_timecycles;
		//clock m_clock;
		//�����κ���ֵ���͵�ֵ����׼ʱ��,Ĭ��float(Ĭ��ʱ�䵥λ:��)
		template<typename VARG = float, typename TIMEUNIT = std::chrono::seconds::period>
		static inline Clock::duration DurationToCycle(const VARG& time)
		{
			std::chrono::duration< Clock::duration::rep, TIMEUNIT> result(time);
			return std::chrono::duration_cast<Clock::duration>(result);
		}
		//ע��:ϵͳʱ�ӵ�λ���Ǹ�����,����뷵�ظ�����,�������ģ�����ָ������������,Ĭ��float(Ĭ��ʱ�䵥λ:��)
		template<typename RETURN = float, typename TIMEUNIT = std::chrono::seconds::period>
		static inline RETURN CycleToDuration(const Clock::duration& dura)
		{
			std::chrono::duration<RETURN, TIMEUNIT> result =
				std::chrono::duration_cast<decltype(result)>(dura);
			return result.count();
		}
	public:
		template<typename VARG = float, typename TIMEUNIT = std::chrono::seconds::period>
		explicit GameClock(const VARG& starttime = {})
			:m_ispaused(false)
			, m_timescale(1.f)
			, m_timecycles(DurationToCycle<VARG,TIMEUNIT>(starttime))
			//, m_clock()
		{}
		template<typename RETURN = Clock::duration::rep, typename TIMEUINT = Clock::duration::period>
		RETURN GetTimeCycles() const _NOEXCEPT
		{
			std::chrono::duration<RETURN, TIMEUINT> result =
			std::chrono::duration_cast<decltype(result)>(m_timecycles);
			return result.count();
		}
		template<typename RETURN = float, typename TIMEUNIT = std::chrono::seconds::period>
		RETURN CalcDeltas(const GameClock& other)
		{
			return CycleToDuration<RETURN, TIMEUNIT>(this->m_timecycles - other.m_timecycles);
		}

		template<typename VARG = float, typename TIMEUNIT = std::chrono::seconds::period>
		void Update(const VARG& dt)
		{
			if (!m_ispaused)
			{
				auto dtScaleCycles = DurationToCycle<VARG, TIMEUNIT>(m_timescale*dt);
				m_timeCycles += dtScaleCycles;
			}
		}

		void Paused(bool ispause)
		{
			m_ispaused = ispause;
		}

		bool Paused() const _NOEXCEPT
		{
			return m_ispaused;
		}

		void TimeScale(decltype(m_timescale) scales)
		{
			m_timescale = scales;
		}

		decltype(m_timescale) TimeScale() const _NOEXCEPT
		{
			return m_timescale;
		}
	};

}
#endif