#include "GameFunc.h"
#include <windows.h>
#include <vector>
using namespace std;

int g_input;	//로켓의 입력값
/*int m_input;*/
std::string g_output, m_output;	//로켓의 출력값
/*std::string m_output*/
int g_X, g_Y;	// 로켓의 위치값
/*int m_X, m_Y;*/
int m_number = 10;	// 미사일 개수

typedef struct missile {	// 미사일 구조체
	int m_X, m_Y;	// 미사일의 위치값
	bool m_input = false;	// 미사일의 출력값
}missile;

vector<missile> mv;	//미사일 구조체 벡터

// 흘러간 시간 기록
double g_elapsed_time_ms;


/////////////////////////////////////////////////////////////
// InitGame() 
// 프로그램이 시작될 때 최초에 한 번 호출되는 함수.
// 이 함수에서 게임에 필요한 자원(이미지, 사운드 등)을 로딩하고, 상태 변수들을 초기화 해야한다.
void InitGame() {
	g_input = 0;
	g_output = "*";	// 로켓(*) = g
	m_output = "!";	// 미사일(!) = m
	g_flag_running = true;

	g_X = 15;
	g_Y = 25;

	g_elapsed_time_ms = 0;

	// Clear the console screen.
	// 표준출력 화면을 깨끗히 지운다.
	system("cls");
}






/////////////////////////////////////////////////////////////
// Update() 
// 게임의 내용을 업데이트하는 함수.
// 실제 게임의 룰을 구현해야하는 곳.
// 게임에서 일어나는 변화는 모두 이 곳에서 구현한다.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Update() {
	if (g_input == 1) {	// g1 왼쪽
		g_X--;
	}
	else if (g_input == 2) {	// g2 오른쪽
		g_X++;
	}
	else if (g_input == 3) {	// g3 앞
		g_Y--;
	}
	else if (g_input == 4) {	// g4 뒤
		g_Y++;
	}

	if (g_input == 5) {	// g5 스페이스-미사일발사!
		// 미사일 추가
		missile m1;
		m1.m_X = g_X;
		m1.m_Y = g_Y - 1;
		m1.m_input = true;
		mv.push_back(m1);
		if (mv.size() > m_number)
			mv.erase(mv.begin());
	}

	for (int i = 0; i < m_number; i++) {
		if (i < mv.size()) {
			if (mv[i].m_input == true) {
				mv[i].m_Y--;
			}
			else
				mv.erase(mv.begin());
		}
	}

	// 로켓이 정해진 프레임을 넘어가지 않도록
	if (g_X > 29) g_X = 29;
	else if (g_X < 0) g_X = 0;
	if (g_Y > 19) g_Y = 19;
	else if (g_Y < 0) g_Y = 0;

	for (int i = 0; i < m_number; i++) {
		// 미사일이 화면 끝에 닿을경우 : m_input = false
		if (i < mv.size() && mv[i].m_Y < 0)
			mv[i].m_input = false;
	}

	/*
	// m0 미사일 장착상태
	if (m_input == 0) {
		m_X = g_X;
		m_Y = g_Y;
	}
	// m1 미사일 발사
	else if (m_input == 1) {
		m_Y--;
		if (m_Y <= -1) {
			m_input = 0;
		}
	}
	*/

	g_elapsed_time_ms += 20;
}




/////////////////////////////////////////////////////////////
// Render() 
// 그림을 그리는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Render() {
	//// 1. 배경 그리기.
	// 1.1. 커서를 콘솔 화면의 왼쪽 위 모서리 부분으로 옮긴다. 좌표(0, 0)
	// <windows.h>에서 제공하는 함수를 사용한다.
	COORD cur;

	cur.X = 0;
	cur.Y = 0;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);

	// 1.2. 배경 부분을 '_'으로 채운다.
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 30; j++) {
			std::cout << "_";
		}
		std::cout << std::endl;
	}

	// 1.3. 배경 아래에 시간을 표시한다,
	std::cout << "Elapsed Time: " << g_elapsed_time_ms / 1000.0f << std::endl;


	//// 2. 캐릭터 그리기.
	// 2.1. 커서를 캐릭터가 그려질 위치로 옮긴다. 
	/*
	cur_m.X = m_X;
	cur_m.Y = m_Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur_m);
	// 2.2. 미사일(!)을 그린다.
	std::cout << m_output;
	*/

	// 2.2. 로켓(*)을 그린다.
	cur.X = g_X;
	cur.Y = g_Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
	cout << g_output;

	// 2.2. 미사일(!)을 그린다.
	vector<COORD> curm;
	for (int i = 0; i < 20; i++) {
		if (i < mv.size()) {
			COORD pos;	// 미사일 벡터 개별 위치값
			pos.X = mv[i].m_X;
			pos.Y = mv[i].m_Y;
			curm.push_back(pos);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curm[i]);
			if (mv[i].m_input == true)	//상태가 true이면 화면에 그림
				cout << m_output;
		}
		else
			break;
	}
}



/////////////////////////////////////////////////////////////
// HandleEvents() 
// 이벤트를 처리하는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void HandleEvents()
{
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


/////////////////////////////////////////////////////////////
// ClearGame() 
// 프로그램이 끝날 때 한 번 호출되는 함수.
// 이 함수에서 사용된 자원(이미지, 사운드 등)과 메모리 등을 해제해야한다.
void ClearGame()
{
}

