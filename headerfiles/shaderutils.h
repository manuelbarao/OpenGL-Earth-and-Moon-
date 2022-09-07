#include "movimentation.h"
#ifndef SHADER_HEADER
#define SHADER_HEADER


class cShadersManager {
public:
	std::string ReadFile(const char* FilePath);
	void CheckShader(GLuint ShaderId);
	GLuint LoadShaders(const char* VertexShaderFile, const char* FragmentShaderFile);
	GLuint LoadTerrain(const char* TextureFile);
	int getcalculatednumStrips();
	int getcalculatednumvertices();
	std::vector<float> getVertices();
	std::vector<unsigned> getIndices();
	int getcalculatednumTrisPerStrip();
private:
	std::vector<float> vertices;
	std::vector<unsigned> indices;
	int calculatednumStrips{ 1 };
	int calculatednumvertices{ 1 };
	int calculatednumTrisPerStrip{ 1 };
};
#endif