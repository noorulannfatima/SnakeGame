#include<iostream>
#include <raylib.h>
#include<deque>
#include<raymath.h>

using namespace std;

    Color green= {245, 245, 245, 255};
    Color darkGreen= {80, 0, 0, 255};

    int cellSize = 20;
    int cellCount = 30;

    double LastUpdateTime= 0 ;  // keep track of time at which last update of snake occured

    bool ElementInDeque(Vector2 element, deque<Vector2> deque){
        for (unsigned int i=0; i< deque.size(); i++){
            if(Vector2Equals(deque[i], element)){
                return true;
            }
        }
        return false;
    }

    bool eventTriggered(double interval){
        double currentTime= GetTime();
        if(currentTime - LastUpdateTime >= interval){
            LastUpdateTime = currentTime;
            return true;
        }
        return false;
    }

    class Snake{
        public:
        deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        Vector2 direction = {1, 0};

        void Draw(){
            for(unsigned int  i=0; i<body.size(); i++){
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segement = Rectangle{x*cellSize, y*cellSize, (float)cellSize, (float)cellSize};
                DrawRectangleRounded(segement, 0.5, 6, darkGreen);

            }
        }
        void Update(){  // moving snake
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
        }

    };

    class Food{
        public:
        Vector2 position;
        Texture2D texture;

        Food(deque<Vector2> snakebody){  //constructor
            Image image = LoadImage("graphics/1.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            position = GenerateRandomPos(snakebody);
        }
        // Destructor
        ~Food(){
            UnloadTexture(texture);
        }

        void Draw(){
            DrawTexture(texture, position.x*cellSize, position.y*cellSize, green);
        }
        Vector2 GenerateRandomCell(){
            float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            return Vector2{x, y};

        }
        //Generating food at random positions
        Vector2 GenerateRandomPos(deque <Vector2> snakebody){
            Vector2 position = GenerateRandomCell();

            while (ElementInDeque(position, snakebody)){  // prevents food on snake body
                position = GenerateRandomCell();
            }
            return position;
        }
    };

    class Game{
        public:
        Snake snake = Snake();
        Food food = Food(snake.body);

        void Draw(){
            snake.Draw();
            food.Draw();
        }
        void Update(){
            snake.Update();
            CheckCollisionWithFood();
        }
        void CheckCollisionWithFood(){
            if(Vector2Equals(snake.body[0], food.position)){
                food.position = food.GenerateRandomPos(snake.body);
            }
            
        }

    };
int main(){

    InitWindow(cellSize*cellCount, cellSize*cellCount, "Retro Snake");
    SetTargetFPS(60);  // defining frame rate means this game will run at same rate on every computer

    Game game = Game();


    while(WindowShouldClose() == false){

        BeginDrawing();

        if(eventTriggered(0.2))
        {
            game.Update();
        }
        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction= {0, -1};
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction= {0, 1};
        }
        if(IsKeyPressed(KEY_LEFT)  && game.snake.direction.x != 1){
            game.snake.direction= {-1, 0};
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction= {1, 0};
        }
        

        // dawing
        ClearBackground(green);
        game.Draw();  // calling draw method of game object

        EndDrawing();
    }


    CloseWindow();
    return 0;
}