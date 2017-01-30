#pragma once
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
			explicit Effect(const std::string i_relativePathVertex, const std::string i_relativePathFragment);
			~Effect();
			cy::GLSLProgram* GetProgram()const;
			void RegisterUniforms(const char * const i_names) const;
		private:
			cy::GLSLProgram* m_program;
		};
	}
}
