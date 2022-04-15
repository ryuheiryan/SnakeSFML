#pragma once
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
using namespace sf;

class Game {
	const int FPS = 60;
	const Color boardWhite = Color(213, 207, 191);
	const Color borderRed = Color(134, 28, 24);
	const Color snakeBlack = Color(23, 31, 33);
	const Color foodGreen = Color(1, 152, 98);
	const std::string title = "BamBamBam";
	const float speed = 300;
	const float width = 1080, height = 720;
	int thickness;
	int delayTurn;

	RenderWindow window;
	RectangleShape borders[4];

	Font quicksand;
	Font rocksalt;
	Text scoreTxt;

	RectangleShape startButton;
	Text startTxt;

	RectangleShape s;
	std::vector<RectangleShape> snake;
	std::vector<Vector2f> snakePath;
	std::string direction;
	std::string current;
	int growth, scoreInt, delay;

	CircleShape food;

	Clock clock;
	float dt;
	bool playing;

	RectangleShape& snakeHead();
	void displayStart();
	void createBorders();
	void displayScore();
	void createSnake();
	void createFood();

	void wiggle();
	void eat();

	void initWindow();
	void initPlay();
	void windowCloser();
	void mainMenu();
	void movement();
	void collision();
	void render();

public:
	Game();
	void run();
	void start();
};