#include "movimentation.h"
#ifndef SHADER_HEADER
#define SHADER_HEADER


class cShadersManager {
public:
	std::string ReadFile(const char* FilePath);
	void CheckShader(GLuint ShaderId);
	GLuint LoadShaders(const char* VertexShaderFile, const char* FragmentShaderFile);
	GLuint LoadTexture(const char* TextureFile);
};
#endif