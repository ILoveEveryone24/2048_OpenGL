#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <time.h>
#include <stdlib.h>

const int WIDTH = 500;
const int HEIGHT = 500;
const int GRID_SIZE = 4; //4x4 grid
const float OFFSET = 0.03f;
const float SIZE = 2.0f/GRID_SIZE;

struct Square{
	public:
		int x;
		int y;
		
		Square(int posX, int posY)
			: x(posX), y(posY) {}

		void addToVBO(std::vector<glm::vec3> &vect, int depth){
				vect.push_back(glm::vec3(-1.0f + this->x*SIZE + OFFSET, 1.0f - this->y*SIZE - OFFSET, 0.99f - 0.01f * depth)); //Top left
				vect.push_back(glm::vec3(-1.0f + this->x*SIZE + OFFSET, 1.0f - this->y*SIZE - SIZE + OFFSET, 0.99f - 0.01f * depth)); //Bottom left
				vect.push_back(glm::vec3(-1.0f + this->x*SIZE + SIZE - OFFSET, 1.0f - this->y*SIZE - OFFSET, 0.99f - 0.01f * depth)); //Top right
				vect.push_back(glm::vec3(-1.0f + this->x*SIZE + SIZE - OFFSET, 1.0f - this->y*SIZE - SIZE + OFFSET, 0.99f - 0.01f * depth)); //Bottom right
			}
		//Maybe add later		
		void addToEBO(std::vector<glm::uvec3> &vect){
		
		}

};

//Define grid based on other squares position?

void moveDown(std::vector<glm::vec3> &vect){
	int cnt = 0;
	for(int i = 0; i < vect.size(); i++){
		if(cnt != i/4)
			cnt++;
		switch(i - 4*cnt){
			case 0:
				if(vect[i].y > -1.0 + OFFSET + SIZE)
					vect[i].y -= SIZE;
				break;	
			case 1:
				if(vect[i].y > -1.0 + OFFSET)
					vect[i].y -= SIZE;
				break;	
			case 2:
				if(vect[i].y > -1.0 + OFFSET + SIZE)
					vect[i].y -= SIZE;
				break;	
			case 3:
				if(vect[i].y > -1.0 + OFFSET)
					vect[i].y -= SIZE;
				break;	
			default:
				break;	
		}
	}
}

void moveUp(std::vector<glm::vec3> &vect){
	int cnt = 0;
	for(int i = 0; i < vect.size(); i++){
		if(cnt != i/4)
			cnt++;
		switch(i - 4*cnt){
			case 0:
				if(vect[i].y < 1.0 - OFFSET)
					vect[i].y += SIZE;
				break;	
			case 1:
				if(vect[i].y < 1.0 - OFFSET - SIZE)
					vect[i].y += SIZE;
				break;	
			case 2:
				if(vect[i].y < 1.0 - OFFSET)
					vect[i].y += SIZE;
				break;	
			case 3:
				if(vect[i].y < 1.0 - OFFSET - SIZE)
					vect[i].y += SIZE;
				break;	
			default:
				break;	
		}
	}

}

void moveLeft(std::vector<glm::vec3> &vect){
	int cnt = 0;
	for(int i = 0; i < vect.size(); i++){
		if(cnt != i/4)
			cnt++;
		switch(i - 4*cnt){
			case 0:
				if(vect[i].x > -1.0 + OFFSET)
					vect[i].x -= SIZE;
				break;	
			case 1:
				if(vect[i].x > -1.0 + OFFSET)
					vect[i].x -= SIZE;
				break;	
			case 2:
				if(vect[i].x > -1.0 + OFFSET + SIZE)
					vect[i].x -= SIZE;
				break;	
			case 3:
				if(vect[i].x > -1.0 + OFFSET + SIZE)
					vect[i].x -= SIZE;
				break;	
			default:
				break;	
		}
	}

}

void moveRight(std::vector<glm::vec3> &vect){
	int cnt = 0;
	for(int i = 0; i < vect.size(); i++){
		if(cnt != i/4)
			cnt++;
		switch(i - 4*cnt){
			case 0:
				if(vect[i].x < 1.0 - OFFSET - SIZE)
					vect[i].x += SIZE;
				break;	
			case 1:
				if(vect[i].x < 1.0 - OFFSET - SIZE)
					vect[i].x += SIZE;
				break;	
			case 2:
				if(vect[i].x < 1.0 - OFFSET)
					vect[i].x += SIZE;
				break;	
			case 3:
				if(vect[i].x < 1.0 - OFFSET)
					vect[i].x += SIZE;
				break;	
			default:
				break;	
		}
	}

}

int main(){

	unsigned int gridVBO, blockVBO, gridVAO, blockVAO, gridEBO, blockEBO;
	
	std::vector<glm::vec3> gridVertices;
	std::vector<glm::uvec3> gridIndices;

	std::vector<glm::vec3> blockVertices;
	std::vector<glm::uvec3> blockIndices;

	std::vector<Square> squares;

	srand(time(NULL));
	
	for(int i = 0; i < GRID_SIZE; i++)
		for(int j = 0; j < GRID_SIZE; j++){
			Square square(i, j);
			square.addToVBO(gridVertices, 0);
		}


	int randX = rand() % 4;
	int randY = rand() % 4;
	//Square square(randX, randY);
	Square square(1, 1);
	square.addToVBO(blockVertices, 1);
	
	int randX2 = rand() % 4;
	int randY2 = rand() % 4;
	while(randX == randX2 && randY == randY2){
		randX2 = rand() % 4;
		randY2 = rand() % 4;
	}

	//Square square2(randX2, randY2);
	Square square2(1, 0);
	square2.addToVBO(blockVertices, 1);

	squares.push_back(square);
	squares.push_back(square2);
	for(int i = 0; i < squares.size(); i++){
		std::cout << "x: " << squares[i].x;
		std::cout << "y: " << squares[i].y << std::endl;
	}
	for(int i = 0; i < 2; i++){
		//0, 2, 1
		//2, 1, 3
		blockIndices.push_back(glm::uvec3(i * GRID_SIZE, i * GRID_SIZE + 2, i * GRID_SIZE + 1));
		blockIndices.push_back(glm::uvec3(i * GRID_SIZE + 2, i * GRID_SIZE + 1, i * GRID_SIZE + 3));
	}


	for(int i = 0; i < GRID_SIZE*GRID_SIZE; i++){
		//0, 2, 1
		//2, 1, 3
		gridIndices.push_back(glm::uvec3(i * GRID_SIZE, i * GRID_SIZE + 2, i * GRID_SIZE + 1));
		gridIndices.push_back(glm::uvec3(i * GRID_SIZE + 2, i * GRID_SIZE + 1, i * GRID_SIZE + 3));
	}

	if(SDL_Init(SDL_INIT_VIDEO) > 0){
		std::cout << "SDL_Init ERROR: " << SDL_GetError() << std::endl;	
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window *window = SDL_CreateWindow("2048", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if(!window){
		std::cout << "Failed to initialize window" << std::endl;
		return -1;	
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if(!glContext){
		std::cout << "Failed to initialize GLContext" << std::endl;
		return -1;	
	}

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;	
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	//Grid binding
	glGenBuffers(1, &gridVBO);
	glGenVertexArrays(1, &gridVAO);
	glGenBuffers(1, &gridEBO);

	glBindVertexArray(gridVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
	glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(glm::vec3), gridVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, gridIndices.size() * sizeof(glm::uvec3), gridIndices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Block binding
	glGenBuffers(1, &blockVBO);
	glGenVertexArrays(1, &blockVAO);
	glGenBuffers(1, &blockEBO);

	glBindVertexArray(blockVAO);
	glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
	glBufferData(GL_ARRAY_BUFFER, blockVertices.size() * sizeof(glm::vec3), blockVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, blockEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, blockIndices.size() * sizeof(glm::uvec3), blockIndices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader gridShader("./shaders/gridShader.ver", "./shaders/gridShader.frag");
	Shader blockShader("./shaders/blockShader.ver", "./shaders/blockShader.frag");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLuint gridLength = (GLuint)gridIndices.size() * 3;
	GLuint blockLength = (GLuint)blockIndices.size() * 3;

	glEnable(GL_DEPTH_TEST);

	bool isRunning = true;
	SDL_Event event;

	while(isRunning){
		glClearColor(155.0f/255.0f, 136.0f/255.0f, 120.0f/255.0f, 1.0f);	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					isRunning = false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_UP:
							moveUp(blockVertices);
							break;
						case SDLK_DOWN:
							moveDown(blockVertices);
							break;
						case SDLK_LEFT:
							moveLeft(blockVertices);
							break;
						case SDLK_RIGHT:
							moveRight(blockVertices);
							break;
						default:
							break;	
					}
				default:
					break;	
			}	
		}
		glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, blockVertices.size()*sizeof(glm::vec3), blockVertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		//draw grid
		
		gridShader.use();
		glBindVertexArray(gridVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridEBO);
		
		glDrawElements(GL_TRIANGLES, gridLength, GL_UNSIGNED_INT, 0);
		
		//draw block
		blockShader.use();
		glBindVertexArray(blockVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, blockEBO);

		glDrawElements(GL_TRIANGLES, blockLength, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		
		SDL_GL_SwapWindow(window);
	}


	return 0;
}

