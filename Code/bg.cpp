#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Background {
    public:
        double x = 215.0;
        double y = 380.0;
        int width = std::round(288 * 1.5);
        int height = std::round(512 * 1.5);
        double x2 = 215.0 + width;
        double x3 = 215.0 + (width * 2);
        SDL_Texture* spriteTexture = NULL;
        SDL_Surface* spriteSurface = NULL;

    bool load(SDL_Renderer* renderer) {
        spriteSurface = IMG_Load("./Assets/Images/background-day.png");
        if (spriteSurface == NULL) {
            std::cerr << "Error loading Background Image.\n";
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
        SDL_Rect spriteRect2;
        spriteRect2.x = x2 - (width / 2);
        spriteRect2.y = y - (height / 2);
        spriteRect2.w = width;
        spriteRect2.h = height;
        SDL_Rect spriteRect3;
        spriteRect3.x = x3 - (width / 2);
        spriteRect3.y = y - (height / 2);
        spriteRect3.w = width;
        spriteRect3.h = height;
        SDL_RenderCopy(renderer, spriteTexture, NULL, &spriteRect);
        SDL_RenderCopy(renderer, spriteTexture, NULL, &spriteRect2);
        SDL_RenderCopy(renderer, spriteTexture, NULL, &spriteRect3);
    }

};
