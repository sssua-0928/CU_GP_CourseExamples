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

#include "DrawingTechniques_GameFunc.h"
#include <windows.h>
#include "SDL_image.h"
#include <vector>
using namespace std;

int g_input;	//로켓의 입력값
std::string g_output, m_output;	//로켓의 출력값
int g_X, g_Y;	// 로켓의 위치값
int m_number = 10;	// 미사일 개수

typedef struct missile {	// 미사일 구조체
	int m_X, m_Y;	// 미사일의 위치값
	bool m_input = false;	// 미사일의 출력값
}missile;

vector<missile> mv;	//미사일 구조체 벡터

// 흘러간 시간 기록
double g_elapsed_time_ms;

// 배경 이미지
SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;
SDL_Texture* g_bg_texture;


int g_oryugen_sprite_num;
int g_current_oryugen_id;

SDL_Texture* g_rocket_texture;
SDL_Rect g_oryugen_source_rects[6];
SDL_Rect g_rocket_destination_rect;

int g_image_mode;
int g_time_mode;


void InitGame() {
	g_flag_running = true;

	// BG
	SDL_Surface* bg_surface = IMG_Load("../../Resources/background.jpg");
	g_bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);

	g_bg_source_rect.x = 0;
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = 1280;
	g_bg_source_rect.h = 720;

	g_bg_destination_rect.x = 0;
	g_bg_destination_rect.y = 0;
	g_bg_destination_rect.w = 1280;
	g_bg_destination_rect.h = 720;



	// Ryu Character
	g_oryugen_sprite_num = 6;
	g_current_oryugen_id = 0;

	SDL_Surface* rocket_surface = IMG_Load("../../Resources/character.png");
	SDL_SetColorKey(rocket_surface, SDL_TRUE, SDL_MapRGB(rocket_surface->format, 0, 0, 248));

	g_rocket_texture = SDL_CreateTextureFromSurface(g_renderer, rocket_surface);
	SDL_FreeSurface(rocket_surface);

	g_oryugen_source_rects[0] = { 7  , 1647, 69, 140 };
	g_oryugen_source_rects[1] = { 94 , 1647, 76, 140 };
	g_oryugen_source_rects[2] = { 171, 1647, 68, 140 };
	g_oryugen_source_rects[3] = { 240, 1647, 61, 140 };
	g_oryugen_source_rects[4] = { 312, 1647, 54, 140 };
	g_oryugen_source_rects[5] = { 390, 1647, 67, 140 };


	g_rocket_destination_rect.x = 300;
	g_rocket_destination_rect.y = 200;
	g_rocket_destination_rect.w = g_oryugen_source_rects[0].w;
	g_rocket_destination_rect.h = g_oryugen_source_rects[0].h;


	g_image_mode = 0;
	g_time_mode = 0;
}

void HandleEvents() {
	// Handle Events
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			g_flag_running = false;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				g_image_mode++;
				if (g_image_mode >= 3) g_image_mode = 0;
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				g_time_mode++;
				if (g_time_mode >= 3) g_time_mode = 0;
			}
		}
	}
}

void Update() {
	// Update

	g_current_oryugen_id++;
	if (g_current_oryugen_id >= g_oryugen_sprite_num) {
		g_current_oryugen_id = 0;
	}

	if (g_time_mode == 0) {
		g_frame_per_sec = 30;
	}
	else if (g_time_mode == 1) {
		g_frame_per_sec = 15;
	}
	else {
		g_frame_per_sec = 10;
	}

}

void Render() {

	// Background
	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);

	// Character
	// g_rocket_texture
	if (g_image_mode == 0) {
		SDL_SetTextureColorMod(g_rocket_texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(g_rocket_texture, 255);
	}
	else if (g_image_mode == 1) {
		SDL_SetTextureColorMod(g_rocket_texture, 255, 255, 0);
		SDL_SetTextureAlphaMod(g_rocket_texture, 100);
	}
	else {
		SDL_SetTextureColorMod(g_rocket_texture, 255, 0, 0);
		SDL_SetTextureAlphaMod(g_rocket_texture, 170);
	}


	// 왼쪽 류
	SDL_Rect r = g_rocket_destination_rect;
	r.w = g_oryugen_source_rects[g_current_oryugen_id].w * 2;	// 가로 2배 크게
	r.h = g_oryugen_source_rects[g_current_oryugen_id].h * 2;	// 세로 2배 크게

	SDL_RenderCopy(g_renderer,
		g_rocket_texture,
		&g_oryugen_source_rects[g_current_oryugen_id],
		&r);


	// 오른쪽 류
	r.x += 200;	// 위치이동

	SDL_RenderCopyEx(g_renderer,
		g_rocket_texture,
		&g_oryugen_source_rects[g_current_oryugen_id],
		&r,
		0, 0, SDL_FLIP_HORIZONTAL);
	// 좌우 반전, 아래 링크 참조
	// https://wiki.libsdl.org/SDL_RenderCopyEx


	SDL_RenderPresent(g_renderer);
}

void ClearGame() {
	SDL_DestroyTexture(g_rocket_texture);
	SDL_DestroyTexture(g_bg_texture);
}