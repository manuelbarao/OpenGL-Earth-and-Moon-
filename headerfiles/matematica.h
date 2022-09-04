
#ifndef matematica_HEADER
#define matematica_HEADER
#define STB_IMAGE_IMPLEMENTATION
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>



class cMatematica {
public:
	struct cMatematica::Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Color;
		glm::vec2 UV;
	};

	struct cMatematica::Triangle
	{
		GLuint V0;
		GLuint V1;
		GLuint V2;
	};

	struct cMatematica::DirectionalLight
	{
		glm::vec3 Direction;
		GLfloat Intensity;
	};
	void GenerateSphere(GLuint Resolution, std::vector<Vertex>& Vertices, std::vector<Triangle>& Indices);
};

#endif