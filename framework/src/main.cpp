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

	// some settings for room
	//Shader roomShader("Resource/Shader/test.vs", "Resource/Shader/test.fs");
	Shader roomShader("Resource/Shader/room.vs", "Resource/Shader/room.fs");
	std::vector<float> Room_floor = GenCubeTopVertices(150, 150);
	std::vector<float> Room_wall;
	GLuint floor_tex = loadTexture("floor.jpg", "Resource/Texture");
	GLuint wall_tex = loadTexture("wall.jpg", "Resource/Texture");

	GLuint floor_VAO, floor_VBO;
	glGenVertexArrays(1, &floor_VAO);
	glBindVertexArray(floor_VAO);
		glGenBuffers(1, &floor_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, floor_VBO);
			glBufferData(GL_ARRAY_BUFFER, Room_floor.size() * sizeof(float), &Room_floor[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(0));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(6*sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int height = 6, length = 10;
	// B->0, L->1, F->2, R->3
	// 这里可以利用L-system去生成,先随便搞一个demo
	// type, x, y, z（墙的类型，墙中心的位置）
	std::vector<int> heart_point{
		0, 0, height/2, -20,
		0, length, height/2, -20,
		0, 2*length, height/2, -20,
		3, 2*length+length/2, height/2, -20+length/2,
		0, 3*length, height/2, -20+length,
		0, 4*length, height/2, -20+length,
		1, 4*length+length/2, height/2, -20 + length/2,
		0, 5*length, height/2, -20,
		3, 5*length+length/2, height/2, -20+length/2,
		3, 5*length+length/2, height/2, -20+length+length/2,
		3, 5*length+length/2, height/2, -20+2*length+length/2,
		2, 5*length, height/2, -20+3*length,
		2, 4*length, height/2, -20+3*length,
		2, 3*length, height/2, -20+3*length,
		2, 2*length, height/2, -20+3*length,
		1, 2*length - length/2, height/2, -20+3*length - length/2,
		2, 2*length, height/2, -20+2*length,
		3, 2*length-length/2, height/2, -20+2*length+length/2,
		2, length, height/2, -20+3*length,
		1, length/2, height/2, -20+2*length+length/2,
		1, -length/2, height/2, -20+2*length - length/2,
		1, -length/2, height/2, -20+length - length/2,
	};

	for (int i = 0; i < heart_point.size(); i+=4)
	{
		std::vector<float> temp_wall;
		switch(heart_point[i])
		{
			case 0: 
				temp_wall = GenCubeBackVertices(length, height, heart_point[i+1], heart_point[i+2], heart_point[i+3]);
				camera.SetinnerBound(glm::vec3(heart_point[i+1]-length/2, heart_point[i+2]-height/2, heart_point[i+3]), 
								glm::vec3(heart_point[i+1] + length/2, heart_point[i+2]+height/2, heart_point[i+3]));
				Room_wall.insert(Room_wall.end(), temp_wall.begin(), temp_wall.end());
				break;
			case 1:
				temp_wall = GenCubeLeftVertices(length, height, heart_point[i+1], heart_point[i+2], heart_point[i+3]);
				camera.SetinnerBound(glm::vec3(heart_point[i+1], heart_point[i+2]-height/2, heart_point[i+3]-length/2), 
								glm::vec3(heart_point[i+1], heart_point[i+2]+height/2, heart_point[i+3]+length/2));
				Room_wall.insert(Room_wall.end(), temp_wall.begin(), temp_wall.end());
				break;
			case 2:
				temp_wall = GenCubeFrontVertices(length, height, heart_point[i+1], heart_point[i+2], heart_point[i+3]);
				camera.SetinnerBound(glm::vec3(heart_point[i+1]-length/2, heart_point[i+2]-height/2, heart_point[i+3]), 
								glm::vec3(heart_point[i+1] + length/2, heart_point[i+2]+height/2, heart_point[i+3]));
				Room_wall.insert(Room_wall.end(), temp_wall.begin(), temp_wall.end());
				break;
			case 3:
				temp_wall = GenCubeRightVertices(length, height, heart_point[i+1], heart_point[i+2], heart_point[i+3]);
				camera.SetinnerBound(glm::vec3(heart_point[i+1], heart_point[i+2]-height/2, heart_point[i+3]-length/2), 
								glm::vec3(heart_point[i+1], heart_point[i+2]+height/2, heart_point[i+3]+length/2));
				Room_wall.insert(Room_wall.end(), temp_wall.begin(), temp_wall.end());
				break;
			default:break;
		}
	}

	GLuint wall_VAO, wall_VBO;
	glGenVertexArrays(1, &wall_VAO);
	glBindVertexArray(wall_VAO);
		glGenBuffers(1, &wall_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, wall_VBO);
			glBufferData(GL_ARRAY_BUFFER, Room_wall.size() * sizeof(float), &Room_wall[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(0));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(6*sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader modelShader("Resource/Shader/model.vs", "Resource/Shader/model.fs");
	Model Nanosuit("Resource/Model/nanosuit/nanosuit.obj");

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
    camera.SetOuterBound(glm::vec4(-500.0f,-500.0f,500.0f,500.0f));
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
			glDrawArrays(GL_TRIANGLES,0,cubicVertex.size()/8);
		glUseProgram(0);
		glBindVertexArray(0);


		// step3 : draw models
		// be sure to activate shader when setting uniforms/drawing objects
		modelShader.use();
			modelShader.setVec3("viewPos", camera.GetPosition());
			modelShader.setMat4("projection", projection);
			modelShader.setMat4("view", view);
			model=glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
			modelShader.setMat4("model", model);
			Nanosuit.Draw(modelShader);
		glUseProgram(0);

		roomShader.use();
			roomShader.setVec3("viewPos", camera.GetPosition());
			roomShader.setVec3("lightPos", glm::vec3(0.0, 10, 0.0));
			roomShader.setMat4("projection", projection);
			roomShader.setMat4("view", view);
			roomShader.setMat4("model", glm::mat4(1.0));
			//roomShader.setMat4("PVM", projection * view);
			roomShader.setBool("floor_wall", 1);
			glBindVertexArray(floor_VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, floor_tex);
			roomShader.setInt("floor_tex", 0);
			glDrawArrays(GL_TRIANGLES, 0, Room_floor.size() / 8);
			glBindVertexArray(wall_VAO);
			roomShader.setBool("floor_wall", 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, wall_tex);
			roomShader.setInt("wall_tex", 1);
			roomShader.setVec3("viewPos", camera.GetPosition());
			roomShader.setVec3("lightPos", glm::vec3(0.0, 10, 0.0));
			roomShader.setMat4("projection", projection);
			roomShader.setMat4("view", view);
			roomShader.setMat4("model", glm::mat4(1.0));
			glDrawArrays(GL_TRIANGLES, 0, Room_wall.size() / 8);
		

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

// GLint main(GLvoid)
// {	
// 	GLFWwindow *window;
// 	if (!init_setting(window))
// 	{
// 		return -1;
// 	}

// 	glm::mat4 model;
// 	glm::mat4 view;
// 	glm::mat4 projection;

// 	// build and compile our shader zprogram
// 	Shader modelShader("Resource/Shader/model.vs", "Resource/Shader/model.fs");
// 	Shader wallShader("Resource/Shader/wall.vs", "Resource/Shader/wall.fs");
// 	std::vector<float> Room_floor = GenCubeTopVertices(50, 50);
// 	Model Nanosuit("Resource/Model/nanosuit/nanosuit.obj");

// 	Shader testShader("Resource/Shader/test.vs", "Resource/Shader/test.fs");
// 	std::vector<float> rectVertex = GenCubeVertices(50, 50, 50);
// 	std::vector<float> sphereVertex = GenSphereVertices(5);
// 	std::vector<float> coneVertex = GenConeVertices(5, 10);
// 	std::vector<float> cylinderVertex = GenCylinderVertices(3, 5);

// 	GLuint UBO;
// 	glGenBuffers(1, &UBO);
// 	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
// 	glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_STATIC_DRAW); // 分配128字节的内存
// 	glBindBuffer(GL_UNIFORM_BUFFER, 0);


// 	GLuint testVAO, testVBO;
// 	glGenVertexArrays(1, &testVAO);
// 	glBindVertexArray(testVAO);
// 		glGenBuffers(1, &testVBO);
// 		glBindBuffer(GL_ARRAY_BUFFER, testVBO);
// 			glBufferData(GL_ARRAY_BUFFER, rectVertex.size()*sizeof(float), &rectVertex[0], GL_STATIC_DRAW);
// 			//glBufferData(GL_ARRAY_BUFFER, cylinderVertex.size() * sizeof(float), &cylinderVertex[0], GL_STATIC_DRAW);
// 			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(0));
// 			glEnableVertexAttribArray(0);
// 			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
// 			glEnableVertexAttribArray(1);
// 			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *)(6*sizeof(GLfloat)));
// 			glEnableVertexAttribArray(2);
// 		glBindBuffer(GL_ARRAY_BUFFER, 0);
// 	glBindVertexArray(0);

// 	GLuint testImg = loadTexture("container2.png","Resource/Texture");

// 	// create test box
//     std::vector<float> cubicVertex = GenCubeVertices();
// 	unsigned int VAOcube,VBOcube;
// 	glGenVertexArrays(1,&VAOcube);
// 	glBindVertexArray(VAOcube);
// 		glGenBuffers(1,&VBOcube);
// 		glBindBuffer(GL_ARRAY_BUFFER,VBOcube);
// 			glBufferData(GL_ARRAY_BUFFER,cubicVertex.size() * sizeof(float),&cubicVertex[0],GL_STATIC_DRAW);
// 			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(0));
// 			glEnableVertexAttribArray(0);
// 			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(3*sizeof(float)));
// 			glEnableVertexAttribArray(1);
// 			glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(6*sizeof(float)));
// 			glEnableVertexAttribArray(2);
// 		glBindBuffer(GL_ARRAY_BUFFER,0);
// 	glBindVertexArray(0);
// 	Shader cubeShader("Resource/Shader/reflect.vs","Resource/Shader/reflect.fs");

// 	// create skybox
//     std::string tmp[]={
// 		"right.jpg",
// 		"left.jpg",
// 		"top.jpg",
// 		"bottom.jpg",
// 		"back.jpg",
// 		"front.jpg"
// 	};
//     std::vector<std::string> faces;
// 	for(int i=0;i<6;i++)faces.push_back(tmp[i]);
// 	SkyBox skybox;
// 	skybox.init(faces, "Resource/Texture/skybox/");
// 	Shader skyboxShader("Resource/Shader/skybox.vs","Resource/Shader/skybox.fs");
// 	skyboxShader.use();
// 		glActiveTexture(GL_TEXTURE0);
// 		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTextId());
// 		skyboxShader.setInt("skybox",0);
// 	glUseProgram(0);

// ////////////////////////////////////////////////////////////////////////////////////
//     // view/projection transformations
//     camera.SetOuterBound(glm::vec4(-50.0f,-50.0f,50.0f,50.0f));
// 	camera.SetinnerBound(glm::vec3(2.5f,0.0f,-1.0f),glm::vec3(4.5f,2.0f,1.0f));
// 	// render loop
// 	while (!glfwWindowShouldClose(window))
// 	{
// 		// per-frame time logic
// 		GLfloat currentFrame = glfwGetTime();
// 		deltaTime = currentFrame - lastFrame;
// 		lastFrame = currentFrame;
		
// 		// input
// 		processInput(window);

// 		// render
// 		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
// 		view = camera.GetViewMatrix();
// 		projection = glm::perspective(glm::radians(camera.GetFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
// 		//step1: draw skybox
// 		skyboxShader.use();
// 			// ... 设置观察和投影矩阵
// 			model=glm::mat4(1.0);
// 			model=glm::scale(model,glm::vec3(100.0,100.0,100.0));
// 			skyboxShader.setMat4("model",model);
// 			skyboxShader.setMat4("view",view);
// 			skyboxShader.setMat4("projection",projection);
// 			skybox.draw(skyboxShader);
// 		glUseProgram(0);
// 		// step2 : draw test cube
// 		glBindVertexArray(VAOcube);
// 		cubeShader.use();
// 			cubeShader.setMat4("projection",projection);
// 			cubeShader.setMat4("view",view);
// 			cubeShader.setVec3("cameraPos",camera.GetPosition());
// 			glActiveTexture(GL_TEXTURE0);
// 			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTextId());
// 			cubeShader.setInt("skybox",0);
// 			model=glm::mat4(1.0f);
// 			model=glm::translate(model,glm::vec3(3.5,1.0,0.0));
// 			model=glm::scale(model,glm::vec3(2.0f,2.0f,2.0f));
// 			cubeShader.setMat4("model", model);
// 			glDrawArrays(GL_TRIANGLES,0,rectVertex.size()/8);
// 		glUseProgram(0);
// 		glBindVertexArray(0);


// 		// step3 : draw models
// 		// be sure to activate shader when setting uniforms/drawing objects
// 		modelShader.use();
// 			modelShader.setVec3("viewPos", camera.GetPosition());
// 			modelShader.setMat4("projection", projection);
// 			modelShader.setMat4("view", view);
// 			model=glm::mat4(1.0f);
// 			model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
// 			//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
// 			modelShader.setMat4("model", model);
// 			Nanosuit.Draw(modelShader);
// 		glUseProgram(0);

// 		// test util function
// 		// testShader.use();
// 		// 	glm::mat4 testModel;
// 		// 	testModel = glm::translate(testModel, glm::vec3(0, 20, 0));
// 		// 	testShader.setMat4("PVM", projection*view*model*testModel);
// 		// 	glBindVertexArray(testVAO);
// 		// 	glBindTexture(GL_TEXTURE_2D, testImg);
// 		// 	glDrawArrays(GL_TRIANGLES, 0, cylinderVertex.size() / 6);
// 		// 	glBindVertexArray(0);
// 		// glUseProgram(0);

// 		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
// 		glfwSwapBuffers(window);
// 		glfwPollEvents();
// 	}

// 	//// optional: de-allocate all resources once they've outlived their purpose:
// 	glDeleteVertexArrays(1, &VAOcube);
// 	//glDeleteVertexArrays(1, &lightVAO);
// 	//glDeleteBuffers(1, &VBO);

// 	// glfw: terminate, clearing all previously allocated GLFW resources.
// 	glfwTerminate();

// 	return 0;
// }
