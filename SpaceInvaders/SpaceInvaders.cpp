#include <iostream>
#include <raylib.h>
#include "Header1.h"
#include <string>

using namespace std;

string FormatWithLeadingZeros(int number, int width)
{
    string numberText = to_string(number);
    int leadingZeros = width - numberText.length();
     return numberText = string(leadingZeros, '0') + numberText;
}


int main()
{
    Color grey = { 29,29,27,255 };
    Color yellow = { 243,216,63,255 };
    int offset = 50;
    int windowWidth = 750;
    int windowHeight = 700;
    InitWindow(windowWidth + offset, windowHeight + offset * 2, "C++ Space Invaders");
    InitAudioDevice(); //Также как и потоки,в конце всего обязательно закрыть!!!
    Font font = LoadFontEx("Font/monogram.ttf",64,0,0); // Важно разместить после инициализации игрового окня во избежания коллизии
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");
    SetTargetFPS(60);
    Game game;
    
    

    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();
        BeginDrawing();
        ClearBackground(grey);
        DrawRectangleRoundedLines({10,10,780,780},0.18f,20,2,yellow);
        DrawLineEx({25,730},{775,730},3,yellow); // Линия координаты начала и конца линии, толщина,цвет

        if (game.run)
        {
            DrawTextEx(font, "LEVEL 01", { 570,740 }, 34, 2, yellow); //Текст интерфейса уровня
        }
        else
        {
            DrawTextEx(font, "GAME OVER", { 570,740 }, 34, 2, yellow); //Текст интерфейса Конец игры
        }
        float x = 50.0;
        for (int i = 0; i < game.lives; i++)
        {
            DrawTextureV(spaceshipImage, { x,745 }, WHITE); //Счетчик жизней
            x += 50;
        }
        DrawTextEx(font, "SCORE", { 50,15 }, 34, 2, yellow); //Счетчик очков
        string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), { 50,40 }, 34, 2, yellow);//конвертирование счетчика из int в string с последующим повышением счета очков

        DrawTextEx(font, "HIGH-SCORE", { 570,15 }, 34, 2, yellow);//Счетчик лучшего счета очков
        string highscoreText = FormatWithLeadingZeros(game.HighScore, 5);
        DrawTextEx(font, highscoreText.c_str(), { 655,40 }, 34, 2, yellow);
        game.Draw();
        
       
       

        EndDrawing();

    }

    CloseWindow();
    CloseAudioDevice();
}