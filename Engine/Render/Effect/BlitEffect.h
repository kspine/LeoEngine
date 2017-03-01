/*! \file Engine\Render\Effect\BlitEffect.h
\ingroup Engine\Render\Effect
\brief ʵ��GDIʽ��BlitЧ����
*/
#ifndef LE_RENDER_EFFECT_BLIT_h
#define LE_RENDER_EFFECT_BLIT_h 1

#include "EffectProperty.h"

namespace platform::Render::Effect {

	class BlitEffect : public Effect {
	public:
		BlitEffect(const std::string& name);

		float3property src_offset;
		leo::lref<Technique> BilinearCopy;
	};
}

#endif