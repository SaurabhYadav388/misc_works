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

using namespace std;

void hideConsoleCursor()
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

class ball
{
public:
    int x=25;
    int y=2;
    float angleRad = (3.14159f) / 4.0f;//45 deg
    float dirX= cosf( angleRad );//cos
    float dirY= sinf( angleRad );//sin

}ballObj;

int main()
{
    hideConsoleCursor();
    int height = 50;
    int width =50;

    int paddleLeft = 22;
    int paddleRight = 28;

    int sleep_ms = 50;

    vector<vector<char>> screen(height, vector<char>(width, ' '));


    float oldX = 25;
    float oldY = 2;

    //int time = 0;//no need
    int vel = 3;//1 dont work //small chnge
    while (1)
    {
        for (int y = height-1; y >= 0; y--)
        {
            for (int x = 0; x < width; x++)
            {
                if (y == 0 || x == 0 || y == height - 1 || x == width - 1)
                    screen[y][x] = 'x';
                else if (y == 1 && (x >= paddleLeft && x <= paddleRight))
                    screen[y][x] = '-';
                //ball
                else if (y == ballObj.y && x == ballObj.x)
                    screen[y][x] = 'O';
                else
                    screen[y][x] = ' ';
            }
        }
        //paint
        for (int y = height-1; y >= 0; y--)
        {
            for (int x = 0; x < width; x++)
            {
                cout << screen[y][x];
            }
            cout << "\n";
        }

        if (_kbhit())
        {
            char key = _getch();
            if (key == 'a')
            {
                paddleLeft-=2; paddleRight-=2;
            }
            else if (key == 'd')
            {
                paddleLeft+=2; paddleRight+=2;
            }
            else if (key == 'x')return 0;
        }

        //if collision-
        if (ballObj.x <= 0 || ballObj.x >= width - 1 || ballObj.y <= 0 || ballObj.y >= height - 1)//less/more than necessary
        {
            int changeX = ballObj.x - oldX;
            int changeY = ballObj.y - oldY;
            //cout << "change X: " << changeX << " Y:" << changeY;
            //Sleep(2000);
            if (changeX > 0 && changeY > 0)
            {
                //cout << "old:" << ballObj.angleRad * (180.0f) / (3.14159);
                //Sleep(2000);
                
                //should propg. changes to both
                ballObj.angleRad = (3.14159f) - ballObj.angleRad;
                ballObj.dirX = cosf(ballObj.angleRad);//pi-angle
                ballObj.dirY = sinf(ballObj.angleRad);
                
            }
            
            else if (changeX < 0 && changeY > 0)
            {
                //cout << "old:" << ballObj.angleRad * (180.0f) / (3.14159);
                //Sleep(2000);
                ballObj.angleRad = 2*(3.14159f) - ballObj.angleRad;
                ballObj.dirX = cosf(ballObj.angleRad);//2pi-angle??
                ballObj.dirY = sinf(ballObj.angleRad);

            }
            else if (changeX < 0 && changeY < 0)
            {
                ballObj.angleRad = 2 * (3.14159f) - (ballObj.angleRad- (3.14159f));
                ballObj.dirX = cosf(ballObj.angleRad);//2pi-angle??
                ballObj.dirY = sinf(ballObj.angleRad);
            }
            else if (changeX > 0 && changeY < 0)
            {
                ballObj.angleRad = ( 2*(3.14159f) - ballObj.angleRad );
                ballObj.dirX = cosf(ballObj.angleRad);//2pi-angle??
                ballObj.dirY = sinf(ballObj.angleRad);
            }



            oldX = ballObj.x;
            oldY = ballObj.y;
        }

        
        //update the ball traj.
        ballObj.x = ballObj.x + vel * ballObj.dirX;
        ballObj.y = ballObj.y + vel * ballObj.dirY;
        //time++;

        Sleep(sleep_ms);
        system("cls");
    }

}






/*
    while (1)
{
    if (_kbhit())
    {
        char key = _getch();
        cout << "pressed: " << key << " ascii:" << int(key) << "\n";

        if (key == 'x')return 0;
    }



    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 || j == 0 || i == height - 1 || j == width - 1)
                cout << "*";
            else if (i == height - 2 && (j >= 22 && j <= 28))
                cout << "-";
            else
                cout << " ";
        }
        cout << "\n";
    }
}








 while (1)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (i == 0 || j == 0 || i == height - 1 || j == width - 1)
                    cout << "*";
                else if (i == height - 2 && (j >= paddleLeft && j <= paddleRight))
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

