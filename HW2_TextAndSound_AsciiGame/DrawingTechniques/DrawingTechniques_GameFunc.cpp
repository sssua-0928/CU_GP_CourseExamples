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

int f_state;  // ����� ����
int f_x, f_y; // �ʱ� ����� ��ġ
int m_state;  // �̻��� ���� 
int m_x, m_y; // �̻��� ��ġ
int score; // ����
const char* score_sum;
char buffer[100];

int i = 0;

Mix_Chunk* effect; // ȿ����
Mix_Music* back;   // �����

TTF_Font* font;    // �۾�ü
SDL_Color white;    // ���� ��ü
SDL_Surface* dynamic_surface;
SDL_Texture* static_score_texture;
SDL_Texture* dynamic_score_texture;
SDL_Rect static_rect;
SDL_Rect dynamic_rect;

/* ���ϼ��� */
SDL_Texture* g_rocket_texture; // ����� �̹���
SDL_Rect g_rocket_source_rect;
SDL_Rect g_rocket_destination_rect;

/* �̻��ϼ���(ms) */
SDL_Texture* g_ms_texture; // �̻��� �̹���
SDL_Rect g_ms_source_rect;
SDL_Rect g_ms_destination_rect[30];

/* ��漼�� */
SDL_Texture* g_bg_texture; // ���ȭ�� �̹���
SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;


void InitGame() {

	g_flag_running = true;

	f_state = 0; // ����� ���� ���� 0�� ��� �ƹ� �ϵ� �Ͼ�� �ʴ´�.
	f_x = 100;   // �ʱ� x�� ��ġ 100
	f_y = 300;   // �ʱ� y�� ��ġ 300
	score = 0;   // �ʱ� ���� 0

	/* ȿ����, ����� ���� */
	effect = Mix_LoadWAV("../../Resources/lasercut.mp3");
	back = Mix_LoadMUS("../../Resources/bgm.mp3");
	Mix_VolumeMusic(256); // ���� ũ��� 256����

	/* ��Ʈ, ���� ���� */
	font = TTF_OpenFont("../../Resources/arose.ttf", 20);
	white = { 255,255,255,255 };
	SDL_Surface* static_surface = TTF_RenderUTF8_Blended(font, CW2A(L"����:", CP_UTF8), white);
	//static_surface = TTF_RenderText_Blended(font, "Score: ", white); // ���ڿ��� surface��
	static_rect.x = 0;
	static_rect.y = 0;
	static_rect.w = static_surface->w;
	static_rect.h = static_surface->h;

	static_score_texture = SDL_CreateTextureFromSurface(g_renderer, static_surface);
	SDL_FreeSurface(static_surface); // ����
	

	/* ���� ���� */
	SDL_Surface* rocket_surface = IMG_Load("../../Resources/character.png");	// structure�� ������ �������� //���� �̹����� cpu��
	SDL_SetColorKey(rocket_surface, SDL_TRUE, SDL_MapRGB(rocket_surface->format, 0, 0, 255));
	g_rocket_texture = SDL_CreateTextureFromSurface(g_renderer, rocket_surface); // ���� �̹����� gpu��
	SDL_FreeSurface(rocket_surface);

	g_rocket_source_rect.x = 0; // �̹��� ���Ͽ��� x���� 0����
	g_rocket_source_rect.y = 0; // �̹��� ���Ͽ��� y���� 0����
	g_rocket_source_rect.w = 155;
	g_rocket_source_rect.h = 169;

	g_rocket_destination_rect.x = f_x; // ��� ��ġ�� �ʱ� ��ġ�� ������ �� ������
	g_rocket_destination_rect.y = f_y;
	g_rocket_destination_rect.w = 50; // ������ �� ���� ��ŭ
	g_rocket_destination_rect.h = 50; // ������ �� ���� ��ŭ


	/* �̻��� ���� */
	SDL_Surface* missile_surface = IMG_Load("../../Resources/character.png"); // �̻��� �̹����� cpu��
	SDL_SetColorKey(missile_surface, 1, SDL_MapRGB(missile_surface->format, 0, 0, 255));
	g_ms_texture = SDL_CreateTextureFromSurface(g_renderer, missile_surface); // �̻��� �̹����� gpu��
	SDL_FreeSurface(missile_surface); // cpu�� �����ִ� �̻��� �̹��� �����
	g_ms_source_rect.x = 155;
	g_ms_source_rect.y = 0;
	g_ms_source_rect.w = 98;
	g_ms_source_rect.h = 87;


	/* ��� ���� */
	SDL_Surface* back_surface = IMG_Load("../../Resources/background.jpg"); // ��� �̹����� cpu��
	g_bg_texture = SDL_CreateTextureFromSurface(g_renderer, back_surface); // cpu�� �ִ� �̹����� gpu��
	SDL_FreeSurface(back_surface); // cpu�� �̹��� ����� 
	g_bg_source_rect.x = 0;
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = 300;
	g_bg_source_rect.h = 500;
	g_bg_destination_rect.x = 0;
	g_bg_destination_rect.y = 0;
	g_bg_destination_rect.w = g_bg_source_rect.w;
	g_bg_destination_rect.h = g_bg_source_rect.h;

	Mix_PlayMusic(back, -1); // ������� ���� -1�� ���� �ݺ�
	system("cls"); // �ܼ�â�� ��µ� �͵��� �����ϰ� �����.
}

/*
void UpdateScoreTexture(int score)
{
	// �̹� �����Ǿ��ִ� texture �ݵ�� �޸� �����
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

	// ������ ������ �������� �Ѿ�� �ʵ���
	if (f_state == 1) {	// g1 ����
		if (g_rocket_destination_rect.x >= 0) g_rocket_destination_rect.x -= 10;
		SDL_RenderCopy(g_renderer, g_rocket_texture, &g_rocket_source_rect, &g_rocket_destination_rect);
	}
	else if (f_state == 2) {	// g2 ������
		if (g_rocket_destination_rect.x <= 250) g_rocket_destination_rect.x += 10;
		SDL_RenderCopy(g_renderer, g_rocket_texture, &g_rocket_source_rect, &g_rocket_destination_rect);
	}
	else if (f_state == 3) {	// g3 ��
		if (g_rocket_destination_rect.y >= 0) g_rocket_destination_rect.y -= 10;
		SDL_RenderCopy(g_renderer, g_rocket_texture, &g_rocket_source_rect, &g_rocket_destination_rect);
	}
	else if (f_state == 4) {	// g4 ��
		if (g_rocket_destination_rect.y <= 450) g_rocket_destination_rect.y += 10;
		SDL_RenderCopy(g_renderer, g_rocket_texture, &g_rocket_source_rect, &g_rocket_destination_rect);
	}

	if (m_state == 1) {
		score += 10;
		//UpdateScoreTexture(score); // UpdateScoreTexture: ���ڿ��� �ٲ���
		// �̹� �����Ǿ��ִ� texture �ݵ�� �޸� �����

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
		if (i > 29) i = 0; // i�� �ִ� 29 (�迭�� ũ�Ⱑ 30���ϱ�)
		g_ms_destination_rect[i].x = g_rocket_destination_rect.x + 16; 
		g_ms_destination_rect[i].y = g_rocket_destination_rect.y;
		g_ms_destination_rect[i].w = g_ms_source_rect.w/3;
		g_ms_destination_rect[i].h = g_ms_source_rect.h/3;
		m_state = 0;  // �迭 �� ���� m_state �� -> 0
	}
}



void Render() {
	// Background
	// SDL_RenderCopy�Լ� : back buffer�� �̹����� ����
	SDL_SetRenderDrawColor(g_renderer, 255, 125, 0, 0);	//pink
	SDL_RenderClear(g_renderer);	//clear
	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);
	SDL_RenderCopy(g_renderer, g_rocket_texture, &g_rocket_source_rect, &g_rocket_destination_rect);	// Texture�κ��� �����ؼ� ǥ��(�߶�� ��, ���� ��) / ����ۿ� ǥ��


	// static_score(Score:)�� ȭ�鿡 ǥ��
	SDL_Rect r;
	r.x = 90;
	r.y = 460;
	r.w = static_rect.w;
	r.h = static_rect.h;
	SDL_RenderCopy(g_renderer, static_score_texture, 0, &r); 

	// dynamic_score(���� ����)�� ȭ�鿡 ǥ��
	SDL_Rect r1;
	r1.x = 160;
	r1.y = 460;
	r1.w = dynamic_rect.w;
	r1.h = dynamic_rect.h;
	SDL_RenderCopy(g_renderer, dynamic_score_texture, 0, &r1);

	if (m_state == 0) { 
		for (int i = 0; i < 30; i++) {
			if (g_ms_destination_rect[i].y > -20) { // y�� ��ġ�� -20���� Ŭ ��� 0�� ��� ���� �����Ǿ� �������� ���ɼ� 
				SDL_RenderCopy(g_renderer, g_ms_texture, &g_ms_source_rect, &g_ms_destination_rect[i]);
				g_ms_destination_rect[i].y -= 15;// y�� ��ġ�� 15�� ���̸鼭 �߻� (������)
			}
		}
	}


	SDL_RenderPresent(g_renderer);	
	// �����-����Ʈ���� ��ü
	// SDL_RenderPresent�Լ�: �̹����� ������ back buffer�� ���� frame buffer�� ��ü�Ͽ� ȭ�鿡 ���

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



void ClearGame() {	// ���� ����� �޸� ����
	SDL_DestroyTexture(g_rocket_texture);
	SDL_DestroyTexture(g_bg_texture);
	SDL_DestroyTexture(g_ms_texture);
	SDL_DestroyTexture(static_score_texture);
	if (effect) Mix_FreeChunk(effect);
	//if (back) Mix_FreeMusic(back);
	TTF_CloseFont(font);
}