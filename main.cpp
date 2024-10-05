#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "shaders/Shader.h"
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

enum Direction {
	UP, 
	DOWN, 
	LEFT, 
	RIGHT
};

struct Square{
	public:
		int x;
		int y;
	private:
		int number = 2;

	public:
		Square(int posX, int posY)
			: x(posX), y(posY) {}

		int getNumber(){
			return this->number;	
		}

		void addSquareToGrid(std::vector<glm::vec3> &vertices, std::vector<glm::uvec3> &indices, int depth){
			vertices.push_back(glm::vec3(-1.0f + this->x*SIZE + OFFSET, 1.0f - this->y*SIZE - OFFSET, 0.99f - 0.01f * depth)); //Top left
			vertices.push_back(glm::vec3(-1.0f + this->x*SIZE + OFFSET, 1.0f - this->y*SIZE - SIZE + OFFSET, 0.99f - 0.01f * depth)); //Bottom left
			vertices.push_back(glm::vec3(-1.0f + this->x*SIZE + SIZE - OFFSET, 1.0f - this->y*SIZE - OFFSET, 0.99f - 0.01f * depth)); //Top right
			vertices.push_back(glm::vec3(-1.0f + this->x*SIZE + SIZE - OFFSET, 1.0f - this->y*SIZE - SIZE + OFFSET, 0.99f - 0.01f * depth)); //Bottom right
			
			int i;
			if(indices.size() < 1)
				i = 0;
			else
				i = indices.back().z + 1;

			indices.push_back(glm::uvec3(i, i + 2, i + 1));
			indices.push_back(glm::uvec3(i + 2, i + 1, i + 3));
		}

};

void updateGrid(std::vector<std::vector<bool>> &grid, std::vector<Square> &squares){
	for(int i = 0; i < GRID_SIZE; i++)
		for(int j = 0; j < GRID_SIZE; j++)
			grid[i][j] = false;

	for(int i = 0; i < squares.size(); i++)
		grid[squares[i].x][squares[i].y] = true;
}

void sortSquares(std::vector<Square> &vect, Direction dir){
	std::vector<Square> tmpSquares;
	int vectSize = vect.size();
	switch(dir){
		case UP:
			while(tmpSquares.size() != vectSize){
				int compareVal = GRID_SIZE;
				int pos = 0;
				for(int i = 0; i < vect.size(); i++)
					if(vect[i].y < compareVal){
						compareVal = vect[i].y;
						pos = i;	
					}
				tmpSquares.push_back(vect[pos]);
				vect.erase(vect.begin()+pos);
			}
			vect.insert(vect.begin(), tmpSquares.begin(), tmpSquares.end());
			break;	
		case DOWN:
			while(tmpSquares.size() != vectSize){
				int compareVal = 0;
				int pos = 0;
				for(int i = 0; i < vect.size(); i++)
					if(vect[i].y > compareVal){
						compareVal = vect[i].y;
						pos = i;	
					}
				tmpSquares.push_back(vect[pos]);
				vect.erase(vect.begin()+pos);
			}
			vect.insert(vect.begin(), tmpSquares.begin(), tmpSquares.end());
			break;	
		case LEFT:
			while(tmpSquares.size() != vectSize){
				int compareVal = GRID_SIZE;
				int pos = 0;
				for(int i = 0; i < vect.size(); i++)
					if(vect[i].x < compareVal){
						compareVal = vect[i].x;
						pos = i;	
					}
				tmpSquares.push_back(vect[pos]);
				vect.erase(vect.begin()+pos);
			}
			vect.insert(vect.begin(), tmpSquares.begin(), tmpSquares.end());
			break;	
		case RIGHT:
			while(tmpSquares.size() != vectSize){
				int compareVal = 0;
				int pos = 0;
				for(int i = 0; i < vect.size(); i++)
					if(vect[i].x > compareVal){
						compareVal = vect[i].x;
						pos = i;	
					}
				tmpSquares.push_back(vect[pos]);
				vect.erase(vect.begin()+pos);
			}
			vect.insert(vect.begin(), tmpSquares.begin(), tmpSquares.end());
			break;	
	}
}

//Define grid based on other squares position?
void moveUp(std::vector<Square> &squares, std::vector<std::vector<bool>> &grid){
	sortSquares(squares, UP);
	for(int i = 0; i < squares.size(); i++){
		while(squares[i].y > 0 && !grid[squares[i].x][squares[i].y-1]){
			squares[i].y--;
			updateGrid(grid, squares);
		}
	}
}

void moveDown(std::vector<Square> &squares, std::vector<std::vector<bool>> &grid){
	sortSquares(squares, DOWN);
	for(int i = 0; i < squares.size(); i++){
		while(squares[i].y < 3 && !grid[squares[i].x][squares[i].y+1]){
			squares[i].y++;
			updateGrid(grid, squares);
		}
	}
}

void moveLeft(std::vector<Square> &squares, std::vector<std::vector<bool>> &grid){
	sortSquares(squares, LEFT);
	for(int i = 0; i < squares.size(); i++){
		while(squares[i].x > 0 && !grid[squares[i].x-1][squares[i].y]){
			squares[i].x--;
			updateGrid(grid, squares);
		}
	}
}

void moveRight(std::vector<Square> &squares, std::vector<std::vector<bool>> &grid){
	sortSquares(squares, RIGHT);
	for(int i = 0; i < squares.size(); i++){
		while(squares[i].x < 3 && !grid[squares[i].x+1][squares[i].y]){
			squares[i].x++;
			updateGrid(grid, squares);
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
	std::vector<std::vector<bool>> grid;

	for(int i = 0; i < GRID_SIZE; i++){
		std::vector<bool> gridBlocks;
		for(int j = 0; j < GRID_SIZE; j++){
			gridBlocks.push_back(false);
		}
		grid.push_back(gridBlocks);
	}

	srand(time(NULL));
	
	for(int i = 0; i < GRID_SIZE; i++)
		for(int j = 0; j < GRID_SIZE; j++){
			Square square(i, j);
			square.addSquareToGrid(gridVertices, gridIndices, 0);
		}
	

	int randX = rand() % 4;
	int randY = rand() % 4;
	//Square square(randX, randY);
	Square square(1, 1);
	square.addSquareToGrid(blockVertices, blockIndices, 1);
	
	int randX2 = rand() % 4;
	int randY2 = rand() % 4;
	while(randX == randX2 && randY == randY2){
		randX2 = rand() % 4;
		randY2 = rand() % 4;
	}

	//Square square2(randX2, randY2);
	Square square2(1, 0);
	square2.addSquareToGrid(blockVertices, blockIndices, 1);

	Square square3(2, 0);
	square3.addSquareToGrid(blockVertices, blockIndices, 1);
	Square squareT(0,0);
	squares.push_back(squareT);
	squareT.addSquareToGrid(blockVertices, blockIndices, 1);

	squares.push_back(square);
	squares.push_back(square2);
	squares.push_back(square3);

	for(int i = 0; i < squares.size(); i++){
		std::cout << "x: " << squares[i].x;
		std::cout << "y: " << squares[i].y << std::endl;
	}

	updateGrid(grid, squares);

	for(int i = 0; i < GRID_SIZE; i++)
		for(int j = 0; j < GRID_SIZE; j++){
			std::cout << "x: " << i << " ";
			std::cout << "y: " << j << std::endl;
			std::cout << grid[i][j] << std::endl;
		}



	/*for(int i = 0; i < GRID_SIZE*GRID_SIZE; i++){
		//0, 2, 1
		//2, 1, 3
		gridIndices.push_back(glm::uvec3(i * GRID_SIZE, i * GRID_SIZE + 2, i * GRID_SIZE + 1));
		gridIndices.push_back(glm::uvec3(i * GRID_SIZE + 2, i * GRID_SIZE + 1, i * GRID_SIZE + 3));
	}*/

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
	//glBufferData(GL_ARRAY_BUFFER, blockVertices.size() * sizeof(glm::vec3), blockVertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, GRID_SIZE*GRID_SIZE*4 * sizeof(glm::vec3), blockVertices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, blockEBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, blockIndices.size() * sizeof(glm::uvec3), blockIndices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, GRID_SIZE*GRID_SIZE*2 * sizeof(glm::uvec3), blockIndices.data(), GL_DYNAMIC_DRAW);

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
		for(int i = 0; i < squares.size(); i++)
			std::cout << squares[i].x << " " << squares[i].y << std::endl;

		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					isRunning = false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_UP:
							moveUp(squares, grid);
							blockVertices.clear();
							blockIndices.clear();
							for(int i = 0; i < squares.size(); i++)
								squares[i].addSquareToGrid(blockVertices, blockIndices, 1);
							break;
						case SDLK_DOWN:
							moveDown(squares, grid);
							blockVertices.clear();
							blockIndices.clear();
							for(int i = 0; i < squares.size(); i++)
								squares[i].addSquareToGrid(blockVertices, blockIndices, 1);
							break;
						case SDLK_LEFT:
							moveLeft(squares, grid);
							blockVertices.clear();
							blockIndices.clear();
							for(int i = 0; i < squares.size(); i++)
								squares[i].addSquareToGrid(blockVertices, blockIndices, 1);
							break;
						case SDLK_RIGHT:
							moveRight(squares, grid);
							blockVertices.clear();
							blockIndices.clear();
							for(int i = 0; i < squares.size(); i++)
								squares[i].addSquareToGrid(blockVertices, blockIndices, 1);
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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, blockEBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, blockIndices.size()*sizeof(glm::uvec3), blockIndices.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		//draw grid
		
		gridShader.use();
		glBindVertexArray(gridVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridEBO);
		
		glDrawElements(GL_TRIANGLES, gridLength, GL_UNSIGNED_INT, 0);
		
		//draw block
		blockLength = (GLuint)blockIndices.size() * 3;
		blockShader.use();
		glBindVertexArray(blockVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, blockEBO);

		glDrawElements(GL_TRIANGLES, blockLength, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		
		SDL_GL_SwapWindow(window);
	}


	return 0;
}

