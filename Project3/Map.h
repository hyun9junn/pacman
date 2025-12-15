#pragma once

#include "Sphere.h"
#include "Constants.h"

#include <array>

/* Block class */

enum Walltype { blank ,width_line, height_line, left_up, right_up, right_down, left_down, ghost_only ,coin_road, item_road, heart_item_road, ghost_item_road};
class Block : public Shape3D
{
public:
	Block();
	Block(float x, float y, float z, float w, float h, Walltype t, bool c, bool i);

	void setWidth(float w);
	void setHeight(float h);
	void setPassable(bool v);
	void setCoin(bool c);
	void setType(Walltype t);
	void setItem(bool i);
	void setHeartItem(bool h);
	void setGhostItem(bool h);

	float getWidth() const;
	float getHeight() const;
	Walltype getType() const;
	bool isPassable() const;
	bool isCoin() const;
	bool isItem() const;
	bool isHeartItem() const;
	bool isGhostItem() const;

	virtual void draw() const;

private:
	Walltype type;
	float width, height;
	bool coin;
	bool bPassable;
	bool item;
	bool heartitem;
	bool ghostitem;
};

/* Map class */
class Map
{
public:
	void createMap();

	Block& getBlock(int r, int c);

	void draw() const;
	int coin_number();

private:
	std::array<std::array<Block, NUM_ROW>, NUM_COL>  blocks;

};