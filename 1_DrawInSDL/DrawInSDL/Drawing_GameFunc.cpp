#include "Drawing_GameFunc.h"
#include <windows.h>
#include "SDL_image.h"

int g_input;	// g_는 전역변수의 의미
int g_elapsed_time_ms;

SDL_Rect g_char_pos;
SDL_Texture* g_ryu_sheet_texture;
SDL_Rect g_source_rect;
SDL_Rect g_destination_rect;


void InitGame() {
	g_input = 0;
	g_flag_running = true;

	g_char_pos.x = 100;
	g_char_pos.y = 100;
	g_char_pos.w = 50;
	g_char_pos.h = 100;

	g_elapsed_time_ms = 0;

	// 이미지 추가 준비단계
	SDL_Surface* ryu_sheet_surface = IMG_Load("../../Resources/60224.png");	// structure의 포인터 가져오기
	g_ryu_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, ryu_sheet_surface);	//GPU로 옮기기 (GPU메모리로 복사)
	SDL_FreeSurface(ryu_sheet_surface);	// (GPU에 옮겼으니까)CPU의 Surface에 할당된 메모리 해제

	// 캐릭터의 위치값
	g_source_rect.x = 171;
	g_source_rect.y = 1647;
	g_source_rect.w = 67;
	g_source_rect.h = 140;

	g_destination_rect.x = 300;
	g_destination_rect.y = 200;
	g_destination_rect.w = g_source_rect.w;	//잘라오는 크기랑 똑같이
	g_destination_rect.h = g_source_rect.h;
}

void HandleEvents() {
	// Handle Events
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			g_flag_running = false;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_LEFT) {
				g_input = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_input = 2;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {
				g_input = 3;
			}
		}
		else if (event.type == SDL_KEYUP) {
			g_input = 0;
		}
	}
}

void Update() {
	// Update
	if (g_input == 1) {
		g_char_pos.w = 50;
		g_char_pos.h = 200;
	}
	else if (g_input == 2) {
		g_char_pos.w = 200;
		g_char_pos.h = 100;
	}
	else if (g_input == 3) {
		g_char_pos.w = 10;
		g_char_pos.h = 10;
	}
	else {
		g_char_pos.w = 50;
		g_char_pos.h = 100;
	}

	g_char_pos.x += 10;
	if (g_char_pos.x > 800 - 50)
		g_char_pos.x = 0;

	g_elapsed_time_ms += 33;
}

void Render() {

	// Background
	SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
	SDL_RenderClear(g_renderer);

	// Character
	SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(g_renderer, &g_char_pos);

	// g_ryu_sheet_texture
	SDL_RenderCopy(g_renderer, g_ryu_sheet_texture, &g_source_rect, &g_destination_rect);	// Texture로부터 복사해서 표시(잘라올 곳, 붙일 곳) / 백버퍼에 표시


	SDL_RenderPresent(g_renderer);	// 백버퍼-프론트버퍼 교체
}

void ClearGame() {	// 게임 종료시 메모리 해제
	SDL_DestroyTexture(g_ryu_sheet_texture);
}