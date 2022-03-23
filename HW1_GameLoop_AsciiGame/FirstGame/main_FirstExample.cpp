#include "GameFunc.h"

bool g_flag_running;
Uint32 g_last_time_ms;

int main(int argc, char* argv[]) {

	// Initializing SDL library : SDL라이브러리에서 세팅되어야하는 기본변수가 세팅됨
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("First Window", 100, 100, 800, 600, 0);	// X,Y,W(가로),Z(세로), Flag(옵션:윈도우형태)
	InitGame();

	g_last_time_ms = SDL_GetTicks();

	while (g_flag_running) {

		Uint32 cur_time_ms = SDL_GetTicks();

		if (cur_time_ms - g_last_time_ms < 33)
			continue;

		HandleEvents();
		Update();
		Render();

		//SDL_Delay(30);	// 30 밀리세컨드 기다린다.
		g_last_time_ms = cur_time_ms;
	}

	ClearGame();
	SDL_DestroyWindow(window);	// 프로그램 메모리 부족을 방지해서 꼭 필요 (동적할당 메모리 삭제)
	SDL_Quit();

	return 0;
}

