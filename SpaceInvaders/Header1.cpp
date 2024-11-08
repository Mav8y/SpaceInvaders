#include "Header1.h"
#include <iostream>
#include <fstream>
Game::Game()
{
	music = LoadMusicStream("Sounds/music.ogg"); //Входит в библиотеку raylib.h звуки можно загружать любые,которые можно скачать
	explosionSound = LoadSound("Sounds/explosion.ogg");
	PlayMusicStream(music);
	InitGame();
	
}
	Game::~Game()
	{
		Alien::UnloadImages(); //очиищает память от изображенией после закрытия игры.код прописан в Alien.cpp 
		UnloadMusicStream(music);
		UnloadSound(explosionSound);// Использован именно метод звука,а не потока ввиду разовости применения
	}

	void Game::Update() //метод обновления различных объектов
	{
		if (run)
		{


			double currentTime = GetTime();
			if (currentTime - MysteryShipLastTimeSpawn > MysteryShipSpawnInterval)
			{
				mysteryship.Spawn();
				MysteryShipLastTimeSpawn = GetTime();
				MysteryShipSpawnInterval = GetRandomValue(10, 20);
			}

			for (auto& laser : spaceship.lasers)
			{
				laser.Update();
			}
			MoveAliens();
			AlienShootLaser();
			for (auto& Laser : alienLasers)
			{
				Laser.Update();
			}
			DeleteInactiveLasers();
			mysteryship.Update();

			CheckForCollisions();
		}
		else
		{
			if (IsKeyDown(KEY_ENTER))
			{
				Reset();
				InitGame();
			}
		}
	}

	void Game::Draw()//Рисование объектов
	{
		spaceship.Draw();
		for (auto& laser : spaceship.lasers)
		{
			laser.Draw();
		}
		for (auto& Obstacle : obstacles)
		{
			Obstacle.Draw();
		}
		for (auto& Alien : aliens)
		{
			Alien.Draw();
		}
		for (auto& Laser : alienLasers)
		{
			Laser.Draw();
		}
		mysteryship.Draw();
	}

	void Game::HandleInput() //Подключение кнопок ручного ввода для действий
	{
		if (run)
		{
			if (IsKeyDown(KEY_LEFT))
			{
				spaceship.MoveLeft();
			}
			else if (IsKeyDown(KEY_RIGHT))
			{
				spaceship.MoveRight();
			}
			else if (IsKeyDown(KEY_SPACE))
			{
				spaceship.FireLaser();
			}
		}
	}

	//Подключение итератора для удаления лазеров с вектора
	void Game::DeleteInactiveLasers() 
	{
		for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();)
		{
			if (!it->active)
			{
				it = spaceship.lasers.erase(it);
			}
			else
			{
				++it;
			}
		}

		for (auto it = alienLasers.begin(); it != alienLasers.end();)
		{
			if (!it->active)
			{
				it = alienLasers.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
		
	std::vector<Obstacle> Game::CreateObstacles()
	{
		int obstacleWidth = Obstacle::grid[0].size() * 3;
		float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5; //Разрфв между объектами

		for (int i=0; i < 4; i++)
		{
			float offsetX = (i + 1) * gap + i * obstacleWidth;
			obstacles.push_back(Obstacle({ offsetX, float(GetScreenHeight() - 200) }));
		}
		return obstacles;
	}

	std::vector<Alien> Game::CreateAliens()
	{
		std::vector<Alien> aliens;
		for (int row = 0; row < 5; row++)
		{
			for (int column = 0; column < 11; column++)
			{
				int AlienType;
				if (row == 0)
				{
					AlienType = 3;
				}
				else if (row == 1 || row == 2)
				{
					AlienType = 2;
				}
				else
				{
					AlienType = 1;
				}
				float x = 75 + column * 55;
				float y = 110 + row * 55;
				aliens.push_back(Alien(AlienType,{x,y}));
			}
		}
		return aliens;
	}

	void Game::MoveAliens()
	{
		for (auto& Alien: aliens)
		{
			if (Alien.position.x + Alien.alienImages[Alien.type - 1].width > GetScreenWidth()- 25)//ограничение экрана для объекта Alien со сменой направления
			{
				AliensDirection = -1;
				MoveDownAliens(4);
			}
			if (Alien.position.x < 25)
			{
				AliensDirection = 1;
				MoveDownAliens(4);
			}
			Alien.Update(AliensDirection);
		}
	}

	void Game::MoveDownAliens(int distance)
	{
		for (auto& Alien : aliens)
		{
			Alien.position.y += distance;
		}
	}

	void Game::AlienShootLaser() // Пришельцы стреляют лазерами с интервалом 0.350 мс случайным образом
	{
		double CurrentTime = GetTime();
		if (CurrentTime - LastTimeAlienFired >= AlienLaserShootInterval && !aliens.empty())
		{
			int randomIndex = GetRandomValue(0, aliens.size() - 1);
			Alien& alien = aliens[randomIndex];
			alienLasers.push_back(Laser({ alien.position.x + alien.alienImages[alien.type - 1].width / 2,
										alien.position.y + alien.alienImages[alien.type - 1].height }, 6));
			LastTimeAlienFired = GetTime();
		}
	}

	void Game::CheckForCollisions() //Метод столкновения лазеров с объектами
	{
		//Spaceship Lasers
		for (auto& laser : spaceship.lasers)
		{
			auto it = aliens.begin();
			while (it != aliens.end())
			{
				if (CheckCollisionRecs(it->getRect(), laser.getRect()))
				{
					PlaySound(explosionSound);
					if (it->type == 1)
					{
						score += 100;
					}
					else if (it->type == 2)
					{
						score += 200;
					}
					else if(it->type ==3)
					{
						score += 300;
					}
					checkForHighScore();
					it = aliens.erase(it);
					laser.active = false;
				}
				else 
				{
					++it;
				}
			}
			for (auto& Obstacle : obstacles)
			{
				auto it = Obstacle.blocks.begin();
				while (it != Obstacle.blocks.end())
				{
					if (CheckCollisionRecs(it->getRect(), laser.getRect()))
					{
						it = Obstacle.blocks.erase(it);
						laser.active = false;
					}
					else
					{
						++it;
					}
				}
			}
			if (CheckCollisionRecs(mysteryship.getRect(), laser.getRect()))
			{
				mysteryship.alive = false;
				laser.active = false;
				score += 500;
				checkForHighScore();
				PlaySound(explosionSound);
			}
		}
		//Alien Lasers
		for (auto& laser : alienLasers)
		{
			if (CheckCollisionRecs(laser.getRect(), spaceship.getRect()))
			{
				laser.active = false;
				lives--;
				if (lives == 0)
				{
					GameOver();
				}
			}
			for (auto& Obstacle : obstacles)
			{
				auto it = Obstacle.blocks.begin();
				while (it != Obstacle.blocks.end())
				{
					if (CheckCollisionRecs(it->getRect(), laser.getRect()))
					{
						it = Obstacle.blocks.erase(it);
						laser.active = false;
					}
					else
					{
						++it;
					}
				}
			}
		}

		//Alien Collision With Obstacles
		for (auto& alien : aliens)
		{
			for (auto& Obstacle : obstacles)
			{
				auto it = Obstacle.blocks.begin();
				while (it != Obstacle.blocks.end())
				{
					if (CheckCollisionRecs(it->getRect(), alien.getRect()))
					{
						it = Obstacle.blocks.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
			if (CheckCollisionRecs(alien.getRect(), spaceship.getRect()))
			{
				GameOver();
			}
		}
	}

	void Game::GameOver()
	{
		run = false;
	}

	void Game::InitGame()
	{
		obstacles = CreateObstacles();
		aliens = CreateAliens();
		AliensDirection = 1;
		LastTimeAlienFired = 0.0;
		MysteryShipLastTimeSpawn = 0.0;
		lives = 3;
		score = 0;
		HighScore = loadHighscoreFromFile();
		run = true;
		MysteryShipSpawnInterval = GetRandomValue(10, 20);
	}

	void Game::checkForHighScore()
	{
		if (score > HighScore)
		{
			HighScore = score;
			saveHighscoreToFile(HighScore);
		}
	}

	void Game::saveHighscoreToFile(int highscore)
	{
		std::ofstream highscoreFile("highscore.txt"); //Файл хранящий лучший результат и не теряется после закрытия окна игры
		if (highscoreFile.is_open())
		{
			highscoreFile << highscore;
			highscoreFile.close();
		}
		else
		{
			std::cerr << "Failed to save highscore to file" << std::endl;
		}
	}

	int Game::loadHighscoreFromFile()
	{
		int loadedHighscore = 0;
		std::ifstream highscoreFile("highscore.txt"); //загрузка последнего лучшего результата из файла
		if (highscoreFile.is_open())
		{
			highscoreFile >> loadedHighscore;
			highscoreFile.close();
		}
		else
		{
			std::cerr << "Failed to load highscore file" << std::endl;
		}
		return loadedHighscore;
	}

	void Game::Reset()
	{
		spaceship.Reset();
		aliens.clear();
		alienLasers.clear();
		obstacles.clear();
	}