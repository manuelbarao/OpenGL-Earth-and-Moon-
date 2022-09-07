//Project by Manuel Barão Rodrigues Soldado 
//Proof of concept:
//Motivation: Interview for a position in OpenGL and C++ programming 

#include "headerfiles/windowmanager.h"
#include <string>

#define STB_IMAGE_IMPLEMENTATION



cMovimentationHandler oMovimentationHandler; //Precisa ser global por motivos de implementação do callback do OpenGL, talvez dê para 
											//fazer local usando ponteiros



int main()
{


	std::cout << "Pelase insert name of image in Textures folder to generate terrain. It will use textures/heightmap.png by default if you don't specify any \n";
	
	std::string heightmapfile = "";
	std::string inputstr;
	getline(std::cin, inputstr);

	if (inputstr.empty()) {
		/* ... nothing entered ... */
		heightmapfile = "heightmap.png";
	}else {
		heightmapfile = inputstr;
	}
	heightmapfile = "textures/" + heightmapfile;
	//Cria a janela usando a classe cWindowManager 
	cWindowManager oWindowManager;
	oWindowManager.setWindowSize(1600, 1200);
	oWindowManager.setWindowTitle("Height Map");

	
	//Cria os objetos para operar as outras classes
	cShadersManager oShadersManager;

	SimpleCamera oCamera;

	cMovimentationHandler::Camera = &oCamera; //atribui o ponteiro global necessário para apontar para a variavel local criada
	

	//Verifica se GLFW está ok
	if (!oWindowManager.initializeGLFW()) { return 1; };

	
	if (!oWindowManager.createWindow()) { return 1; };


	//Cria faz da janela criada o contexto do openGL
	glfwMakeContextCurrent(oWindowManager.getWindow());
	glfwSwapInterval(1);


	if (oWindowManager.glewInitInterface() == false) { return 1; };

	
	//Faz os callbacks de input
	//Use shift para voar mais rápido
	//OBS: essa câmera tem um pequeno bug na implementação  de rotação, mas é funcional
	glfwSetMouseButtonCallback(oWindowManager.getWindow(), oMovimentationHandler.MouseButtonCallback);
	glfwSetCursorPosCallback(oWindowManager.getWindow(), oMovimentationHandler.MouseMotionCallback);
	glfwSetKeyCallback(oWindowManager.getWindow(), oMovimentationHandler.KeyCallback);




	// Habilita o Buffer de Profundidade
	glEnable(GL_DEPTH_TEST);

	// Escolhe a função de teste de profundidade.
	//Optei por GL_LESS para que não haja sobreposição de estruturas
	glDepthFunc(GL_LESS);

	

	// Compilar o vertex e o fragment shader
	GLuint ProgramId = oShadersManager.LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");
	// Compilar o vertex e o fragment shader da luz
	//GLuint Program2Id = oShadersManager.LoadShaders("shaders/light_vert.glsl", "shaders/light_frag.glsl");
	


	//Gerar o terreno
	//Aqui entra uma função matemática que mapeia os pixels conforme a cor e gera vertices conforme o mapeamento
	GLuint generatedTerrain = oShadersManager.LoadTerrain(heightmapfile.c_str());
	
	




	
	oCamera.Light.Intensity = 0.8f;
	oCamera.Light.Direction = glm::vec3(0.0f, 0.0f, -5.0f);



	
	
	
	
	// Configura a cor de fundo
	glClearColor(0.3f, 0.3f, 0.2f, 1.0);

	


	// Configura os arrays e conjuga para a GPU
	unsigned int terrainVAO, terrainVBO, terrainIBO;
	glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);

	glGenBuffers(1, &terrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	glBufferData(GL_ARRAY_BUFFER, oShadersManager.getVertices().size() * sizeof(float), &oShadersManager.getVertices()[0], GL_STATIC_DRAW);

// Habilita o atributo na posição 0, normalmente é o atributo de vértices
// Esse vai ser o identificador que vamos usar no shader para ler a posição
// de cada vértice.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Gera os buffers
	glGenBuffers(1, &terrainIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, oShadersManager.getIndices().size() * sizeof(unsigned), &oShadersManager.getIndices()[0], GL_STATIC_DRAW);
	

	




	

	// Model matrix
	glm::mat4 ModelMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(00.0f), glm::vec3{ 0.0f, 1.0f, 0.0f });







	//Handler para contabilizar tempo
	double PreviousTime = glfwGetTime();

	

	

	while (!glfwWindowShouldClose(oWindowManager.getWindow()))
	{
		double CurrentTime = glfwGetTime();
		double DeltaTime = CurrentTime - PreviousTime;
		if (DeltaTime > 0.0)
		{
			oCamera.Update(static_cast<float>(DeltaTime));
			
		
			
		
			

			PreviousTime = CurrentTime;
		}
	
		//Limpa o buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );



		//Usa o shader compilado acima
		glUseProgram(ProgramId);


		//Cria as matrizes de projeção, visualização e uma matriz normal, pode ser usada posteriormente para iluminação, no momento nao usa
		glm::mat4 ViewMatrix = oCamera.GetView();
		glm::mat4 NormalMatrix = glm::transpose(glm::inverse(ViewMatrix * ModelMatrix));

		
		glm::mat4 ModelViewMatrix = ViewMatrix *  ModelMatrix;
		glm::mat4 ModelViewProjectionMatrix = oCamera.GetViewProjection() * ModelMatrix;

		
		//Linka ao shader
		GLint TimeLoc = glGetUniformLocation(ProgramId, "Time");
		glUniform1f(TimeLoc, CurrentTime);



	//	GLint NormalMatrixLoc = glGetUniformLocation(ProgramId, "NormalMatrix");
	//	glUniformMatrix4fv(NormalMatrixLoc, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

		



		GLint ModelViewMatrixLoc = glGetUniformLocation(ProgramId, "ModelViewMatrix");
		glUniformMatrix4fv(ModelViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(ModelViewMatrix));

		GLint ModelViewProjectionLoc = glGetUniformLocation(ProgramId, "ModelViewProjection");
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

		GLint NormalMatrixLoc = glGetUniformLocation(ProgramId, "NormalMatrix");
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

		//Usa shader da luz






		GLint LightIntensityLoc = glGetUniformLocation(ProgramId, "LightIntensity");
		glUniform1f(LightIntensityLoc, oCamera.Light.Intensity);

		glm::vec4 LightDirectionViewSpace = ViewMatrix * glm::vec4{ oCamera.Light.Direction, 0.0f };

		GLint LightDirectionLoc = glGetUniformLocation(ProgramId, "LightDirection");
		glUniform3fv(LightDirectionLoc, 1, glm::value_ptr(LightDirectionViewSpace));

		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix * ModelMatrix));
		//glDrawElements(GL_TRIANGLES, terrainVAO * 3, GL_UNSIGNED_INT, nullptr);



		

		//Pressione CTRL esquerdo para alterar o modo de visualização
	
		glPolygonMode(GL_FRONT_AND_BACK, oCamera.getPolygonMode());

		//Conjuga o VAO e desenha
		glBindVertexArray(terrainVAO);

		for (unsigned strip = 0; strip < oShadersManager.getcalculatednumStrips(); strip++)
		{
			glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
				oShadersManager.getcalculatednumTrisPerStrip() + 2,   // number of indices to render
				GL_UNSIGNED_INT,     // index data type
				(void*)(sizeof(unsigned) * (oShadersManager.getcalculatednumTrisPerStrip() + 2) * strip)); // offset to starting index
		}

		
		//Limpa para proximas funcionalidades
		glBindVertexArray(0);





		//encerra

		glfwPollEvents();
		glfwSwapBuffers(oWindowManager.getWindow());
	}

	glDeleteBuffers(1, &terrainIBO);
	glDeleteVertexArrays(1, &terrainVAO);
	glDeleteProgram(ProgramId);

	//O glTerminate já está no destrutor da classe cwindowManager, então nao precisa
	

	return 0;

}










