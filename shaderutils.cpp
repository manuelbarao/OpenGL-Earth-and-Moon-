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

GLuint cShadersManager::LoadTexture(const char* TextureFile)
{
	std::cout << "Carregando Textura " << TextureFile << std::endl;

	int TextureWidth = 0;
	int TextureHeight = 0;
	int NumberOfComponents = 0;
	unsigned char* TextureData = stbi_load(TextureFile, &TextureWidth, &TextureHeight, &NumberOfComponents, 3);
	assert(TextureData);

	// Gerar o Identifador da Textura
	GLuint TextureId;
	glGenTextures(1, &TextureId);

	// Habilita a textura para ser modificada
	glBindTexture(GL_TEXTURE_2D, TextureId);

	// Copia a textura para a memória da GPU
	GLint Level = 0;
	GLint Border = 0;
	glTexImage2D(GL_TEXTURE_2D, Level, GL_RGB, TextureWidth, TextureHeight, Border, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(TextureData);
	return TextureId;
}