#pragma comment(linker, "/STACK:1024000000,1024000000") 

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
#include "Header/Render.h"
#include "Header/pointShadow.h"

#include "Header/particles.h"
#include "Header/Gan.h"
#include "Header/Ground.h"
#include "Header/Font.h"
using namespace settings;
void RenderQuad();

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

//|||||||||||||||||||||||||||||||||||||||
	// Ground !
	Ground ground;

	// some settings for room
	Shader roomShader("Resource/Shader/room.vs", "Resource/Shader/room.fs");

	std::vector<glm::vec3> roomtranslation;
	std::vector<float> roomrotation;
	int roomNUM1 = 5;
	int roomNUM2 = 5;
	float scaleSize = 2.0;
	for (int i = 0; i<roomNUM1; i++) {
		for (int j = 0; j<roomNUM2; j++) {
			glm::vec3 tmp(i*scaleSize * 90 * 3.0f, 0.0f, j*scaleSize * 70 * 3.0f);
			roomtranslation.push_back(tmp);
		}
	}
	for (int i = 0; i<roomNUM1; i++) {
		for (int j = 0; j<roomNUM2; j++) {
			roomrotation.push_back(PI / 2.0f*(i + j));
		}
	}
	std::vector<Room> rooms;
	glm::vec3 roombound1, roombound2;
	for (int i = 0; i<roomtranslation.size(); i++) {
		glm::mat4 RoomModelMatrix(1.0f);
		RoomModelMatrix = glm::translate(RoomModelMatrix, roomtranslation[i]);
		RoomModelMatrix = glm::rotate(RoomModelMatrix, roomrotation[i], glm::vec3(0.0f, 1.0f, 0.0f));
		RoomModelMatrix = glm::scale(RoomModelMatrix, glm::vec3(scaleSize, scaleSize, scaleSize));
		rooms.push_back(Room(camera, RoomModelMatrix, roombound1, roombound2));
		//std::cout << "roombound1 = " << roombound1.x << ", " << roombound1.y << ", " << roombound1.z << std::endl;
		//std::cout << "roombound2 = " << roombound2.x << ", " << roombound2.y << ", " << roombound2.z << std::endl;
		ground.setRoomBound(roombound1, roombound2);
	}
//|||||||||||||||||||||||||||||||||||||||

	//GLuint testLOAD = loadTexture("SK_Material_BaseColor.png", "Resource/Model/SKgan");
	// build and compile our shader zprogram

	// some settings for room
	//Shader roomShader("Resource/Shader/test.vs", "Resource/Shader/test.fs");

	Shader crystalShader;
	// test demo
	std::vector<glm::vec3>cubepositions;
	std::vector<glm::mat4>cubemodelMatrixs;
	int stepNUM = 100;
	for (int i = 0; i<stepNUM; i++) {
		cubepositions.push_back(glm::vec3(-1.5f*i, 1.5f*i, -1.5f*i) + glm::vec3(-10.0f, 0.0f, -10.0f));
	}
	for (int i = 0; i<cubepositions.size(); i++) {
		glm::mat4 cubemodelMatrix = glm::mat4(1.0f);
		cubemodelMatrix = glm::translate(cubemodelMatrix, cubepositions[i]);
		cubemodelMatrix = glm::scale(cubemodelMatrix, glm::vec3(2.0f));
		camera.SetinnerBound(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), cubemodelMatrix);
		cubemodelMatrixs.push_back(cubemodelMatrix);
	}



	// nanosuit model with collision detection !!
	Shader modelShader("Resource/Shader/model.vs", "Resource/Shader/model.fs", nullptr, nullptr, "Resource/Shader/model.gs");
	Model Nanosuit("Resource/Model/nanosuit/nanosuit.obj");
	glm::vec3 nanolb;
	glm::vec3 nanoru;
	glm::mat4 nanoModel;
	nanoModel = glm::mat4(1.0f);
	nanoModel = glm::translate(nanoModel, glm::vec3(0, 0, -20));
	nanoModel = glm::scale(nanoModel, glm::vec3(0.3, 0.3, 0.3));
	Nanosuit.GetCollisionBox(nanolb,nanoru);
	camera.SetinnerBound(nanolb,nanoru,nanoModel);
	
	// draw platform
	Shader pfShader("Resource/Shader/platform.vs", "Resource/Shader/platform.fs");
	std::vector<float> pfvertex = GenCubeTopVertices(2*GROUND_SIZE, 2*GROUND_SIZE, 0, YGap, 0);
	GLuint pfVAO, pfVBO;
	glGenVertexArrays(1, &pfVAO);
		glBindVertexArray(pfVAO);
		glGenBuffers(1, &pfVBO);
			glBindBuffer(GL_ARRAY_BUFFER, pfVBO);
			glBufferData(GL_ARRAY_BUFFER, pfvertex.size() * sizeof(float), &pfvertex[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)(0));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(3*sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(6*sizeof(float)));
			glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	Material floor_material(
		"ground.jpeg",
        glm::vec3(0.384, 0.392, 0.286),//#fbffb9
        16
	); 
	glm::vec3 lightDir(-1.0, -1.0, 1.0);
	DirLight dir_light(lightDir, glm::vec3(1), 0.3, 0.8, 0.5);
	// hero's gan
	Gan SKgan("Resource/Model/SKgan/SK.obj");

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

	// !!!!! particle System !!!!!!!
	// bullet's flame
	Particlesystem flameParticles;
	Shader flameShader("Resource/Shader/testparticle.vs","Resource/Shader/testparticle.fs");
	// gan's fire
	Particlesystem ganfireParticles;
	Shader ganfireShader("Resource/Shader/testparticle.vs","Resource/Shader/testparticle.fs");
	// fountain
	/*Particlesystem fountainParticles("water2.jpg");
	Shader fountainShader("Resource/Shader/waterparticles.vs","Resource/Shader/waterparticles.fs");*/
	// fireworks
	Particlesystem fireworksParticles;
	Shader fireworksShader("Resource/Shader/testparticle.vs","Resource/Shader/testparticle.fs");

	// !!!!!!! word !!!!!!!!
	Font typewritter;

	//// test demo
	//std::vector<glm::vec3>cubeposition;
	//cubeposition.push_back(glm::vec3(0.0f,1.0f,-2.0f));
	//cubeposition.push_back(glm::vec3(-10.0f,2.5f,-10.0f));
	//cubeposition.push_back(glm::vec3(10.0f,5.0f, -10.0f));
	//cubeposition.push_back(glm::vec3(-10.0f,2.5f, -100.0f));
	//cubeposition.push_back(glm::vec3(-10.0f,5.0f,-20.0f));
	//cubeposition.push_back(glm::vec3(0.0f,4.0f,10.0f));
	//
	//// camera.SetinnerBound(glm::vec3(-1.0f,0.0f,-1.0f),glm::vec3(1.0f,2.0f,1.0f));
	//// camera.SetinnerBound(glm::vec3(9.0f,1.5f,-1.0f),glm::vec3(11.0f,3.5f,1.0f));
	//// camera.SetinnerBound(glm::vec3(9.0f,4.0f,9.0f),glm::vec3(11.0f,6.0f,11.0f));
	//// camera.SetinnerBound(glm::vec3(-11.0f,1.5f,9.0f),glm::vec3(-9.0f,3.5f,11.0f));
	//// camera.SetinnerBound(glm::vec3(-11.0f,4.0f,-11.0f),glm::vec3(-9.0f,6.0f,-9.0f));
	//// camera.SetinnerBound(glm::vec3(-1.0f,0.0f,9.0f),glm::vec3(1.0f,2.0f,11.0f));

	//Framebuffer off_screan_render;
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
Shader depthShader("Resource/Shader/depth.vs", "Resource/Shader/depth.fs");
// Shader debugDepthShader("Resource/Shader/debugD.vs", "Resource/Shader/debugD.fs");
GLuint depthMapFBO;
glGenFramebuffers(1, &depthMapFBO);
const GLuint SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
GLuint depthMap;
glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
glBindFramebuffer(GL_FRAMEBUFFER, 0);



//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//// test pointer shadow
//Shader psShader("Resource/Shader/cubeDepth.vs", "Resource/Shader/cubeDepth.fs", nullptr, nullptr, "Resource/Shader/cubeDepth.gs");
//glm::vec3 pointDir(RoomModelMatrix * glm::vec4(35, 10-0.1, 25, 1.0));
//PointShadow pshadow(pointDir);

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	// explode
	modelShader.use();
	bool explode_first = true;
    // view/projection transformations
    camera.SetOuterBound(glm::vec4(-500.0f,-500.0f,500.0f,500.0f));
	int closeEnough=0,damage=0,bullet=10,score=0,lastscore=0;
	bool nolackBullet = true;
	char str_score[100],str_blood[10];
	char str_bullet[10];

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	std::vector<GLuint> outerWalls;
	size_t wall_size = 8;
	for (int i = 0; i < wall_size; ++i)
	{
		GLuint ID = loadTexture("roomOutside" + std::to_string(i) + ".jpg", "Resource/Texture");
		outerWalls.push_back(ID);
	}

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.GetFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, PROJECT_NEAR, PROJECT_FAR);
/*+-----------------------------------------------------------------+
 *+                        first stage                              +
 *+-----------------------------------------------------------------+*/
		// per-frame time logic
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		processInput(window);
		crystalsystem.generateCrystal(glm::vec3(0.0f,0.0f,0.0f),60.0f,0.5f,0.3f,currentFrame);
		
		// render
		//off_screan_render.pre_use();
		GLfloat near_plane = -100.0f, far_plane = 150.0f;
		glm::vec3 pos = camera.GetPosition();
		
		glm::mat4 lightProjection = glm::ortho(-150.0f, 150.0f, -100.0f, +100.0f,  near_plane, far_plane);

		glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f), lightDir, glm::vec3(0.0, 1.0, 0.0));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		lightSpaceMatrix = glm::translate(lightSpaceMatrix, -camera.GetPosition());

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			depthShader.use();
			depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
/*+-----------------------------------------------------------------+
 *+                        render scene                             +
 *+-----------------------------------------------------------------+*/	
			depthShader.setMat4("model", nanoModel);
			Nanosuit.Draw(depthShader);
			for (size_t i = 0; i < rooms.size(); ++i)
				rooms[i].Draw(depthShader);
			/*glBindVertexArray(VAOcube);
			for (size_t i = 0; i < cubemodelMatrixs.size(); i++) {
				depthShader.setMat4("model", cubemodelMatrixs[i]);
				glDrawArrays(GL_TRIANGLES, 0, cubicVertex.size() / 8);
			}*/
		crystalsystem.drawAll(depthShader);


		//glBindVertexArray(VAOcube);
		//	for(size_t i=0;i<cubeposition.size();i++){
		//		model=glm::mat4(1.0f);
		//		model=glm::translate(model,cubeposition[i]);
		//		model=glm::scale(model,glm::vec3(2.0f,2.0f,2.0f));
		//		depthShader.setMat4("model", model);
		//		glDrawArrays(GL_TRIANGLES,0,cubicVertex.size()>>3);
		//	}
		glBindVertexArray(pfVAO);
			depthShader.setMat4("model", glm::mat4(1.0));
			glDrawArrays(GL_TRIANGLES, 0, pfvertex.size()>>3);
		glBindVertexArray(0);

/*+-----------------------------------------------------------------+
 *+                        second stage                             +
 *+-----------------------------------------------------------------+*/
		


/*+-----------------------------------------------------------------+
 *+                        second stage                             +
 *+-----------------------------------------------------------------+*/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Reset viewport       
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //      debugDepthShader.use();
		//debugDepthShader.setFloat("near_plane", near_plane);
		//debugDepthShader.setFloat("far_plane", far_plane);
  //      glActiveTexture(GL_TEXTURE0);
  //      glBindTexture(GL_TEXTURE_2D, depthMap);
        //RenderQuad();
		

		
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
		if(!(starting || restart)){
			crystalsystem.updateAll(camera.GetPosition(),deltaTime);
			crystalsystem.updateHeroState(camera.GetPosition(),closeEnough,damage,bullet);
			crystalsystem.drawAll(projection,view,camera.GetPosition(),skybox.getTextId(),currentFrame,deltaTime,score);
		}
		
		
		// step3: draw bullet !!
		heroBullet.SetDirection(camera.GetEyeFront());
		heroBullet.SetStartPos(camera.GetPosition());
		if(cur_button_mode == left){
			if(!(starting || restart) && !heroBullet.IsAttacking){
				heroBullet.Attack(bullet);
			}
			if(starting)starting = 0;
			if(restart){
				restart = 0;
				closeEnough=0,damage=0,bullet=10,score=0;
				nolackBullet = true;
			}
		}
		heroBullet.updatePosition(deltaTime,currentFrame);
        heroBullet.draw(projection,view,camera.GetPosition(),skybox.getTextId(),deltaTime);

		////// step4 : draw test cube
		//glBindVertexArray(VAOcube);
		//cubeShader.use();
		//	cubeShader.setMat4("projection",projection);
		//	cubeShader.setMat4("view",view);
		//	cubeShader.setVec3("cameraPos",camera.GetPosition());
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTextId());
		//	cubeShader.setInt("skybox",0);
		//	for (size_t i = 0; i < cubemodelMatrixs.size(); i++) {
		//		cubeShader.setMat4("model", cubemodelMatrixs[i]);
		//		glDrawArrays(GL_TRIANGLES, 0, cubicVertex.size() / 8);
		//	}
		//glUseProgram(0);
		//glBindVertexArray(0);

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
			
			modelShader.setMat4("model", nanoModel);
			Nanosuit.Draw(modelShader);
		glUseProgram(0);
		
		// step6 : draw rooms
		roomShader.use();
			roomShader.setVec3("viewPos", camera.GetPosition());
			roomShader.setMat4("projection", projection);
			roomShader.setMat4("view", view);
			roomShader.setBool("flash", flashlight_on);
			roomShader.setBool("phong", phong);
        	glActiveTexture(GL_TEXTURE2);
        	glBindTexture(GL_TEXTURE_2D, depthMap);
        	roomShader.setInt("shadowMap", 2);
			if (flashlight_on)
			{
				roomShader.setVec3("spot_light.position",  camera.GetPosition());
				roomShader.setVec3("spot_light.direction", camera.GetEyeFront());
				roomShader.setVec3("spot_light.color", glm::vec3(1.0, 0, 0));
				roomShader.setFloat("spot_light.ambient", 0.3);
				roomShader.setFloat("spot_light.diffuse", 1.2);
				roomShader.setFloat("spot_light.specular",0.5);
			}else{
				roomShader.setFloat("spot_light.ambient", 0);
				roomShader.setFloat("spot_light.diffuse", 0);
				roomShader.setFloat("spot_light.specular",0);
			}
			for (size_t i = 0; i < rooms.size(); ++i)
			{
				rooms[i].setWallTexture(outerWalls[i%wall_size]);
				rooms[i].Draw(roomShader);
			}

		pfShader.use();
		glBindVertexArray(pfVAO);
			pfShader.setVec3("viewPos", camera.GetPosition());
			pfShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
			pfShader.setMat4("projection", projection);
			pfShader.setMat4("view", view);
			pfShader.setMat4("model", glm::mat4(1.0));
			pfShader.setBool("phong", phong);
			pfShader.setVec3("viewPos", camera.GetPosition());
			setDirLight(pfShader, "dir_light", dir_light);
			setMaterial(pfShader, "floor_material", floor_material);
			glActiveTexture(GL_TEXTURE2);
        	glBindTexture(GL_TEXTURE_2D, depthMap);
        	pfShader.setInt("shadowMap", 2);
			glDrawArrays(GL_TRIANGLES, 0, pfvertex.size()>>3);
		glBindVertexArray(0);

		//// step7 : !!!!!!!!!!!! draw all special effects !!!!!!!!!!!!
		//draw fountain
		//if (egg)
		//{
		//	 fountainShader.use();
		// 		fountainShader.setMat4("projection",projection);
		// 		fountainShader.setMat4("view",view);
		// 		fountainShader.setVec3("eyeFront",camera.GetEyeFront());
		// 		fountainShader.setVec3("eyeUp",camera.GetUp());
		// 		fountainShader.setVec3("eyeRight",camera.GetRight());

		// 		fountainParticles.updateParticlesFountain(deltaTime,camera.GetPosition(),glm::vec3(0.0, 0.0, -20));
		// 		fountainParticles.updateBuffer();
		// 		fountainParticles.drawParticles(fountainShader);
		//	 glUseProgram(0);
		//}

		//draw flame
		flameShader.use();
			flameShader.setMat4("projection",projection);
			flameShader.setMat4("view",view);
			flameShader.setVec3("eyeFront",camera.GetEyeFront());
			flameShader.setVec3("eyeUp",camera.GetUp());
			flameShader.setVec3("eyeRight",camera.GetRight());

			flameParticles.updateParticlesFlame(heroBullet.IsAttacking, deltaTime, heroBullet.getPosition(), heroBullet.direction, camera.GetPosition());
			flameParticles.updateBuffer();
			flameParticles.drawParticles(flameShader);
		glUseProgram(0);
		//draw gan fire
		ganfireShader.use();
			ganfireShader.setMat4("projection",projection);
			ganfireShader.setMat4("view",view);
			ganfireShader.setVec3("eyeFront",camera.GetEyeFront());
			ganfireShader.setVec3("eyeUp",camera.GetUp());
			ganfireShader.setVec3("eyeRight",camera.GetRight());

			ganfireParticles.updateParticlesOpenFire(heroBullet.IsAttacking,deltaTime,currentFrame,SKgan.muzzlePos,SKgan.direction,camera.GetPosition());
			ganfireParticles.updateBuffer();
			ganfireParticles.drawParticles(ganfireShader);
		glUseProgram(0);
		////draw fireworks
		fireworksShader.use();
			fireworksShader.setMat4("projection",projection);
			fireworksShader.setMat4("view",view);
			fireworksShader.setVec3("eyeFront",camera.GetEyeFront());
			fireworksShader.setVec3("eyeUp",camera.GetUp());
			fireworksShader.setVec3("eyeRight",camera.GetRight());

			fireworksParticles.updateParticlesFireworks(deltaTime,camera.GetPosition(),60.0f,heroBullet.getPosition(),heroBullet.IsAttacking);
			fireworksParticles.updateBuffer();
			fireworksParticles.drawParticles(fireworksShader);
		glUseProgram(0);


		//!!!!!! hero's gan !!!!!!!
		SKgan.updateModel(camera,deltaTime);
		if( !(starting || restart) )
			SKgan.draw(projection,view,camera.GetPosition());

		// !!!!!!!!!! ground !!!!!!!!!!
		ground.updateGrass(camera.GetPosition());
		ground.updateBuffer();
		ground.draw(projection,view,camera);

		// !!!!!!!! words !!!!!!!!
		if(starting){
			typewritter.RenderText("START NOW",0.4*SCR_WIDTH,0.45*SCR_HEIGHT,1.0,glm::vec3(1.0,0.0,0.0));
		}
		else if(!restart)
		{
			sprintf(str_score,"%d",score*10);
			sprintf(str_blood,"%d",200 - damage);
			sprintf(str_bullet,"%d",bullet);
			typewritter.RenderText(str_score,0.1*SCR_WIDTH,0.9*SCR_HEIGHT,1.0,glm::vec3(1.0,0.0,0.0));
			typewritter.RenderText(str_blood,0.9*SCR_WIDTH,0.9*SCR_HEIGHT,1.0,glm::vec3(1.0,0.0,0.0));
			typewritter.RenderText(str_bullet,0.1*SCR_WIDTH,0.1*SCR_HEIGHT,1.0,glm::vec3(1.0,0.0,0.0));
			if(bullet <= 0)
				typewritter.RenderText("Lack Bullet",0.4*SCR_WIDTH,0.15*SCR_HEIGHT,1.0,glm::vec3(1.0,0.0,0.0));
		}
		if(200 - damage < 0){
			typewritter.RenderText("Game Over",0.4*SCR_WIDTH,0.65*SCR_HEIGHT,1.0,glm::vec3(1.0,0.0,0.0));
			lastscore = score * 10;
			sprintf(str_score,"Score: %d",lastscore);
			typewritter.RenderText(str_score,0.4*SCR_WIDTH,0.55*SCR_HEIGHT,1.0,glm::vec3(1.0,0.0,0.0));
			typewritter.RenderText("RESTART NOW",0.35*SCR_WIDTH,0.45*SCR_HEIGHT,1.0,glm::vec3(1.0,0.0,0.0));
			restart = 1;
		}

		//step final: draw UI
		myUI.updateAlpha(closeEnough,currentFrame);
		myUI.draw( starting || restart );

/*+-----------------------------------------------------------------+
 *+                        second stage                             +
 *+-----------------------------------------------------------------+*/
		//off_screan_render.draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAOcube);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();

	return 0;
}

GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
    if (quadVAO == 0)
    {
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
             1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
