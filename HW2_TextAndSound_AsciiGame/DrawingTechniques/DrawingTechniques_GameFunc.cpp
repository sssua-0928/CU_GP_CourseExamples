#include "DrawingTechniques_GameFunc.h"
#include <windows.h>
#include "SDL_image.h"
#include <iostream>
#include <windows.h>
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <string>
#include <atlstr.h>

extern SDL_Renderer* g_renderer;
extern bool g_flag_running;

int f_state;  // 비행기 상태
int f_x, f_y; // 초기 비행기 위치
int m_state;  // 미사일 상태 
int m_x, m_y; // 미사일 위치
int score; // 점수
const char* score_sum;
char buffer[100];

int i = 0;

Mix_Chunk* effect; // 효과음
Mix_Music* back;   // 배경음

TTF_Font* font;    // 글씨체
SDL_Color white;    // 색상 객체
SDL_Surface* dynamic_surface;
SDL_Texture* static_score_texture;
SDL_Texture* dynamic_score_texture;
SDL_Rect static_rect;
SDL_Rect dynamic_rect;

/* 로켓세팅 */
SDL_Texture* g_rocket_texture; // 비행기 이미지
SDL_Rect g_rocket_source_rect;
SDL_Rect g_rocket_destination_rect;

/* 미사일세팅(ms) */
SDL_Texture* g_ms_texture; // 미사일 이미지
SDL_Rect g_ms_source_rect;
SDL_Rect g_ms_destination_rect[30];

/* 배경세팅 */
SDL_Texture* g_bg_texture; // 배경화면 이미지
SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;


void InitGame() {

	g_flag_running = true;

	f_state = 0; // 비행기 상태 조정 0일 경우 아무 일도 일어나지 않는다.
	f_x = 100;   // 초기 x축 위치 100
	f_y = 300;   // 초기 y축 위치 300
	score = 0;   // 초기 점수 0

	/* 효과음, 배경음 설정 */
	effect = Mix_LoadWAV("../../Resources/lasercut.mp3");
	back = Mix_LoadMUS("../../Resources/bgm.mp3");
	Mix_VolumeMusic(256); // 음악 크기는 256으로

	/* 폰트, 색상 설정 */
	font = TTF_OpenFont("../../Resources/arose.ttf", 20);
	white = { 255,255,255,255 };
	SDL_Surface* static_surface = TTF_RenderUTF8_Blended(font, CW2A(L"점수:", CP_UTF8), white);
	//static_surface = TTF_RenderText_Blended(font, "Score: ", white); // 문자열을 surface로
	static_rect.x = 0;
	static_rect.y = 0;
	static_rect.w = static_surface->w;
	static_rect.h = static_surface->h;

	static_score_texture = SDL_CreateTextureFromSurface(g_renderer, static_surface);
	SDL_FreeSurface(static_surface); // 해제
	

	/* 로켓 설정 */
	SDL_Surface* rocket_surface = IMG_Load("../../Resources/character.png");	// structure의 포인터 가져오기 //로켓 이미지를 cpu로
	SDL_SetColorKey(rocket_surface, SDL_TRUE, SDL_MapRGB(rocket_surface->format, 0, 0, 255));
	g_rocket_texture = SDL_CreateTextureFromSurface(g_renderer, rocket_surface); // 로켓 이미지를 gpu로
	SDL_FreeSurface(rocket_surface);

	g_rocket_source_rect.x = 0; // 이미지 파일에서 x축은 0부터
	g_rocket_source_rect.y = 0; // 이미지 파일에서 y축은 0부터
	g_rocket_source_rect.w = 155;
	g_rocket_source_rect.h = 169;

	g_rocket_destination_rect.x = f_x; // 출력 위치는 초기 위치로 설정해 준 곳으로
	g_rocket_destination_rect.y = f_y;
	g_rocket_destination_rect.w = 50; // 설정해 준 넓이 만큼
	g_rocket_destination_rect.h = 50; // 설정해 준 높이 만큼


	/* 미사일 설정 */
	SDL_Surface* missile_surface = IMG_Load("../../Resources/character.png"); // 미사일 이미지를 cpu로
	SDL_SetColorKey(missile_surface, 1, SDL_MapRGB(missile_surface->format, 0, 0, 255));
	g_ms_texture = SDL_CreateTextureFromSurface(g_renderer, missile_surface); // 미사일 이미지를 gpu로
	SDL_FreeSurface(missile_surface); // cpu에 남아있는 미사일 이미지 지우기
	g_ms_source_rect.x = 155;
	g_ms_source_rect.y = 0;
	g_ms_source_rect.w = 98;
	g_ms_source_rect.h = 87;


	/* 배경 설정 */
	SDL_Surface* back_surface = IMG_Load("../../Resources/background.jpg"); // 배경 이미지를 cpu로
	g_bg_texture = SDL_CreateTextureFromSurface(g_renderer, back_surface); // cpu에 있는 이미지를 gpu로
	SDL_FreeSurface(back_surface); // cpu의 이미지 지우기 
	g_bg_source_rect.x = 0;
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = 300;
	g_bg_source_rect.h = 500;
	g_bg_destination_rect.x = 0;
	g_bg_destination_rect.y = 0;
	g_bg_destination_rect.w = g_bg_source_rect.w;
	g_bg_destination_rect.h = g_bg_source_rect.h;

	Mix_PlayMusic(back, -1); // 배경음악 설정 -1은 무한 반복
	system("cls"); // 콘솔창에 출력된 것들을 깨끗하게 지운다.
}

/*
void UpdateScoreTexture(int score)
{
	// 이미 생성되어있는 texture 반드시 메모리 지우기
	if (dynamic_score_texture != 0){
		SDL_DestroyTexture(dynamic_score_texture);	
	}

	sprintf(buffer, "%05d", score);
	score_sum = buffer;

	//dynamic_surface = TTF_RenderText_Blended(font, std::to_string((long)score).c_str(), white);
	dynamic_surface = TTF_RenderText_Blended(font, score_sum, white);

	dynamic_rect.x = 0;
	dynamic_rect.y = 0;
	dynamic_rect.w = dynamic_surface->w;
	dynamic_rect.h = dynamic_surface->h;

	dynamic_score_texture = SDL_CreateTextureFromSurface(g_renderer, dynamic_surface);

	SDL_FreeSurface(dynamic_surface);
}
*/


void Update() {

	if (score == 0) UpdateScoreTexture(score);

	// 로켓이 정해진 프레임을 넘어가지 않도록
	if (f_state == 1) {	// g1 왼쪽
		if (g_rocket_destination_rect.x >= 0) g_rocket_destination_rect.x -= 10;
		SDL_RenderCopy(g_renderer, g_rocket_texture, &g_rocket_source_rect, &g_rocket_destination_rect);
	}
	else if (f_state == 2) {	// g2 오른쪽
		if (g_rocket_destination_rect.x <= 250) g_rocket_destination_rect.x += 10;
		SDL_RenderCopy(g_renderer, g_rocket_texture, &g_rocket_source_rect, &g_rocket_destination_rect);
	}
	else if (f_state == 3) {	// g3 앞
		if (g_rocket_destination_rect.y >= 0) g_rocket_destination_rect.y -= 10;
		SDL_RenderCopy(g_renderer, g_rocket_texture, &g_rocket_source_rect, &g_rocket_destination_rect);
	}
	else if (f_state == 4) {	// g4 뒤
		if (g_rocket_destination_rect.y <= 450) g_rocket_destination_rect.y += 10;
		SDL_RenderCopy(g_renderer, g_rocket_texture, &g_rocket_source_rect, &g_rocket_destination_rect);
	}

	if (m_state == 1) {
		score += 10;
		//UpdateScoreTexture(score); // UpdateScoreTexture: 문자열로 바꿔줌
		// 이미 생성되어있는 texture 반드시 메모리 지우기

		if (dynamic_score_texture != 0) {
			SDL_DestroyTexture(dynamic_score_texture);
		}

		sprintf(buffer, "%05d", score);
		score_sum = buffer;

		//dynamic_surface = TTF_RenderText_Blended(font, std::to_string((long)score).c_str(), white);
		dynamic_surface = TTF_RenderText_Blended(font, score_sum, white);

		dynamic_rect.x = 0;
		dynamic_rect.y = 0;
		dynamic_rect.w = dynamic_surface->w;
		dynamic_rect.h = dynamic_surface->h;

		dynamic_score_texture = SDL_CreateTextureFromSurface(g_renderer, dynamic_surface);

		SDL_FreeSurface(dynamic_surface);
		
		
		i += 1;
		if (i > 29) i = 0; // i의 최댓값 29 (배열의 크기가 30개니까)
		g_ms_destination_rect[i].x = g_rocket_destination_rect.x + 16; 
		g_ms_destination_rect[i].y = g_rocket_destination_rect.y;
		g_ms_destination_rect[i].w = g_ms_source_rect.w/3;
		g_ms_destination_rect[i].h = g_ms_source_rect.h/3;
		m_state = 0;  // 배열 값 들어가면 m_state 값 -> 0
	}
}



void Render() {
	// Background
	// SDL_RenderCopy함수 : back buffer에 이미지를 입힘
	SDL_SetRenderDrawColor(g_renderer, 255, 125, 0, 0);	//pink
	SDL_RenderClear(g_renderer);	//clear
	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);
	SDL_RenderCopy(g_renderer, g_rocket_texture, &g_rocket_source_rect, &g_rocket_destination_rect);	// Texture로부터 복사해서 표시(잘라올 곳, 붙일 곳) / 백버퍼에 표시


	// static_score(Score:)를 화면에 표시
	SDL_Rect r;
	r.x = 90;
	r.y = 460;
	r.w = static_rect.w;
	r.h = static_rect.h;
	SDL_RenderCopy(g_renderer, static_score_texture, 0, &r); 

	// dynamic_score(실제 점수)를 화면에 표시
	SDL_Rect r1;
	r1.x = 160;
	r1.y = 460;
	r1.w = dynamic_rect.w;
	r1.h = dynamic_rect.h;
	SDL_RenderCopy(g_renderer, dynamic_score_texture, 0, &r1);

	if (m_state == 0) { 
		for (int i = 0; i < 30; i++) {
			if (g_ms_destination_rect[i].y > -20) { // y축 위치가 -20보다 클 경우 0일 경우 위에 고정되어 남아있을 가능성 
				SDL_RenderCopy(g_renderer, g_ms_texture, &g_ms_source_rect, &g_ms_destination_rect[i]);
				g_ms_destination_rect[i].y -= 15;// y축 위치를 15씩 줄이면서 발사 (앞으로)
			}
		}
	}


	SDL_RenderPresent(g_renderer);	
	// 백버퍼-프론트버퍼 교체
	// SDL_RenderPresent함수: 이미지가 입혀진 back buffer를 현재 frame buffer와 교체하여 화면에 출력

	/*COORD cur;
	cur.X = 0;
	cur.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);

	for (int i = 0; i<20; i++)
	{
	for (int j = 0; j<30; j++)
	{
	std::cout << "-";
	}
	std::cout << std::endl;
	}

	cur.X = f_x;
	cur.Y = f_y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
	std::cout << "*";*/
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
				f_state = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				f_state = 2;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				f_state = 3;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				f_state = 4;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {
				m_state = 1;
				Mix_PlayChannel(-1, effect, 0);
			}
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LEFT
				|| event.key.keysym.sym == SDLK_RIGHT
				|| event.key.keysym.sym == SDLK_UP
				|| event.key.keysym.sym == SDLK_DOWN){
				
				f_state = 0;
			}
			break;

		default:
			break;
		}
	}
}



void ClearGame() {	// 게임 종료시 메모리 해제
	SDL_DestroyTexture(g_rocket_texture);
	SDL_DestroyTexture(g_bg_texture);
	SDL_DestroyTexture(g_ms_texture);
	SDL_DestroyTexture(static_score_texture);
	if (effect) Mix_FreeChunk(effect);
	//if (back) Mix_FreeMusic(back);
	TTF_CloseFont(font);
}