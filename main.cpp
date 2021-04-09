# include <iostream>
# include <SDL.h>
# include <SDL_ttf.h>

using namespace std;

#define WIDTH 720
#define HEIGHT 720
#define FONT_SIZE 32

SDL_Renderer* renderer;
SDL_Window* window;
TTF_Font* font;
SDL_Color color;
bool running;
int frameCount, timerFPS, lastFrame, fps;
static int lastTime = 0;

void render() {
	frameCount++;
	timerFPS = SDL_GetTicks();
	if (timerFPS < (1000 / 60)) {
		SDL_Delay((1000 / 60) - timerFPS);
	}
}

void update() {}
void input() {}


int main() {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) cout << "Failed at SDL_Init()" << endl;
	if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) cout << "Failed at SDL_CreateWindowAndRenderer()" << endl;

	TTF_Init();
	font = TTF_OpenFont("Peepo.ttf", FONT_SIZE);	

	running = 1;
	while (running) {
		lastFrame = SDL_GetTicks();
		if (lastFrame >= (lastTime + 1000)) {
			lastTime = lastFrame;
			fps = frameCount;
			frameCount = 0;
		}
		update();
		input();
		render();
	}
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

