#include"Map.h"
#include<GL\freeglut.h>
#include<iostream>
using namespace std;

Block::Block() {
    width = 0; height = 0, bPassable = false, type = blank, coin = false, item = false;
}

Block::Block(float x, float y, float z, float w, float h, Walltype t, bool c, bool i) {
    center[0] = x; center[1] = y; center[2] = z; width = w; height = h, bPassable = false, type = t, coin = c, item = i;
}

void Block::setWidth(float w) {
	width = w;
}
void Block::setHeight(float h) {
	height = h;
}
void Block::setPassable(bool v) {
	bPassable = v;
}
void Block::setCoin(bool c) {
    coin = c;
}
void Block::setItem(bool i) {
    item = i;
}
void Block::setHeartItem(bool h) {
    heartitem = h;
}
void Block::setGhostItem(bool g) {
    ghostitem = g;
}
void Block::setType(Walltype t) {
	type = t;
}
float Block::getWidth() const {
	return width;
}
float Block::getHeight() const {
	return height;
}
bool Block::isPassable() const {
	return bPassable;
}
bool Block::isCoin() const {
    return coin;
}
bool Block::isItem() const {
    return item;
}
bool Block::isHeartItem() const {
    return heartitem;
}
bool Block::isGhostItem() const {
    return ghostitem;
}

Walltype Block::getType() const{
	return type;
}
void Block::draw() const {
    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);

    switch (type) {
    case blank:
        break;

    case width_line:
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex3f(-BLOCK_SIZE / 2, 0, 0);
        glVertex3f(BLOCK_SIZE / 2, 0, 0);
        glEnd();
        break;

    case height_line:
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex3f(0, -BLOCK_SIZE / 2, 0);
        glVertex3f(0, BLOCK_SIZE / 2, 0);
        glEnd();
        break;

    case left_up:
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex3f(0, -BLOCK_SIZE / 2, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(BLOCK_SIZE / 2, 0, 0);
        glEnd();
        break;

    case right_up:
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex3f(0, -BLOCK_SIZE / 2, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(-BLOCK_SIZE / 2, 0, 0);
        glEnd();
        break;

    case right_down:
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex3f(0, BLOCK_SIZE / 2, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(-BLOCK_SIZE / 2, 0, 0);
        glEnd();
        break;

    case left_down:
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex3f(0, BLOCK_SIZE / 2, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(BLOCK_SIZE / 2, 0, 0);
        glEnd();
        break;

    case ghost_only:
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(-BLOCK_SIZE / 2, 0, 0);
        glVertex3f(BLOCK_SIZE / 2, 0, 0);
        glEnd();
        break;

    case coin_road:
        if (coin) {
            glColor3f(1.0f, 1.0f, 0.0f);
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0, 0, 0);
            for (int i = 0; i <= 50; i++) {
                float theta = 2.0f * 3.1415926f * float(i) / 50;
                float x = BLOCK_SIZE / 6 * cosf(theta);
                float y = BLOCK_SIZE / 6 * sinf(theta);
                glVertex3f(x, y, 0);
            }
            glEnd();
        }
        break;
    case item_road:
        if (item) {
            glColor3f(1.0f, 0.0f, 0.0f);
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0, 0, 0);
            for (int i = 0; i <= 50; i++) {
                float theta = 2.0f * 3.1415926f * float(i) / 50;
                float x = BLOCK_SIZE / 3 * cosf(theta);
                float y = BLOCK_SIZE / 3 * sinf(theta);
                glVertex3f(x, y, 0);
            }
            glEnd();
        }
        break;
    case heart_item_road:
        if (heartitem) {
            glColor3f(1.0f, 1.0f, 0.0f);
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0, 0, 0);
            for (int i = 0; i <= 4; i++) {
                float theta = 2.0f * 3.1415926f * float(i) / 4;
                float x = BLOCK_SIZE / 3 * cosf(theta);
                float y = BLOCK_SIZE / 3 * sinf(theta);
                glVertex3f(x, y, 0);
            }
            glEnd();
        }
        break;
    case ghost_item_road:
        if (ghostitem) {
            glColor3f(0.0f, 0.0f, 1.0f);
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0, 0, 0);
            for (int i = 0; i <= 4; i++) {
                float theta = 2.0f * 3.1415926f * float(i) / 4;
                float x = BLOCK_SIZE / 3 * cosf(theta);
                float y = BLOCK_SIZE / 3 * sinf(theta);
                glVertex3f(x, y, 0);
            }
            glEnd();
        }
        break;
    }


    glPopMatrix();
}


int map_structure[NUM_COL][NUM_ROW] = { { 0, 0, 6, 2, 2, 2, 2, 3, 6, 2, 2, 2, 2, 3, 0, 0, 0, 1, 0, 1, 0, 0, 0, 6, 2, 2, 2, 2, 2, 2, 2, 2, 3, 0, 0, 0 },
                                        { 0, 0, 1, 8, 8, 8, 8, 1, 1, 8, 8, 8, 8, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 6, 3, 8, 5, 4, 8, 6, 3, 8, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 8, 6, 3, 8, 6, 2, 3, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 1, 8, 9, 8, 8, 1, 1, 8, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 8, 1, 1, 8, 1, 0, 1, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 1, 8, 6, 2, 2, 4, 1, 8, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 8, 1, 1, 8, 1, 0, 1, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 1, 8, 5, 2, 2, 2, 4, 8, 5, 2, 2, 2, 4, 0, 5, 2, 2, 2, 4, 8, 5, 4, 8, 5, 2, 4, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 5, 2, 2, 3, 8, 6, 3, 8, 6, 2, 2, 2, 3, 0, 6, 2, 2, 2, 2, 2, 2, 3, 8, 6, 2, 3, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 6, 2, 2, 4, 8, 5, 4, 8, 5, 2, 2, 2, 4, 0, 5, 2, 2, 3, 6, 2, 2, 4, 8, 1, 0, 1, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 1, 8, 8, 11, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 8, 8, 8, 8, 1, 0, 1, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 1, 8, 6, 3, 8, 6, 3, 8, 6, 3, 0, 6, 2, 2, 2, 3, 0, 1, 1, 8, 6, 3, 8, 1, 0, 1, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 5, 4, 8, 1, 1, 8, 1, 1, 8, 1, 1, 0, 1, 0, 0, 0, 1, 0, 5, 4, 8, 1, 1, 8, 5, 2, 4, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 8, 8, 8, 1, 1, 8, 1, 1, 8, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 8, 1, 1, 8, 8, 8, 8, 9, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 6, 2, 2, 4, 1, 8, 1, 1, 8, 1, 1, 0, 1, 0, 0, 0, 7, 0, 6, 2, 2, 4, 1, 8, 6, 2, 2, 2, 4, 0, 0, 0 },
                                        { 0, 0, 1, 8, 5, 2, 2, 3, 1, 8, 1, 1, 8, 1, 1, 0, 1, 0, 0, 0, 7, 0, 5, 2, 2, 3, 1, 8, 5, 2, 2, 2, 3, 0, 0, 0 },
                                        { 0, 0, 1, 8, 8, 8, 8, 1, 1, 8, 1, 1, 8, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 8, 1, 1, 8, 8, 8, 8, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 6, 3, 8, 1, 1, 8, 1, 1, 8, 1, 1, 0, 1, 0, 0, 0, 1, 0, 6, 3, 8, 1, 1, 8, 6, 2, 3, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 1, 8, 5, 4, 8, 5, 4, 8, 5, 4, 0, 5, 2, 2, 2, 4, 0, 1, 1, 8, 5, 4, 8, 1, 0, 1, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 1, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 8, 8, 8, 8, 1, 0, 1, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 5, 2, 2, 3, 8, 6, 3, 8, 6, 2, 2, 2, 3, 0, 6, 2, 2, 4, 5, 2, 2, 3, 8, 1, 0, 1, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 6, 2, 2, 4, 8, 5, 4, 8, 5, 2, 2, 2, 4, 0, 5, 2, 2, 2, 2, 2, 2, 4, 8, 5, 2, 4, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 1, 9, 6, 2, 2, 2, 3, 8, 6, 2, 2, 2, 3, 0, 6, 2, 2, 2, 3, 8, 6, 3, 8, 6, 2, 3, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 1, 8, 5, 2, 2, 3, 1, 8, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 8, 1, 1, 8, 1, 0, 1, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 1, 1, 8, 8, 8, 8, 1, 1, 8, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 8, 1, 1, 8, 1, 0, 1, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 5, 4, 8, 6, 3, 8, 5, 4, 8, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 10, 5, 4, 8, 5, 2, 4, 8, 1, 0, 0, 0 },
                                        { 0, 0, 1, 8, 8, 8, 8, 1, 1, 8, 8, 8, 8, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 0, 0 },
                                        { 0, 0, 5, 2, 2, 2, 2, 4, 5, 2, 2, 2, 2, 4, 0, 0, 0, 1, 0, 1, 0, 0, 0, 5, 2, 2, 2, 2, 2, 2, 2, 2, 4, 0, 0, 0 } };



int Map::coin_number() {
    int count_coin = 0;
    for (int i = 0; i < NUM_COL; i++) {
        for (int j = 0; j < NUM_ROW; j++) {
            if (map_structure[i][j] == 8) {
                count_coin++;
            }
        }
    }
    return count_coin;
}

void Map::createMap() {
	float x = LEFT_BOUNDARY;
	float y = BOTTOM_BOUNDARY;
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < blocks[i].size(); j++) {
			blocks[i][j].setCenter(x + i * BLOCK_SIZE + BLOCK_SIZE / 2, y + j * BLOCK_SIZE + BLOCK_SIZE / 2, 0.0f);
			blocks[i][j].setWidth(BLOCK_SIZE);
			blocks[i][j].setHeight(BLOCK_SIZE);
			blocks[i][j].setType(static_cast<Walltype>(map_structure[i][j]));
		}
	}
    for (int i = 0; i < blocks.size(); i++) {
	    for (int j = 0; j < blocks[i].size(); j++) {
		    if (blocks[i][j].getType()==0||blocks[i][j].getType()==7|| blocks[i][j].getType() == 8|| blocks[i][j].getType() == 9 || blocks[i][j].getType() == 10 || blocks[i][j].getType() == 11) {
			    blocks[i][j].setPassable(1);
		    }
            if (blocks[i][j].getType() == coin_road) {
                blocks[i][j].setCoin(true);
            }
            if (blocks[i][j].getType() == item_road) {
                blocks[i][j].setItem(true);
            }
            if (blocks[i][j].getType() == heart_item_road) {
                blocks[i][j].setHeartItem(true);
            }
            if (blocks[i][j].getType() == ghost_item_road) {
                blocks[i][j].setGhostItem(true);
            }
	    }
    }
}

Block& Map::getBlock(int r, int c) {
	return blocks[r][c];
}

void Map::draw() const {
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < blocks[i].size(); j++) {
			blocks[i][j].draw();
		}
	}
}


