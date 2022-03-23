#include "GameFunc.h"
#include <windows.h>

int g_input, m_input;	//로켓의 입력값, 미사일의 입력값
std::string g_output, m_output;	//로켓의 출력값, 미사일의 출력값

int g_X, g_Y;	// 로켓의 위치값
int m_X, m_Y;	// 미사일의 위치값

// 흘러간 시간 기록
double g_elapsed_time_ms;


/////////////////////////////////////////////////////////////
// InitGame() 
// 프로그램이 시작될 때 최초에 한 번 호출되는 함수.
// 이 함수에서 게임에 필요한 자원(이미지, 사운드 등)을 로딩하고, 상태 변수들을 초기화 해야한다.
void InitGame() {
	// 로켓(*) = g
	g_input = 0;
	g_output = "*";
	g_flag_running = true;

	// 미사일(!) = m
	m_input = 0;
	m_output = "!";

	g_X = 9;
	g_Y = 20;
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
	// 정해진 범위를 넘어가지 않도록
	if (g_X > 20) g_X = 19;
	else if (g_X < 0) g_X = 0;
	if (g_Y > 30) g_Y = 29;
	else if (g_Y < 0) g_Y = 0;

	// g1 왼쪽
	if (g_input == 1) {		
		g_X--;
	}
	// g2 오른쪽
	else if (g_input == 2) {	
		g_X++;
	}
	// g3 앞
	else if (g_input == 3) {	
		g_Y--;
	}
	// g4 뒤
	else if (g_input == 4) {
		g_Y++;
	}

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

	g_elapsed_time_ms += 30;
}




/////////////////////////////////////////////////////////////
// Render() 
// 그림을 그리는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Render() {
	//// 1. 배경 그리기.
	// 1.1. 커서를 콘솔 화면의 왼쪽 위 모서리 부분으로 옮긴다. 좌표(0, 0)
	// <windows.h>에서 제공하는 함수를 사용한다.
	COORD cur, cur_m;
	cur.X, cur.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);

	// 1.2. 배경 부분을 '_'으로 채운다.
	// 가로 세로는 30:20
	for ( int i=0; i<=30; i++ ) {
		for ( int j=0; j<=20; j++ ) {
			std::cout << "_";
		}
		std::cout << std::endl;
	}

	// 1.3. 배경 아래에 시간을 표시한다,
	std::cout << "\nElapsed Time: " << g_elapsed_time_ms / 1000.0f << std::endl;


	//// 2. 캐릭터 그리기.
	// 2.1. 커서를 캐릭터가 그려질 위치로 옮긴다. 
	cur_m.X = m_X;
	cur_m.Y = m_Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur_m);
	// 2.2. 미사일(!)을 그린다.
	std::cout << m_output;

	cur.X = g_X;
	cur.Y = g_Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
	// 2.2. 로켓(*)을 그린다.
	std::cout << g_output;

	
}



/////////////////////////////////////////////////////////////
// HandleEvents() 
// 이벤트를 처리하는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void HandleEvents()
{
	SDL_Event event;
	if(SDL_PollEvent(&event)) {
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
				m_input = 1;
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

