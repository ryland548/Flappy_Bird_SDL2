#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Score {
    public:
        double x = 35.0;
        double y = 15.0;
        int width = 96;
        int height = 96;
        int fontSize = 96;
        int score = 0;
        TTF_Font* font = NULL;
        SDL_Surface* textSurface;
        SDL_Texture* textTexture;
        SDL_Color textColor = {255, 255, 255, 0};

    bool load(SDL_Renderer* renderer) {
        font = TTF_OpenFont("./Assets/Fonts/FlappyBird.ttf", fontSize);
        if (!font) {
            std::cerr << "Error loading Score Font.\n";
            return false;
        }
        std::string text = std::to_string(score);
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

    void render(SDL_Renderer* renderer, int changedScore) {
        if (changedScore != score) {
            score = changedScore;
            std::string text = std::to_string(score);
            textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
            if (!textSurface) {
                std::cerr << "Error: Text Surface Could not be loaded\n";
            }
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);
            if (!textTexture) {
                std::cerr << "Error: Text Texture Could Not Be Loaded\n";
            }
        }
        SDL_Rect textRect;
        textRect.x = x;
        textRect.y = y;

        if (score < 10) {
            textRect.w = 96 / 2;
        } else if (score >= 10 && score < 100) {
            textRect.w = 96;
        } else if (score >= 100) {
            textRect.w = 96 * 1.5;
        }
        textRect.h = height;
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }
};
