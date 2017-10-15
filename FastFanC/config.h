#include <math.h>

#define SIZE_P 100
#define SIZE_U 3000

typedef unsigned char byte;
typedef unsigned char uint8;
typedef char int8;
typedef unsigned short int uint16;
typedef short int int16;
typedef unsigned long int uint32;
typedef long int int32;

const float PI = 3.142857f;

const int32 rps = 1000 / 60;	// ������� ���� � �������

const int32 SIZE_PLAYER = SIZE_P;
const int32 SIZE_UNIT = SIZE_U;

const float MAP_WIDTH = 180.0f;
const float MAP_HEIGHT = 90.0f;

const float RADIUS = 50.0f;				// ���� ������ ������ (?)


const uint8 PIG_MODEL = 0;
const uint8 DOG_MODEL = 1;
const uint8 THREE_MODEL = 2;

const float PIG_SPEED = 0.5f;
// ..

const float SPEED_PORATION = 0.068f;	// � �� ������� ��� �������� �� ����� ������ (�������� ��������� ��� �� ������!)
// ..

const float PIG_RADIUS = 3.0f;
// ..


// �������� ���� ������

// 36 ���� (?)
typedef struct unit_t
{
	int32 id;	// ������������� �������(send byte = 4)
	float x;	// ���������� x ������	(send byte = 4)
	float y;	// ���������� y ������	(send byte = 4)
	float z;	// ���������� z ������	(send byte = 4)
	float dz;	// ���������� ���� 	(no send)
	float a;	// ���� �������� ������	(send byte = 4)
	float r;    // ������ ������������� ������� (no send)
	uint8 model;// ����� ������ ������	(send  byte = 1)
	uint8 anim;	// ����� ����������� ������	(send  byte = 1)

	uint32 exp;		// ����
	uint32 score;	// ����


	uint8 active;	// �������� ������ {0:������ �� �����; 1:�������� �����;2...}

	float nvXs; // ����������������� ������ X
	float nvYs; // ����������������� ������ Y

	uint8 status;	//  0 ����� �� � ����; 1: ����� �����������; 2: ����� � ����; 3: ����� ����������; (no send)
	uint8 refn;		// ���������� ��� ���������� (send byte = 1)
} Unit;

// ?? ����
typedef struct player_t
{
	struct lws* wsi;	// ������ �� ����������� ������
	int32 id;			// ������������� ������	(��������� � �������� � ������� ��� ������ ��� � �������)
	wchar_t name[21];	// ��� ������ 42 ����� 
	//List* list;		// ������� �������� ������ ������((
} Player;

Unit units[SIZE_U];
Player players[SIZE_P];
int32 del_array[SIZE_U];	// ������ �������� ��������


float randomFloat(float min, float max) {
	return (float)rand() / RAND_MAX * (max - min) + min;
}


// ����������� ����� // ������� ����� � ������ ���� ����� ��� ��� ���! //
// �������� ������� (15:23 14.10.17)
void setFloat(byte* buffer, float a) {
	memcpy(buffer, &a, sizeof(float));
	byte tmp = buffer[0];
	buffer[0] = buffer[3];
	buffer[3] = tmp;
	tmp = buffer[1];
	buffer[1] = buffer[2];
	buffer[2] = tmp;
}

void getFloat(const byte* buffer, float* a) {
	memcpy(a, buffer, sizeof(float));
}

void setInt32(byte* buffer, int32 a) {
	memcpy(buffer, &a, sizeof(int32));
	byte tmp = buffer[0];
	buffer[0] = buffer[3];
	buffer[3] = tmp;
	tmp = buffer[1];
	buffer[1] = buffer[2];
	buffer[2] = tmp;
}

void getInt32(const byte* buffer, int32* a) {
	memcpy(a, buffer, sizeof(int32));
}

// ������ � ���������� � ������� ������ 
void setUint32(byte* buffer, uint32 a) {
	memcpy(buffer, &a, sizeof(uint32));
	// �������������� ������
	byte tmp = buffer[0];
	buffer[0] = buffer[3];
	buffer[3] = tmp;
	tmp = buffer[1];
	buffer[1] = buffer[2];
	buffer[2] = tmp;
}
void getUint32(const byte* buffer, uint32* a) {
	memcpy(a, buffer, sizeof(uint32));
}


void setUint16(byte* buffer, uint16 a) {
	memcpy(buffer, &a, sizeof(uint16));
	byte tmp = buffer[0];
	buffer[0] = buffer[1];
	buffer[1] = tmp;
}

void getUint16(byte* buffer, uint16* a) {
	memcpy(a, buffer, sizeof(uint16));
}

// ��� ������������� ������� (16.08.17)
void setUint8(byte* buffer, uint8 a) {
	memcpy(buffer, &a, sizeof(uint8));
}

void getUint8(byte* buffer, uint8* a) {
	memcpy(a, buffer, sizeof(uint8));
}

void setString(byte* buffer, wchar_t* str, int len) {	// len = 40! ������� �������, ��� ����������� �� ��� �� ��� ���
	memcpy(buffer, str, len);
	/*byte ch;
	for (int c = 0; c < 40; c += 2) {
	ch = buffer[c];
	buffer[c] = buffer[c + 1];
	buffer[c + 1] = ch;
	}*/
}

void setNameBytes(byte* buffer, char* str, int len) {
	memcpy(buffer, str, len);
	/*byte ch;
	for (int c = 0; c < 40; c += 2) {
	ch = buffer[c];
	buffer[c] = buffer[c + 1];
	buffer[c + 1] = ch;
	}*/
}

void getNameUTF16(byte* buffer, wchar_t* str) {

	byte arr[40];
	for (int c = 0; c < 40; c++)
		arr[c] = (buffer)[c + 1];


	byte ch;
	for (int c = 0; c < 40; c += 2) {
		ch = arr[c];
		arr[c] = arr[c + 1];
		arr[c + 1] = ch;
	}
	memcpy(str, arr, 40);
	str[20] = L'\0';
}

void getNameBytes(byte* buffer, char* str) {

	byte arr[40];
	for (int c = 0; c < 40; c++)
		arr[c] = (buffer)[c + 1];

	byte ch;
	for (int c = 0; c < 40; c += 2) {
		ch = arr[c];
		arr[c] = arr[c + 1];
		arr[c + 1] = ch;
	}
	memcpy(str, arr, 40);
	str[40] = '\0';
}