/*! \file pseudo_mutex.h
\ingroup LBase
\brief α��������
*/

#ifndef LBase_pseudo_mutex_h
#define LBase_pseudo_mutex_h 1

#include "LBase/sutility.h" // for noncopyable, nonmovable, std::declval;
#include "LBase/exception.h" // for throw_error, std::errc;
#include <chrono> // for std::chrono::duration, std::chrono::time_point;

namespace leo
{
	/*!
	\brief ���̲߳�������֤���̻߳����½ӿڼ����϶�Ӧ�� std �����ռ��µĽӿڡ�
	\note ����������������صĽӿڡ�
	\since build 1.3
	\todo ��� ISO C++ 14 ��������
	*/
	namespace single_thread
	{

		//! \since build 1.3
		//@{
		lconstexpr const struct defer_lock_t
		{} defer_lock{};

		lconstexpr const struct try_to_lock_t
		{} try_to_lock{};

		lconstexpr const struct adopt_lock_t
		{} adopt_lock{};


		//! \warning ����֤�̰߳�ȫ�����̻߳����¼ٶ��߳�ͬ�������������δ������Ϊ��
		//@{
		//! \see ISO C++11 [thread.mutex.requirements.mutex] ��
		//@{
		class LB_API mutex : private limpl(noncopyable), private limpl(nonmovable)
		{
		public:
			lconstfn
				mutex() limpl(= default);
			~mutex() limpl(= default);

			//! \pre �����̲߳���������
			void
				lock() limpl(lnothrow)
			{}

			bool
				try_lock() limpl(lnothrow)
			{
				return true;
			}

			//! \pre �����̳߳�������
			void
				unlock() limpl(lnothrow)
			{}
		};


		class LB_API recursive_mutex
			: private limpl(noncopyable), private limpl(nonmovable)
		{
		public:
			recursive_mutex() limpl(= default);
			~recursive_mutex() limpl(= default);

			void
				lock() limpl(lnothrow)
			{}

			bool
				try_lock() limpl(lnothrow)
			{
				return true;
			}

			//! \pre �����̳߳�������
			void
				unlock() limpl(lnothrow)
			{}
		};


		//! \see ISO C++11 [thread.timedmutex.requirements] ��
		//@{
		class LB_API timed_mutex : private limpl(noncopyable), private limpl(nonmovable)
		{
		public:
			timed_mutex() limpl(= default);
			~timed_mutex() limpl(= default);

			//! \pre �����̲߳���������
			//@{
			void
				lock() limpl(lnothrow)
			{}

			bool
				try_lock() limpl(lnothrow)
			{
				return true;
			}

			template<typename _tRep, typename _tPeriod>
			bool
				try_lock_for(const std::chrono::duration<_tRep, _tPeriod>&) limpl(lnothrow)
			{
				return true;
			}

			template<typename _tClock, typename _tDuration>
			bool
				try_lock_until(const std::chrono::time_point<_tClock, _tDuration>&)
				limpl(lnothrow)
			{
				return true;
			}
			//@}

			//! \pre �����̳߳�������
			void
				unlock() limpl(lnothrow)
			{}
		};


		class LB_API recursive_timed_mutex
			: private limpl(noncopyable), private limpl(nonmovable)
		{
		public:
			recursive_timed_mutex() limpl(= default);
			~recursive_timed_mutex() limpl(= default);

			void
				lock() limpl(lnothrow)
			{}

			bool
				try_lock() limpl(lnothrow)
			{
				return true;
			}

			template<typename _tRep, typename _tPeriod>
			bool
				try_lock_for(const std::chrono::duration<_tRep, _tPeriod>&) limpl(lnothrow)
			{
				return true;
			}

			template<typename _tClock, typename _tDuration>
			bool
				try_lock_until(const std::chrono::time_point<_tClock, _tDuration>&)
				limpl(lnothrow)
			{
				return true;
			}

			//! \pre �����̳߳�������
			void
				unlock() limpl(lnothrow)
			{}
		};
		//@}
		//@}


		template<class _tMutex>
		class lock_guard : private limpl(noncopyable), private limpl(nonmovable)
		{
		public:
			using mutex_type = _tMutex;

#ifdef NDEBUG
			explicit
				lock_guard(mutex_type&) limpl(lnothrow)
			{}
			lock_guard(mutex_type&, adopt_lock_t) limpl(lnothrow)
			{}

#else
		private:
			mutex_type& pm;

		public:
			/*!
			\pre �� mutex_type �ǵݹ����������̲߳���������
			\post <tt>pm == &m</tt> ��
			*/
			explicit
				lock_guard(mutex_type& m) limpl(lnothrow)
				: pm(m)
			{
				m.lock();
			}
			/*!
			\pre �����̳߳�������
			\post <tt>pm == &m</tt> ��
			*/
			lock_guard(mutex_type& m, adopt_lock_t) limpl(lnothrow)
				: pm(&m)
			{}
			~lock_guard() limpl(lnothrow)
			{
				pm.unlock();
			}
#endif
		};


		//! \note ����� \c NDEBUG ʱ�����м�飬�Ż�ʵ��Ϊ�ղ�����
		//@{
		template<class _tMutex>
		class unique_lock : private limpl(noncopyable)
		{
		public:
			using mutex_type = _tMutex;

#ifdef NDEBUG
			//! \since build 1.4
			//@{
			unique_lock() limpl(= default);
			explicit
				unique_lock(mutex_type&) limpl(lnothrow)
			{}
			//! \since build 1.4
			unique_lock(mutex_type&, defer_lock_t) lnothrow
			{}
			unique_lock(mutex_type&, try_to_lock_t) limpl(lnothrow)
			{}
			unique_lock(mutex_type&, adopt_lock_t) limpl(lnothrow)
			{}
			template<typename _tClock, typename _tDuration>
			unique_lock(mutex_type&,
				const std::chrono::time_point<_tClock, _tDuration>&) limpl(lnothrow)
			{}
			template<typename _tRep, typename _tPeriod>
			unique_lock(mutex_type&, const std::chrono::duration<_tRep, _tPeriod>&)
				limpl(lnothrow)
			{}
			unique_lock(unique_lock&&) limpl(= default);

			explicit
				operator bool() const lnothrow
			{
				return true;
			}

			void
				lock()
			{}

			bool
				owns_lock() const lnothrow
			{
				return true;
			}

			mutex_type*
				release() lnothrow
			{
				return{};
			}

			void
				swap(unique_lock&) lnothrow
			{}

			bool
				try_lock() limpl(lnothrow)
			{
				return true;
			}

			template<typename _tRep, typename _tPeriod>
			bool
				try_lock_for(const std::chrono::duration<_tRep, _tPeriod>&) limpl(lnothrow)
			{
				return true;
			}

			template<typename _tClock, typename _tDuration>
			bool
				try_lock_until(const std::chrono::time_point<_tClock, _tDuration>&)
				limpl(lnothrow)
			{
				return true;
			}

			void
				unlock() limpl(lnothrow)
			{}

			mutex_type*
				mutex() const lnothrow
			{
				return{};
			}
			//@}
#else
		private:
			mutex_type* pm;
			bool owns;

		public:
			unique_lock() lnothrow
				: pm(), owns()
			{}

			//! \post <tt>pm == &m</tt> ��
			//@{
			explicit
				unique_lock(mutex_type& m) limpl(lnothrow)
				: unique_lock(m, defer_lock)
			{
				lock();
				owns = true;
			}
			unique_lock(mutex_type& m, defer_lock_t) lnothrow
				: pm(&m), owns()
			{}
			/*!
			\pre mutex_type ���� Lockable Ҫ��
			\pre �� mutex_type �ǵݹ����������̲߳���������
			*/
			unique_lock(mutex_type& m, try_to_lock_t) limpl(lnothrow)
				: pm(&m), owns(pm->try_lock())
			{}
			//! \pre �����̳߳�������
			unique_lock(mutex_type& m, adopt_lock_t) limpl(lnothrow)
				: pm(&m), owns(true)
			{}
			/*!
			\pre mutex_type ���� TimedLockable Ҫ��
			\pre �� mutex_type �ǵݹ����������̲߳���������
			*/
			//@{
			template<typename _tClock, typename _tDuration>
			unique_lock(mutex_type& m, const std::chrono::time_point<_tClock,
				_tDuration>& abs_time) limpl(lnothrow)
				: pm(&m), owns(pm->try_lock_until(abs_time))
			{}
			template<typename _tRep, typename _tPeriod>
			unique_lock(mutex_type& m,
				const std::chrono::duration<_tRep, _tPeriod>& rel_time) limpl(lnothrow)
				: pm(&m), owns(pm->try_lock_for(rel_time))
			{}
			//@}
			//@}
			/*!
			\post <tt>pm == u_p.pm && owns == u_p.owns</tt> �� \c u_p �� u ֮ǰ��״̬��
			\post <tt>!u.pm && !u.owns</tt> ��
			*/
			unique_lock(unique_lock&& u) lnothrow
				: pm(u.pm), owns(u.owns)
			{
				lunseq(u.pm = {}, u.owns = {});
			}
			~unique_lock()
			{
				if (owns)
					unlock();
			}

			/*!
			\post <tt>pm == u_p.pm && owns == u_p.owns</tt> �� \c u_p �� u ֮ǰ��״̬��
			\post <tt>!u.pm && !u.owns</tt> ��
			\see http://wg21.cmeerw.net/lwg/issue2104 ��
			*/
			unique_lock&
				operator=(unique_lock&& u) limpl(lnothrow)
			{
				if (owns)
					unlock();
				unique_lock(std::move(u)).swap(*this);
				u.clear_members();
				return *this;
			}

			explicit
				operator bool() const lnothrow
			{
				return owns;
			}

		private:
			void
				check_lock() limpl(lnothrow)
			{
				using namespace std;

				if (!pm)
					throw_error(errc::operation_not_permitted);
				if (owns)
					throw_error(errc::resource_deadlock_would_occur);
			}

			void
				clear_members() limpl(lnothrow)
			{
				lunseq(pm = {}, owns = {});
			}

		public:
			void
				lock() limpl(lnothrow)
			{
				check_lock();
				pm->lock();
				owns = true;
			}

			bool
				owns_lock() const lnothrow
			{
				return owns;
			}

			mutex_type*
				release() lnothrow
			{
				const auto res(pm);

				clear_members();
				return res;
			}

			void
				swap(unique_lock& u) lnothrow
			{
				std::swap(pm, u.pm),
					std::swap(owns, u.owns);
			}

			//! \pre mutex_type ���� Lockable Ҫ��
			bool
				try_lock() limpl(lnothrow)
			{
				check_lock();
				return owns = pm->lock();
			}

			//! \pre mutex_type ���� TimedLockable Ҫ��
			//@{
			template<typename _tRep, typename _tPeriod>
			bool
				try_lock_for(const std::chrono::duration<_tRep, _tPeriod>& rel_time)
				limpl(lnothrow)
			{
				check_lock();
				return owns = pm->try_lock_for(rel_time);
			}

			template<typename _tClock, typename _tDuration>
			bool
				try_lock_until(const std::chrono::time_point<_tClock, _tDuration>& abs_time)
				limpl(lnothrow)
			{
				check_lock();
				return owns = pm->try_lock_until(abs_time);
			}
			//@}

			void
				unlock() limpl(lnothrow)
			{
				if (!owns)
					throw_error(std::errc::operation_not_permitted);
				if (pm)
				{
					pm->unlock();
					owns = {};
				}
			}

			mutex_type*
				mutex() const lnothrow
			{
				return pm;
			}
#endif
		};


		//! \since build 1.4
		struct LB_API once_flag : private limpl(noncopyable), private limpl(nonmovable)
		{
			limpl(bool) state = {};

			lconstfn
				once_flag() lnothrow limpl(= default);
		};


		/*!
		\brief ����ʶ���ú�������֤����һ�Ρ�
		\note ���� std::call_once ��������֤�̰߳�ȫ�ԡ�
		\note ISO C++11���� N3691 �� 30.4 synopsis �����������ڴ���
		\bug δʵ��֧�ֳ�Աָ�롣
		\see https://github.com/cplusplus/draft/issues/151 ��
		\see http://wg21.cmeerw.net/cwg/issue1591 ��
		\see http://wg21.cmeerw.net/cwg/issue2442 ��
		\since build 1.4

		����ʶΪ�ǳ�ֵʱ�������ã�������ú�����
		*/
		template<typename _fCallable, typename... _tParams>
		inline void
			call_once(once_flag& flag, _fCallable&& f, _tParams&&... args)
		{
			if (!flag.state)
			{
				f(lforward(args)...);
				flag.state = true;
			}
		}
		//@}

		//! \relates unique_lock
		template<class _tMutex>
		inline void
			swap(unique_lock<_tMutex>& x, unique_lock<_tMutex>& y) lnothrow
		{
			x.swap(y);
		}


		template<class _tLock1, class _tLock2, class... _tLocks>
		void
			lock(_tLock1&&, _tLock2&&, _tLocks&&...) limpl(lnothrow)
		{}

		template<class _tLock1, class _tLock2, class... _tLocks>
		lconstfn int
			try_lock(_tLock1&&, _tLock2&&, _tLocks&&...) limpl(lnothrow)
		{
			return -1;
		}
		//@}
		//@}

	} // namespace single_thread;


	  /*!
	  \brief �ڵ��̻߳����Ͷ��̻߳����¶����õ��߳�ͬ���ӿڡ�
	  \since build 1.3
	  */
	namespace threading
	{

		/*!
		\brief ����ɾ������
		\pre _tMutex ���� \c BasicLockable Ҫ��
		\since build 1.4
		*/
		template<class _tMutex = single_thread::mutex,
			class _tLock = single_thread::unique_lock<_tMutex>>
			class unlock_delete : private noncopyable
		{
		public:
			using mutex_type = _tMutex;
			using lock_type = _tLock;

			mutable lock_type lock;

			unlock_delete(mutex_type& mtx)
				: lock(mtx)
			{}
			template<typename
				= limpl(enable_if_t)<is_nothrow_move_constructible<lock_type>::value>>
				unlock_delete(lock_type&& lck) lnothrow
				: lock(std::move(lck))
			{}
			template<typename... _tParams>
			unlock_delete(mutex_type& mtx, _tParams&&... args) lnoexcept(
				std::declval<mutex_type&>()(std::declval<_tParams&&>()...))
				: lock(mtx, lforward(args)...)
			{}

			//! \brief ɾ����������
			template<typename _tPointer>
			void
				operator()(const _tPointer&) const lnothrow
			{
				lock.unlock();
			}
		};


		/*!
		\brief ��ռ����Ȩ������ָ�롣
		\since build 1.3
		*/
		template<typename _type, class _tMutex = single_thread::mutex,
			class _tLock = single_thread::unique_lock<_tMutex>>
			using locked_ptr = std::unique_ptr<_type, unlock_delete<_tMutex, _tLock>>;

	} // namespace threading;

} // namespace leo;

#endif