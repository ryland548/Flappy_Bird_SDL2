#include <iostream>
#include <vector>
#include <random>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "./Code/bird.cpp"
#include "./Code/ground.cpp"
#include "./Code/bg.cpp"
#include "./Code/pipePair.cpp"
#include "./Code/startMsg.cpp"
#include "./Code/gameOver.cpp"
#include "./Code/flappyLabel.cpp"
#include "./Code/restartMsg.cpp"
#include "./Code/score.cpp"

void restart(Bird &bird, Ground &ground, std::vector<PipePair> &pipes, int &scoreNum, bool &hitSoundPlayed) {
    bird.dead = false;
    bird.start = false;
    bird.y = 300;
    bird.frame = 1;
    bird.velocity.y = 0;
    bird.angle = 0;
    bird.idleTime = 0.0;
    ground.offset = 250;
    pipes.clear();
    hitSoundPlayed = false;
    scoreNum = 0;
}

void quit(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    Mix_CloseAudio();
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main() {
    int scoreNum = 0;
    const float spawnDelay = 1450;
    const int screenWidth = 1000;
    const int screenHeight = 800;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Color bgColor = {0, 0, 0, 0};
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error: Could not initalize SDL2.\n";
        return 1;
    }
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        std::cerr << "Error: Could not initalize SDL2 Audio.\n";
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        std::cerr << "Error: Could not initalize SDL2 Audio.\n";
        return 1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "Error: Could not initalize SDL2 Font.\n";
    }
    window = SDL_CreateWindow(
        "Flappy Bird",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        std::cerr << "Error: Could not create SDL2 Window.\n";
        quit(window, renderer);
        return 1;
    }
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (renderer == NULL) {
        std::cerr << "Error: Could not create SDL2 Renderer.\n";
        quit(window, renderer);
        return 1;
    }
    Bird bird = Bird();
    if (!bird.load(renderer)) {
        quit(window, renderer);
        return 1;
    }
    Ground ground = Ground();
    if (!ground.load(renderer)) {
        quit(window, renderer);
        return 1;
    }
    Background background = Background();
    if (!background.load(renderer)) {
        quit(window, renderer);
        return 1;
    }
    StartMsg startMsg = StartMsg();
    if (!startMsg.load(renderer)) {
        quit(window, renderer);
        return 1;
    }
    GameOver gameOver = GameOver();
    if (!gameOver.load(renderer)) {
        quit(window, renderer);
        return 1;
    }
    FlappyLabel flappyLabel = FlappyLabel();
    if (!flappyLabel.load(renderer)) {
        quit(window, renderer);
        return 1;
    }
    Score score = Score();
    if (!score.load(renderer)) {
        quit(window, renderer);
        return 1;
    }
    RestartMsg restartMsg = RestartMsg();
    if (!restartMsg.load(renderer)) {
        quit(window, renderer);
        return 1;
    }
    Mix_Chunk* jumpSound = NULL;
    jumpSound = Mix_LoadWAV("./Assets/Audio/wing.wav");
    if (jumpSound == NULL) {
        quit(window, renderer);
        return 1;
    }
    Mix_Chunk* pointSound = NULL;
    pointSound = Mix_LoadWAV("./Assets/Audio/point.wav");
    if (pointSound == NULL) {
        quit(window, renderer);
        return 1;
    }
    Mix_Chunk* hitSound = NULL;
    hitSound = Mix_LoadWAV("./Assets/Audio/hit.wav");
    if (hitSound == NULL) {
        quit(window, renderer);
        return 1;
    }
    bool hitSoundPlayed = false;
    std::random_device rd;
    std::mt19937 gen(rd());
    SDL_Surface* pipeSurface = IMG_Load("./Assets/Images/pipe.png");
    SDL_Texture* pipeTexture = SDL_CreateTextureFromSurface(renderer, pipeSurface);
    SDL_FreeSurface(pipeSurface);
    std::vector<PipePair> pipes;
    Uint32 lastPipeSpawn = SDL_GetTicks();
    const Uint32 frameCapMillisecond = 1000 / 90;
    bool running = true;
    SDL_Event event;
    Uint32 lastTick = SDL_GetTicks();
    while (running) {
        Uint32 frameCapStart = SDL_GetTicks();
        Uint32 currentTick = SDL_GetTicks();
        float deltaTime = static_cast<float>(currentTick - lastTick) / 1000.0;
        lastTick = currentTick;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_UP) {
                        if (!bird.dead) {
                            bird.jump();
                            Mix_PlayChannel(-1, jumpSound, 0);
                            if (!bird.start) {
                                bird.start = true;
                            }
                        }
                    }
                    if (event.key.keysym.sym == SDLK_r) {
                        if (bird.dead) {
                            restart(bird, ground, pipes, scoreNum, hitSoundPlayed);
                        }
                    }
            }
        }
        if (currentTick - lastPipeSpawn >= spawnDelay && bird.start && !bird.dead) {
            std::uniform_int_distribution<> distrib(-130, 200);
            PipePair pipe = PipePair();
            pipe.load(pipeTexture, distrib(gen));
            pipes.push_back(pipe);
            lastPipeSpawn = currentTick;
        }
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        background.render(renderer);
        bird.render(renderer);
        if (bird.start && !bird.dead) {
            bird.move(deltaTime);
            bird.rotate(deltaTime);
            bird.animation(deltaTime);
        }
        if (!bird.start) {
            bird.idleMove(deltaTime);
        }
        for (PipePair &pipe: pipes) {
            if (pipe.x <= -200) {
                pipe.remove = true;
            }
            pipe.render(renderer);
            if (pipe.x <= 150 && bird.y <= -50) {
                bird.dead = true;
            }
            if (pipe.x <= 150 && !bird.dead && !pipe.point) {
                Mix_PlayChannel(-1, pointSound, 0);
                scoreNum += 1;
                pipe.point = true;
            }
            if (!bird.dead) {
                pipe.move(deltaTime);
            }
            if (pipe.x <= 500) {
                if (pipe.collision(bird.spriteRect)) {
                    bird.dead = true;
                    if (!hitSoundPlayed) {
                        hitSoundPlayed = true;
                        Mix_PlayChannel(-1, hitSound, 0);
                    }
                }
            }
        }
        score.render(renderer, scoreNum);
        pipes.erase(
            std::remove_if(
                pipes.begin(), pipes.end(), [](const PipePair& pipe) { return pipe.remove; }
            ),
            pipes.end()
        );

        if (!bird.dead) {
            ground.move(deltaTime);
        }
        if (ground.render(renderer, bird.spriteRect)) {
            bird.dead = true;
            if (!hitSoundPlayed) {
                hitSoundPlayed = true;
                Mix_PlayChannel(-1, hitSound, 0);
            }
        }
        if (!bird.start) {
            startMsg.render(renderer);
        }
        if (bird.dead) {
            gameOver.render(renderer);
            gameOver.move(deltaTime);
            if (gameOver.animation) {
                flappyLabel.render(renderer);
                restartMsg.render(renderer);
            }
        }
        SDL_RenderPresent(renderer);
        Uint32 frameElapsed = SDL_GetTicks() - frameCapStart;
        if (frameElapsed < frameCapMillisecond) {
            SDL_Delay(frameCapMillisecond - frameElapsed);
        }
    }
    quit(window, renderer);
    return 0;
}
