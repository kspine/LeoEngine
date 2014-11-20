#include "platform.h"

#include "memory.hpp"
#include <set>

namespace leo
{
	GeneralAllocPolicy GeneralAllocatedObject::impl;


	GeneralAllocPolicy DataAllocatedObject<GeneralAllocPolicy>::impl;
	struct __memroy_track_struct
	{
		void *p;
		std::uint8_t alignsize;
		bool placement;
	};

	bool operator<(const __memroy_track_struct& lhs, const __memroy_track_struct & rhs)
	{
		return lhs.p < rhs.p && lhs.alignsize < rhs.alignsize && lhs.placement < rhs.placement;
	}

	class ABCDEFGHIGK
	{
	public:
		~ABCDEFGHIGK()
		{
			if (!sets.empty())
			{
				MessageBoxW(nullptr, L"�ڴ�й¶!", L"���д���:", MB_OK);
				std::abort();
			}
		}

		void record_alloc(void * p, std::uint8_t alignsize, bool placement)
		{
			sets.insert({ p, alignsize, placement });
		}
		void dealloc_record(void * p, std::uint8_t alignsize, bool placement)
		{
			__memroy_track_struct s{ p, alignsize, placement };
			auto pos = sets.find(s);
			if (pos == sets.cend())
			{
				MessageBoxW(nullptr, L"�ͷ�ͬһ���ڴ���!", L"���д���:", MB_ERR_INVALID_CHARS);
				std::abort();
			}
			sets.erase(pos);
		}
		static std::multiset<__memroy_track_struct> sets;
	}
	__memory_track_sets;

	std::multiset<__memroy_track_struct> ABCDEFGHIGK::sets;

	void __memory_track_record_alloc(void * p, std::size_t count, std::uint8_t alignsize, const char* file, int line, const char* funcname, bool placement)
	{
		__memory_track_sets.record_alloc(p, alignsize, placement);
			
	}
	void __memory_track_dealloc_record(void * p, std::uint8_t alignsize, bool placement)
	{
		__memory_track_sets.dealloc_record(p, alignsize, placement);
	}
}