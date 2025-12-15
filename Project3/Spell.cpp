#include "spell.h"


Spell::Spell(int key, int cost) {
	this->key = key;
	this->cost = cost;
}

void Spell::setkey(int key) {
	this->key = key;
}

int Spell:: getkey() {
	return this->key;
}

void Spell::setcost(int cost) {
	this->cost = cost;
}

int Spell::getcost() {
	return cost;
}