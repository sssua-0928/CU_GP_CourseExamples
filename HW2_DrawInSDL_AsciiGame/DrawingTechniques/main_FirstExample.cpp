#include "GameFunc.h"

bool g_flag_running;
Uint32 g_last_time_ms;

int main(int argc, char* argv[]) {

	// Initializing SDL library : SDL���̺귯������ ���õǾ���ϴ� �⺻������ ���õ�
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("First Window", 100, 100, 800, 600, 0);	// X,Y,W(����),Z(����), Flag(�ɼ�:����������)
	InitGame();

	g_last_time_ms = SDL_GetTicks();

	while (g_flag_running) {

		Uint32 cur_time_ms = SDL_GetTicks();

		if (cur_time_ms - g_last_time_ms < 33)
			continue;

		HandleEvents();
		Update();
		Render();

		//SDL_Delay(30);	// 30 �и������� ��ٸ���.
		g_last_time_ms = cur_time_ms;
	}

	ClearGame();
	SDL_DestroyWindow(window);	// ���α׷� �޸� ������ �����ؼ� �� �ʿ� (�����Ҵ� �޸� ����)
	SDL_Quit();

	return 0;
}

