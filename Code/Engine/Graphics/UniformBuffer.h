#pragma once

typedef unsigned int GLuint;
namespace cs6610
{
	namespace Graphics
	{
		enum struct UniformBufferType { MATERIAL };
		class UniformBuffer
		{
		public:
			void Bind()const;
			void Update(void* i_newBufferData, size_t i_newUniformBufferSize) const;
			UniformBuffer(const UniformBufferType i_uniformBufferType, const size_t i_uniformBufferSize, const void*const i_initialBufferData = nullptr);
			~UniformBuffer();
		private:
			bool Initialize(const void* const i_initialBufferData);
			bool CleanUp();
			GLuint m_uniformBufferId;
			UniformBufferType m_uniformBufferType;
			size_t m_uniformBufferSize;
		};
	}
}