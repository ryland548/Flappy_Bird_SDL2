#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class PipePair {
    public:
        double x = 1200.0;
        double topY = 0.0;
        double bottomY = 0.0;
        bool remove = false;
        int gap = 580;
        int width = 85;
        int height = 430;
        int speed = 250;
        bool point = false;
        SDL_Texture* spriteTexture = NULL;
        SDL_Rect spriteRectTop;
        SDL_Rect spriteRectBottom;

    bool load(SDL_Texture* loadedTexture, int pipeVariationValue) {
        topY = pipeVariationValue;
        bottomY = topY + gap;
        spriteTexture = loadedTexture;
        return true;
    }

    void render(SDL_Renderer* renderer) {
        spriteRectTop.x = x - (width / 2);
        spriteRectTop.y = topY - (height / 2);
        spriteRectTop.w = width;
        spriteRectTop.h = height;
        spriteRectBottom.x = x - (width / 2);
        spriteRectBottom.y = bottomY - (height / 2);
        spriteRectBottom.w = width;
        spriteRectBottom.h = height;
        SDL_RenderCopyEx(renderer, spriteTexture, NULL, &spriteRectTop, 180, NULL, SDL_FLIP_HORIZONTAL);
        SDL_RenderCopy(renderer, spriteTexture, NULL, &spriteRectBottom);
    }

    void move(float deltaTime) {
        x -= speed * deltaTime;
    }

    bool collision(SDL_Rect& birdSpriteRect) {
        if (SDL_HasIntersection(&spriteRectTop, &birdSpriteRect)) {
            return true;
        }
        if (SDL_HasIntersection(&spriteRectBottom, &birdSpriteRect)) {
            return true;
        }
        return false;
    }
};
