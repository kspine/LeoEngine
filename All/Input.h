#pragma once
#include "IndePlatform\clock.hpp"
#include "IndePlatform\Singleton.hpp"
#include "Mgr.hpp"
#include <thread>
namespace leo
{
	namespace win
	{
		class LB_API KeysState : leo::Singleton<KeysState>
		{
		public:
			using KeyState = std::uint8_t;
			~KeysState() = default;
		public:
			void Update();
			bool KeyRisingEdge(std::uint8_t key);
			bool KeyFallingEdge(std::uint8_t key);
			bool KeyPress(std::uint8_t key);
		private:
			// 7 6 5 4 3     2      1      0
			//             ����  �½���  ������
			static const std::uint8_t risingedge_mask = 0x1;
			static const std::uint8_t fallingedge_mask = risingedge_mask << 1;
			static const std::uint8_t press_mask = fallingedge_mask << 1;
		private:
			static KeyState m_currstates[256];
			static KeyState m_prevstates[256];
		public:
			static KeysState* GetInstance()
			{
				return &global::globalKeysState;
			}
		};

		class KeyAction
		{
		protected:
			KeyAction();
			~KeyAction();
		protected:
			virtual void Update() = 0
			{
				assert(0);
			}
			friend class KeysState;
		};

		inline bool KeyFallingEdge(std::uint8_t key)
		{
			return leo::global::globalKeysState.KeyFallingEdge(key);
			//return GetAsyncKeyState(key) == 0x8000;
		}
		inline bool KeyRisingEdge(std::uint8_t key)
		{
			return leo::global::globalKeysState.KeyRisingEdge(key);
			//return GetAsyncKeyState(key) == 0x1;
		}
		inline bool KeyPress(std::uint8_t key)
		{
			return leo::global::globalKeysState.KeyPress(key);
			//return GetAsyncKeyState(key) & 0x8000;
		}


		class KeyDown {
			std::uint8_t m_key;
			std::function<void()> m_callback;
		public:
			KeyDown(std::uint8_t key, const std::function<void()>& callback)
				:m_key(key), m_callback(callback) {

			}

			void Update()
			{
				if(KeyFallingEdge(m_key))
				{
					std::thread t(m_callback);
					t.detach();
				}
			}
		};

		class KeyUp {
			std::uint8_t m_key;
			std::function<void()> m_callback;
		public:
			KeyUp(std::uint8_t key, const std::function<void()>& callback)
				:m_key(key), m_callback(callback) {

			}

			void Update()
			{
				if (KeyRisingEdge(m_key))
				{
					std::thread t(m_callback);
					t.detach();
				}
			}
		};

		//���ٻ���ĳ����(����)
		class KeyTapDetector : public KeyAction
		{
			std::uint8_t m_key;
			bool m_valid = false;
			float m_dtMax;//��λ,��
			clock::time_point m_tLast;//�ϴ��½�������ʱ��ʱ���
		public:
			KeyTapDetector(std::uint8_t key, float dtMax) :
				m_key(key), m_dtMax(dtMax), m_tLast(clock::now())
			{
				m_tLast +=clock::to_duration<>(-dtMax);
			}
			//��ʱ���û�ȡ�Ƿ���������
			bool GetValid() const
			{
				return m_valid;
			}
		protected:
			//ÿ֡����
			void Update()
			{
				if (win::KeyFallingEdge(m_key))
				{
					auto curr = clock::now();
					auto dt = clock::duration_to<>(curr - m_tLast);
					m_tLast = curr;
					if (dt < m_dtMax)
						m_valid = true;
				}
				else
					m_valid = false;
			}
		};

		//��������ĳЩ����(����)
		class KeySequenceDetector : public KeyAction
		{
			std::unique_ptr<std::uint8_t[]> m_keys;
			uint8 m_count;
			float m_dtMax;
			std::function<void()> m_callback;
			uint8 m_curr;
			clock::time_point m_start;
		public:
			KeySequenceDetector(std::initializer_list<std::uint8_t> keys, float dtmax, std::function<void()> callback)
				:m_keys(std::make_unique<std::uint8_t[]>(keys.size())), m_count(static_cast<uint8>(keys.size())), m_dtMax(dtmax), m_callback(callback), m_curr(0), m_start(clock::now())
			{
				std::copy(keys.begin(), keys.end(), m_keys.get());
			}
		protected:
			void Update()
			{
				for (std::uint8_t key = 'A'; key <= 'Z'; ++key)
				{
					if (key != m_keys[m_curr])
						if (win::KeyFallingEdge(key))
						{
						m_curr = 0;
						return;
						}
				}
				if (win::KeyFallingEdge(m_keys[m_curr]))
				{
					if (m_curr == 0)
					{
						m_start = clock::now();
						++m_curr;
						return;
					}

					float dt = clock::duration_to<>(clock::now() - m_start);
					if (dt < m_dtMax)
					{
						++m_curr;
						if (m_curr == m_count)
						{
							std::thread t(m_callback);
							t.detach();
							m_curr = 0;
						}
					}
					else
						m_curr = 0;
				}
				assert(m_curr < m_count);
			}
		};

		//����ĳ������(����)
		class KeyHoldDetector : public KeyAction
		{
			std::uint8_t m_key;
			bool m_valid = false;
			bool m_flag = false;
			float m_dtMin;
			clock::time_point m_tLast;//�ϴΰ��¼���ʱ���
		public:
			KeyHoldDetector(std::uint8_t key, float dtmin = 1.f)
				:m_key(key), m_dtMin(dtmin), m_tLast(clock::now())
			{}

			bool GetValid() const
			{
				return m_valid;
			}
		protected:
			void Update()
			{
				auto curr = clock::now();
				if (leo::win::KeyRisingEdge(m_key))
				{
					m_flag = false;
					return;
				}
				if (leo::win::KeyFallingEdge(m_key))
				{
					m_tLast = curr;
					m_flag = true;
					return;
				}
				if (m_flag)
				{
					auto dt = clock::duration_to<>(curr - m_tLast);
					if (dt > m_dtMin)
					{
						m_tLast = curr;
						m_valid = true;
					}
					else
						m_valid = false;
				}
			}
		};

		//�Ӱ���ĳ�����ͷŴﵽһ��ʱ��(����)
		class KeySeqHoldDetector : public KeyAction
		{
			std::uint8_t m_key;
			float m_dtMin;
			std::function<void()> m_callback;
			clock::time_point m_tLast;//�½��ص�ʱ���
		public:
			KeySeqHoldDetector(std::uint8_t key, float dtmin, std::function<void()> callback)
				:m_key(key), m_dtMin(dtmin), m_callback(callback), m_tLast(clock::now())
			{
			}
		protected:
			void Update()
			{
				auto curr = clock::now();
				auto dt = m_dtMin;
				if (leo::win::KeyRisingEdge(m_key))
				{
					dt = clock::duration_to<>(curr - m_tLast);
					if (dt > m_dtMin)
					{
						std::thread t(m_callback);
						t.detach();
					}
				}
				if (leo::win::KeyFallingEdge(m_key))
					m_tLast = curr;
			}
		};
	}
	
}