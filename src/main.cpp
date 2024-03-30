#include<iostream>
#include <raylib.h>
#include<deque>

using namespace std;

    Color green= {177, 204, 96, 255};
    Color darkGreen= {43, 51, 24, 255};

    int cellSize = 20;
    int cellCount = 30;

    class Snake{
        public:
        deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};

        void Draw(){
            for(unsigned int  i=0; i<body.size(); i++){
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segement = Rectangle{x*cellSize, y*cellSize, (float)cellSize, (float)cellSize};
                DrawRectangleRounded(segement, 0.5, 6, darkGreen);

            }
        }

    };

    class Food{
        public:
        Vector2 position;
        Texture2D texture;

        Food(){  //constructor
            Image image = LoadImage("graphics/1.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            position = GenerateRandomPos();
        }
        // Destructor
        ~Food(){
            UnloadTexture(texture);
        }

        void Draw(){
            DrawTexture(texture, position.x*cellSize, position.y*cellSize, green);
        }
        //Generating food at random positions
        Vector2 GenerateRandomPos(){
            float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            return Vector2{x, y};
        }


    };

int main(){

    InitWindow(cellSize*cellCount, cellSize*cellCount, "Retro Snake");
    SetTargetFPS(60);  // defining frame rate means this game will run at same rate on every computer

    Food food= Food(); // calling food object
    Snake snake = Snake(); // calling snake object

    while(WindowShouldClose() == false){

        BeginDrawing();

        // dawing
        ClearBackground(green);
        food.Draw();  // calling draw method of food object
        snake.Draw();

        EndDrawing();
    }


    CloseWindow();
    return 0;
}