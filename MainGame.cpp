#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
   
}

MainGame::~MainGame() 
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	using namespace std;

	cout << "Oh no those monkeys are trying to smash your egg\nbeat those monkeys up before they smash your egg\nPress A & D to move left and right\n W & S to move up and down" << endl;
	cout << totalMonkeys << endl;
	cout << pHealth << endl;
	_gameDisplay.initDisplay(); 
	whistle = audioDevice.loadSound("..\\res\\bang.wav");
	backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	texture.init("..\\res\\bricks.jpg"); //load texture
	texture1.init("..\\res\\water.jpg"); //load texture
	// initalise all shaders
	shaderPass.init("..\\res\\shaderRim.vert","..\\res\\shaderRim.frag");
	shaderBlur.init("..\\res\\shaderBlur.vert", "..\\res\\shaderBlur.frag");
	shaderToon.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag");
	shaderRim.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	shaderSkybox.init("..\\res\\shaderSkyBox.vert", "..\\res\\shaderSkyBox.frag");
	customShader.init("..\\res\\customShader.vert", "..\\res\\customShader.frag");
	shaderReflect.init("..\\res\\shaderReflect.vert", "..\\res\\shaderReflect.frag");
	
	shaderGeo.initGeoShader("..\\res\\geomShader.vert", "..\\res\\geomShader.geom", "..\\res\\geomShader.frag");
	
// skybox images
	vector<std::string>faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	cubemapTexture = skybox.loadCubemap(faces); //Load the cubemap using "faces" into cubemapTextures
// sky box vertices
	
	float skyboxVertices[] = {
		// positions          
		-6.0f,  6.0f, -6.0f,
		-6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		-6.0f,  6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f,  6.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	overlay.init("..\\res\\brick.jpg");
	// load all my models
	// keep models under 1000 vertices or you'll break it again
	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\sword_gladius_final.obj");
	mesh3.loadModel("..\\res\\monkey3.obj");
	mesh4.loadModel("..\\res\\egg.obj");
	mesh5.loadModel("..\\res\\monkey3.obj");
	mesh6.loadModel("..\\res\\monkey3.obj");
	bang = audioDevice.loadSound("..\\res\\bang.wav");

	
	myCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;
}



void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
		playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;


				// Player controls for moving
			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
					// move left and rotates sword
				case SDLK_a: 
				{
					std::cout << "Move Left" << std::endl;
					xPlayerMov += 0.50f;
					yPlayerRotate = 0.0f;
					break;
				}
				// moves right and rotates sword
				case SDLK_d:  
				{
					std::cout << "Move Right" << std::endl;
					xPlayerMov -= 0.50f;
					yPlayerRotate = 0.0f;
					break;
				}
				// moves up and rotates sword
				case SDLK_w:
				{
					std::cout << "Move Up" << std::endl;
					yPlayerMov += 0.50f;
					yPlayerRotate = 1.6f;
					break;
				}
				// moves down and rotates sword
				case SDLK_s:
				{
					std::cout << "Move Down" << std::endl;
					yPlayerMov -= 0.50f;
					yPlayerRotate = 1.6f;
					break;
				}
				
				case SDLK_g:
				{
					gamestart =true;
					std::cout << "game started" << std::endl;
					totalMonkeys = 4;
				}

				case SDLK_p:
				{
					gamestart =false;
				}


				}




			}
			break;

		}


	}
	
}

void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	/*
	Possible values of state
	AL_INITIAL
	AL_STOPPED
	AL_PLAYING
	AL_PAUSED
	*/
	if (AL_PLAYING != state)
	{
		audioDevice.playSound(Source, pos);
	}
}

void MainGame::setADSLighting()
{
	modelView = transform.GetModel() * myCamera.GetView();
	
	shaderPass.setMat4("ModelViewMatrix", modelView);
	shaderPass.setMat4("ProjectionMatrix", myCamera.GetProjection()); 
	
	glm::mat4 normalMatrix = transpose(inverse(modelView));
	
	shaderPass.setMat4("NormalMatrix", normalMatrix);

	shaderPass.setVec4("Position", glm::vec4(10.0,10.0,10.0,1.0));
	shaderPass.setVec3("Intensity", glm::vec3(0.0, 0.0, 0.0));

	shaderPass.setVec3("ka", glm::vec3(0.5, 0.5, 0.5));
	shaderPass.setVec3("kd", glm::vec3(0.5, 0.5, 0.5));
	shaderPass.setVec3("ks", glm::vec3(0.5, 0.5, 0.5));

	shaderPass.setFloat("Shininess", 0.5);
}

void MainGame::setToonLighting()
{
	shaderToon.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
}

void MainGame::setRimShader()
{
	shaderRim.setMat4("u_vm", myCamera.GetView());
	shaderRim.setMat4("u_pm", myCamera.GetProjection());
}

// not used

void MainGame:: setCheckShader()
{
	//shaderCheck.setMat3 normalMatrix;
	shaderCheck.setMat4("u_vm", myCamera.GetView());
	shaderCheck.setMat4("u_pm", myCamera.GetProjection());
}

void MainGame::linkGeoShader()
{
	shaderGeo.setFloat("time", counter);
	
}

void MainGame::blobEffect()
{
	GLuint blockIndex = glGetUniformBlockIndex(shaderBlur.getProgram(), "BlobSettings");

	GLint blockSize;
	glGetActiveUniformBlockiv(shaderBlur.getProgram(), blockIndex,
		GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize); //get information about blobsettings and save it in blockSize

	GLubyte * blockBuffer = (GLubyte *)malloc(blockSize); //allocates the requested memory and returns a pointer to it.

														  // Query for the offsets of each block variable
	const GLchar *names[] = { "InnerColor", "OuterColor",
		"RadiusInner", "RadiusOuter" };

	GLuint indices[4];
	glGetUniformIndices(shaderBlur.getProgram(), 4, names, indices); // glGetUniformIndices retrieves the indices of a number of uniforms within program

	GLint offset[4];
	glGetActiveUniformsiv(shaderBlur.getProgram(), 4, indices, GL_UNIFORM_OFFSET, offset); //Returns information about several active uniform variables for the specified program object

	GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };

	GLfloat innerRadius = 0.0f, outerRadius = 3.0f;

	memcpy(blockBuffer + offset[0], innerColor,
		4 * sizeof(GLfloat)); //destination, source, no of bytes. 
	memcpy(blockBuffer + offset[1], outerColor,
		4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[2], &innerRadius,
		sizeof(GLfloat));
	memcpy(blockBuffer + offset[3], &outerRadius,
		sizeof(GLfloat));

	GLuint uboHandle;

	glGenBuffers(1, &uboHandle);

	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer,
		GL_DYNAMIC_DRAW); //creates and initializes a buffer object's data store - targer, size, data, usage

	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uboHandle); // bind a buffer object to an indexed buffer target - trager, index, buffer
}

void MainGame::Skybox()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shaderSkybox.Bind();
	shaderSkybox.setInt("skybox", 0);
	//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	shaderSkybox.setMat4("view", myCamera.GetView());
	shaderSkybox.setMat4("projection", myCamera.GetProjection());
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad )
{
// Pythagoras used to calculate distance between spheres.
	float distance = glm::sqrt((m2Pos.x - m1Pos.x) * (m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y) * (m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z) * (m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		//audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		//playAudio(whistle, m1Pos);

		std::cout << "hit" << std::endl;

		return true;
	}
	else
	{
		return false;
	}
}



void MainGame::setCustomShader()
{

	customShader.setFloat("minDist", 0.0f);
	customShader.setFloat("maxDist", 20.0f);
	customShader.setVec3("lightDirection", glm::vec3(1.0f, 1.0f, 1.0f));
	customShader.setVec3("fogColour", glm::vec3(0.5f, 1.0f, 0.0f));
	

}

void MainGame::setReflectShader()
{
	shaderReflect.setMat4("model", modelView);
	//shaderReflect.setMat4("view", myCamera.GetView());
	//shaderReflect.setMat4("projection", myCamera.GetProjection());
	shaderReflect.setVec3("camerPos", myCamera.getPos ());
}

void MainGame::drawGame()
{
	
		// random number generator for monkey speed
		srand(time(0));
		static bool rndGen;
		if (!rndGen)
		{

			rXMonkeyMovAc = (rand() % 3 + 1);
			cout << rXMonkeyMovAc << endl;
			lXMonkeyMovAc = (rand() % 3 + 1);
			cout << lXMonkeyMovAc << endl;
			tYMonkeyMovAc = (rand() % 3 + 1);
			cout << tYMonkeyMovAc << endl;
			bYMonkeyMovAc = (rand() % 3 + 1);
			cout << bYMonkeyMovAc << endl;


			rndGen = true;
		}

		//float mHeadX, mHeadY; // not currently used // saved for potential future use

		Transform transform;
		Transform transform2;
		Transform transform3;
		Transform transform4;
		Transform transform5;
		Transform transform6;
		playAudio(backGroundMusic, glm::vec3(0.0f, 0.0f, 0.0f));

		_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

		linkGeoShader();

		Skybox();
		
		// sets the monkeys to draw only if mdead is false
		if (m1Dead == false)
		{
			// Model 2 / Monkey / right
			//transform.SetPosition(glm::vec3(-sinf(counter), -1.0, -sinf(counter)*5));
			//transform.SetPosition(glm::vec3(-2.8 +-sinf(counter) * 2, 0.0, 0.0));

			transform.SetPosition(glm::vec3(-4.0 * sinf(counter) * rXMonkeyMovAc, 0.0f, 0.0f));
			transform.SetRotation(glm::vec3(0.0, 3.2, 0.0));
			transform.SetScale(glm::vec3(0.4, 0.4, 0.4));
			// binds toon effects
			shaderToon.Bind();
			setToonLighting();
			shaderToon.Update(transform, myCamera);
			mesh1.draw();
			mesh1.updateSphereData(transform.GetPostition(), 0.62f);

		}

		if (m2Dead  == false)
		{

			// Model 2 / Monkey / Left
			//transform.SetPosition(glm::vec3(-sinf(counter), -1.0, -sinf(counter)*5));
			transform2.SetPosition(glm::vec3(2.8 * sinf(counter) * lXMonkeyMovAc, 0.0, 0.0));
			transform2.SetRotation(glm::vec3(0.0, 3.5, 0.0));
			transform2.SetScale(glm::vec3(0.4, 0.4, 0.4));
			// binds toon effects
			shaderToon.Bind();
			setToonLighting();
			shaderToon.Update(transform2, myCamera);
			mesh5.draw();
			mesh5.updateSphereData(transform2.GetPostition(), 0.62f);
		}

		if (m3Dead == false)
		{
			// Model 2 / Monkey / top
			//transform.SetPosition(glm::vec3(-sinf(counter), -1.0, -sinf(counter)*5));
			transform3.SetPosition(glm::vec3(0.0, 4.0 * sinf(counter) * tYMonkeyMovAc, 0.0));
			transform3.SetRotation(glm::vec3(0.0, 3.2, 0.0));
			transform3.SetScale(glm::vec3(0.4, 0.4, 0.4));
			// binds toon effects
			shaderToon.Bind();
			setToonLighting();
			shaderToon.Update(transform3, myCamera);
			mesh3.draw();
			mesh3.updateSphereData(transform3.GetPostition(), 0.62f);
		}

		if (m4Dead == false)
		{
			// Model 2 / Monkey / bottom
			//transform.SetPosition(glm::vec3(-sinf(counter), -1.0, -sinf(counter)*5));
			transform4.SetPosition(glm::vec3(0.0, -4.0 * sinf(counter) * bYMonkeyMovAc, 0.0));
			transform4.SetRotation(glm::vec3(0.0, 3.2, 0.0));
			transform4.SetScale(glm::vec3(0.4, 0.4, 0.4));
			// binds toon effects
			shaderToon.Bind();
			setToonLighting();
			shaderToon.Update(transform4, myCamera);
			mesh6.draw();
			mesh6.updateSphereData(transform4.GetPostition(), 0.62f);
		}


		// Model 4 player sword with reflect
		transform5.SetPosition(glm::vec3(xPlayerMov, yPlayerMov, 0.0));
		transform5.SetRotation(glm::vec3(0.0, 0.0, yPlayerRotate));
		transform5.SetScale(glm::vec3(0.6, 0.6, 0.6));

		// binds reflect effects
		shaderReflect.Bind();
		setReflectShader();
		shaderReflect.Update(transform5, myCamera);
		mesh2.draw();
		mesh2.updateSphereData(transform5.GetPostition(), 0.62f);
		// controls the collision between player and monkey
		collision(mesh2.getSpherePos(), mesh2.getSphereRadius(), mesh1.getSpherePos(), mesh1.getSphereRadius());
		collision(mesh2.getSpherePos(), mesh2.getSphereRadius(), mesh3.getSpherePos(), mesh3.getSphereRadius());
		collision(mesh2.getSpherePos(), mesh2.getSphereRadius(), mesh5.getSpherePos(), mesh5.getSphereRadius());
		collision(mesh2.getSpherePos(), mesh2.getSphereRadius(), mesh6.getSpherePos(), mesh6.getSphereRadius());
		// collision happens carries out the following actions
		if (collision(mesh2.getSpherePos(), mesh2.getSphereRadius(), mesh1.getSpherePos(), mesh1.getSphereRadius()))
		{
			m1Dead = true;
			mesh1.~Mesh();
			playAudio(bang, glm::vec3(0, 0, 0));
			std::cout << totalMonkeys << std::endl;
			std::cout << "You hit the right monkey" << std::endl;
			rightMonkeyDead = true;

		}

		if (rightMonkeyDead == true)
		{
			totalMonkeys -= 1;
			transform.SetPosition(glm::vec3(-1000.0f, -1000.0f, -500.0f));
			mesh1.updateSphereData(transform.GetPostition(), 0.0f);
			std::cout << "monkey1 dead" << std::endl;
			std::cout << totalMonkeys << std::endl;
			rightMonkeyDead = false;
		}

		if (collision(mesh2.getSpherePos(), mesh2.getSphereRadius(), mesh3.getSpherePos(), mesh3.getSphereRadius()))
		{
			m3Dead = true;
			mesh3.~Mesh();
			playAudio(bang, glm::vec3(0, 0, 0));
			std::cout << totalMonkeys << std::endl;
			std::cout << "You hit the top monkey" << std::endl;
			topMonkeyDead = true;
		}


		if (topMonkeyDead == true)
		{
			totalMonkeys -= 1;
			transform3.SetPosition(glm::vec3(-1000.0f, -1000.0f, -500.0f));
			mesh3.updateSphereData(transform3.GetPostition(), 0.0f);
			std::cout << "monkey3 dead" << std::endl;
			std::cout << totalMonkeys << std::endl;
			topMonkeyDead = false;
			
		}

		if (collision(mesh2.getSpherePos(), mesh2.getSphereRadius(), mesh5.getSpherePos(), mesh5.getSphereRadius()))
		{
			m2Dead = true;
			mesh5.~Mesh();
			playAudio(bang, glm::vec3(0, 0, 0));
			std::cout << totalMonkeys << std::endl;
			std::cout << "You hit the  left monkey" << std::endl;
			leftMonkeyDead = true;
		}

		if (leftMonkeyDead == true)
		{
			totalMonkeys -= 1;
			transform2.SetPosition(glm::vec3(1000.0f, 1000.0f, 500.0f));
			mesh5.updateSphereData(transform2.GetPostition(), 0.0f);
			std::cout << "monkey2 dead" << std::endl;
			std::cout << totalMonkeys << std::endl;
			leftMonkeyDead = false;

		}
		

		if (collision(mesh2.getSpherePos(), mesh2.getSphereRadius(), mesh6.getSpherePos(), mesh6.getSphereRadius()))
		{
			m4Dead = true;
			mesh6.~Mesh();
			playAudio(bang, glm::vec3(0, 0, 0));
			
			//std::cout << totalMonkeys << std::endl;
			std::cout << "You hit the bottom monkey" << std::endl;
			bottomMonkeyDead = true;
		}

		if (bottomMonkeyDead == true)
		{
			totalMonkeys -= 1;
			transform4.SetPosition(glm::vec3(-1000.0f, -1000.0f, -500.0f));
			mesh6.updateSphereData(transform4.GetPostition(), 0.0f);
			std::cout << "monkey4 dead" << std::endl;
			std::cout << totalMonkeys << std::endl;
			bottomMonkeyDead = false;

			

		}

		if (m4Dead == true  && m3Dead == true && m2Dead == true && m1Dead == true)
		{
			totalMonkeys = 0;
		}
		// Model 5 - egg
		transform6.SetPosition(glm::vec3(0.0, 0.0, 0.0));
		transform6.SetRotation(glm::vec3(0.0, 2.0, 0.0));
		transform6.SetScale(glm::vec3(0.2, 0.2, 0.2));
		// binds custom shader effects
		shaderToon.Bind();
		setToonLighting();
		shaderToon.Update(transform6, myCamera);
		mesh4.draw();
		mesh4.updateSphereData(transform6.GetPostition(), 0.62f);
		collision(mesh4.getSpherePos(), mesh4.getSphereRadius(), mesh1.getSpherePos(), mesh1.getSphereRadius());
		collision(mesh4.getSpherePos(), mesh4.getSphereRadius(), mesh3.getSpherePos(), mesh3.getSphereRadius());
		collision(mesh4.getSpherePos(), mesh4.getSphereRadius(), mesh5.getSpherePos(), mesh5.getSphereRadius());
		collision(mesh4.getSpherePos(), mesh4.getSphereRadius(), mesh6.getSpherePos(), mesh6.getSphereRadius());

		// collsion between egg and monkeys
		if (collision(mesh4.getSpherePos(), mesh4.getSphereRadius(), mesh1.getSpherePos(), mesh1.getSphereRadius()) || collision(mesh4.getSpherePos(), mesh4.getSphereRadius(), mesh3.getSpherePos(), mesh3.getSphereRadius())
			|| collision(mesh4.getSpherePos(), mesh4.getSphereRadius(), mesh5.getSpherePos(), mesh5.getSphereRadius()) || collision(mesh4.getSpherePos(), mesh4.getSphereRadius(), mesh6.getSpherePos(), mesh6.getSphereRadius()))
		{
			std::cout << pHealth << std::endl;
			playAudio(bang, glm::vec3(0, 0, 0));
			pHealth -= 1;
			if (pHealth <= 1)
			{
				lostGame = true;
			}
			

		}
		// wind and lose statements
		if (lostGame==true)
		{
			mesh4.~Mesh();
			std::cout << "\nthose Damn Monkeys broke your egg" << std::endl;
			lostGame = false;
			
		}

		if (totalMonkeys == 0)
		{
			wonGame = true;
			
		}

		if (wonGame == true)
		{
			std::cout << "\nCongrats you beat those monkeys up" << std::endl;
			
			
		}

		processInput(); //calls processInput function

		counter = counter + 0.001f;


		glEnableClientState(GL_COLOR_ARRAY);
		glEnd();

		_gameDisplay.swapBuffer();

		
		
	
} 


