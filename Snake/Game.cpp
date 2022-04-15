#include "Game.h"

Game::Game() {
	srand(time(NULL));
	initWindow();
}

RectangleShape& Game::snakeHead() {
	return snake[0];
}

void Game::displayStart() {
	startButton.setSize(Vector2f(width / 3, height / 6));
	startButton.setOutlineThickness(startButton.getSize().y / 50);
	startButton.setOutlineColor(snakeBlack);
	startButton.setFillColor(boardWhite);
	startButton.setPosition((width - startButton.getSize().x) / 2, (height - startButton.getSize().y) / 2);

	startTxt.setCharacterSize(thickness * 3);
	startTxt.setFont(rocksalt);
	startTxt.setFillColor(snakeBlack);
	startTxt.setPosition((width - startTxt.getLocalBounds().width) / 2, (height - startTxt.getLocalBounds().height) / 2);
}

void Game::createBorders() {
	RectangleShape hBorder(Vector2f(width, thickness));
	RectangleShape vBorder(Vector2f(thickness, height));
	hBorder.setFillColor(borderRed); vBorder.setFillColor(borderRed);

	borders[0] = hBorder, borders[1] = hBorder, borders[2] = vBorder, borders[3] = vBorder;
	borders[1].setPosition(Vector2f(0, height - thickness));
	borders[3].setPosition(Vector2f(width - thickness, 0));
}

void Game::displayScore() {
	if (!quicksand.loadFromFile("Fonts/Quicksand-Medium.ttf"))
		throw("could not load font");
	if (!rocksalt.loadFromFile("Fonts/RockSalt.ttf"))
		throw("could not load font");
	scoreTxt.setString("Score: 0");
	scoreTxt.setFont(rocksalt);
	scoreTxt.setCharacterSize(thickness * 1.5);
	scoreTxt.setFillColor(snakeBlack);
	scoreTxt.setOrigin(scoreTxt.getLocalBounds().width, 0);
	scoreTxt.setPosition(width - thickness * 3, thickness * 2);
}

void Game::createSnake(){
	s.setSize(Vector2f(thickness, thickness));
	s.setFillColor(snakeBlack);
	snake.push_back(s);
	snakeHead().setPosition(thickness * 2, thickness * 2);
	snakePath.push_back(snakeHead().getPosition());
}

void Game::createFood() {
	food.setRadius(thickness / 2);
	food.setFillColor(foodGreen);
	food.setPosition(round(rand() % int(width - thickness * 6) + thickness * 3), round(rand() % int(height - thickness * 6) + thickness * 3));
}

void Game::wiggle() {
	if (direction == "right")
		snakeHead().move((speed + scoreInt) * dt, 0);
	if (direction == "left")
		snakeHead().move(-(speed + scoreInt) * dt, 0);
	if (direction == "up")
		snakeHead().move(0, -(speed + scoreInt) * dt);
	if (direction == "down")
		snakeHead().move(0, (speed + scoreInt) * dt);
}

void Game::eat() {
	food.setPosition(round(rand() % int(width - thickness * 6) + thickness * 3), round(rand() % int(height - thickness * 6) + thickness * 3));
	scoreInt++;
}

void Game::initWindow() {
	thickness = height / 40.f;
	delayTurn = floor(thickness / 5);

	window.create(VideoMode(width, height), title);
	window.setFramerateLimit(FPS);
	createBorders();
	displayScore();
	displayStart();
}

void Game::initPlay() {
	snake.clear();
	snakePath.clear();
	direction = "right", growth = 5, scoreInt = 0, delay = 0;
	startTxt.setString("Start");
	scoreTxt.setString("Score: " + std::to_string(scoreInt));
	createSnake();
	createFood();
}

void Game::windowCloser() {
	Event e;
	while (window.pollEvent(e)) {
		if (e.type == Event::Closed)
			window.close();
	}
}

void Game::mainMenu() {
	startTxt.setString("Continue");
	if(!playing)
		initPlay();
	startTxt.setPosition((width - startTxt.getLocalBounds().width) / 2, (height - startTxt.getLocalBounds().height) / 2);
	bool menu = true;
	while (menu) {
		windowCloser();
		if (Mouse::isButtonPressed(Mouse::Left) && startButton.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			break;
		window.clear(boardWhite);
		window.draw(startButton);
		window.draw(startTxt);
		window.display();
	}
}

void Game::movement() {
	current = direction;
	if (delay) delay--;
	else if (Keyboard::isKeyPressed(Keyboard::Right) && current != "left" && current != "right")
		direction = "right", delay = delayTurn;
	else if (Keyboard::isKeyPressed(Keyboard::Left) && current != "left" && current != "right")
		direction = "left", delay = delayTurn;
	else if (Keyboard::isKeyPressed(Keyboard::Up) && current != "down" && current != "up")
		direction = "up", delay = delayTurn;
	else if (Keyboard::isKeyPressed(Keyboard::Down) && current != "down" && current != "up")
		direction = "down", delay = delayTurn;
	wiggle();
	for (size_t i = 1; i < snake.size(); i++) {
		snake[i].setPosition(snakePath[i - 1]);
		if (i > round(thickness / 3) && snakeHead().getGlobalBounds().intersects(snake[i].getGlobalBounds()))
			playing = false;
	}
}

void Game::collision() {
	for (int i = 0; i < 4; i++) {
		if (snakeHead().getGlobalBounds().intersects(borders[i].getGlobalBounds()))
			playing = false;
	}
	if (snakeHead().getGlobalBounds().intersects(food.getGlobalBounds())) {
		eat();
		growth = 5;
	}
	if (growth-- > 0) {
		s.setPosition(snakePath[snakePath.size() - 1]);
		snake.push_back(s);
		snakePath.push_back(s.getPosition());
	}

	scoreTxt.setString("Score: " + std::to_string(scoreInt));
}

void Game::render() {
	window.clear(boardWhite);

	for (int i = 0; i < 4; i++)
		window.draw(borders[i]);
	for (int i = 0; i < snake.size(); i++) {
		window.draw(snake[i]);
		snakePath[i] = snake[i].getPosition();
	}
	window.draw(food);
	window.draw(scoreTxt);
	window.display();
}

void Game::run() {
	int starting = FPS / 6;
	playing = true;
	while (playing) {
		dt = clock.restart().asSeconds();
		if (Keyboard::isKeyPressed(Keyboard::Escape) || Keyboard::isKeyPressed(Keyboard::Space))
			break;
		windowCloser();
		if (!starting) {
			movement();
			collision();
		}
		else starting--;
		render();
	}
}

void Game::start(){
	playing = false;
	while (window.isOpen()) {
		mainMenu();
		run();
	}
}