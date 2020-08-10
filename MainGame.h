#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
//#include "ShaderGeo.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Camera.h"
#include "Audio.h"
#include "Overlay.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void playAudio(unsigned int Source, glm::vec3 pos);
	void blobEffect();
	void setADSLighting();
	void setToonLighting();
	void setRimShader();
	void setCheckShader();
	void setFogShader();
	void setReflectShader();
	void Skybox();
	//GEO SHADER
	void linkGeoShader();
	void setCustomShader();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);

	GLuint skyboxVAO, skyboxVBO, cubemapTexture;
	vector<std::string> faces;
	Texture skybox;
	Shader shaderSkybox;

	Display _gameDisplay;
	GameState _gameState;
	Audio audioDevice;
	float counter2;
	unsigned int backGroundMusic;
	unsigned int bang;
	// player move and rotate 
	float xPlayerMov = 0.0f; 
	float yPlayerMov = 0.0f;
	float yPlayerRotate = 0.0f;
	float xPlayerRotate = 0.0f;
	int pHealth = 10;
	// monkey values
	int totalMonkeys = 4;
	float lXMonkeyMov = 0.0f; 
	float rXMonkeyMov = 0.0f;
	float rXMonkeyMovAc = 0.0f;
	float lXMonkeyMovAc = 0.0f;
	float tYMonkeyMovAc = 0.0f;
	float bYMonkeyMovAc = 0.0f;
	float tYMonkeyMove = 0.0f;
	float bYMonkeyMove = 0.0f; 

	// Game Values
	bool gamestart = false;
	bool m1Dead;
	bool m2Dead;
	bool m3Dead;
	bool m4Dead;
	bool rightMonkeyDead;
	bool leftMonkeyDead;
	bool topMonkeyDead;
	bool bottomMonkeyDead;
	bool wonGame = false;
	bool lostGame = false;

	bool monkeyMoveRight = true;
	bool lMonkeyKIA = false;
	bool rMonkeyKIA = false;
	bool tMonkeyKIA = false;
	bool bMonkeyKIA = false;

	// mesh texture and shaders
	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;
	Mesh mesh4;
	Mesh mesh5;
	Mesh mesh6;
	Texture texture;
	Texture texture1;
	Overlay overlay;
	Shader shaderPass;
	Shader shaderBlur;
	Shader shaderToon;
	Shader shaderRim;
	Shader shaderCheck;
	Shader customShader;
	Shader shaderGeo;
	Shader shaderReflect;
	//ShaderGeo shaderGeo;

	glm::mat4 modelView;

	Camera myCamera;
	

	float counter;
	unsigned int whistle;
	
};

