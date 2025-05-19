#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include<cstring>
#include <array>
#include <vector>
#include "Constant.h"

using namespace std; 
using namespace sf; 

class Engine{
public:
	Engine(); 
	void MakeMove(); 
	void UpdBoard(int square, int view, int type);  // white is 0, black is 1. 
	void Reset(bool view);
	void print(); 
	void upd_castle(bool side, int type, int played, bool dir); 
	array<int, 4> getDecision(bool t); 
private: 
};