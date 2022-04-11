#include "GameFunc.h"
#include <windows.h>
#include <vector>
using namespace std;

int g_input;	//������ �Է°�
/*int m_input;*/
std::string g_output, m_output;	//������ ��°�
/*std::string m_output*/
int g_X, g_Y;	// ������ ��ġ��
/*int m_X, m_Y;*/
int m_number = 10;	// �̻��� ����

typedef struct missile {	// �̻��� ����ü
	int m_X, m_Y;	// �̻����� ��ġ��
	bool m_input = false;	// �̻����� ��°�
}missile;

vector<missile> mv;	//�̻��� ����ü ����

// �귯�� �ð� ���
double g_elapsed_time_ms;


/////////////////////////////////////////////////////////////
// InitGame() 
// ���α׷��� ���۵� �� ���ʿ� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���ӿ� �ʿ��� �ڿ�(�̹���, ���� ��)�� �ε��ϰ�, ���� �������� �ʱ�ȭ �ؾ��Ѵ�.
void InitGame() {
	g_input = 0;
	g_output = "*";	// ����(*) = g
	m_output = "!";	// �̻���(!) = m
	g_flag_running = true;

	g_X = 15;
	g_Y = 25;

	g_elapsed_time_ms = 0;

	// Clear the console screen.
	// ǥ����� ȭ���� ������ �����.
	system("cls");
}






/////////////////////////////////////////////////////////////
// Update() 
// ������ ������ ������Ʈ�ϴ� �Լ�.
// ���� ������ ���� �����ؾ��ϴ� ��.
// ���ӿ��� �Ͼ�� ��ȭ�� ��� �� ������ �����Ѵ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Update() {
	if (g_input == 1) {	// g1 ����
		g_X--;
	}
	else if (g_input == 2) {	// g2 ������
		g_X++;
	}
	else if (g_input == 3) {	// g3 ��
		g_Y--;
	}
	else if (g_input == 4) {	// g4 ��
		g_Y++;
	}

	if (g_input == 5) {	// g5 �����̽�-�̻��Ϲ߻�!
		// �̻��� �߰�
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

	// ������ ������ �������� �Ѿ�� �ʵ���
	if (g_X > 29) g_X = 29;
	else if (g_X < 0) g_X = 0;
	if (g_Y > 19) g_Y = 19;
	else if (g_Y < 0) g_Y = 0;

	for (int i = 0; i < m_number; i++) {
		// �̻����� ȭ�� ���� ������� : m_input = false
		if (i < mv.size() && mv[i].m_Y < 0)
			mv[i].m_input = false;
	}

	/*
	// m0 �̻��� ��������
	if (m_input == 0) {
		m_X = g_X;
		m_Y = g_Y;
	}
	// m1 �̻��� �߻�
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
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Render() {
	//// 1. ��� �׸���.
	// 1.1. Ŀ���� �ܼ� ȭ���� ���� �� �𼭸� �κ����� �ű��. ��ǥ(0, 0)
	// <windows.h>���� �����ϴ� �Լ��� ����Ѵ�.
	COORD cur;

	cur.X = 0;
	cur.Y = 0;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);

	// 1.2. ��� �κ��� '_'���� ä���.
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 30; j++) {
			std::cout << "_";
		}
		std::cout << std::endl;
	}

	// 1.3. ��� �Ʒ��� �ð��� ǥ���Ѵ�,
	std::cout << "Elapsed Time: " << g_elapsed_time_ms / 1000.0f << std::endl;


	//// 2. ĳ���� �׸���.
	// 2.1. Ŀ���� ĳ���Ͱ� �׷��� ��ġ�� �ű��. 
	/*
	cur_m.X = m_X;
	cur_m.Y = m_Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur_m);
	// 2.2. �̻���(!)�� �׸���.
	std::cout << m_output;
	*/

	// 2.2. ����(*)�� �׸���.
	cur.X = g_X;
	cur.Y = g_Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
	cout << g_output;

	// 2.2. �̻���(!)�� �׸���.
	vector<COORD> curm;
	for (int i = 0; i < 20; i++) {
		if (i < mv.size()) {
			COORD pos;	// �̻��� ���� ���� ��ġ��
			pos.X = mv[i].m_X;
			pos.Y = mv[i].m_Y;
			curm.push_back(pos);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curm[i]);
			if (mv[i].m_input == true)	//���°� true�̸� ȭ�鿡 �׸�
				cout << m_output;
		}
		else
			break;
	}
}



/////////////////////////////////////////////////////////////
// HandleEvents() 
// �̺�Ʈ�� ó���ϴ� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
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
// ���α׷��� ���� �� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���� �ڿ�(�̹���, ���� ��)�� �޸� ���� �����ؾ��Ѵ�.
void ClearGame()
{
}

