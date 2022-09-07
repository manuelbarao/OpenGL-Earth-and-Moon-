#include "headerfiles/shaderutils.h"

#include "deps/stb/stb_image.h"

std::string cShadersManager::ReadFile(const char* FilePath)
{
	std::string FileContents;
	if (std::ifstream FileStream{ FilePath, std::ios::in })
	{
		FileContents.assign((std::istreambuf_iterator<char>(FileStream)), std::istreambuf_iterator<char>());
	}
	return FileContents;
}

void cShadersManager::CheckShader(GLuint ShaderId)
{
	// Verificar se o shader foi compilado
	GLint Result = GL_TRUE;
	glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Result);

	if (Result == GL_FALSE)
	{
		// Erro ao compilar o shader, imprimir o log para saber o que está errado
		GLint InfoLogLength = 0;
		glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);

		std::string ShaderInfoLog(InfoLogLength, '\0');
		glGetShaderInfoLog(ShaderId, InfoLogLength, nullptr, &ShaderInfoLog[0]);

		if (InfoLogLength > 0)
		{
			std::cout << "Erro no Vertex Shader: " << std::endl;
			std::cout << ShaderInfoLog << std::endl;

			assert(false);
		}
	}
}

GLuint cShadersManager::LoadShaders(const char* VertexShaderFile, const char* FragmentShaderFile)
{
	// Criar os identificadores de cada um dos shaders
	GLuint VertShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	std::string VertexShaderSource = ReadFile(VertexShaderFile);
	std::string FragmentShaderSource = ReadFile(FragmentShaderFile);

	assert(!VertexShaderSource.empty());
	assert(!FragmentShaderSource.empty());

	std::cout << "Compilando " << VertexShaderFile << std::endl;
	const char* VertexShaderSourcePtr = VertexShaderSource.c_str();
	glShaderSource(VertShaderId, 1, &VertexShaderSourcePtr, nullptr);
	glCompileShader(VertShaderId);
	CheckShader(VertShaderId);

	std::cout << "Compilando " << FragmentShaderFile << std::endl;
	const char* FragmentShaderSourcePtr = FragmentShaderSource.c_str();
	glShaderSource(FragShaderId, 1, &FragmentShaderSourcePtr, nullptr);
	glCompileShader(FragShaderId);
	CheckShader(FragShaderId);

	std::cout << "Linkando Programa" << std::endl;
	GLuint ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertShaderId);
	glAttachShader(ProgramId, FragShaderId);
	glLinkProgram(ProgramId);

	// Verificar o programa
	GLint Result = GL_TRUE;
	glGetProgramiv(ProgramId, GL_LINK_STATUS, &Result);

	if (Result == GL_FALSE)
	{
		GLint InfoLogLength = 0;
		glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			std::string ProgramInfoLog(InfoLogLength, '\0');
			glGetProgramInfoLog(ProgramId, InfoLogLength, nullptr, &ProgramInfoLog[0]);

			std::cout << "Erro ao linkar programa" << std::endl;
			std::cout << ProgramInfoLog << std::endl;

			assert(false);
		}
	}

	glDetachShader(ProgramId, VertShaderId);
	glDetachShader(ProgramId, FragShaderId);

	glDeleteShader(VertShaderId);
	glDeleteShader(FragShaderId);

	return ProgramId;
}

GLuint cShadersManager::LoadTerrain(const char* TextureFile)
{
	std::cout << "Carregando terreno " << TextureFile << std::endl;

	int TextureWidth = 0;
	int TextureHeight = 0;
	int NumberOfComponents = 0;
	unsigned char* TextureData = stbi_load(TextureFile, &TextureWidth, &TextureHeight, &NumberOfComponents, 0);
	assert(TextureData);

	// Gerar o Identifador da Textura
	GLuint TextureId = 0;
	//glGenTextures(1, &TextureId); //nao é necessário
	
	
	float yScale = 64.0f / 256.0f, yShift = 16.0f;
	int rez = 1;
	unsigned bytePerPixel = NumberOfComponents;
	for (int i = 0; i < TextureHeight; i++)
	{
		for (int j = 0; j < TextureWidth; j++)
		{
			unsigned char* pixelOffset = TextureData + (j + TextureWidth * i) * bytePerPixel;
			unsigned char y = pixelOffset[0];

			// vertex
			vertices.push_back(-TextureHeight / 2.0f + TextureHeight * i / (float)TextureHeight);   // vx
			vertices.push_back((int)y * yScale - yShift);   // vy
			vertices.push_back(-TextureWidth / 2.0f + TextureWidth * j / (float)TextureWidth);   // vz
		}
	}
	std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;

    
	
	
	for (unsigned i = 0; i < TextureHeight - 1; i += rez)
	{
		for (unsigned j = 0; j < TextureWidth; j += rez)
		{
			for (unsigned k = 0; k < 2; k++)
			{
				indices.push_back(j + TextureWidth * (i + k * rez));
			}
		}
	}
	std::cout << "Loaded " << indices.size() << " indices" << std::endl;


	const int numStrips = (TextureHeight - 1) / rez;
	const int numTrisPerStrip = (TextureWidth / rez) * 2 - 2;
	std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
	std::cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << std::endl;
	calculatednumStrips = numStrips;
	calculatednumTrisPerStrip = numTrisPerStrip;
	stbi_image_free(TextureData);
	return TextureId;
}

int cShadersManager::getcalculatednumStrips()
{
	return calculatednumStrips;
}

int cShadersManager::getcalculatednumvertices()
{
	return calculatednumvertices;
}

std::vector<float> cShadersManager::getVertices()
{
	return vertices;

}

std::vector<unsigned> cShadersManager::getIndices()
{
	return indices;
}

int cShadersManager::getcalculatednumTrisPerStrip()
{
	return calculatednumTrisPerStrip;
}
