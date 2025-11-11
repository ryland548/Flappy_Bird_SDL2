#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class GameOver {
    public:
        double x = 500.0;
        double y = 800.0;
        int width = 192 * 1.2;
        int height = 42 * 1.2;
        int speed = 350;
        bool animation = false;
        SDL_Surface* spriteSurface;
        SDL_Texture* spriteTexture;

    bool load(SDL_Renderer* renderer) {
        spriteSurface = IMG_Load("./Assets/Images/gameover.png");
        if (spriteSurface == NULL) {
            std::cerr << "Error loading Game Over Image.\n";
            return false;
        }
        spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
        SDL_FreeSurface(spriteSurface);
        return true;
    }

    void render(SDL_Renderer* renderer) {
        SDL_Rect spriteRect;
        spriteRect.x = x - (width / 2);
        spriteRect.y = y - (height / 2);
        spriteRect.w = width;
        spriteRect.h = height;
        SDL_RenderCopy(renderer, spriteTexture, NULL, &spriteRect);
    }

    void move(float deltaTime) {
        if (y > 400 && !animation) {
            y -= speed * deltaTime;
        } else {
            animation = true;
        }
    }
};
