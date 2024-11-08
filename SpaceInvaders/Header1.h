#pragma once
#include "Header.h"
#include "Obstacle.h"
#include "Alien.h"
#include "MysteryShip.h"
class Game
{
public:
	Game();
	~Game();
	void Draw();
	void Update();
	void HandleInput();
	bool run;
	int lives;
	int score;
	int HighScore;
	Music music; /*��������� ��������� �������������(�� ������� ����������� ��������� ������������� struck
	��-�� ����������� � �������� ����� ����������� ������ ������ ������ InitAudioDevice,��� ������ � ���������� raylib.h */
private:
	Spaceship spaceship;
	std::vector<Obstacle> CreateObstacles(); //������ �������� ��������� �������� Obstacle
	std::vector<Alien> CreateAliens(); //������ ����������
	void MoveAliens();
	void MoveDownAliens(int distance);
	void DeleteInactiveLasers();
	void AlienShootLaser();
	void CheckForCollisions();
	void GameOver();
	void Reset();
	void InitGame();
	void checkForHighScore();
	void saveHighscoreToFile(int highscore);
	int loadHighscoreFromFile();
	std::vector <Obstacle> obstacles;
	std::vector <Alien> aliens;
	int AliensDirection;
	std::vector<Laser> alienLasers;
	constexpr static float AlienLaserShootInterval= 0.35; // ��� ���� ����������  ����� ����� �� �������
	float LastTimeAlienFired;
	MysteryShip mysteryship;
	float MysteryShipSpawnInterval;
	float MysteryShipLastTimeSpawn;
	Sound explosionSound;
};
