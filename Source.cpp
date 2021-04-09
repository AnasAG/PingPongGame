/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/


# include <iostream>
# include <SDL.h>
# include <SDL_ttf.h>
#include <string>

using namespace std;

#define WIDTH 720
#define HEIGHT 720
#define FONT_SIZE 32
#define SIZE 16
#define BALL_SPEED 16
#define SPEED 9
#define PI 3.14159265358979323846


// Question: explain the "SDL_Renderer* renderer;" structure
SDL_Renderer* renderer;
SDL_Window* window;
TTF_Font* font;
SDL_Color color;
bool running;
int frameCount, timerFPS, lastFrame, fps;

SDL_Rect l_paddle, r_paddle, ball, score_board;
float velX, velY;
string score;
int l_s, r_s;
bool turn;

void serve(){
	l_paddle.y = r_paddle.y = (HEIGHT / 2) - (l_paddle.h / 2);
	if (turn) {
		ball.x = l_paddle.x + (l_paddle.w * 4);
		velX = BALL_SPEED / 2;
	} else {
		ball.x = r_paddle.x - (r_paddle.w * 4);
		velX = -BALL_SPEED / 2;
	}
	velX = BALL_SPEED / 2;
	velY = 0;
	ball.y = HEIGHT / 2 - (SIZE / 2);
	// 
	turn = !turn;
}

void write(string text, int x, int y) {
	SDL_Surface *surface;
	SDL_Texture* texture;
	const char* t = text.c_str();


	// assertion code
	/*
	if (!font) {
		cout << "Problem in Font object" << endl;
	}
	*/
	surface = TTF_RenderText_Solid(font, t, color);
	
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	score_board.w = surface->w;
	score_board.h = surface->h;
	score_board.x = x - score_board.w;
	score_board.y = y - score_board.h;
	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, NULL, &score_board);
	SDL_DestroyTexture(texture);

}


void render() {
	//
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
	SDL_RenderClear(renderer);
	
	frameCount++;
	// SDL_GetTicks -> Gets the number of milliseconds since SDL initialization
	timerFPS = SDL_GetTicks();
	if (timerFPS < (1000 / 60)) {
		// Wait a specified number of milliseconds
		SDL_Delay((1000 / 60) - timerFPS);
	}	
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(renderer, &l_paddle);
	SDL_RenderFillRect(renderer, &r_paddle);
	SDL_RenderFillRect(renderer, &ball);

	write(score, WIDTH / 2 + FONT_SIZE, FONT_SIZE * 2);

	SDL_RenderPresent(renderer);
	
}

void update() {

	if (SDL_HasIntersection(&ball, &r_paddle)) {
		double rel = (r_paddle.y + (r_paddle.h / 2)) - (ball.y + (SIZE / 2));
		double norm = rel / (r_paddle.h / 2);
		double bounce = norm * (5 * PI / 12);

		velX = -BALL_SPEED * cos(bounce);
		velY = BALL_SPEED * -sin(bounce);
	}
	if (SDL_HasIntersection(&ball, &l_paddle)) {
		double rel = (l_paddle.y + (l_paddle.h / 2)) - (ball.y + (SIZE / 2));
		double norm = rel / (l_paddle.h / 2);
		double bounce = norm * (5 * PI / 12);

		velX = BALL_SPEED * cos(bounce);
		velY = BALL_SPEED * -sin(bounce);
	}

	if (ball.y > r_paddle.y + (r_paddle.h / 2)) r_paddle.y += SPEED;
	if (ball.y < r_paddle.y + (r_paddle.h / 2)) r_paddle.y -= SPEED;

	if (ball.x <= 0) { r_s++; serve(); }
	if (ball.x + SIZE >= WIDTH) { l_s++; serve(); }
	
	if (ball.y <= 0 || ball.y + SIZE >= HEIGHT) velY = -velY;
	
	ball.x += velX;
	ball.y += velY;
	
	score = to_string(l_s) + "   " + to_string(r_s);
	
	if (l_paddle.y < 0)l_paddle.y = 0;
	if (l_paddle.y + l_paddle.h > HEIGHT)l_paddle.y = HEIGHT - l_paddle.h;
	if (r_paddle.y < 0)r_paddle.y = 0;
	if (r_paddle.y + r_paddle.h > HEIGHT)r_paddle.y = HEIGHT - r_paddle.h;

}

void input() {
	SDL_Event e;
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&e)) if (e.type == SDL_QUIT) running = false;
	if (keystates[SDL_SCANCODE_ESCAPE]) running = false;
	if (keystates[SDL_SCANCODE_ESCAPE]) l_paddle.y-=SPEED;
	if (keystates[SDL_SCANCODE_ESCAPE]) l_paddle.y += SPEED;
}


int main(int argc, char* argv[]) {

	// to initialize SDL lib
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) cout << "Failed at SDL_Init()" << endl;
	
	if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) cout << "Failed at SDL_CreateWindowAndRenderer()" << endl;

	// initialize TTF
	TTF_Init();
	font = TTF_OpenFont("C:\libs\peepo\Peepo.ttf", FONT_SIZE); // C:\libs\peepo\Peepo.ttf

	running = 1;
	static int lastTime = 0;
	color.r = color.g = color.b = 255;
	
	l_s = r_s = 0;
	l_paddle.x = 32; l_paddle.h = HEIGHT / 4;
	l_paddle.y = (HEIGHT / 2) - (l_paddle.h / 2);
	l_paddle.w = 12;
	r_paddle= l_paddle;
	r_paddle.x = WIDTH - r_paddle.w - 32;
	ball.w = ball.h = SIZE;

	serve();

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
	// Destroy the rendering context for a window and free associated textures
	SDL_DestroyRenderer(renderer);
	// Destroy a window
	SDL_DestroyWindow(window);
	// shuts down all SDL subsystems
	SDL_Quit();

	return 0;
}


