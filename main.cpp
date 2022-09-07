//Project by Manuel Barão Rodrigues Soldado 
//Proof of concept:
//Motivation: PrimeIT interview for a position in OpenGL and C++ programming 

#include "headerfiles/windowmanager.h"


#define STB_IMAGE_IMPLEMENTATION



cMovimentationHandler oMovimentationHandler; //Needs to be global



int main()
{
	//Creates window object 
	cWindowManager oWindowManager;
	oWindowManager.setWindowSize(1600, 1200);
	oWindowManager.setWindowTitle("Blue marble");

	

	cShadersManager oShadersManager;

	SimpleCamera oCamera;

	cMatematica oMath;

	//Return error if GLFW can't start for some reason
	if (!oWindowManager.initializeGLFW()) { return 1; };

	//Return error if GLFW can't start for some reason
	if (!oWindowManager.createWindow()) { return 1; };



	glfwMakeContextCurrent(oWindowManager.getWindow());
	glfwSwapInterval(1);

	if (oWindowManager.glewInitInterface() == false) { return 1; };

	
	cMovimentationHandler::Camera = &oCamera;

	glfwSetMouseButtonCallback(oWindowManager.getWindow(), oMovimentationHandler.MouseButtonCallback);
	glfwSetCursorPosCallback(oWindowManager.getWindow(), oMovimentationHandler.MouseMotionCallback);
	glfwSetKeyCallback(oWindowManager.getWindow(), oMovimentationHandler.KeyCallback);




	// Habilita o Buffer de Profundidade
	glEnable(GL_DEPTH_TEST);

	// Escolhe a função de teste de profundidade.
	glDepthFunc(GL_LESS);

	glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);

	// Compilar o vertex e o fragment shader
	GLuint ProgramId = oShadersManager.LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");
	GLuint Program2Id = oShadersManager.LoadShaders("shaders/trianglemoon_vert.glsl", "shaders/trianglemoon_frag.glsl");
	


	// Gera a Geometria da esfera e copia os dados para a GPU (memória da placa de vídeo)
	std::vector<cMatematica::Vertex> SphereVertices;
	std::vector<cMatematica::Triangle> SphereIndices;
	oMath.GenerateSphere(100, SphereVertices, SphereIndices);
	GLuint SphereVertexBuffer, SphereElementBuffer;
	glGenBuffers(1, &SphereVertexBuffer);
	glGenBuffers(1, &SphereElementBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, SphereVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, SphereVertices.size() * sizeof(cMatematica::Vertex), SphereVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SphereElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, SphereIndices.size() * sizeof(cMatematica::Triangle), SphereIndices.data(), GL_STATIC_DRAW);


	// Criar uma fonte de luz direcional
	cMatematica::DirectionalLight Light;
	Light.Direction = glm::vec3(0.0f, 0.0f, -1.0f);
	Light.Intensity = 1.0f;

	// Model View Projection
	glm::mat4 ModelMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(90.0f), glm::vec3{ 1.0f, 0.0f, 0.0f });

	glm::mat4 ModelMatrix2 = glm::rotate(glm::identity<glm::mat4>(), glm::radians(0.0f), glm::vec3{ 1.0f, 0.0f, 0.0f });


	ModelMatrix2 = glm::translate(ModelMatrix2, glm::vec3{ 1.0f,1.0f,1.0f });
	ModelMatrix2 = glm::scale(ModelMatrix2, glm::vec3{ 0.3,0.3, 0.3 });
	// Carregar a Textura para a Memória de Vídeo
	GLuint EarthTextureId = oShadersManager.LoadTexture("textures/earth_2k.jpg");
	GLuint CloudsTextureId = oShadersManager.LoadTexture("textures/earth_clouds_2k.jpg");
	GLuint MoonTextureId = oShadersManager.LoadTexture("textures/moon.jpg");
	// Configura a cor de fundo
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);

	// Identificador do Vertex Array Object (VAO)
	GLuint SphereVAO;

	// Gerar o identificador do VAO
	glGenVertexArrays(1, &SphereVAO);

	// Habilitar o VAO
	glBindVertexArray(SphereVAO);

	// Habilita o atributo na posição 0, normalmente é o atributo de vértices
	// Esse vai ser o identificador que vamos usar no shader para ler a posição
	// de cada vértice, mas não se preocupe com isso agora. Vai ficar tudo mais
	// claro quando formos falar de shaders
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	// Diz para o OpenGL que o VertexBuffer vai ficar associado ao atributo 0
	// glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);		
	glBindBuffer(GL_ARRAY_BUFFER, SphereVertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SphereElementBuffer);

	// Informa ao OpenGL onde, dentro do VertexBuffer, os vértices estão. No
	// nosso caso o array Triangles é tudo o que a gente precisa
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(cMatematica::Vertex), nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(cMatematica::Vertex), reinterpret_cast<void*>(offsetof(cMatematica::Vertex, Normal)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(cMatematica::Vertex), reinterpret_cast<void*>(offsetof(cMatematica::Vertex, Color)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(cMatematica::Vertex), reinterpret_cast<void*>(offsetof(cMatematica::Vertex, UV)));

	// Disabilitar o VAO
	glBindVertexArray(0);
	float theta = 0;
	double PreviousTime = glfwGetTime();

	glm::mat4 ModelMatrix3 = ModelMatrix2;
	
	glm::mat4 RotationMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(0.01f), glm::vec3{ 0.0f, 1.0f, 0.0f });
	glm::mat4 RotationMatrix2 = glm::rotate(glm::identity<glm::mat4>(), glm::radians(0.05f), glm::vec3{ 0.0f, 1.0f, 0.0f });
	glm::mat4 RotationMatrix3 = glm::rotate(glm::mat4(1.0f), glm::radians(0.05f), glm::vec3(1.0));
	

	

	while (!glfwWindowShouldClose(oWindowManager.getWindow()))
	{
		double CurrentTime = glfwGetTime();
		double DeltaTime = CurrentTime - PreviousTime;
		if (DeltaTime > 0.0)
		{
			oCamera.Update(static_cast<float>(DeltaTime));
			ModelMatrix = RotationMatrix * ModelMatrix;

			theta = theta + DeltaTime;
			//ModelMatrix2 = RotationMatrix * ModelMatrix2;
			ModelMatrix2 = RotationMatrix2 * ModelMatrix2;
			

			PreviousTime = CurrentTime;
		}
		if (theta >= 2*3.1415) {
			theta = 0;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ProgramId);

		glm::mat4 ViewMatrix = oCamera.GetView();
		glm::mat4 NormalMatrix = glm::transpose(glm::inverse(ViewMatrix * ModelMatrix));

		glm::mat4 NormalMatrix2 = glm::transpose(glm::inverse(ViewMatrix * ModelMatrix2));
		
		glm::mat4 ModelViewMatrix = ViewMatrix *  ModelMatrix;
		glm::mat4 ModelViewProjectionMatrix = oCamera.GetViewProjection() * ModelMatrix;

		glm::mat4 ModelViewMatrix2 = ViewMatrix * ModelMatrix2;
		glm::mat4 ModelViewProjectionMatrix2 = oCamera.GetViewProjection() * ModelMatrix2;

		GLint TimeLoc = glGetUniformLocation(ProgramId, "Time");
		glUniform1f(TimeLoc, CurrentTime);

		GLint NormalMatrixLoc = glGetUniformLocation(ProgramId, "NormalMatrix");
		glUniformMatrix4fv(NormalMatrixLoc, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

		GLint RotationMatrixLoc = glGetUniformLocation(ProgramId, "RotationMatrix");
		glUniformMatrix4fv(RotationMatrixLoc, 1, GL_FALSE, glm::value_ptr(RotationMatrix));



		GLint ModelViewMatrixLoc = glGetUniformLocation(ProgramId, "ModelViewMatrix");
		glUniformMatrix4fv(ModelViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(ModelViewMatrix));

		GLint ModelViewProjectionLoc = glGetUniformLocation(ProgramId, "ModelViewProjection");
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

		GLint LightIntensityLoc = glGetUniformLocation(ProgramId, "LightIntensity");
		glUniform1f(LightIntensityLoc, Light.Intensity);

		glm::vec4 LightDirectionViewSpace = ViewMatrix * glm::vec4{ Light.Direction, 0.0f };

		GLint LightDirectionLoc = glGetUniformLocation(ProgramId, "LightDirection");
		glUniform3fv(LightDirectionLoc, 1, glm::value_ptr(LightDirectionViewSpace));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, EarthTextureId);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, CloudsTextureId);

	

		GLint TextureSamplerLoc = glGetUniformLocation(ProgramId, "EarthTexture");
		glUniform1i(TextureSamplerLoc, 0);

		GLint CloudsTextureSamplerLoc = glGetUniformLocation(ProgramId, "CloudsTexture");
		glUniform1i(CloudsTextureSamplerLoc, 1);

		


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(SphereVAO);



		glDrawElements(GL_TRIANGLES, SphereIndices.size() * 3, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);


		
		





		glUseProgram(Program2Id);


		GLint TimeLoc2 = glGetUniformLocation(Program2Id, "Time");
		glUniform1f(TimeLoc2, CurrentTime);

		GLint NormalMatrixLoc2 = glGetUniformLocation(Program2Id, "NormalMatrix");
		glUniformMatrix4fv(NormalMatrixLoc2, 1, GL_FALSE, glm::value_ptr(NormalMatrix2));

		GLint ModelViewMatrixLoc2 = glGetUniformLocation(Program2Id, "ModelViewMatrix");
		glUniformMatrix4fv(ModelViewMatrixLoc2, 1, GL_FALSE, glm::value_ptr(ModelViewMatrix2));

		GLint ModelViewProjectionLoc2 = glGetUniformLocation(Program2Id, "ModelViewProjection");
		glUniformMatrix4fv(ModelViewProjectionLoc2, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix2));

		GLint LightIntensityLoc2 = glGetUniformLocation(Program2Id, "LightIntensity");
		glUniform1f(LightIntensityLoc2, Light.Intensity);

		glm::vec4 LightDirectionViewSpace2 =  ViewMatrix * glm::vec4{  Light.Direction, 0.0f } * ModelMatrix2;

		GLint LightDirectionLoc2 = glGetUniformLocation(Program2Id, "LightDirection");
		glUniform3fv(LightDirectionLoc2, 1, glm::value_ptr(LightDirectionViewSpace2));
















		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, MoonTextureId);


		
		


		GLint MoonTextureSamplerLoc = glGetUniformLocation(Program2Id, "MoonTexture");
		glUniform1i(MoonTextureSamplerLoc, 0);

		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(SphereVAO);



		
		
	
		glUniformMatrix4fv(ModelViewProjectionLoc2, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix2 * ModelMatrix2));
		glDrawElements(GL_TRIANGLES, SphereIndices.size() * 3, GL_UNSIGNED_INT, nullptr);


		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(oWindowManager.getWindow());
	}

	glDeleteBuffers(1, &SphereElementBuffer);
	glDeleteBuffers(1, &SphereVertexBuffer);
	glDeleteVertexArrays(1, &SphereVAO);
	glDeleteProgram(ProgramId);
	glDeleteProgram(Program2Id);
	glDeleteTextures(1, &EarthTextureId);

	

	return 0;

}










