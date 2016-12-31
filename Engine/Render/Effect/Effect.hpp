/*! \file Engine\Render\Effect\Effect.hpp
\ingroup Engine\Render\Effect
\brief Effect��������
*/
#ifndef LE_RENDER_EFFECT_h
#define LE_RENDER_EFFECT_h 1

#include <LBase/linttype.hpp>
#include <tuple>
#include <vector>
#include <memory>

namespace platform::Render {
	class PipleState {

	};

	class ShaderCompose {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		using ShaderBlob = std::pair<std::unique_ptr<stdex::byte[]>, std::size_t>;
	};
}

namespace platform::Render::Effect {
	class Effect {
	public:
		void Bind(leo::uint8 index);
		ShaderCompose& GetShader(leo::uint8 index);
	private:
		std::vector<ShaderCompose> shaders;
	};

	class Pass {
	public :
		void Bind(Effect &);
		void UnBind(Effect &);
		ShaderCompose& GetShader(Effect&);
		PipleState& GetState();
	private:
		PipleState state;
		leo::uint8 bind_index;
	};

	class Technique {
	public:
		Pass& GetPass(leo::uint8 index);
	private:
		std::vector<Pass> passes;
	};

}

#endif