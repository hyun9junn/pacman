#pragma once


class Spell
{
public:
	Spell(int key, int cost);
	void setkey(int key);
	int getkey();
	void setcost(int cost);
	int getcost();

private:
	int cost;
	int key;
};

