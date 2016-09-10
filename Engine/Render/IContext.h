/*! \file Engine\Render\IContext.h
\ingroup Engine
\brief ���ƴ����ӿ��ࡣ
*/
#ifndef LE_RENDER_IContext_h
#define LE_RENDER_IContext_h 1

namespace platform {
	namespace Render {
		class Device {};

		class Context {
		private:
			virtual void CreateDeviceAndDisplay() = 0;
		};
	}
}

#endif