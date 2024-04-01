#include<iostream>
#include <raylib.h>
#include<deque>
#include<raymath.h>

using namespace std;

    Color green= {173, 204, 96, 255};
    Color darkGreen= {43, 51, 24, 255};

    int cellSize = 21;
    int cellCount = 25;
    int offset = 40; // width of border


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
        bool addSegment = false;

        void Draw(){
            for(unsigned int  i=0; i<body.size(); i++){
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segement = Rectangle{offset + x*cellSize, offset + y*cellSize, (float)cellSize, (float)cellSize};
                DrawRectangleRounded(segement, 0.5, 6, darkGreen);

            }
        }
        void Update(){  // moving snake & adding body part
        body.push_front(Vector2Add(body[0], direction));
        if(addSegment == true){
            addSegment = false;
        }else{
            body.pop_back();
            }    
        }
        void Reset(){
            body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
            direction = {1, 0};
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
            DrawTexture(texture, offset + position.x*cellSize, offset + position.y*cellSize, green);
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
        bool running = true;

        void Draw(){
            snake.Draw();
            food.Draw();
        }
        void Update(){
            if (running){
                snake.Update();
                CheckCollisionWithFood();
                CheckCollisionWithEdges();
                CheckCollisionWithTail();
            }

        }
        void CheckCollisionWithFood(){
            if(Vector2Equals(snake.body[0], food.position)){
                food.position = food.GenerateRandomPos(snake.body);
                snake.addSegment = true;
            }
            
        }
        void CheckCollisionWithEdges(){
            if(snake.body[0].x == cellCount || snake.body[0].x == -1){
                GameOver();
            }
            if(snake.body[0].y == cellCount || snake.body[0].y == -1){
                GameOver();
            }
        }
        void GameOver(){
            snake.Reset();
            food.position = food.GenerateRandomPos(snake.body);
            running = false;
        }
        void CheckCollisionWithTail(){
            deque<Vector2> headlessbody = snake.body;
            headlessbody.pop_front();
            if(ElementInDeque(snake.body[0], headlessbody)){
                GameOver();
            }
        }

    };
int main(){

    InitWindow(2*offset + cellSize * cellCount, 2*offset + cellSize * cellCount, "Retro Snake");
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
            game.running = true;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction= {0, 1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_LEFT)  && game.snake.direction.x != 1){
            game.snake.direction= {-1, 0};
            game.running = true;
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction= {1, 0};
            game.running = true;
        }
        

        // dawing
        ClearBackground(green);
        game.Draw();  // calling draw method of game object
        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellSize*cellCount+10, (float)cellSize*cellCount+10}, 5, darkGreen);
        DrawText("Forest Snake", offset -5, 10, 20, darkGreen);

        EndDrawing();
    }


    CloseWindow();
    return 0;
}