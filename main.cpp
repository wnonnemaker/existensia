#include <raylib.h>
#include <iostream>
#include <cmath>

enum SpriteDirection {
    Left, 
    Right,
    Up,
    Down,
    Idle,
};

typedef struct Sprite {
    Texture2D texture;
    Rectangle dest_rec;
    Vector2 vel;
    SpriteDirection dir;
}Sprite;

void move_player(Sprite* player) {
    player->vel.x = 0.0f;
    player->vel.y = 0.0f;
    float def_vel = 175.0;
    player->dir = SpriteDirection::Idle;
    if(IsKeyDown(KEY_D)) {
        player->vel.x = def_vel;
        player->dir = SpriteDirection::Right;
    }
    if(IsKeyDown(KEY_A)) {
        player->vel.x = -def_vel;
        player->dir = SpriteDirection::Left;
    }
    if(IsKeyDown(KEY_S)) {
        player->vel.y = def_vel;
        player->dir = SpriteDirection::Down;
    }
    if(IsKeyDown(KEY_W)) {
        player->vel.y = -def_vel;
        player->dir = SpriteDirection::Up;
    }
}

void apply_velocity(Sprite* sprite) {
    //normalize the vector
    float veclength = sqrt(pow(sprite->vel.x, 2.0) + pow(sprite->vel.y, 2.0));
    sprite->dest_rec.x += sprite->vel.x / veclength * GetFrameTime();
    sprite->dest_rec.y += sprite->vel.y / veclength * GetFrameTime();
}

int main() {


    // Some initialization 
    const int screenWidth = 600;
    const int middleScreenWidth = screenWidth / 2;
    const int screenHeight = 800;
    const int middleScreenHeight = screenHeight /2;
    InitWindow(screenWidth, screenHeight, "mywindow");

    //Load main hero textures
    Texture2D player_idle = LoadTexture("assets/hero_idle.png"); 
    Texture2D player_walking_right = LoadTexture("assets/hero_walking_right.png"); 
    Texture2D player_walking_left = LoadTexture("assets/hero_walking_left.png"); 
    Texture2D player_walking_up = LoadTexture("assets/hero_walking_up.png"); 
    Texture2D player_walking_down = LoadTexture("assets/hero_walking_down.png"); 
    Texture2D girl_idle = LoadTexture("assets/girl_idle.png"); 

    const int animframes = 8;

    Rectangle src_rec = {0.0f, 0.0f, (float) player_idle.width, (float) player_idle.height,};
    Rectangle girl_rec = {0.0f, 0.0f, (float) player_idle.width, (float) player_idle.height,};
    Rectangle frame_rec = {0.0f, 0.0f, (float) player_walking_right.width/animframes, (float) player_walking_right.height,};
    Vector2 position = {(float) middleScreenWidth, (float) middleScreenHeight,};
    
    Sprite player = {
        .texture = player_idle,
        .dest_rec = {
            .x = (float) middleScreenWidth,
            .y = (float) middleScreenHeight - 100,
            .width = 160,
            .height = 160,
        },
        .dir = SpriteDirection::Idle,
    };

    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 6;

    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        //Update

        //need to do player movement to calculate reverse 
        //frame processing kinda jank but i am lazy
        move_player(&player);
        apply_velocity(&player);

        framesCounter++;

        if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame >= animframes) currentFrame = 0;

            frame_rec.x = 
               (float)currentFrame*(float)player_walking_right.width/animframes;
        }

        //Drawing 

        switch (player.dir) {
            case SpriteDirection::Right:
                player.texture = player_walking_right;
                break;
            case SpriteDirection::Left:
                player.texture = player_walking_left;
                break;
            case SpriteDirection::Up:
                player.texture = player_walking_up;
                break;
            case SpriteDirection::Down:
                player.texture = player_walking_down;
                break;
            case SpriteDirection::Idle:
                frame_rec.x = player_idle.width;
                player.texture = player_idle;
                break;
            default:
                player.texture = player_idle;
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);

        DrawText("Welcome to the game", 190, 200, 20, BLACK);
        DrawText(TextFormat("destrec x: %f destrec y: %f", player.dest_rec.x, player.dest_rec.y), 200, 400, 20, BLACK);
        
        //This is just to see frame data
        /*DrawTexture(player_walking_left, 15, 40, WHITE);
        DrawRectangleLines(15, 40, player_walking_right.width, player_walking_right.height, LIME);
        DrawRectangleLines(15 + (int)frame_rec.x, 40 + (int)frame_rec.y, 
                (int)frame_rec.width, (int)frame_rec.height, RED);
        */ 
        //Draw girl

        DrawTexturePro(girl_idle, girl_rec, {60.0, 300.0, 200.0, 200.0}, {0, 0}, 0.0, WHITE);
        //
        //Now actually draw this dude
        //
        //
        DrawTexturePro(player.texture, frame_rec, player.dest_rec, {0, 0}, 0.0, WHITE);
        EndDrawing();
    }

    UnloadTexture(player_idle);
    UnloadTexture(girl_idle);
    UnloadTexture(player_walking_right);
    UnloadTexture(player_walking_left);
    UnloadTexture(player_walking_down);
    UnloadTexture(player_walking_up);
    CloseWindow();

    return 0;
}
