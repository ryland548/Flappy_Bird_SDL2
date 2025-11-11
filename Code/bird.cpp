#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Velocity {
    double x = 0.0;
    double y = 0.0;
};

class Bird {
    public:
        double x = 175.0;
        double y = 300.0;
        double angle = 0.0;
        bool start = false;
        const int width = 65 / 1.2;
        const int height = 46 / 1.2;
        const int jumpPower = 430;
        const int fallSpeed = 1300;
        const int rotationSpeed = 200;
        float frameTimer = 0.f;
        const float frameDelay = 0.2f;
        int frame = 1;
        bool dead = false;
        Velocity velocity = Velocity();
        double idleTime = 0.0;
        const double idleAmplitude = 30.0;
        const double idleFrequency = 3.0;
        double startY = y;
        std::vector<SDL_Texture*> frameTextures;
        SDL_Texture* spriteTextureMid = NULL;
        SDL_Texture* spriteTextureDown = NULL;
        SDL_Texture* spriteTextureUp = NULL;
        SDL_Surface* spriteSurface = NULL;
        SDL_Rect spriteRect;


    bool load(SDL_Renderer* renderer) {
        spriteSurface = IMG_Load("./Assets/Images/yellowbird-midflap.png");
        if (spriteSurface == NULL) {
            std::cerr << "Error loading Bird Image.\n";
            return false;
        }
        spriteTextureMid = SDL_CreateTextureFromSurface(renderer, spriteSurface);

        spriteSurface = IMG_Load("./Assets/Images/yellowbird-downflap.png");
        if (spriteSurface == NULL) {
            std::cerr << "Error loading Bird Image.\n";
            return false;
        }
        spriteTextureDown = SDL_CreateTextureFromSurface(renderer, spriteSurface);

        spriteSurface = IMG_Load("./Assets/Images/yellowbird-upflap.png");
        if (spriteSurface == NULL) {
            std::cerr << "Error loading Bird Image.\n";
            return false;
        }
        spriteTextureUp = SDL_CreateTextureFromSurface(renderer, spriteSurface);

        SDL_FreeSurface(spriteSurface);
        frameTextures.push_back(spriteTextureDown);
        frameTextures.push_back(spriteTextureMid);
        frameTextures.push_back(spriteTextureUp);
        frameTextures.push_back(spriteTextureMid);
        return true;
    }

    void idleMove(float deltaTime) {
        idleTime += deltaTime;
        y = startY + idleAmplitude * sin(idleFrequency * idleTime);
    }

    void render(SDL_Renderer* renderer) {
        spriteRect.x = x - (width / 2);
        spriteRect.y = y - (height / 2);
        spriteRect.w = width;
        spriteRect.h = height;
        SDL_Point spriteCenterPoint = {spriteRect.w / 2, spriteRect.h / 2};
        SDL_RenderCopyEx(renderer, frameTextures[frame], NULL, &spriteRect, angle, &spriteCenterPoint, SDL_FLIP_NONE);
    }

    void move(float deltaTime) {
        velocity.y += fallSpeed * deltaTime;
        y += velocity.y * deltaTime;
    }

    void jump() {
        velocity.y = -jumpPower;
        angle = -60;
    }

    void rotate(float deltaTime) {
        if (angle < 90) {
            angle += rotationSpeed * deltaTime;
        }
    }

    void animation(float deltaTime) {
        frameTimer += deltaTime;
        if (frameTimer >= frameDelay) {
            if (frame <= 2) {
                frame += 1;
            } else {
                frame = 0;
            }
            frameTimer = 0.f;
        }
    }
};
