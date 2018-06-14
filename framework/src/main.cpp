#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Header/Camera.h"
#include "Header/Shader.h"
#include "Header/Model.h"
#include "Header/texture.h"
#include "Header/skybox.h"
#include "Header/config.h"
#include "Header/util.h"

using namespace settings;

GLint main(GLvoid)
{	
	GLFWwindow *window;
	if (!init_setting(window))
	{
		return -1;
	}

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	// build and compile our shader zprogram
	Shader modelShader("Resource/Shader/model.vs", "Resource/Shader/model.fs");
	Model Nanosuit("Resource/Model/nanosuit/nanosuit.obj");

	Shader testShader("Resource/Shader/test.vs", "Resource/Shader/test.fs");
	std::vector<float> rectVertex = GenCubeVertices(50, 50, 50);
	std::vector<float> sphereVertex = GenSphereVertices(5);
	std::vector<float> coneVertex = GenConeVertices(5, 10);
	std::vector<float> cylinderVertex = GenCylinderVertices(3, 5);

	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_STATIC_DRAW); // 分配128字节的内存
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	GLuint testVAO, testVBO;
	glGenVertexArrays(1, &testVAO);
	glBindVertexArray(testVAO);
		glGenBuffers(1, &testVBO);
		glBindBuffer(GL_ARRAY_BUFFER, testVBO);
			glBufferData(GL_ARRAY_BUFFER, rectVertex.size()*sizeof(float), &rectVertex[0], GL_STATIC_DRAW);
			//glBufferData(GL_ARRAY_BUFFER, cylinderVertex.size() * sizeof(float), &cylinderVertex[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(0));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(6*sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint testImg = loadTexture("container2.png","Resource/Texture");

	// create test box
    std::vector<float> cubicVertex = GenCubeVertices();
	unsigned int VAOcube,VBOcube;
	glGenVertexArrays(1,&VAOcube);
	glBindVertexArray(VAOcube);
		glGenBuffers(1,&VBOcube);
		glBindBuffer(GL_ARRAY_BUFFER,VBOcube);
			glBufferData(GL_ARRAY_BUFFER,cubicVertex.size() * sizeof(float),&cubicVertex[0],GL_STATIC_DRAW);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(0));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(3*sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(6*sizeof(float)));
			glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	Shader cubeShader("Resource/Shader/reflect.vs","Resource/Shader/reflect.fs");

	// create skybox
    std::string tmp[]={
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"back.jpg",
		"front.jpg"
	};
    std::vector<std::string> faces;
	for(int i=0;i<6;i++)faces.push_back(tmp[i]);
	SkyBox skybox;
	skybox.init(faces, "Resource/Texture/skybox/");
	Shader skyboxShader("Resource/Shader/skybox.vs","Resource/Shader/skybox.fs");
	skyboxShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTextId());
		skyboxShader.setInt("skybox",0);
	glUseProgram(0);

////////////////////////////////////////////////////////////////////////////////////
    // view/projection transformations
    camera.SetOuterBound(glm::vec4(-50.0f,-50.0f,50.0f,50.0f));
	camera.SetinnerBound(glm::vec3(2.5f,0.0f,-1.0f),glm::vec3(4.5f,2.0f,1.0f));
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// input
		processInput(window);

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.GetFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//step1: draw skybox
		skyboxShader.use();
			// ... 设置观察和投影矩阵
			model=glm::mat4(1.0);
			model=glm::scale(model,glm::vec3(100.0,100.0,100.0));
			skyboxShader.setMat4("model",model);
			skyboxShader.setMat4("view",view);
			skyboxShader.setMat4("projection",projection);
			skybox.draw(skyboxShader);
		glUseProgram(0);
		// step2 : draw test cube
		glBindVertexArray(VAOcube);
		cubeShader.use();
			cubeShader.setMat4("projection",projection);
			cubeShader.setMat4("view",view);
			cubeShader.setVec3("cameraPos",camera.GetPosition());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTextId());
			cubeShader.setInt("skybox",0);
			model=glm::mat4(1.0f);
			model=glm::translate(model,glm::vec3(3.5,1.0,0.0));
			model=glm::scale(model,glm::vec3(2.0f,2.0f,2.0f));
			cubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES,0,rectVertex.size()/8);
		glUseProgram(0);
		glBindVertexArray(0);


		// step3 : draw models
		// be sure to activate shader when setting uniforms/drawing objects
		modelShader.use();
			modelShader.setVec3("viewPos", camera.GetPosition());
			modelShader.setMat4("projection", projection);
			modelShader.setMat4("view", view);
			model=glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
			//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			modelShader.setMat4("model", model);
			Nanosuit.Draw(modelShader);
		glUseProgram(0);

		// test util function
		// testShader.use();
		// 	glm::mat4 testModel;
		// 	testModel = glm::translate(testModel, glm::vec3(0, 20, 0));
		// 	testShader.setMat4("PVM", projection*view*model*testModel);
		// 	glBindVertexArray(testVAO);
		// 	glBindTexture(GL_TEXTURE_2D, testImg);
		// 	glDrawArrays(GL_TRIANGLES, 0, cylinderVertex.size() / 6);
		// 	glBindVertexArray(0);
		// glUseProgram(0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAOcube);
	//glDeleteVertexArrays(1, &lightVAO);
	//glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();

	return 0;
}
