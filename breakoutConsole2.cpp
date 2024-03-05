// BreakoutExp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
Made line height .6
*/
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <math.h>
#include <unordered_map>//not working?
#include <map>


using namespace std;

void gotoxy(int x, int y)
{
    COORD coordinate;
    coordinate.X = x;
    coordinate.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

const float pi = 3.14159;
const int screenHeight = 50;
const int screenWidth = 50;
int sleep_ms = 10;

int paddleLeft = screenWidth / 2 - 4;
int paddleRight = screenWidth / 2 + 4;

vector<vector<char>> screen(screenHeight, vector<char>(screenWidth, ' '));

void hideConsoleCursor()
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

class Ball
{
public:
    float x = screenWidth / 2;
    float y = 2;
    float angleRad = pi / 6.0f;//rad//no need
    float dirX = cosf(angleRad);
    float dirY = sinf(angleRad);
    float velocity = 1.0f;//1 dont work //small chnge

    void updateBallPosition()
    {
        ballObj.x = ballObj.x + velocity * ballObj.dirX;
        ballObj.y = ballObj.y + velocity * ballObj.dirY;
    }

}ballObj;

map<pair<int, int>, bool> bricks;
int bricksTotal;
int bricksBreakCount=0;

void initializeBricks()
{
    for (int y = 40; y > 20; y--)
    {
        for (int x = 10; x < 40; x++)
        {
            bricks[{y, x}] = true;
            bricksTotal++;//////
        }
    }
}

void calculateScreen()
{
    for (int y = screenHeight - 1; y >= 0; y--)
    {
        for (int x = 0; x < screenWidth; x++)
        {
            if ( x == 0 || y == screenHeight - 1 || x == screenWidth - 1) //y == 0 //bottom wall
                screen[y][x] = 'x';
            else if (y == 1 && (x >= paddleLeft && x <= paddleRight))
                screen[y][x] = '-';
            //ball
            else if (y == int(roundf(ballObj.y)) && x == int( roundf(ballObj.x) ))
                screen[y][x] = 'O';
            //brick
            else if (bricks[{y, x}] == true)
                screen[y][x] = '@';
            else
                screen[y][x] = ' ';
        }
    }
}

void paintScreen()
{
    for (int y = screenHeight - 1; y >= 0; y--)
    {
        for (int x = 0; x < screenWidth; x++)
        {
            cout << screen[y][x];
        }
        cout << "\n";
    }
}

void listenUserInput()
{
    if (_kbhit())
    {
        char key = _getch();
        int paddleShift = 2;
        if (key == 'a')
        {
            paddleLeft -= paddleShift; paddleRight -= paddleShift;
        }
        else if (key == 'd')
        {
            paddleLeft += paddleShift; paddleRight += paddleShift;
        }
        else if (key == 'x')return;
    }
}

void checkCollision()
{
    //wall collision
    if (ballObj.x <= 0 || ballObj.x >= screenWidth - 1 || ballObj.y <= 0 || ballObj.y >= screenHeight - 1)//less/more than is necessary
    {
        if (ballObj.x <= 0 || ballObj.x >= screenWidth - 1)
        {
            // Bounce off the left or right wall
            ballObj.dirX *= -1.0f;                                                                                                                                                                                                                                                                                                                                         
        }

        else if (ballObj.y >= screenHeight - 1 )
        {
            // Bounce off the top
            ballObj.dirY *= -1.0f;
        }
        else
        {
            //bottom wall
            //ballObj.y <= 0

            //gameover
            exit(0);

        }
    }
    //brick collision
    if (bricks[{int(roundf(ballObj.y)), int(roundf(ballObj.x))}] == true)
    {
        //cout << "x:" <<int(roundf(ballObj.y))<<" y:"<<int(roundf(ballObj.x));
        //Sleep(2000);
        if ((ballObj.dirX > 0 && ballObj.dirY > 0) || (ballObj.dirX < 0 && ballObj.dirY < 0)) //if direction NE or SW
        {
            ballObj.dirY *= -1.0f;
        }
        else if ((ballObj.dirX < 0 && ballObj.dirY > 0) || (ballObj.dirX > 0 && ballObj.dirY < 0))//if direction SE or NW
        {
            ballObj.dirX *= -1.0f;
        }
        bricks[{int(roundf(ballObj.y)), int(roundf(ballObj.x))}] = false;
        bricksBreakCount++;/////////
    }

    //paddle collision
    if ( int(roundf(ballObj.y)) <= 1 && int(roundf(ballObj.x)) >= paddleLeft && int(roundf(ballObj.x)) <= paddleRight)
    {
        ballObj.dirY *= -1.0f;
    }
}

int main()
{
    hideConsoleCursor();
    initializeBricks();

    while (1)
    {

        calculateScreen();
        //paint
        cout << "Points: " << bricksBreakCount<<"\n\n";
        paintScreen();
        listenUserInput();
        //if collision-
        checkCollision();
        //update the ball traj.
        ballObj.updateBallPosition();
        Sleep(sleep_ms);
        system("cls");
    }

}






/*

     cout << "old:" << ballObj.angleRad * (180.0f) / (3.14159);
    Sleep(2000);

    while (1)
{
    if (_kbhit())
    {
        char key = _getch();
        cout << "pressed: " << key << " ascii:" << int(key) << "\n";

        if (key == 'x')return 0;
    }



    for (int i = 0; i < screenHeight; i++)
    {
        for (int j = 0; j < screenWidth; j++)
        {
            if (i == 0 || j == 0 || i == screenHeight - 1 || j == screenWidth - 1)
                cout << "*";
            else if (i == screenHeight - 2 && (j >= 22 && j <= 28))
                cout << "-";
            else
                cout << " ";
        }
        cout << "\n";
    }
}








 while (1)
    {
        for (int i = 0; i < screenHeight; i++)
        {
            for (int j = 0; j < screenWidth; j++)
            {
                if (i == 0 || j == 0 || i == screenHeight - 1 || j == screenWidth - 1)
                    cout << "*";
                else if (i == screenHeight - 2 && (j >= paddleLeft && j <= paddleRight))
                    cout << "-";
                else
                    cout << " ";
            }
            cout << "\n";
        }

        if (_kbhit())
        {
            char key = _getch();
            if (key == 'a')
            {
                paddleLeft--; paddleRight--;
            }
            else if (key == 'd')
            {
                paddleLeft++; paddleRight++;
            }
            else if (key == 'x')return 0;
        }
        Sleep(500);
        system("cls");
    }








    //if collision-
        if (ballObj.x < 0 || ballObj.x > screenWidth - 1 || ballObj.y < 0 || ballObj.y > screenHeight - 1)//less/more than is necessary
        {
            int changeX = ballObj.x - oldX;
            int changeY = ballObj.y - oldY;

            if (changeX > 0 && changeY > 0)
            {

                //should propg. changes to both
                ballObj.angleRad = pi - ballObj.angleRad;
                ballObj.dirX = cosf(ballObj.angleRad);//pi-angle
                ballObj.dirY = sinf(ballObj.angleRad);

            }

            else if (changeX < 0 && changeY > 0)
            {

                ballObj.angleRad = 2 * pi - ballObj.angleRad;
                ballObj.dirX = cosf(ballObj.angleRad);//2pi-angle??
                ballObj.dirY = sinf(ballObj.angleRad);

            }
            else if (changeX < 0 && changeY < 0)
            {

                //ballObj.angleRad = 2 * pi - (ballObj.angleRad - pi);
                //ballObj.angleRad = 3 * pi - (ballObj.angleRad);
                ballObj.angleRad = pi - (ballObj.angleRad);

                ballObj.dirX = cosf(ballObj.angleRad);//2pi-angle??
                ballObj.dirY = sinf(ballObj.angleRad);
            }
            else if (changeX > 0 && changeY < 0)
            {

                ballObj.angleRad = (2 * pi - ballObj.angleRad);
                ballObj.dirX = cosf(ballObj.angleRad);
                ballObj.dirY = sinf(ballObj.angleRad);
            }



            oldX = ballObj.x;
            oldY = ballObj.y;
        }



        //if collision-
        if (ballObj.x <= 0 || ballObj.x >= width - 1 || ballObj.y <= 0 || ballObj.y >= height - 1)//less/more than is necessary
        {
            int changeX = ballObj.x - oldX;
            int changeY = ballObj.y - oldY;

            if (ballObj.x >= width - 1)
            {

                //should propg. changes to both
                ballObj.angleRad = pi - ballObj.angleRad;
                ballObj.dirX = cosf(ballObj.angleRad);//pi-angle
                ballObj.dirY = sinf(ballObj.angleRad);

            }

            else if (ballObj.y >= height - 1)
            {
                ballObj.angleRad = 2 * pi - ballObj.angleRad;
                ballObj.dirX = cosf(ballObj.angleRad);//2pi-angle??
                ballObj.dirY = sinf(ballObj.angleRad);

            }
            else if (ballObj.x <= 0)
            {
                //ballObj.angleRad = 2 * pi - (ballObj.angleRad - pi);
                //ballObj.angleRad = 3 * pi - (ballObj.angleRad);
                ballObj.angleRad = pi - (ballObj.angleRad);

                ballObj.dirX = cosf(ballObj.angleRad);//2pi-angle??
                ballObj.dirY = sinf(ballObj.angleRad);
            }
            else if (ballObj.y <= 0)
            {

                ballObj.angleRad = (2 * pi - ballObj.angleRad);
                ballObj.dirX = cosf(ballObj.angleRad);
                ballObj.dirY = sinf(ballObj.angleRad);
            }

            oldX = ballObj.x;
            oldY = ballObj.y;
        }


*/

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file