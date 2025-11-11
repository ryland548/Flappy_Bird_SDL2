#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class RestartMsg {
    public:
        double x = 377.0;
        double y = 450.0;
        int width = 250;
        int height = 30;
        int fontSize = 96;
        TTF_Font* font = NULL;
        SDL_Surface* textSurface;
        SDL_Texture* textTexture;
        SDL_Color textColor = {255, 215, 181, 0};

    bool load(SDL_Renderer* renderer) {
        font = TTF_OpenFont("./Assets/Fonts/FlappyBird.ttf", fontSize);
        if (!font) {
            std::cerr << "Error loading Score Font.\n";
            return false;
        }
        std::string text = "Press R To Restart";
        textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
        if (!textSurface) {
            std::cerr << "Error: Text Surface Could not be loaded\n";
            return false;
        }
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        if (!textTexture) {
            std::cerr << "Error: Text Texture Could Not Be Loaded\n";
            return false;
        }
        return true;
    }

    void render(SDL_Renderer* renderer) {
        SDL_Rect textRect;
        textRect.x = x;
        textRect.y = y;
        textRect.w = width;
        textRect.h = height;
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }
};
