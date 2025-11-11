#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class StartMsg {
    public:
        double x = 500.0;
        double y = 400.0;
        int width = 184 * 1.1;
        int height = 267 * 1.1;
        SDL_Surface* spriteSurface;
        SDL_Texture* spriteTexture;

    bool load(SDL_Renderer* renderer) {
        spriteSurface = IMG_Load("./Assets/Images/message.png");
        if (spriteSurface == NULL) {
            std::cerr << "Error loading Start Msg Image.\n";
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
};
