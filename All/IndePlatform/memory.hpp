// CopyRight 2014. LeoHawke. All rights reserved.

#ifndef IndePlatform_memory_Hpp
#define IndePlatform_memory_Hpp


#include <memory>// smart ptr
#include <cstdlib>
#include <cstring>
#include "type_op.hpp"

//�洢������ָ������
namespace leo
{
	template<typename _type>
	lconstfn _type*
		//ȡ�ڽ�ָ��
		get_raw(_type* const& p) lnothrow
	{
		return p;
	}
		template<typename _type>
	lconstfn auto
		get_raw(const std::unique_ptr<_type>& p) lnothrow -> decltype(p.get())
	{
		return p.get();
	}
		template<typename _type>
	lconstfn _type*
		get_raw(const std::shared_ptr<_type>& p) lnothrow
	{
		return p.get();
	}
#ifdef LEO_USE_WEAK_PTR
		template<typename _type>
	lconstfn _type*
		get_raw(const std::weak_ptr<_type>& p) lnothrow
	{
		return p.lock().get();
	}
#endif

		template<typename _type>
	inline bool
		reset(std::unique_ptr<_type>& p) lnothrow
	{
		if (p.get())
		{
			p.reset();
			return true;
		}
		return false;
	}
		template<typename _type>
	inline bool
		reset(std::shared_ptr<_type>& p) lnothrow
	{
		if (p.get())
		{
			p.reset();
			return true;
		}
		return false;
	}

		template<typename _type, typename _pSrc>
	lconstfn std::unique_ptr<_type>
		//_pSrc���ڽ�ָ��
		unique_raw(const _pSrc& p)
	{
		static_assert(is_pointer<_pSrc>::value, "Invalid type found.");

		return std::unique_ptr<_type>(p);
	}

	template<typename _type, typename _pSrc>
	lconstfn std::unique_ptr<_type>
		//_pSrc���ڽ�ָ��
		unique_raw(_pSrc&& p)
	{
		static_assert(is_pointer<_pSrc>::value, "Invalid type found.");

		return std::unique_ptr<_type>(p);
	}

	template<typename _type>
	lconstfn std::unique_ptr<_type>
		unique_raw(_type* p)
	{
		return std::unique_ptr<_type>(p);
	}

	template<typename _type>
	lconstfn std::unique_ptr<_type>
		unique_raw(nullptr_t) lnothrow
	{
		return std::unique_ptr<_type>();
	}

		template<typename _type, typename _pSrc>
	lconstfn std::shared_ptr<_type>
		share_raw(const _pSrc& p)
	{
		static_assert(is_pointer<_pSrc>::value, "Invalid type found.");

		return std::shared_ptr<_type>(p);
	}

	template<typename _type, typename _pSrc>
	lconstfn std::shared_ptr<_type>
		share_raw(_pSrc&& p)
	{
		static_assert(is_pointer<_pSrc>::value, "Invalid type found.");

		return std::shared_ptr<_type>(p);
	}
	template<typename _type>
	lconstfn std::shared_ptr<_type>
		share_raw(_type* p)
	{
		return std::shared_ptr<_type>(p);
	}

	template<typename _type>
	lconstfn std::shared_ptr<_type>
		share_raw(nullptr_t) lnothrow
	{
		return std::shared_ptr<_type>();
	}

	//Visual C++ 2012���ϻ�c++11����
#if LB_IMPL_MSCPP >= 1800 || LB_IMPL_CPP > 201103L
	using std::make_unique;
#else
	//make_unique����
	//ʹ��new��ָ����������ָ�����͵�std::unique_ptr����.
	//ref http://herbsutter.com/gotw/_102/
	ISO WG21 / N3797 20.7.2[memory.syn]
	template<typename _type, typename... _tParams>
	lconstfn limpl(enable_if_t<!is_array<_type>::value, std::unique_ptr<_type>>)
		make_unique(_tParams&&... args)
	{
		return std::unique_ptr<_type>(new _type(lforward(args)...));
	}
	template<typename _type, typename... _tParams>
	lconstfn limpl(enable_if_t<is_array<_type>::value && extent<_type>::value == 0,
		std::unique_ptr<_type >> )
		make_unique(size_t size)
	{
		return std::unique_ptr<_type>(new remove_extent_t<_type>[size]());
	}
	template<typename _type, typename... _tParams>
	limpl(enable_if_t<extent<_type>::value != 0, void>)
		make_unique(_tParams&&...) = delete;
#endif

	//make_shared
	//ʹ�� new ��ָ����������ָ�����͵� std::shared_ptr
	// ��ͬ��std::make_shared(�ᵼ��Ŀ���ļ�����)
	template<typename _type, typename... _tParams>
	lconstfn std::shared_ptr<_type>
		make_shared(_tParams&&... args)
	{
		return std::shared_ptr<_type>(new _type(lforward(args)...));
	}
}

#include <new>//::operator new
#include <cstdint>//std::uint8_t
#include <cstddef>//std::size_t
#include <array>
#include <cassert>

#include "BaseMacro.h"
#include "..\debug.hpp"
//��������operator new


#if defined LEO_MEMORY_TRACKER
namespace leo
{
	void __memory_track_record_alloc(void * p, std::size_t count, std::uint8_t alignsize, const char* file, int line, const char* funcname, bool placement = false);
	void __memory_track_dealloc_record(void * p, std::uint8_t alignsize, bool placement = false);
}
#endif

namespace leo
{
	//  Description: Implements an aligned allocator for STL
	//               based on the Mallocator (http://blogs.msdn.com/b/vcblog/archive/2008/08/28/the-mallocator.aspx)
	// -------------------------------------------------------------------------
	template<typename T, int AlignSize>
	//���������
	class aligned_alloc
	{
		static_assert(AlignSize > 1, "AlignSize must more than 1");
		static_assert(!(AlignSize &(AlignSize - 1)), "AlignSize must be power of 2");
	public:
		// The following will be the same for virtually all allocators
		//��׼����������������Ͷ���ͺ���
		typedef T * pointer;
		typedef const T * const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		pointer address(T& r) const {
			return &r;
		}

		const_pointer address(const T& s) const{
			return &s;
		}

		std::size_t max_size() const{
			return (static_cast<std::size_t>(-1)) / sizeof(value_type);
		}

		template<typename U> struct rebind{
			typedef aligned_alloc<U, AlignSize> other;
		};

		bool operator!=(const aligned_alloc& other) const{
			return !(*this == other);
		}

		void construct(pointer const p, const_reference t) const{
			void * const pv = static_cast<void*>(p);
			new (pv)value_type(t);
		}

		void destroy(pointer const p) const;

		//��״̬������
		bool operator==(const aligned_alloc& other) const{
			return true;
		}
	private:
		std::allocator<std::uint8_t> impl;
	public:
		aligned_alloc() = default;
		~aligned_alloc() = default;
		aligned_alloc(const aligned_alloc&) = default;
		template <typename U>
		aligned_alloc(const aligned_alloc<U, AlignSize>&)
		{}
		aligned_alloc& operator=(const aligned_alloc&) = delete;

		// The following will be different for each allocator.
		pointer allocate(const size_t n)
		{
			// The return value of allocate(0) is unspecified.
			// aligned_alloc returns NULL in order to avoid depending
			// on malloc(0)'s implementation-defined behavior
			// (the implementation can define malloc(0) to return NULL,
			// in which case the bad_alloc check below would fire).
			// All allocators can return NULL in this case.
			if (n == 0)
			{
				return nullptr;
			}

			// All allocators should contain an integer overflow check.
			// The Standardization Committee recommends that std::length_error
			// be thrown in case of integer overflow.
			if (n > max_size())
			{
				throw std::length_error("aligned_alloc<T>::allocate() - Integer overflow.");
			}

			// aligned_alloc wraps allocator<T>.allocate().

			auto size = n *sizeof(value_type) + AlignSize;
			std::uintptr_t rawAddress = reinterpret_cast<std::uintptr_t>(impl.allocate(size));
			DebugPrintf(L"align_alloc.allocate:Address: %p,Size: %u,Alignesize: %u\n", rawAddress, size, AlignSize);

			std::uint8_t missalign = AlignSize - (rawAddress&(AlignSize - 1));
			std::uintptr_t alignAddress = rawAddress + missalign;

			std::uint8_t* storemissalign = (std::uint8_t*)(alignAddress)-1;
			*storemissalign = missalign;

			auto pv = reinterpret_cast<pointer>(alignAddress);
			// Allocators should throw std::bad_alloc in the case of memory allocation failure.
			if (!pv)
			{
				throw std::bad_alloc();
			}
			return pv;
		}

		void deallocate(pointer const p, const size_t n)
		{
			std::uint8_t adjust = *reinterpret_cast<std::uint8_t*>((std::uintptr_t)p - 1);
			std::uint8_t* rawAddress = reinterpret_cast<std::uint8_t*>((std::uintptr_t)p - adjust);
			auto rawn = n * sizeof(value_type) + AlignSize;
			DebugPrintf(L"align_alloc.deallocate Adddress: %p,Alignesize: %u\n", rawAddress, AlignSize);
			impl.deallocate(rawAddress, rawn);
		}

		// The following will be the same for all allocators that ignore hints.

		template <typename U>
		pointer allocate(const std::size_t n, const U* /* const hint */) const
		{
			return allocate(n);
		}
	};

	// The definition of destroy() must be the same for all allocators.
	template <typename T, int AlignSize>
	void aligned_alloc<T, AlignSize>::destroy(pointer const p) const
	{
		p->~T();
	}
}

namespace leo
{
	//ģ��Orge

	//hack�Ӷ��Ϸ����ڴ���Ϊ
	template<typename ALLOC>
	class AllocPolicy
	{
	private:
		ALLOC _impl;
	public:
		inline void * allocate(std::size_t count, const char * file = nullptr, int line = 0, const char* func = 0)
		{
			auto p = _impl.allocate(count);
#if defined LEO_MEMORY_TRACKER
			if (file)
			{
				//DebugPrintf("memory_alloc(address: %p size: %u(align: 1) where: %s file %d line %s function\n", p, count, 
					//file, line, func);
			}
			else
			{
				//DebugPrintf("memory_alloc(address: %p size: %u(align: 1) where: unknown \n", p, count);
			}
			__memory_track_record_alloc(p, count, 1, file, line, func);
#endif
			return p;
		}
		inline void deallocate(void * p, const char * file = nullptr, int line = 0, const char* func = 0)
		{
#if defined LEO_MEMORY_TRACKER
			if (file)
			{
				//DebugPrintf("memory_dealloc(address: %p (align: 1) where: %s file %d line %s function\n", p,
					//file, line, func);
			}
			else
			{
				//DebugPrintf("memory_dealloc(address: %p (align: 1) where: unknown \n", p);
			}
			__memory_track_dealloc_record(p, 1);
#endif
			_impl.deallocate(reinterpret_cast<typename ALLOC::pointer>(p), 0);
		}
		inline void * aligned_alloc(std::size_t count, std::uint8_t alignsize, const char * file = nullptr, int line = 0, const char* func = 0)
		{
			//���ٴ���1
			assert(alignsize > 1);
			//Ϊ2^n
			assert(!(alignsize &(alignsize - 1)));

			count += alignsize;
			std::uintptr_t rawAddress = (std::uintptr_t)(_impl.allocate(count));

			std::uint8_t missalign = alignsize - (rawAddress&(alignsize - 1));
			std::uintptr_t alignAddress = rawAddress + missalign;

			std::uint8_t* storemissalign = (std::uint8_t*)(alignAddress)-1;
			*storemissalign = missalign;
#if defined LEO_MEMORY_TRACKER
			if (file)
			{
				DebugPrintf("memory_alloc(address: %p size: %u(align: %d) where: %s file %d line %s function\n", alignAddress, count,alignsize,file, line, func);
			}
			else
			{
				DebugPrintf("memory_alloc(address: %p size: %u(align: %d) where: unknown \n", alignAddress, count,alignsize);
			}
			__memory_track_record_alloc(rawAddress, count, alignsize, file, line, func);
#endif
			return (void*)alignAddress;
		}
		inline void aligned_dealloc(void *p, std::uint8_t alignsize, std::size_t size = 0, const char * file = nullptr, int line = 0, const char* func = 0 /* const hint */)
		{
			std::uint8_t adjust = *(reinterpret_cast<std::uint8_t*>(p)-1);
			std::uintptr_t rawAddress = (std::uintptr_t)p - adjust;
			_impl.deallocate((std::uint8_t*)rawAddress, size);
#if defined LEO_MEMORY_TRACKER
			if (file)
			{
				DebugPrintf("memory_dealloc(address: %p (align: %d) where: %s file %d line %s function\n", p,alignsize,
					file, line, func);
			}
			else
			{
				DebugPrintf("memory_dealloc(address: %p (align: %d) where: unknown \n", p, alignsize);
			}
			__memory_track_dealloc_record(p, alignsize);
#endif
		}
	};

	enum class MemoryCategory
	{
		/// General purpose
		MEMCATEGORY_GENERAL = 0,
		/// Geometry held in main memory
		MEMCATEGORY_GEOMETRY = 1,
		/// Animation data like tracks, bone matrices
		MEMCATEGORY_ANIMATION = 2,
		/// Nodes, control data
		MEMCATEGORY_SCENE_CONTROL = 3,
		/// Scene object instances
		MEMCATEGORY_SCENE_OBJECTS = 4,
		/// Other resources
		MEMCATEGORY_RESOURCE = 5,
		/// Scripting
		MEMCATEGORY_SCRIPTING = 6,
		/// Rendersystem structures
		MEMCATEGORY_RENDERSYS = 7,


		// sentinel value, do not use 
		MEMCATEGORY_COUNT = 8
	};

	//�����ϵķ�����.ʹ�ñ�׼������(uint8_t)
	namespace details
	{
		using base_alloc = ::leo::aligned_alloc < std::uint8_t, 16 > ;
	}
	template<MemoryCategory cate>
	class CateAlloc : public details::base_alloc{
	};

	typedef AllocPolicy<CateAlloc<MemoryCategory::MEMCATEGORY_GENERAL>> GeneralAllocPolicy;
	typedef AllocPolicy<CateAlloc<MemoryCategory::MEMCATEGORY_GEOMETRY>> GeometryAllocPolicy;
	typedef AllocPolicy<CateAlloc<MemoryCategory::MEMCATEGORY_ANIMATION>> AnimationAllocPolicy;
	typedef AllocPolicy<CateAlloc<MemoryCategory::MEMCATEGORY_SCENE_CONTROL>> SceneCtlAllocPolicy;
	typedef AllocPolicy<CateAlloc<MemoryCategory::MEMCATEGORY_SCENE_OBJECTS>> SceneObjAllocPolicy;
	typedef AllocPolicy<CateAlloc<MemoryCategory::MEMCATEGORY_RESOURCE>> ResourceAllocPolicy;
	typedef AllocPolicy<CateAlloc<MemoryCategory::MEMCATEGORY_SCRIPTING>> ScriptingAllocPolicy;
	typedef AllocPolicy<CateAlloc<MemoryCategory::MEMCATEGORY_RENDERSYS>> RenderSysAllocPolicy;

	
	template<typename AllocPolice>
	class AllocatedObject
	{
	private:
		static AllocPolice impl;
	public:
		explicit AllocatedObject()
		{ }
#if 1
		virtual ~AllocatedObject()
		{ }
#endif
		/// operator new, with debug line info
		void* operator new(size_t sz, const char* file, int line, const char* func)
		{
			return impl.allocate(sz, file, line, func);
		}

		void* operator new(size_t sz)
		{
			return impl.allocate(sz);
		}

			/// placement operator new
		void* operator new(size_t sz, void* ptr)
		{
			(void)sz;
			return ptr;
		}

			/// array operator new, with debug line info
		void* operator new[](size_t sz, const char* file, int line, const char* func)
		{
			return impl.allocate(sz, file, line, func);
		}

		void* operator new[](size_t sz)
		{
			return impl.allocate(sz);
		}

		void operator delete(void* ptr)
		{
			impl.deallocate(ptr);
		}

		// Corresponding operator for placement delete (second param same as the first)
		void operator delete(void* ptr, void*)
		{
			impl.deallocate(ptr);
		}

		// only called if there is an exception in corresponding 'new'
		void operator delete(void* ptr, const char* file, int line, const char* func)
		{
			impl.deallocate(ptr, file, line, func);
		}

		void operator delete[](void* ptr)
		{
			impl.deallocate(ptr);
		}

		void operator delete[](void* ptr, const char*, int, const char*)
		{
			impl.deallocate(ptr);
		}
	};

	typedef AllocatedObject<GeneralAllocPolicy> GeneralAllocatedObject;
	typedef AllocatedObject<GeometryAllocPolicy> GeometryAllocatedObject;
	typedef AllocatedObject<AnimationAllocPolicy> AnimationAllocatedObject;
	typedef AllocatedObject<SceneCtlAllocPolicy> SceneCtlAllocatedObject;
	typedef AllocatedObject<SceneObjAllocPolicy> SceneObjAllocatedObject;
	typedef AllocatedObject<ResourceAllocPolicy> ResourceAllocatedObject;
	typedef AllocatedObject<ScriptingAllocPolicy> ScriptingAllocatedObject;
	typedef AllocatedObject<RenderSysAllocPolicy> RenderSysAllocatedObject;

	typedef ScriptingAllocatedObject    AbstractNodeAlloc;
	typedef AnimationAllocatedObject    AnimableAlloc;
	typedef AnimationAllocatedObject    AnimationAlloc;
	typedef GeneralAllocatedObject      ArchiveAlloc;
	typedef GeometryAllocatedObject     BatchedGeometryAlloc;
	typedef RenderSysAllocatedObject    BufferAlloc;
	typedef GeneralAllocatedObject      CodecAlloc;
	typedef ResourceAllocatedObject     CompositorInstAlloc;
	typedef GeneralAllocatedObject      ConfigAlloc;
	typedef GeneralAllocatedObject      ControllerAlloc;
	typedef GeometryAllocatedObject     DebugGeomAlloc;
	typedef GeneralAllocatedObject      DynLibAlloc;
	typedef GeometryAllocatedObject     EdgeDataAlloc;
	typedef GeneralAllocatedObject      FactoryAlloc;
	typedef SceneObjAllocatedObject     FXAlloc;
	typedef GeneralAllocatedObject      ImageAlloc;
	typedef GeometryAllocatedObject     IndexDataAlloc;
	typedef GeneralAllocatedObject      LogAlloc;
	typedef SceneObjAllocatedObject     MoveableAlloc;
	typedef SceneCtlAllocatedObject     NodeAlloc;
	typedef SceneObjAllocatedObject     OverlayAlloc;
	typedef RenderSysAllocatedObject    GpuParamsAlloc;
	typedef ResourceAllocatedObject     PassAlloc;
	typedef GeometryAllocatedObject     PatchAlloc;
	typedef GeneralAllocatedObject      PluginAlloc;
	typedef GeneralAllocatedObject      ProfilerAlloc;
	typedef GeometryAllocatedObject     ProgMeshAlloc;
	typedef SceneCtlAllocatedObject     RenderQueueAlloc;
	typedef RenderSysAllocatedObject    RenderSysAlloc;
	typedef GeneralAllocatedObject      RootAlloc;
	typedef ResourceAllocatedObject     ResourceAlloc;
	typedef GeneralAllocatedObject      SerializerAlloc;
	typedef SceneCtlAllocatedObject     SceneMgtAlloc;
	typedef ScriptingAllocatedObject    ScriptCompilerAlloc;
	typedef ScriptingAllocatedObject    ScriptTranslatorAlloc;
	typedef SceneCtlAllocatedObject     ShadowDataAlloc;
	typedef GeneralAllocatedObject      StreamAlloc;
	typedef SceneObjAllocatedObject     SubEntityAlloc;
	typedef ResourceAllocatedObject     SubMeshAlloc;
	typedef ResourceAllocatedObject     TechniqueAlloc;
	typedef GeneralAllocatedObject      TimerAlloc;
	typedef ResourceAllocatedObject     TextureUnitStateAlloc;
	typedef GeneralAllocatedObject      UtilityAlloc;
	typedef GeometryAllocatedObject     VertexDataAlloc;
	typedef RenderSysAllocatedObject    ViewportAlloc;
	typedef SceneCtlAllocatedObject     LodAlloc;
	typedef GeneralAllocatedObject      FileSystemLayerAlloc;
	typedef GeneralAllocatedObject      StereoDriverAlloc;
}


namespace leo
{
	//�ӿ���,��class_interface�̳е�
	class alloc :public std::allocator<std::uint8_t>//, leo::class_interface
	{
	private:
		using std::allocator<std::uint8_t>::address;
		using std::allocator<std::uint8_t>::allocate;
		using std::allocator<std::uint8_t>::construct;
		using std::allocator<std::uint8_t>::deallocate;
		using std::allocator<std::uint8_t>::destroy;
	public:
		virtual void* Alloc(std::size_t size) = 0;
		virtual void	Free(pointer p) = 0;
		void*	AllocWithAlign(std::size_t size, std::uint8_t alignsize)
		{
			//���ٴ���1
			assert(alignsize > 1);
			//Ϊ2^n
			assert(!(alignsize &(alignsize - 1)));

			size += alignsize;
			std::uintptr_t rawAddress = (std::uintptr_t)(this->Alloc(size));

			std::uint8_t missalign = alignsize - (rawAddress&(alignsize-1));
			std::uintptr_t alignAddress = rawAddress + missalign;

			std::uint8_t* storemissalign = (std::uint8_t*)(alignAddress)-1;
			*storemissalign = missalign;
			return (void*)alignAddress;
		}
		void	FreeWithAlign(pointer alignAddress)
		{
			std::uint8_t adjust = *reinterpret_cast<std::uint8_t*>((std::uintptr_t)alignAddress - 1);
			std::uintptr_t rawAddress = (std::uintptr_t)alignAddress - adjust;
			this->Free((pointer)rawAddress);
		}
	};

	//��ջ������,����ά������״̬,������,�ѷ�������,raw pointer
	template<std::size_t SIZE>
	class StackAlloc : public alloc
	{
	private:
		static_assert(SIZE >= 1024,"SIZE < 1024");
		std::uint8_t buff[SIZE];
		std::size_t pos;
	public:
		//enum { _EEN_SIZE = SIZE };	// helper for expression evaluator
	private:
		using alloc::FreeWithAlign;
		void Free(pointer)
		{
			std::uintptr_t alignAddress{};
			std::uint8_t adjust = *((pointer)alignAddress - 1);
			std::uintptr_t rawAddress = alignAddress - adjust;
			Free((pointer)rawAddress);
		}
	public:
		explicit StackAlloc()
			:pos()
		{}
		~StackAlloc() = default;
		void*	Alloc(std::size_t size)
		{
			assert(pos + size < SIZE);
			void* p = buff + pos;
			pos += size;
			return	p;
		}
		void Free(std::size_t p)
		{
			this->pos = p;
		}
		void	Clear()
		{
			pos = 0;
		}
		//һ�ֽ������Alloc
		DefGetter(const lnothrow, decltype(pos), Pos, pos)
	public:
	};
}

namespace leo
{
#undef min
	template<typename T,typename U>
	void inline memcpy(T& dst, const U& src)
	{
		auto size = std::min(sizeof(T), sizeof(U));
		std::memcpy(&dst, &src, size);
	}

	template<typename T>
	void inline memset(T& dst,int val)
	{
		std::memset(&dst, val, sizeof(T));
	}

	template<typename T>
	void inline BZero(T& dst)
	{
		memset(dst, 0);
	}
}


namespace oo
{
	class object
	{
	public:
		virtual ~object() = default;
	};
}
#endif