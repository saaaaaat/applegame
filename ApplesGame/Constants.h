#pragma once
#include <string>

namespace ApplesGame
{
	const std::string RESOURCES_PATH = "Resources/";
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const float INITIAL_SPEED = 100.f; 
	const float PLAYER_SIZE = 20.f;
	const float ACCELERATION = 20.f;
	//const int NUM_APPLES = 20;// не нужно
	const float APPLE_SIZE = 20.f;
	const float PAUSE_LENGTH = 3.f;
	const int NUM_ROCKS = 10;
	const float ROCK_SIZE = 15.f;
	const uint8_t MODE_FINITE_APPLES = 1 << 0;
	const uint8_t MODE_INFINITE_APPLES = 1 << 1;
	const uint8_t MODE_ACCELERATION = 1 << 2;
	const uint8_t MODE_NO_ACCELERATION = 1 << 3;

	const int LEADERBOARD_SIZE = 5;
	const int MAX_SCORE = 200;
	const int MIN_SCORE = 10;
}

