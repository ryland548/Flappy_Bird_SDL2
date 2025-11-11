#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Ground {
    public:
        double offset = 250.0;
        int groundCount = 4;
        int y = 725;
        int width = 500;
        int height = 150;
        int speed = 250;
        SDL_Texture* spriteTexture = NULL;
        SDL_Surface* spriteSurface = NULL;

    bool load(SDL_Renderer* renderer) {
        spriteSurface = IMG_Load("./Assets/Images/base.png");
        if (spriteSurface == NULL) {
            std::cerr << "Error loading Ground Image.\n";
            return false;
        }
        spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
        SDL_FreeSurface(spriteSurface);
        return true;
    }

    bool render(SDL_Renderer* renderer, SDL_Rect& birdSpriteRect) {
        bool collided = false;
        for (int i = 0; i < groundCount; i += 1) {
            double offsetMove = std::round(offset + i * width - width / 2);
            SDL_Rect spriteRect;
            spriteRect.x = static_cast<int>(offsetMove);
            spriteRect.y = y - (height / 2);
            spriteRect.w = width;
            spriteRect.h = height;
            SDL_RenderCopy(renderer, spriteTexture, NULL, &spriteRect);
            if (collide(spriteRect, birdSpriteRect)) {
                collided = true;
            }
        }
        return collided;
    }

    bool collide(SDL_Rect& groundSpriteRect, SDL_Rect& birdSpriteRect) {
        if (SDL_HasIntersection(&birdSpriteRect, &groundSpriteRect)) {
            return true;
        }
        return false;
    }

    void move(float deltaTime) {
        offset -= speed * deltaTime;
        offset = std::round(offset);
        if (offset <= -width) {
            offset += width;
        }
    }
};
