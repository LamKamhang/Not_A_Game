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
#include "Header/Crystal.h"
#include "Header/bullet.h"
#include "Header/UI.h"
#include "Header/util.h"
#include "Header/Room.h"

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
	//GLuint testLOAD = loadTexture("SK_Material_BaseColor.png", "Resource/Model/SKgan");
	// build and compile our shader zprogram

	// some settings for room
	//Shader roomShader("Resource/Shader/test.vs", "Resource/Shader/test.fs");
	Shader roomShader("Resource/Shader/room.vs", "Resource/Shader/room.fs");

	glm::mat4 RoomModelMatrix(1.0f); 
	RoomModelMatrix = glm::translate(RoomModelMatrix,glm::vec3(-10,0,-10));
	RoomModelMatrix = glm::scale(RoomModelMatrix,glm::vec3(1.2f,1.2f,1.2f));
	Room room1(camera, RoomModelMatrix);

	Shader modelShader("Resource/Shader/model.vs", "Resource/Shader/model.fs", nullptr, nullptr, "Resource/Shader/model.gs");
	Model Nanosuit("Resource/Model/nanosuit/nanosuit.obj");

	Shader ganShader("Resource/Shader/gan.vs", "Resource/Shader/gan.fs");
	Model SKgan("Resource/Model/SKgan/SK.obj");

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


	// !!!!! UI !!!!!
	UI myUI;

	// bullet!
	Bullet heroBullet(camera.physicsEngine);

	// !!!!! crystal !!!!!
	CrystalSystem crystalsystem(camera.physicsEngine, &heroBullet);
	crystalsystem.addCrystal(glm::vec3(0.0f,0.0f,-9.0f),2.5f,0);//position, height, type
	crystalsystem.addCrystal(glm::vec3(10.0f,0.0f,-9.0f),2.5f,0);
	crystalsystem.addCrystal(glm::vec3(10.0f,0.0f,0.0f),2.5f,1);
	crystalsystem.addCrystal(glm::vec3(0.0f,0.0f,9.0f),2.5f,1);

	// test demo
	std::vector<glm::vec3>cubeposition;
	// cubeposition.push_back(glm::vec3(0.0f,1.0f,0.0f));
	cubeposition.push_back(glm::vec3(10.0f,2.5f,0.0f));
	cubeposition.push_back(glm::vec3(10.0f,5.0f,10.0f));
	cubeposition.push_back(glm::vec3(-10.0f,2.5f,10.0f));
	cubeposition.push_back(glm::vec3(-10.0f,5.0f,-10.0f));
	cubeposition.push_back(glm::vec3(0.0f,1.0f,10.0f));

	// camera.SetinnerBound(glm::vec3(-1.0f,0.0f,-1.0f),glm::vec3(1.0f,2.0f,1.0f));
	// camera.SetinnerBound(glm::vec3(9.0f,1.5f,-1.0f),glm::vec3(11.0f,3.5f,1.0f));
	// camera.SetinnerBound(glm::vec3(9.0f,4.0f,9.0f),glm::vec3(11.0f,6.0f,11.0f));
	// camera.SetinnerBound(glm::vec3(-11.0f,1.5f,9.0f),glm::vec3(-9.0f,3.5f,11.0f));
	// camera.SetinnerBound(glm::vec3(-11.0f,4.0f,-11.0f),glm::vec3(-9.0f,6.0f,-9.0f));
	// camera.SetinnerBound(glm::vec3(-1.0f,0.0f,9.0f),glm::vec3(1.0f,2.0f,11.0f));

////////////////////////////////////////////////////////////////////////////////////
Shader screenShader("Resource/Shader/render.vs", "Resource/Shader/render.fs");
float quadVertices[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};
// screen quad VAO
unsigned int quadVAO, quadVBO;
glGenVertexArrays(1, &quadVAO);
glGenBuffers(1, &quadVBO);
glBindVertexArray(quadVAO);
glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
glEnableVertexAttribArray(1);
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

screenShader.use();
screenShader.setInt("screenTexture", 0);
// framebuffer configuration
unsigned int framebuffer;
glGenFramebuffers(1, &framebuffer);
glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
// create a color attachment texture
unsigned int textureColorbuffer;
glGenTextures(1, &textureColorbuffer);
glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
unsigned int rbo;
glGenRenderbuffers(1, &rbo);
glBindRenderbuffer(GL_RENDERBUFFER, rbo);
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); 
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); 
if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
glBindFramebuffer(GL_FRAMEBUFFER, 0);
////////////////////////////////////////////////////////////////////////////////////

	// explode
	modelShader.use();
	bool explode_first = true;
    // view/projection transformations
    camera.SetOuterBound(glm::vec4(-500.0f,-500.0f,500.0f,500.0f));
	int closeEnough=0,damage=0,bullet=0,score=0;
	// render loop
	while (!glfwWindowShouldClose(window))
	{
/*+-----------------------------------------------------------------+
 *+                        first stage                              +
 *+-----------------------------------------------------------------+*/
		// per-frame time logic
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// std::cout<<"time="<<currentFrame<<std::endl;
		// input
		processInput(window);
		crystalsystem.generateCrystal(glm::vec3(0.0f,0.0f,0.0f),30.0f,0.1f,0.3f,currentFrame);
		
		// render
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.GetFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, PROJECT_NEAR, PROJECT_FAR);
		
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
		
		// step2 : draw crystal
		crystalsystem.updateAll(camera.GetPosition(),deltaTime);
		crystalsystem.updateHeroState(camera.GetPosition(),closeEnough,damage,bullet);
		crystalsystem.drawAll(projection,view,camera.GetPosition(),skybox.getTextId(),currentFrame,deltaTime,score);
		
		
		// step3: draw bullet !!
		if(cur_button_mode == left){
			if(!heroBullet.IsAttacking){
				heroBullet.SetDirection(camera.GetEyeFront());
				heroBullet.SetStartPos(camera.GetPosition());
				heroBullet.Attack();
			}
		}
		heroBullet.updatePosition(deltaTime,currentFrame);
        heroBullet.draw(projection,view,camera.GetPosition(),skybox.getTextId(),deltaTime);

		// std::cout<<"mousebutton="<<cur_button_mode<<std::endl;
        // std::cout<<"damage="<<damage<<std::endl;
        // std::cout<<"bullet="<<bullet<<std::endl;
		// std::cout<<"score="<<score<<std::endl;

		// step4 : draw test cube
		// glBindVertexArray(VAOcube);
		// cubeShader.use();
		// 	cubeShader.setMat4("projection",projection);
		// 	cubeShader.setMat4("view",view);
		// 	cubeShader.setVec3("cameraPos",camera.GetPosition());
		// 	glActiveTexture(GL_TEXTURE0);
		// 	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTextId());
		// 	cubeShader.setInt("skybox",0);
		// 	for(size_t i=0;i<cubeposition.size();i++){
		// 		model=glm::mat4(1.0f);
		// 		model=glm::translate(model,cubeposition[i]);
		// 		model=glm::scale(model,glm::vec3(2.0f,2.0f,2.0f));
		// 		cubeShader.setMat4("model", model);
		// 		glDrawArrays(GL_TRIANGLES,0,cubicVertex.size()/8);
		// 	}
		// glUseProgram(0);
		// glBindVertexArray(0);

		// step5 : draw test model
		// be sure to activate shader when setting uniforms/drawing objects
		modelShader.use();
			if(explode && explode_first)
			{
				explode_first = false;
				modelShader.setFloat("initTime", currentFrame);
			}else if (!explode){
				explode_first = true;
			}
			modelShader.setFloat("time", currentFrame);
			modelShader.setBool("explode_now", explode);
			modelShader.setVec3("viewPos", camera.GetPosition());
			modelShader.setMat4("projection", projection);
			modelShader.setMat4("view", view);
			model=glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
			modelShader.setMat4("model", model);
			Nanosuit.Draw(modelShader);
		glUseProgram(0);
		
		// step6 : draw rooms
		roomShader.use();
			roomShader.setVec3("viewPos", camera.GetPosition());
			roomShader.setMat4("projection", projection);
			roomShader.setMat4("view", view);
			roomShader.setBool("flash", flashlight_on);
			roomShader.setBool("phong", phong);
			if (flashlight_on)
			{
				roomShader.setVec3("spot_light.position",  camera.GetPosition());
				roomShader.setVec3("spot_light.direction", camera.GetEyeFront());
				roomShader.setVec3("spot_light.color", glm::vec3(1.0, 0, 0));
				roomShader.setFloat("spot_light.ambient", 1);
				roomShader.setFloat("spot_light.diffuse", 8);
				roomShader.setFloat("spot_light.specular",5);
			}else{
				roomShader.setFloat("spot_light.ambient", 0);
				roomShader.setFloat("spot_light.diffuse", 0);
				roomShader.setFloat("spot_light.specular",0);
			}
			room1.Draw(roomShader);

		//!!!!!! hero's gan !!!!!!!
		ganShader.use();
			ganShader.setVec3("viewPos", camera.GetPosition());
			ganShader.setMat4("projection", projection);
			ganShader.setMat4("view", view);
			model=glm::mat4(1.0f);
			glm::vec3 HeroEyeFront = camera.GetEyeFront();
			glm::vec3 HeroEyeUp = camera.GetUp();
			glm::vec3 HeroEyeRight =camera.GetRight();
			glm::vec3 HeroEyePos = camera.GetPosition();

			model = glm::translate(model,2.9f * HeroEyeFront);
			model = glm::translate(model,0.6f * HeroEyeRight);
			model = glm::translate(model,-1.5f * HeroEyeUp);
			model = glm::translate(model,glm::vec3(HeroEyePos.x,HeroEyePos.y,HeroEyePos.z));
			model=glm::rotate(model,(float)PI/13.0f,HeroEyeUp);
			glm::mat4 GanRotateMatrix = glm::mat4(glm::vec4(-1.0f * HeroEyeFront,0.0f),glm::vec4(HeroEyeUp,0.0f),glm::vec4(-1.0f*HeroEyeRight,0.0f),glm::vec4(0.0f,0.0f,0.0f,1.0f));
			model = model * GanRotateMatrix;
			model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
			ganShader.setMat4("model", model);
			SKgan.Draw(ganShader);
		glUseProgram(0);

		//step final: draw UI
		myUI.updateAlpha(closeEnough,currentFrame);
		myUI.draw();

/*+-----------------------------------------------------------------+
 *+                        second stage                             +
 *+-----------------------------------------------------------------+*/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

		screenShader.use();
		screenShader.setBool("Inversion", Inversion);
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAOcube);
	//glDeleteVertexArrays(1, &lightVAO);
	//glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);

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
