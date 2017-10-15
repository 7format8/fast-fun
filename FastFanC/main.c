#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <stdio.h>
#include <conio.h>

#include <SFML/Window.h>
#include <SFML/System.h>

#include <libwebsockets.h>

#include "config.h"
#include "action.h"
#include "network.h"

int main(void) {

	setlocale(LC_CTYPE, "Russian");
	SetConsoleCP(1251); 		//��������� ������� �������� win-cp 1251 � ����� �����
	SetConsoleOutputCP(1251); 	//��������� ������� �������� win-cp 1251 � ����� 

	sfTime fps = sfMilliseconds(1000/20);
	//printf("int(%u)\n", sizeof(int));

	



	// ���������� � ������������ �������
	struct lws_context_creation_info info = InitInfo(8080);
	context = lws_create_context(&info);
	if (context == NULL) {
		printf("������ �������� �������\n");
		_getch();
		return -1;
	}


	// ���������
	for (int i = SIZE_PLAYER; i--;) {
		players[i].id = -1;
	}
		
	for (int i = SIZE_UNIT; i--;) {
		units[i].id = -1;
		units[i].status = 0;	// �� � ����
		units[i].exp = 0;
		units[i].score = 0;
		units[i].nvXs = 0.0f;
		units[i].nvYs = 0.0f;
		// "���������" ������� ��������
		del_array[i] = -1;
	}
		
	// ���������� ����� 30 ��.
	for (int i = SIZE_PLAYER; i < SIZE_PLAYER  + 60; i++) {
		units[i].id = i;
		units[i].status = 2;	// �� 15.10.17 ������ � ����
		units[i].x = randomFloat(-180.0f, 180.0f);
		units[i].y = randomFloat(-90.0f, 90.0f);
		units[i].model = 5;
		//printf("model: %u\n"+ units[i].model);
	}


	
	//int count = 0;

	// ������� ������ � �������:
	// 1 - ����� �������
	// 2 - ��������� ������ �������
	// 3 - �������� �������������/���� ������
	// 4 - ������������ � �������� ������
	// 5 - �������� 16/33 ��

	while (!sfKeyboard_isKeyPressed(sfKeyEscape)) {
		// 1 - ����� �������
		// 2 - ��������� ������ �������
		lws_service(context, 0);


		// 3 - �������� �������������/��������/������ �� ������� ������� �����/���� ������/(�� ������������)

		CollisionUnit();
		Action();
		GameArea();

		// 4 - ������������ � �������� ������
		for (int i = 0; i < SIZE_PLAYER; i++) {
			if (players[i].id != -1) {
				if (units[players[i].id].status == 2)
					SendData(&players[i]);
				else
					AddAllData(&players[i]);
			}
		}

		for (int i = 0; i < SIZE_UNIT; i++) units[i].refn = 0;


		for (int i = 0; i < SIZE_UNIT; i++) {
			del_array[i] = -1;
		}

		// 5 - �������� 16/33 ��
		sfSleep(fps);
	}

	printf("������������ ��������...\n");
	lws_context_destroy(context);


	_getch();
	return 0;
}
