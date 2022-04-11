#include "DrawingTechniques_GameFunc.h"
#include <windows.h>
#include "SDL_image.h"
#include <vector>
using namespace std;

int g_input;	//로켓의 입력값
int m_number = 10;	// 미사일 개수
double g_elapsed_time_ms;	// 흘러간 시간 기록

// 배경 세팅
SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;
SDL_Texture* g_bg_texture;

// 로켓 세팅
SDL_Texture* g_rocket_texture;
SDL_Rect g_rocket_source_rect;
SDL_Rect g_rocket_destination_rect;

// 미사일(ms) 세팅
typedef struct missile {	// 미사일 구조체
	SDL_Texture* g_ms_texture;
	SDL_Rect g_ms_source_rect;
	SDL_Rect g_ms_destination_rect;
	bool m_input = false;	// 미사일의 입력값
} missile;

vector<missile> mv;	//미사일 구조체 벡터
missile m1;

int g_image_mode;
int g_time_mode;

void InitGame() {
	g_input = 0;
	g_flag_running = true;

	// BG
	SDL_Surface* bg_surface = IMG_Load("../../Resources/background.jpg");
	g_bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);

	g_bg_source_rect.x = 0;
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = 800;
	g_bg_source_rect.h = 600;

	g_bg_destination_rect.x = 0;
	g_bg_destination_rect.y = 0;
	g_bg_destination_rect.w = 1280;
	g_bg_destination_rect.h = 720;

	g_elapsed_time_ms = 0;

	// 캐릭터
	SDL_Surface* rocket_surface = IMG_Load("../../Resources/character.png");	// structure의 포인터 가져오기
	SDL_SetColorKey(rocket_surface, SDL_TRUE, SDL_MapRGB(rocket_surface->format, 0, 0, 255));
	g_rocket_texture = SDL_CreateTextureFromSurface(g_renderer, rocket_surface);	//GPU로 옮기기 (GPU메모리로 복사)
	SDL_FreeSurface(rocket_surface);	// (GPU에 옮겼으니까)CPU의 Surface에 할당된 메모리 해제

	g_rocket_source_rect.x = 0;
	g_rocket_source_rect.y = 0;
	g_rocket_source_rect.w = 155;
	g_rocket_source_rect.h = 169;

	g_rocket_destination_rect.x = 300;
	g_rocket_destination_rect.y = 500;
	g_rocket_destination_rect.w = (g_rocket_source_rect.w)/3;	
	g_rocket_destination_rect.h = (g_rocket_source_rect.h)/3;

	// 미사일
	SDL_Surface* ms_surface = IMG_Load("../../Resources/character.png");	
	SDL_SetColorKey(ms_surface, SDL_TRUE, SDL_MapRGB(ms_surface->format, 0, 0, 255));
	m1.g_ms_texture = SDL_CreateTextureFromSurface(g_renderer, ms_surface);
	SDL_FreeSurface(ms_surface);

	m1.g_ms_source_rect.x = 155;
	m1.g_ms_source_rect.y = 0;
	m1.g_ms_source_rect.w = 98;
	m1.g_ms_source_rect.h = 87;

	m1.g_ms_destination_rect.x = 300;
	m1.g_ms_destination_rect.y = 500;
	m1.g_ms_destination_rect.w = (m1.g_ms_source_rect.w) / 3;
	m1.g_ms_destination_rect.h = (m1.g_ms_source_rect.h) / 3;
}



void HandleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_LEFT) {
				g_input = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_input = 2;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				g_input = 3;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				g_input = 4;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {
				g_input = 5;
			}
			break;

		case SDL_KEYUP:
			g_input = 0;
			break;

		default:
			break;
		}
	}
}

void Update() {
	// Update
	if (g_input == 1) {	// g1 왼쪽
		g_rocket_destination_rect.x -= 10;
	}
	else if (g_input == 2) {	// g2 오른쪽
		g_rocket_destination_rect.x += 10;
	}
	else if (g_input == 3) {	// g3 앞
		g_rocket_destination_rect.y -= 10;
	}
	else if (g_input == 4) {	// g4 뒤
		g_rocket_destination_rect.y += 10;
	}

	if (g_input == 5) {	// g5 스페이스-미사일발사!
		missile m1;
		m1.g_ms_destination_rect.x = g_rocket_destination_rect.x;
		m1.g_ms_destination_rect.y = g_rocket_destination_rect.y -30;
		m1.m_input = true;
		mv.push_back(m1);
		if (mv.size() > m_number)
			mv.erase(mv.begin());
	}


	// 로켓이 정해진 프레임을 넘어가지 않도록
	if (g_rocket_destination_rect.x > g_bg_destination_rect.w - g_rocket_source_rect.w - 375)
		g_rocket_destination_rect.x = g_bg_destination_rect.w - g_rocket_source_rect.w - 375;
	else if (g_rocket_destination_rect.x < 0)
		g_rocket_destination_rect.x = 0;
	if (g_rocket_destination_rect.y > g_bg_destination_rect.h - g_rocket_source_rect.h)
		g_rocket_destination_rect.y = g_bg_destination_rect.h - g_rocket_source_rect.h;
	else if (g_rocket_destination_rect.y < 0)
		g_rocket_destination_rect.y = 0;

	// 미사일이 화면 끝에 닿을경우 : m_input = false
	for (int i = 0; i < m_number; i++) {
		if (i < mv.size() && mv[i].g_ms_destination_rect.y < 0)
			mv[i].m_input = false;
	}



	g_elapsed_time_ms += 33;
}

void Render() {

	// Background
	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);

	// Rocket
	SDL_RenderCopy(g_renderer, g_rocket_texture, &g_rocket_source_rect, &g_rocket_destination_rect);	// Texture로부터 복사해서 표시(잘라올 곳, 붙일 곳) / 백버퍼에 표시

	// Missile
	//SDL_RenderCopy(g_renderer, mv.g_ms_texture, &g_ms_source_rect, &g_ms_destination_rect);	// Texture로부터 복사해서 표시(잘라올 곳, 붙일 곳) / 백버퍼에 표시

	SDL_RenderPresent(g_renderer);	// 백버퍼-프론트버퍼 교체
}

void ClearGame() {	// 게임 종료시 메모리 해제
	//SDL_DestroyTexture(g_rocket_texture);
	SDL_DestroyTexture(g_bg_texture);
}