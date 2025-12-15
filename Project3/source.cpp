#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include "Constants.h"
#include "TextureHandler.h"

#include "Light.h"
#include "Sphere.h"
#include "Map.h"

#include "CollisionHandler.h"

#include <GL/freeglut.h>
#include <FreeImage.h>

using namespace std;



const int FPS = 60;
int sTime = 0;
int eTime = 0;
int stateTime = 0;
int score = 0;
int coin = 0;
bool init = false;
ostringstream oss;

multimap<string,int> Ranking; //
string userName;

Light light(BOUNDARY_X, BOUNDARY_Y, BOUNDARY_X / 2, GL_LIGHT0);

PacMan pacman(BLOCK_SIZE / 2.0f, 20, 20, false, 3);
Ghost blinky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::CHASE,0);
Ghost pinky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::CHASE,0);
Ghost inky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::CHASE,0);
Ghost clyde(BLOCK_SIZE / 2.0f, 20, 20, Ghost::CHASE,0);

Map map_0;
CollisionHandler colHandler;

GLuint scoreboardTextureID;
GLuint nameScreenTextureID;
GLuint helpScreenTextureID;
GLuint startScreenTextureID;
GLuint gameOverTextureID;
GLuint gameWinTextureID;
GLubyte* textureData;
int imageWidth, imageHeight;

enum GameState {  //
	START_SCREEN,
	NAME_SCREEN,
	HELP,
	SCOREBOARD,
	GAME_RUNNING,
	GAME_OVER,
	GAME_WIN
};

GameState gameState = START_SCREEN;  //


bool blinkState = true;

void toggleBlinkState(int value) {
	blinkState = !blinkState; 
	glutTimerFunc(500, toggleBlinkState, 0); 
	glutPostRedisplay(); 
}


void initTextures() {

	std::vector<std::string> blinkyTextures = {
		"blinky_0.png",
		"blinky_1.png",
		"ghost_frightened.png",
		"ghost_eaten.png"
	};
	std::vector<std::string> pinkyTextures = {
		"pinky_0.png",
		"pinky_1.png",
		"ghost_frightened.png",
		"ghost_eaten.png"
	};
	std::vector<std::string> inkyTextures = {
		"inky_0.png",
		"inky_1.png",
		"ghost_frightened.png",
		"ghost_eaten.png"
	};
	std::vector<std::string> clydeTextures = {
		"clyde_0.png",
		"clyde_1.png",
		"ghost_frightened.png",
		"ghost_eaten.png"
	};
	std::vector<std::string> pacmanTextures = {
		"pacman_up_0.png",
		"pacman_up_1.png",
		"pacman_up_2.png",
		"pacman_up_3.png",
		"pacman_left_0.png",
		"pacman_left_1.png",
		"pacman_left_2.png",
		"pacman_left_3.png",
		"pacman_down_0.png",
		"pacman_down_1.png",
		"pacman_down_2.png",
		"pacman_down_3.png",
		"pacman_right_0.png",
		"pacman_right_1.png",
		"pacman_right_2.png",
		"pacman_right_3.png"
	};

	// Ghost 객체별 텍스처 로드
	blinky.loadTextures(blinkyTextures);
	pinky.loadTextures(pinkyTextures);
	inky.loadTextures(inkyTextures);
	clyde.loadTextures(clydeTextures);
	pacman.loadTextures(pacmanTextures);

	scoreboardTextureID = TextureHandler::createTexture("scoreboard.png");
	nameScreenTextureID = TextureHandler::createTexture("name_screen.png");
	helpScreenTextureID = TextureHandler::createTexture("help_screen.png");
	startScreenTextureID = TextureHandler::createTexture("start_screen.png");
	gameOverTextureID = TextureHandler::createTexture("game_over.png");
	gameWinTextureID = TextureHandler::createTexture("game_win.png");
}



void initialize() {
	// Spell
	stateTime = 0;


	// Light
	light.setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
	light.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	light.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);

	Material mtl[5];	// basic material

	// PacMan
	mtl[0].setEmission(0.0f, 0.0f, 0.0f, 1.0f); // 약한 노란빛 방출
	mtl[0].setAmbient(0.6f, 0.6f, 0.0f, 1.0f); // 부드러운 노란색
	mtl[0].setDiffuse(0.9f, 0.9f, 0.0f, 1.0f); // 선명한 노란색
	mtl[0].setSpecular(1.0f, 1.0f, 0.8f, 1.0f); // 약간의 노란빛 반사
	mtl[0].setShininess(50.0f); // 적당한 반짝임

	pacman.setIndexPosition(1, 4);
	pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	pacman.setMTL(mtl[0]);
	pacman.setCurrentDirection(Sphere::NONE);
	pacman.setNextDirection(Sphere::NONE);

	// Ghost: blinky
	mtl[1].setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtl[1].setAmbient(0.6f, 0.0f, 0.0f, 1.0f);
	mtl[1].setDiffuse(0.8f, 0.0f, 0.0f, 1.0f);
	mtl[1].setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtl[1].setShininess(30.0f);

	blinky.setIndexPosition(12, 18);
	blinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	blinky.setMTL(mtl[1]);
	blinky.setCurrentDirection(Sphere::NONE);
	blinky.setNextDirection(Sphere::NONE);

	// Ghost: pinky
	mtl[2].setEmission(0.3f, 0.1f, 0.3f, 1.0f);
	mtl[2].setAmbient(0.8f, 0.4f, 0.8f, 1.0f);
	mtl[2].setDiffuse(1.0f, 0.6f, 1.0f, 1.0f);
	mtl[2].setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtl[2].setShininess(50.0f);

	pinky.setIndexPosition(13, 18);
	pinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	pinky.setMTL(mtl[2]);
	pinky.setCurrentDirection(Sphere::NONE);
	pinky.setNextDirection(Sphere::NONE);

	// Ghost: inky
	mtl[3].setEmission(0.1f, 0.3f, 0.3f, 1.0f);
	mtl[3].setAmbient(0.4f, 0.8f, 0.8f, 1.0f);
	mtl[3].setDiffuse(0.6f, 1.0f, 0.8f, 1.0f);
	mtl[3].setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtl[3].setShininess(50.0f);

	inky.setIndexPosition(14, 18);
	inky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	inky.setMTL(mtl[3]);
	inky.setCurrentDirection(Sphere::NONE);
	inky.setNextDirection(Sphere::NONE);

	// Ghost: clyde
	mtl[4].setEmission(0.3f, 0.2f, 0.0f, 1.0f);
	mtl[4].setAmbient(0.8f, 0.4f, 0.1f, 1.0f);
	mtl[4].setDiffuse(1.0f, 0.6f, 0.2f, 1.0f);
	mtl[4].setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtl[4].setShininess(50.0f);

	clyde.setIndexPosition(15, 18);
	clyde.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	clyde.setMTL(mtl[4]);
	clyde.setCurrentDirection(Sphere::NONE);
	clyde.setNextDirection(Sphere::NONE);

	score = 0;
	stateTime = 0;
	// Map
	map_0.createMap();
	coin = map_0.coin_number();
}

void updateDirectionOfPacMan() {
	int xIdx = pacman.getXIndex();
	int yIdx = pacman.getYIndex();

	const Block& lBlock = map_0.getBlock(xIdx - 1, yIdx);	// left
	const Block& rBlock = map_0.getBlock(xIdx + 1, yIdx);	// right
	const Block& tBlock = map_0.getBlock(xIdx, yIdx + 1);	// top
	const Block& bBlock = map_0.getBlock(xIdx, yIdx - 1);	// bottom

	// update direction
	Sphere::DIRECTION nextDir = pacman.getNextDirection();
	switch (nextDir)
	{
	case Sphere::NONE:
		break;
	case Sphere::LEFT:
		if (lBlock.isPassable())
			pacman.updateDirection();
		break;
	case Sphere::UP:
		if (tBlock.isPassable())
			pacman.updateDirection();
		break;
	case Sphere::RIGHT:
		if (rBlock.isPassable())
			pacman.updateDirection();
		break;
	case Sphere::DOWN:
		if (bBlock.isPassable()) {
			if (!((pacman.getYIndex() == 21 && pacman.getXIndex() == 13) || (pacman.getYIndex() == 21 && pacman.getXIndex() == 14))) {
				pacman.updateDirection();
			}
		}
		break;
	default:
		break;
	}
}

void updateDirectionOfGhost(Ghost& ghost, int targetX, int targetY) {
	/* Implement */

	int idx[2] = { ghost.getXIndex(), ghost.getYIndex() };
	
	int lIdx[2] = { idx[0] - 1,idx[1] };
	int tIdx[2] = { idx[0], idx[1] + 1 };
	int rIdx[2] = { idx[0] + 1, idx[1] };
	int bIdx[2] = { idx[0], idx[1] - 1 };

	const Block& lBlock = map_0.getBlock(lIdx[0], lIdx[1]);	// left
	const Block& rBlock = map_0.getBlock(rIdx[0], rIdx[1]);	// right
	const Block& tBlock = map_0.getBlock(tIdx[0], tIdx[1]);	// top
	const Block& bBlock = map_0.getBlock(bIdx[0], bIdx[1]);	// bottom

	Sphere::DIRECTION currDir = ghost.getCurrentDirection();
	Sphere::DIRECTION newDir1 = Sphere::DIRECTION::NONE;
	Sphere::DIRECTION newDir2 = Sphere::DIRECTION::NONE;
	int minIdxDist = INT_MAX;
	int maxIdxDist = 0;

	if (lBlock.isPassable() && currDir != Sphere::RIGHT) {
		if (abs(lIdx[0] - targetX) + abs(lIdx[1] - targetY) < minIdxDist) {
			minIdxDist = abs(lIdx[0] - targetX) + abs(lIdx[1] - targetY);
			newDir1 = Sphere::DIRECTION::LEFT;
		}
		if (abs(lIdx[0] - targetX) + abs(lIdx[1] - targetY) > maxIdxDist) {
			maxIdxDist = abs(lIdx[0] - targetX) + abs(lIdx[1] - targetY);
			newDir2 = Sphere::DIRECTION::LEFT;
		}
	}
	if (tBlock.isPassable() && currDir != Sphere::DOWN) {
		if (abs(tIdx[0] - targetX) + abs(tIdx[1] - targetY) < minIdxDist) {
			minIdxDist = abs(tIdx[0] - targetX) + abs(tIdx[1] - targetY);
			newDir1 = Sphere::DIRECTION::UP;
		}
		if (abs(tIdx[0] - targetX) + abs(tIdx[1] - targetY) > maxIdxDist) {
			maxIdxDist = abs(tIdx[0] - targetX) + abs(tIdx[1] - targetY);
			newDir2 = Sphere::DIRECTION::UP;
		}
	}
	if (rBlock.isPassable() && currDir != Sphere::LEFT) {
		if (abs(rIdx[0] - targetX) + abs(rIdx[1] - targetY) < minIdxDist) {
			minIdxDist = abs(rIdx[0] - targetX) + abs(rIdx[1] - targetY);
			newDir1 = Sphere::DIRECTION::RIGHT;
		}
		if (abs(rIdx[0] - targetX) + abs(rIdx[1] - targetY) > maxIdxDist) {
			maxIdxDist = abs(rIdx[0] - targetX) + abs(rIdx[1] - targetY);
			newDir2 = Sphere::DIRECTION::RIGHT;
		}
	}
	if (bBlock.isPassable() && currDir != Sphere::UP) {
		if (abs(bIdx[0] - targetX) + abs(bIdx[1] - targetY) < minIdxDist) {
			minIdxDist = abs(bIdx[0] - targetX) + abs(bIdx[1] - targetY);
			newDir1 = Sphere::DIRECTION::DOWN;
		}
		if (abs(bIdx[0] - targetX) + abs(bIdx[1] - targetY) > maxIdxDist) {
			maxIdxDist = abs(bIdx[0] - targetX) + abs(bIdx[1] - targetY);
			newDir2 = Sphere::DIRECTION::DOWN;
		}
	}
	if (ghost.getState() == Ghost::CHASE || ghost.getState() == Ghost::SCATTER||ghost.getState()==Ghost::EATEN) {
		ghost.setNextDirection(newDir1);
	}
	else if (ghost.getState() == Ghost::FRIGHTENED) {
		ghost.setNextDirection(newDir2);
	}
	//cout << "nextDir" << blinky.getNextDirection() << endl;
	ghost.updateDirection();
	//cout << "currDir" << blinky.getCurrentDirection() << endl;
}

void updatePacMan() {
	// update PacMan
	bool bNoVel = (pacman.getVelocity()[0] == 0.0f) && (pacman.getVelocity()[1] == 0.0f) && (pacman.getVelocity()[2] == 0.0f);
	bool bNoDir = pacman.getCurrentDirection() == Sphere::DIRECTION::NONE;
	bool bIdxPosUpdated = pacman.isIndexPositionUpdated();
	//cout << pacman.getCurrentDirection() << " next : " << pacman.getNextDirection() << endl;
	if (bNoVel || bNoDir || bIdxPosUpdated) {
		updateDirectionOfPacMan();
		switch (pacman.getCurrentDirection()) {
		case Sphere::DIRECTION::UP:
			pacman.setVelocity(0.0f, 1.0f, 0.0f);
			break;
		case Sphere::DIRECTION::RIGHT:
			pacman.setVelocity(1.0f, 0.0f, 0.0f);
			break;
		case Sphere::DIRECTION::DOWN:
			pacman.setVelocity(0.0f, -1.0f, 0.0f);
			break;
		case Sphere::DIRECTION::LEFT:
			pacman.setVelocity(-1.0f, 0.0f, 0.0f);
			break;
		default:
			break;
		}
		colHandler(pacman, map_0);
	}
	if (pacman.getghost_time() > 5*FPS) {
		pacman.setVelocity(1.5*pacman.getVelocity());
	}
	else if (pacman.getghost_time() > 3 * FPS) {
		pacman.setVelocity(1.3 * pacman.getVelocity());
	}
	else if (pacman.getghost_time() > 0) {
		pacman.setVelocity(1.1 * pacman.getVelocity());
	}
	pacman.move();
}


void updateGhost() {
	/* Implement */
	bool bNoDir[4] = { blinky.getCurrentDirection() == Sphere::DIRECTION::NONE, pinky.getCurrentDirection() == Sphere::DIRECTION::NONE, inky.getCurrentDirection() == Sphere::DIRECTION::NONE, clyde.getCurrentDirection() == Sphere::DIRECTION::NONE };
	bool bIdxPosUpdated[4] = { blinky.isIndexPositionUpdated(), pinky.isIndexPositionUpdated(),inky.isIndexPositionUpdated(),clyde.isIndexPositionUpdated() };
	int targetX[4] = { 0,0,0,0 };
	int targetY[4] = { 0,0,0,0 };
	//cout << "bNoDir : " << bNoDir << "bIdxPosUpdated : " << bIdxPosUpdated << endl;
	if (bNoDir[0] || bIdxPosUpdated[0]) {
		//cout << targetX << " " << targetY << endl;
		switch (blinky.getState()) {
		case Ghost::CHASE:
			if (blinky.getXIndex() > 10 && blinky.getXIndex() < 17 && blinky.getYIndex() > 16 && blinky.getYIndex() < 21) {
				targetX[0] = 13; targetY[0] = 21;
			}
			else {
				targetX[0] = pacman.getXIndex(); targetY[0] = pacman.getYIndex();
			}
			break;
		case Ghost::SCATTER:
			if (blinky.getXIndex() > 10 && blinky.getXIndex() < 17 && blinky.getYIndex() > 16 && blinky.getYIndex() < 21) {
				targetX[0] = 13; targetY[0] = 21;
			}
			else {
				targetX[0] = 1; targetY[0] = 31;
			}
			break;
		case Ghost::FRIGHTENED:
			targetX[0] = pacman.getXIndex(); targetY[0] = pacman.getYIndex();
			break;
		case Ghost::EATEN:
			targetX[0] = 13; targetY[0] = 18;
			break;
		}
		//cout << targetX[0] << " " << targetY[0] << " " << pacman.getlife() << endl;
		updateDirectionOfGhost(blinky, targetX[0], targetY[0]);
	}
	if (bNoDir[1] || bIdxPosUpdated[1]) {
		//cout << targetX << " " << targetY << endl;
		switch (pinky.getState()) {
		case Ghost::CHASE:
			if (pinky.getXIndex() > 10 && pinky.getXIndex() < 17 && pinky.getYIndex() > 16 && pinky.getYIndex() < 21) {
				targetX[1] = 14; targetY[1] = 21;
			}
			else {
				targetX[1] = pacman.getXIndex() + (int)pacman.getVelocity()[0] * 4; targetY[1] = pacman.getYIndex() + (int)pacman.getVelocity()[1] * 4;
			}
			break;
		case Ghost::SCATTER:
			if (pinky.getXIndex() > 10 && pinky.getXIndex() < 17 && pinky.getYIndex() > 16 && pinky.getYIndex() < 21) {
				targetX[1] = 14; targetY[1] = 21;
			}
			else {
				targetX[1] = 1; targetY[1] = 4;
			}
			break;
		case Ghost::FRIGHTENED:
			targetX[1] = pacman.getXIndex(); targetY[1] = pacman.getYIndex();
			break;
		case Ghost::EATEN:
			targetX[1] = 14; targetY[1] = 18;
			break;
		}
		
		updateDirectionOfGhost(pinky, targetX[1], targetY[1]);
	}
	if (bNoDir[2] || bIdxPosUpdated[2]) {
		//cout << targetX << " " << targetY << endl;
		switch (inky.getState()) {
		case Ghost::CHASE:
			if (inky.getXIndex() > 10 && inky.getXIndex() < 17 && inky.getYIndex() > 16 && inky.getYIndex() < 21) {
				targetX[2] = 13; targetY[2] = 21;
			}
			else {
				targetX[2] = 2 * (pacman.getXIndex() + (int)pacman.getVelocity()[0] * 2) - blinky.getXIndex(); targetY[2] = 2 * (pacman.getYIndex() + (int)pacman.getVelocity()[1] * 2) - blinky.getYIndex();
			}
			break;
		case Ghost::SCATTER:
			if (inky.getXIndex() > 10 && inky.getXIndex() < 17 && inky.getYIndex() > 16 && inky.getYIndex() < 21) {
				targetX[2] = 13; targetY[2] = 21;
			}
			targetX[2] = 26; targetY[2] = 31;
			break;
		case Ghost::FRIGHTENED:
			targetX[2] = pacman.getXIndex(); targetY[2] = pacman.getYIndex();
			break;
		case Ghost::EATEN:
			targetX[2] = 13; targetY[2] = 18;
			break;
		}
		
		
		//cout << "targetX : " << targetX << " " << "targetY : " << targetY << endl;
		updateDirectionOfGhost(inky, targetX[2], targetY[2]);
	}
	if (bNoDir[3] || bIdxPosUpdated[3]) {
		//cout << targetX << " " << targetY << endl;
		switch (clyde.getState()) {
		case Ghost::CHASE:
			if (clyde.getXIndex() > 10 && clyde.getXIndex() < 17 && clyde.getYIndex() > 15 && clyde.getYIndex() < 21) {
				targetX[3] = 14; targetY[3] = 22;
			}
			else {
				if (dotProduct(pacman.getCenter() - clyde.getCenter(), pacman.getCenter() - clyde.getCenter()) > (8 * BLOCK_SIZE) * (8 * BLOCK_SIZE)) {
					targetX[3] = pacman.getXIndex(); targetY[3] = pacman.getYIndex();
				}
				else {
					targetX[3] = 26; targetY[3] = 4;
				}
			}
			break;
		case Ghost::SCATTER:
			if (clyde.getXIndex() > 10 && clyde.getXIndex() < 17 && clyde.getYIndex() > 15 && clyde.getYIndex() < 21) {
				targetX[3] = 14; targetY[3] = 22;
			}
			else {
				targetX[3] = 26; targetY[3] = 4;
			}
			break;
		case Ghost::FRIGHTENED:
			targetX[3] = pacman.getXIndex(); targetY[3] = pacman.getYIndex();
			break;
		case Ghost::EATEN:
			targetX[3] = 14; targetY[3] = 18;
			break;
		}
		//cout << targetX[3] << " " << targetY[3] << endl;
		//cout << clyde.getXIndex() << " " << clyde.getYIndex() << endl;
		updateDirectionOfGhost(clyde, targetX[3], targetY[3]);
		
	}
	//cout << "blinky's currentDirection : " << blinky.getCurrentDirection() << endl;
	blinky.move();
	pinky.move();
	inky.move();
	clyde.move();
}

void updatestateofghost(Ghost& ghost, int timer) {
	switch (ghost.getCurrentDirection()) {
	case Sphere::DIRECTION::UP:
		ghost.setVelocity(0.0f, 1.0f, 0.0f);
		break;
	case Sphere::DIRECTION::RIGHT:
		ghost.setVelocity(1.0f, 0.0f, 0.0f);
		break;
	case Sphere::DIRECTION::DOWN:
		ghost.setVelocity(0.0f, -1.0f, 0.0f);
		break;
	case Sphere::DIRECTION::LEFT:
		ghost.setVelocity(-1.0f, 0.0f, 0.0f);
		break;
	default:
		break;
	}
	if (ghost.getState() == Ghost::EATEN) {
		ghost.setVelocity(3*ghost.getVelocity());
		if ((ghost.getXIndex() == 13 || ghost.getXIndex() == 14) && (ghost.getYIndex() == 18)) {
			if (timer % (1000 * 7 * 2) >= 0 && timer % (1000 * 7 * 2) < 7000) {
				ghost.setState(Ghost::CHASE);
			}
			else
				ghost.setState(Ghost::SCATTER);
		}
	}
	else if (ghost.getfright_time()>0) {
		ghost.setState(Ghost::FRIGHTENED);
		ghost.setVelocity(0.5 * ghost.getVelocity());
	}
	else {
		if (timer % (1000 * 7 * 2) >= 0 && timer % (1000 * 7 * 2) < 7000) {
			ghost.setState(Ghost::CHASE);
			ghost.setVelocity(1.0 * ghost.getVelocity());
		}
		else {
			ghost.setState(Ghost::SCATTER);
			ghost.setVelocity(1.5 * ghost.getVelocity());
		}
	}
}

void updatecoin(Block & block) {
	if (block.isCoin()) {
		block.setCoin(false);
		score += 100;
		coin--;
	}
}
void updateitem(Block& block) {
	if (block.isItem()) {
		block.setItem(false);
		for (int i = 0; i < 4; i++) {
			blinky.setfright_time(FPS * 7);
			pinky.setfright_time(FPS * 7);
			inky.setfright_time(FPS * 7);
			clyde.setfright_time(FPS * 7);
		}
		blinky.setState(Ghost::FRIGHTENED);
		pinky.setState(Ghost::FRIGHTENED);
		inky.setState(Ghost::FRIGHTENED);
		clyde.setState(Ghost::FRIGHTENED);
	}
}

void updateghostitem(Block& block) {
	if (block.isGhostItem()) {
		block.setGhostItem(false);
		pacman.setghost_time(FPS * 7);
	}
}

void updateheartitem(Block& block) {
	if (block.isHeartItem()) {
		block.setHeartItem(false);
		pacman.setlife(pacman.getlife() + 1);
	}
}

void initialize_position() {
	if (init) {
		init = false;
		pacman.setIndexPosition(1, 4);
		pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
		blinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
		pinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
		inky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
		clyde.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
		pacman.setCurrentDirection(Sphere::NONE);
		pacman.setNextDirection(Sphere::NONE);
		blinky.setCurrentDirection(Sphere::NONE);
		blinky.setNextDirection(Sphere::NONE);
		pinky.setCurrentDirection(Sphere::NONE);
		pinky.setNextDirection(Sphere::NONE);
		inky.setCurrentDirection(Sphere::NONE);
		inky.setNextDirection(Sphere::NONE);
		clyde.setCurrentDirection(Sphere::NONE);
		clyde.setNextDirection(Sphere::NONE);
		blinky.setIndexPosition(12, 18);
		pinky.setIndexPosition(13, 18);
		inky.setIndexPosition(14, 18);
		clyde.setIndexPosition(15, 18);
		pacman.setghost_time(0);
	}
}

void idle() {
	if (gameState == GAME_RUNNING) {
		float spf = 1000.0f / FPS;
		eTime = glutGet(GLUT_ELAPSED_TIME);
		stateTime = glutGet(GLUT_ELAPSED_TIME);
		// cout << 1000000000 / stateTime << endl;
		if (eTime - sTime > spf) {

			if (pacman.getlife() <= 0) {
				gameState = GAME_OVER;
				Ranking.insert(pair<string, int>(userName, score));
				userName.clear();
				glutPostRedisplay();
			}

			if (coin <= 0) {
				score += 500000000 / stateTime;
				score += pacman.getlife()*5000;
				gameState = GAME_WIN;
				Ranking.insert(pair<string, int>(userName, score));
				userName.clear();
				glutPostRedisplay();
			}

			//cout << blinky.getfright_time() << " " << pinky.getfright_time() << " " << inky.getfright_time() << "  " << clyde.getfright_time()<< endl;
			if (blinky.getfright_time() > 0) {
				blinky.setfright_time(blinky.getfright_time() - 1);
			}
			if (pinky.getfright_time() > 0) {
				pinky.setfright_time(pinky.getfright_time() - 1);
			}
			if (inky.getfright_time() > 0) {
				inky.setfright_time(inky.getfright_time() - 1);
			}
			if (clyde.getfright_time() > 0) {
				clyde.setfright_time(clyde.getfright_time() - 1);
			}
			if (pacman.getghost_time() > 0) {
				pacman.setghost_time(pacman.getghost_time() - 1);
			}
			if (collision_counter > 0) {
				collision_counter -= 1;
			}

			pacman.settime(stateTime);
			blinky.settime(stateTime);
			pinky.settime(stateTime);
			inky.settime(stateTime);
			clyde.settime(stateTime);

			updatestateofghost(blinky, eTime);
			updatestateofghost(pinky, eTime);
			updatestateofghost(inky, eTime);
			updatestateofghost(clyde, eTime);
			updatePacMan();
			updateGhost();
			updatecoin(map_0.getBlock(pacman.getXIndex(), pacman.getYIndex()));
			updateitem(map_0.getBlock(pacman.getXIndex(), pacman.getYIndex()));
			updateheartitem(map_0.getBlock(pacman.getXIndex(), pacman.getYIndex()));
			updateghostitem(map_0.getBlock(pacman.getXIndex(), pacman.getYIndex()));
			colHandler(pacman, blinky);
			colHandler(pacman, pinky);
			colHandler(pacman, inky);
			colHandler(pacman, clyde);
			initialize_position();

			sTime = eTime;
			glutPostRedisplay();
		}
	}
	
	//cout << pacman.getEmission()[0];
}


void displayCharacters(void* font, string str, float x, float y) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(x, y);
	for (int i = 0; i < str.size(); i++)
		glutBitmapCharacter(font, str[i]);
}

void display_START_SCREEN() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);
	//gluOrtho2D(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	TextureHandler::drawTexture(startScreenTextureID, -BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

	glutSwapBuffers();
}

void display_HELP() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, WINDOW_W, WINDOW_H);

	TextureHandler::drawTexture(helpScreenTextureID, -BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

	const char* backMessage = "Press Enter to go back";
	float backMessageX = 0 * BOUNDARY_X;
	float backMessageY = -BOUNDARY_Y + 20;
	if (blinkState) {
		displayCharacters(GLUT_BITMAP_HELVETICA_18, backMessage, backMessageX, backMessageY);
	}

	glutSwapBuffers();
}

void display_SCOREBOARD() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	TextureHandler::drawTexture(scoreboardTextureID, -BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

	const char* title = "Scoreboard";
	float titleX = -0.2f * BOUNDARY_X;
	float titleY = BOUNDARY_Y - 80;

	displayCharacters(GLUT_BITMAP_HELVETICA_18, title, titleX, titleY);

	float startX = -0.7 * BOUNDARY_X + 10;
	float startY = BOUNDARY_Y - 140;
	float lineHeight = 20;

	std::vector<std::pair<std::string, int>> sortedRanking(Ranking.begin(), Ranking.end());
	std::sort(sortedRanking.begin(), sortedRanking.end(), [](const auto& a, const auto& b) {
		return a.second > b.second; 
		});

	
	int rank = 1;
	for (const auto& entry : sortedRanking) {
		if (rank > 10) break; 
		std::stringstream line;
		line << rank << ". " << entry.first << " - " << entry.second;

		displayCharacters(GLUT_BITMAP_HELVETICA_18, line.str(), startX, startY);
		startY -= lineHeight;
		rank++;
	}

	const char* backMessage = "Press Enter to go back";
	float backMessageX = -0 * BOUNDARY_X;
	float backMessageY = -BOUNDARY_Y + 20;
	if (blinkState) {
		displayCharacters(GLUT_BITMAP_HELVETICA_18, backMessage, backMessageX, backMessageY);
	}

	glutSwapBuffers();
}


void display_NAME_SCREEN() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	TextureHandler::drawTexture(nameScreenTextureID, -BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

	const char* labelText = "Input your ID(english): ";
	float startX = - 0.7 * BOUNDARY_X + 10;
	float startY = BOUNDARY_Y - 140;

	displayCharacters(GLUT_BITMAP_HELVETICA_18, labelText, startX, startY);

	float textWidth = 0.0f;
	for (const char* c = labelText; *c != '\0'; ++c) {
		textWidth += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c) * (2.0f * BOUNDARY_X) / WINDOW_W;
	}

	displayCharacters(GLUT_BITMAP_HELVETICA_18, userName.c_str(), startX + textWidth, startY);


	const char* backMessage = "Press Enter to start!";
	float backMessageX = 0 * BOUNDARY_X;
	float backMessageY = -BOUNDARY_Y + 20;
	if (blinkState) {
		displayCharacters(GLUT_BITMAP_HELVETICA_18, backMessage, backMessageX, backMessageY);
	}

	glutSwapBuffers();
}

void display_GAME_RUNNING() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draw 2D
	map_0.draw();
	
	//oss << "current state (Chase), position: (" << blinky.getXIndex() << ", " << blinky.getYIndex() << ")";
	oss.str("");                  // Clear previous contents
	oss.clear();                  // Clear error flags
	oss << "Pacman's life: " << pacman.getlife()<< "       " << "Score : " << score;
	displayCharacters(GLUT_BITMAP_HELVETICA_18, oss.str().c_str(), -BOUNDARY_X * 0.9f, BOUNDARY_Y * 0.9f);

	// Draw 3D
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(light.getID());
	light.draw();

	pacman.draw();
	blinky.draw();
	pinky.draw();
	inky.draw();
	clyde.draw();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(light.getID());

	glutSwapBuffers();
}

void display_GAME_OVER() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	TextureHandler::drawTexture(gameOverTextureID, -BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

	const char* backMessage = "Press Enter to Continue";
	float backMessageX = 0 * BOUNDARY_X;
	float backMessageY = -BOUNDARY_Y + 20;
	if (blinkState) {
		displayCharacters(GLUT_BITMAP_HELVETICA_18, backMessage, backMessageX, backMessageY);
	}

	glutSwapBuffers();
}

void display_GAME_WIN() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	TextureHandler::drawTexture(gameWinTextureID, -BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

	const char* backMessage = "Press Enter to Continue";
	float backMessageX = 0 * BOUNDARY_X;
	float backMessageY = -BOUNDARY_Y + 20;
	if (blinkState) {
		displayCharacters(GLUT_BITMAP_HELVETICA_18, backMessage, backMessageX, backMessageY);
	}

	glutSwapBuffers();
}


void display() {
	switch (gameState) {
	case START_SCREEN:
		display_START_SCREEN();
		break;
	case NAME_SCREEN:
		display_NAME_SCREEN();
		break;
	case HELP:
		display_HELP();
		break;
	case SCOREBOARD:
		display_SCOREBOARD();
		break;
	case GAME_RUNNING:
		display_GAME_RUNNING();
		break;
	case GAME_OVER:
		display_GAME_OVER();
		break;
	case GAME_WIN:
		display_GAME_WIN();
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void specialKeyDown(int key, int x, int y) {
	if (gameState == GAME_RUNNING) { // GAME_RUNNING 일 때만
		switch (key) {
		case GLUT_KEY_LEFT:
			pacman.setNextDirection(Sphere::DIRECTION::LEFT);
			break;
		case GLUT_KEY_UP:
			pacman.setNextDirection(Sphere::DIRECTION::UP);
			break;
		case GLUT_KEY_RIGHT:
			pacman.setNextDirection(Sphere::DIRECTION::RIGHT);
			break;
		case GLUT_KEY_DOWN:
			pacman.setNextDirection(Sphere::DIRECTION::DOWN);
			break;
		default:
			break;
		}
	}

	glutPostRedisplay();
}


void keyboardDown(unsigned char key, int x, int y) {
	switch (gameState) {
	case START_SCREEN:
		switch (key) {
		case 13: // Enter
			gameState = NAME_SCREEN;
			break;
		case 104: //h
			gameState = HELP;
			break;
		case 115: //s
			gameState = SCOREBOARD;
			break;
		default:
			break;
		}
		break;
	case NAME_SCREEN:
		if (key == 8) {  // Backspace
			if (!userName.empty()) {
				userName.pop_back();
			}
		}
		else if (key == 13 && !userName.empty()) {  // Enter
			gameState = GAME_RUNNING;
			pacman.setlife(3);
			initialize();
		}
		else if (isalpha(key)) {
			if (userName.size() < 20) {
				userName.push_back(key);
			}
		}
		break;
	case HELP:
		if (key == 13) {  // Enter
			gameState = START_SCREEN;
		}
		break;
	case SCOREBOARD:
		if (key == 13) {  // Enter
			gameState = START_SCREEN;
		}
		break;
	case GAME_OVER:
		if (key == 13) {  // Enter
			gameState = SCOREBOARD;
		}
		break;
	case GAME_WIN:
		if (key == 13) {  // Enter
			gameState = SCOREBOARD;
		}
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv) {
	// init GLUT and create Window

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutCreateWindow("Move Pac-Man");

	initTextures();

	initialize();

	// register callbacks
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutSpecialFunc(specialKeyDown);
	glutKeyboardFunc(keyboardDown); //

	glutTimerFunc(0, toggleBlinkState, 0);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
