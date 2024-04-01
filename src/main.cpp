#include<iostream>
#include <raylib.h>  // Including the Raylib library for graphics and input handling
#include<deque>  // Including deque for using double-ended queue
#include<raymath.h>  // Including raymath for vector math operations

using namespace std;
    // Defining colors for the game
    Color green= {173, 204, 96, 255};
    Color darkGreen= {43, 51, 24, 255};

    // Constants for the game
    int cellSize = 22;
    int cellCount = 25;
    int offset = 40; // width of border

    static bool allowMove = false;  //Flag to allow movement

    double LastUpdateTime= 0 ;  //Keeps track of time at which last update of snake occured

    // Function to check if an element is in a deque
    bool ElementInDeque(Vector2 element, deque<Vector2> deque){
        for (unsigned int i=0; i< deque.size(); i++){
            if(Vector2Equals(deque[i], element)){
                return true;
            }
        }
        return false;
    }
    // Function to check if a certain time interval has passed
    bool eventTriggered(double interval){
        double currentTime= GetTime();
        if(currentTime - LastUpdateTime >= interval){
            LastUpdateTime = currentTime;
            return true;
        }
        return false;
    }
    // Class representing the Snake
    class Snake{
        public:
        deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        Vector2 direction = {1, 0};
        bool addSegment = false;
        // Method to draw the snake  
        void Draw(){
            for(unsigned int  i=0; i<body.size(); i++){
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segement = Rectangle{offset + x*cellSize, offset + y*cellSize, (float)cellSize, (float)cellSize};
                DrawRectangleRounded(segement, 0.5, 6, darkGreen);

            }
        }
        // Method to update the snake's position
        void Update(){  
        body.push_front(Vector2Add(body[0], direction)); // moving snake & adding body part
        if(addSegment == true){
            addSegment = false;
        }else{
            body.pop_back();
            }    
        }
        // Method to reset the snake's position and direction
        void Reset(){
            body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
            direction = {1, 0};
        }
    };
    // Class representing the Food
    class Food{
        public:
        Vector2 position;  // Food's position
        Texture2D texture; // Food's texture
        // Constructor to initialize the food
        Food(deque<Vector2> snakebody){  
            Image image = LoadImage("graphics/food.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            position = GenerateRandomPos(snakebody);
        }
        // Destructor to unload the texture
        ~Food(){
            UnloadTexture(texture);
        }
        // Method to draw the food
        void Draw(){
            DrawTexture(texture, offset + position.x*cellSize, offset + position.y*cellSize, green);
        }
        Vector2 GenerateRandomCell(){
            float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            return Vector2{x, y};

        }
        // Method to generate a random position for the food
        Vector2 GenerateRandomPos(deque <Vector2> snakebody){
            Vector2 position = GenerateRandomCell();

            while (ElementInDeque(position, snakebody)){  // prevents food from overlap on snake body
                position = GenerateRandomCell();
            }
            return position;
        }
    };
    // Class representing the game
    class Game{
        public:
        Snake snake = Snake();   // The snake object
        Food food = Food(snake.body);   // The food object
        bool running = true;  // Flag to indicate if the game is running
        int score = 0;  // The player's score
        Sound eatSound;  // Sound played when the snake eats food
        Sound wallSound;  // Sound played when the snake hits the wall

        // Constructor to initialize audio and sounds
        Game(){
            InitAudioDevice();
            eatSound = LoadSound("Sounds/eat.mp3");
            wallSound = LoadSound("Sounds/wall.mp3");
        }
        // Destructor to unload sounds and close audio device
        ~Game(){
            UnloadSound(eatSound);
            UnloadSound(wallSound);
            CloseAudioDevice(); 
        }

        // Method to draw the game objects
        void Draw(){
            snake.Draw();
            food.Draw();
        }

        // Method to update the game state
        void Update(){
            if (running){
                snake.Update();
                CheckCollisionWithFood();
                CheckCollisionWithEdges();
                CheckCollisionWithTail();
            }
        }

        // Method to check collision with the food
        void CheckCollisionWithFood(){
            if(Vector2Equals(snake.body[0], food.position)){
                food.position = food.GenerateRandomPos(snake.body);
                snake.addSegment = true;
                score ++;
                PlaySound(eatSound);
            }    
        }

        // Method to check collision with the edges of the game screen
        void CheckCollisionWithEdges(){
            if(snake.body[0].x == cellCount || snake.body[0].x == -1){
                GameOver();
            }
            if(snake.body[0].y == cellCount || snake.body[0].y == -1){
                GameOver();
            }
        }

        // Method to handle game over
        void GameOver(){
            snake.Reset();
            food.position = food.GenerateRandomPos(snake.body);
            running = false;
            score = 0;
            PlaySound(wallSound);
        }

        // Method to check collision with the snake's tail
        void CheckCollisionWithTail(){
            deque<Vector2> headlessbody = snake.body;
            headlessbody.pop_front();
            if(ElementInDeque(snake.body[0], headlessbody)){
                GameOver();
            }
        }

    };
int main(){

    InitWindow(2*offset + cellSize * cellCount, 2*offset + cellSize * cellCount, "Forest Snake");
    SetTargetFPS(60);  // defining frame rate means this game will run at same rate on every computer
    Game game = Game();  // Create game object

    // Main game loop
    while(WindowShouldClose() == false){
        BeginDrawing();
        if(eventTriggered(0.2)){
            allowMove = true; 
            game.Update();
        }

         // Handle user input for snake movement
        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction= {0, -1};
            game.running = true;
            allowMove = false;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction= {0, 1};
            game.running = true;
            allowMove = false;
        }
        if(IsKeyPressed(KEY_LEFT)  && game.snake.direction.x != 1){
            game.snake.direction= {-1, 0};
            game.running = true;
            allowMove = false;
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction= {1, 0};
            game.running = true;
            allowMove = false;
        }
        
        // drawing
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellSize*cellCount+10, (float)cellSize*cellCount+10}, 5, darkGreen);
        DrawText("Forest Snake", offset -5, 10, 20, darkGreen);
        DrawText("Score : ", offset -5, offset+cellSize*cellCount+10, 20, darkGreen);
        DrawText(TextFormat("%i", game.score), offset +80, offset+cellSize*cellCount+10, 20, darkGreen);

        game.Draw();  // calling draw method of game object

        EndDrawing();
    }
    CloseWindow();  // Close window
    return 0;
} 