#pragma once
#include <list>
#include <vector>
#include <string>

typedef unsigned int GLuint;

namespace cy
{
	class GLSLProgram;
}

namespace cs6610
{
	namespace Graphics
	{
		class Effect
		{
		public:
			void Bind()const;
			//Effect();
			Effect(const std::vector<std::string> i_shaderPaths);
			~Effect();
			cy::GLSLProgram* GetProgram()const;
			void RegisterUniforms(const char * const i_names) const;
			static void ReCompile();
		private:
			void Compile() const;
			cy::GLSLProgram* m_program;
			std::vector<std::string> m_shaderPaths;
			static std::list<Effect*> allActiveEffects;
		};
	}
}
