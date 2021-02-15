// 영상처리 소프트웨어 Ver 0.01
#define   _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "includePython.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h> // 픽셀을 출력해주는 기능 포함.
#include <math.h>

/////// 함수 선언부 (이름만 나열)
void print_menu(); void openImage(); unsigned char** malloc2D(int, int); void displayImage();
void freeInputImage();  void freeOutputImage();
/* 화소점 처리에 대한 함수 선언 */
void bright(int); void dark(int); void black_and_white(); void reversal(); void gamma_transformation(double);
/* 화소 영역 처리를 위한 함수 선언 */
void filter_image(int(*mask)[3]); //2차원 배열을 매개변수로 사용
/* 기하학 처리를 위한 함수 선언 */
void up_down_change(); void move_image(int, int); void rotate(int); void zoom_in(); void zoom_out();

void openImage();
void equal_image();
void saveOutputImage(char, char);
char* find_out_image_name();

/////// 전역변수 선언부
unsigned char** inImage = NULL, ** outImage = NULL;
int inH = 0, inW = 0, outH = 0, outW = 0;
char file_name[200];
// 윈도 화면용
HWND  hwnd;  HDC  hdc;

/////// 메인코드부
void main() {
	char action1 = '0', action2, action3;
	int color;
	int result;
	int mask[3][3];
	int move_x, move_y;
	int angle;
	char save;
	double gamma;

	hwnd = GetForegroundWindow();   hdc = GetWindowDC(hwnd);

	printf("\t ## 디지털 영상처리 (Beta 1) ##\n");
	while (action1 != '1')
	{
		printf("0.처리 1.종료\n");
		scanf(" %c", &action1);
		switch (action1)
		{
			case '0':
				openImage();
				printf("0. 화소점 처리 1. 화소 영역 처리 2. 기하학 처리\n");
				// scanf를 여러번 사용할 때 이전 scanf에서 \n을 받아오므로 이에 대한 방어 코드로 " %c"로 공백을 두고 작성 
				scanf(" %c", &action2); 
				switch (action2)
				{
				case '0':
					printf("A. 밝게 B. 어둡게 C.반전 D. 흑백 E. 감마변환\n");
					scanf(" %c", &action3);
					switch (action3)
					{
						case 'A':
							printf("얼마나 밝게 하겠습니까? (0~255입력)\n");
							scanf("%d", &color);
							printf("%d", color);
							bright(color);
							break;
						case 'B':
							printf("얼마나 어둡게 하겠습니까? (0~255입력)\n");
							scanf("%d", &color);
							dark(color);
							break;
						case 'C':
							reversal();
							break;
						case 'D':
							black_and_white();
							break;
						case 'E':
							printf("감마 값을 입력하시오 (ex : 2.5)\n");
							scanf("%lf", &gamma);
							printf("%lf", gamma);
							gamma_transformation(gamma);
							break;
						default:
							break;
					}
					printf("사진을 저장하시겠습니까?(y)\n");
					scanf(" %c", &save);
					if (save == 'y')
						saveOutputImage(action2, action3);
					break;
				case '1':
					printf("A. Sharpen B. Blur C.Edge Enhance D. Edge Detect E. Emboss\n");
					scanf(" %c", &action3);
					switch (action3)
					{
						/* mask의 성질에 따라 mask 요소에 각각 다른 값 입력 */
						case 'A':
							mask[0][0] = 0;
							mask[0][1] = -1;
							mask[0][2] = 0;
							mask[1][0] = -1;
							mask[1][1] = 5;
							mask[1][2] = -1;
							mask[2][0] = 0;
							mask[2][1] = -1;
							mask[2][2] = 0;
							filter_image(mask);
							break;
						case 'B':
							mask[0][0] = 1;
							mask[0][1] = 1;
							mask[0][2] = 1;
							mask[1][0] = 1;
							mask[1][1] = 1;
							mask[1][2] = 1;
							mask[2][0] = 1;
							mask[2][1] = 1;
							mask[2][2] = 1;
							filter_image(mask);
							break;
						case 'C':
							mask[0][0] = 0;
							mask[0][1] = 0;
							mask[0][2] = 0;
							mask[1][0] = -1;
							mask[1][1] = 1;
							mask[1][2] = 0;
							mask[2][0] = 0;
							mask[2][1] = 0;
							mask[2][2] = 0;
							filter_image(mask);		
							break;
						case 'D':
							mask[0][0] = 0;
							mask[0][1] = 1;
							mask[0][2] = 0;
							mask[1][0] = 1;
							mask[1][1] = -4;
							mask[1][2] = 1;
							mask[2][0] = 0;
							mask[2][1] = 1;
							mask[2][2] = 0;
							filter_image(mask);
							break;
						case 'E':
							mask[0][0] = -2;
							mask[0][1] = -1;
							mask[0][2] = 0;
							mask[1][0] = -1;
							mask[1][1] = 1;
							mask[1][2] = 1;
							mask[2][0] = 0;
							mask[2][1] = 1;
							mask[2][2] = 2;
							filter_image(mask);
						default:
							break;
					}
					printf("사진을 저장하시겠습니까?(y)\n");
					scanf(" %c", &save);
					if (save == 'y')
						saveOutputImage(action2, action3);
					break;
				case '2':
					printf("A. Move B. Reverse(upsidedown) C. Rotate D. zoom in E. zoom out\n");
					scanf(" %c", &action3);
					switch (action3)
					{
						case 'A':
							printf(" x좌표로 얼만큼 이동하시겠습니까? (0~255입력)\n");
							scanf("%d", &move_x);
							printf(" y좌표로 얼만큼 이동하시겠습니까? (0~255입력)\n");
							scanf("%d", &move_y);
							move_image(move_x, move_y);
							break;
						case 'B':
							up_down_change();
							break;
						case 'C':
							printf(" 반시계 방향으로 얼마만큼 회전하시겠습니까? (0~360입력)\n");
							scanf("%d", &angle);
							rotate(angle);
							break;	
						case 'D':
							zoom_in();
							break;
						case 'E':
							zoom_out();
							break;
						default:
							break;
					}
					printf("사진을 저장하시겠습니까?(y)\n");
					scanf(" %c", &save);
					if (save == 'y')
						saveOutputImage(action2, action3);
					break;
				default:
					break;
				}
			case '1':
				break;
			default:
				break;
		}
		freeInputImage(); freeOutputImage();
	}
	////// 공통 함수 정의부
}
void openImage()
{
	FILE* rfp;
	char file_path[100];
	strcpy(file_name, "");
	do
	{
		strcpy(file_path, "C:\\images\\RAW\\");
		printf("\n 오픈할 파일명-->");  scanf(" %s", file_name);
		strcat(file_path, file_name);
		strcat(file_path, ".raw");
		//puts(fileName);
		// 파일을 열고, 파일의 크기를 알아내기.
		
		rfp = fopen(file_path, "rb");
		if (rfp == NULL) {
			MessageBox(hwnd, L"파일명이 없어요", L"출력창", NULL);
		}
	} while (rfp == NULL);

	fseek(rfp, 0L, SEEK_END);
	long fsize = ftell(rfp); // 예 : 262144
	fclose(rfp); rfp = fopen(file_path, "rb");
	// 기존에 작업한 것이 있으면 해제
	freeInputImage();
	// 중요! 입력이미지의 높이, 폭 알아내기
	inH = inW = (int)sqrt(fsize);
	inImage = malloc2D(inH, inW);
	for (int i = 0; i < inH; i++)
		fread(inImage[i], sizeof(unsigned char), inW, rfp);
	fclose(rfp);

	equal_image();
}
unsigned char** malloc2D(int h, int w) {
	unsigned char** p;
	p = (unsigned char**)malloc(h * sizeof(unsigned char*));
	for (int i = 0; i < h; i++)
		p[i] = (unsigned char*)malloc(w * sizeof(unsigned char));
	return p;
}
void displayImage() {
	system("cls");
	unsigned char px;
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++) {
			px = outImage[i][k];
			SetPixel(hdc, k + 50, i + 200, RGB(px, px, px));
		}
}
void freeInputImage() {
	if (inImage == NULL)
		return;
	for (int i = 0; i < inH; i++)
		free(inImage[i]);
	free(inImage);
	inImage = NULL;
}
void freeOutputImage() {
	if (outImage == NULL)
		return;
	for (int i = 0; i < outH; i++)
		free(outImage[i]);
	free(outImage);
	outImage = NULL;
}
void saveOutputImage(char action2, char action3)
{
	char file_path[100] = "C:\\images\\RAW\\saved_image\\";
	char tmpname[50];
	char autosave;
	printf("이미지 인식 기능을 사용하시겠습니까? y/n \n");
	scanf(" %c", &autosave);
	if (autosave == 'y')
	{
		strcpy(tmpname, find_out_image_name());		
		strcat(file_path, tmpname);
	}
	else
		strcat(file_path, file_name);
	strcat(file_path, "_");
	switch (action2)
	{
		case '0':
			switch (action3)
			{
				case 'A':
					strcat(file_path, "brigter.raw");
					break;
				case 'B':
					strcat(file_path, "darker.raw");
					break;
				case 'C':
					strcat(file_path, "black_and_white.raw");
					break;
				case 'D':
					strcat(file_path, "reversal.raw");
					break;
				case 'E':
					strcat(file_path, "gamma_trans.raw");
					break;
				default:
					break;
			}
			break;
		case '1':
			switch (action3)
			{
				case 'A':
					strcat(file_path, "Sharpen.raw");
					break;
				case 'B':
					strcat(file_path, "Blur.raw");
					break;
				case 'C':
					strcat(file_path, "Edge_inhance.raw");
					break;
				case 'D':
					strcat(file_path, "Edge_detect.raw");
					break;
				case 'E':
					strcat(file_path, "Emboss.raw");
					break;
				default:
					break;
			}
			break;
		case '2':
			switch (action3)
			{
			case 'A':
				strcat(file_path, "Move.raw");
				break;
			case 'B':
				strcat(file_path, "Reverse.raw");
				break;
			case 'C':
				strcat(file_path, "Rotate.raw");
				break;
			case 'D':
				strcat(file_path, "Zoom_in.raw");
				break;
			case 'E':
				strcat(file_path, "Zoom_out.raw");
				break;
			default:
				break;
			}
			break;
		default:
			break;
	}

	FILE* wfp = fopen(file_path, "wb");
	for (int i = 0; i < inH; i++)
		fwrite(outImage[i], sizeof(unsigned char), outW, wfp);
	fclose(wfp);
}

char* find_out_image_name()
{
	PyObject* module;
	PyObject* func;
	PyObject* ret;
	int i_ret;
	char* result;
	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(r'C:\\Users\\kccistc\\source\\repos\\Mission007\\Mission007')");
	module = PyImport_ImportModule("imagename");
	func = PyObject_GetAttrString(module, "guess_image");

	strcat(file_name, ".raw");

	ret = PyObject_CallObject(func, Py_BuildValue("(z)", file_name));
	PyObject* objectsRepresentation = PyObject_Repr(ret); // 객체를 문자열로 표현한다.
	PyObject* str = PyUnicode_AsEncodedString(objectsRepresentation, "utf-8", "~E~"); // 객체를 인코딩한다.
	result = PyBytes_AsString(str); // string으로 변환하여 준다.
	Py_DECREF(ret);
	Py_Finalize();
	result = strtok(result, "'");
	return result;
}

////// 영상처리 함수 정의부
void equal_image() {  // 동일영상 알고리즘
	if (inImage == NULL)
		return;
	// 기존에 작업한 것이 있으면 해제
	freeOutputImage();
	// 중요! 출력이미지의 높이, 폭을 결정  --> 알고리즘에 영향
	outH = inH;  outW = inW;
	outImage = malloc2D(outH, outW);
	// *** 진짜 영상처리 알고리즘을 구현 ***
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k];
		}
	/////////////////////////////////////////////
	displayImage();
}

void bright(int color) {
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++) {
			if ((inImage[i][k] + color) > 255)
				outImage[i][k] = 255;
			else
				outImage[i][k] = inImage[i][k] + color;
		}
	displayImage();
}

void dark(int color) {
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++) {
			if ((inImage[i][k] - color) < 0)
				outImage[i][k] = 0;
			else
				outImage[i][k] = inImage[i][k] - color;
		}
	displayImage();
}

void black_and_white()
{
	for (int i = 0; i < inH; i++)
	{
		for (int j = 0; j < inW; j++)
		{
			/* 흑백으로 만들기 위해 중간값보다 어두운 값은 검은색으로 설정 */
			if (inImage[i][j] < 127)
			{
				outImage[i][j] = 0;
			}
			/* 흑백으로 만들기 위해 중간값보다 밝은 값은 흰 색으로 설정 */
			else
			{
				outImage[i][j] = 255;
			}
		}
	}
	displayImage();
}

void reversal()
{
	for (int i = 0; i < inH; i++)
	{
		for (int j = 0; j < inW; j++)
		{
			outImage[i][j] = 255 - inImage[i][j];
		}
	}
	displayImage();
}

void up_down_change()
{
	for (int i = 0; i < inH; i++)
	{
		for (int j = 0; j < inW; j++)
		{
			outImage[i][j] = inImage[inH - 1 - i][j];
		}
	}
	displayImage();
}

void move_image(int move_x, int move_y)
{
	for (int i = 0; i < inH; i++)
		for (int j = 0; j < inW; j++)
			/* outImage 초기화 수행 */
			outImage[i][j] = 0;
	for (int i = 0; i < inH; i++)
	{
		for (int j = 0; j < inW; j++)
		{
			/* 경계를 벗어나지 않는 영역에 대해서만 outImage값 입력 */
			if (i + move_y < inH && j + move_y < inW)
				outImage[i + move_y][j + move_x] = inImage[i][j];
			else
				continue;
		}
	}
	displayImage();
}

void filter_image(int(*mask)[3])
{
	int sum=0;
	for (int i = 0; i < inH-2; i++)
	{
		for (int j = 0; j < inW-2; j++)
		{
			/* 기존 image와 3*3 filter에 대해서 convolution 곱 수행 */
			for (int k = 0; k < 3; k++)
			{
				for (int l = 0; l < 3; l++)
				{
					/* 결과를 sum에 저장 */
					sum += inImage[i + k][j + l] * mask[k][l];
				}	
			}
			if (sum > 255)
				outImage[i][j] = 255;
			else if (sum < 0)
				outImage[i][j] = 0;
			else
				outImage[i][j] = sum;
			/* 다음 계산을 위해 sum 초기화 */
			sum = 0;
		}
	}
	displayImage();
}

void zoom_in()
{
	for (int i = inH/4; i < 3*inH/4; i++)
	{
		for (int j = inW/4; j < 3*inW/4; j++)
		{
			/* 2배 확대를 위해 inImage의 중간 영역에 하나의 pixel을 outImage의 4개의 pixel에 대입 */
			outImage[i*2- inH/2][j*2-inW/2] = inImage[i][j];
			outImage[i*2 - inH/2+1][j*2 - inW / 2] = inImage[i][j];
			outImage[i*2 - inH/2][j*2 - inW/2+1] = inImage[i][j];
			outImage[i*2 - inH/2+1][j*2 - inW/2+1] = inImage[i][j];
		}
	}
	displayImage();
}

void zoom_out()
{
	int temp = -1;
	int max;
	for (int i = 0; i < inH; i++)
		for (int j = 0; j < inW; j++)
			outImage[i][j] = 0;

	for (int i = inH / 4; i < 3 * inH / 4; i++)
	{
		for (int j = inW / 4; j < 3 * inW / 4; j++)
		{
			/* 2배 축소를 위해 inImage 정사각형 영역의 4개의 pixel중 최댓값만 outImage에 대입*/
			if (temp < inImage[i * 2 - inH / 2][j * 2 - inW / 2])
				max = inImage[i * 2 - inH / 2][j * 2 - inW / 2];
			else if (temp < inImage[i * 2 - inH / 2 + 1][j * 2 - inW / 2])
				max = inImage[i * 2 - inH / 2 + 1][j * 2 - inW / 2];
			else if (temp < inImage[i * 2 - inH / 2][j * 2 - inW / 2 + 1])
				max = inImage[i * 2 - inH / 2][j * 2 - inW / 2 + 1];
			else if (temp < inImage[i * 2 - inH / 2 + 1][j * 2 - inW / 2 + 1])
				max = inImage[i * 2 - inH / 2 + 1][j * 2 - inW / 2 + 1];
			outImage[i][j] = max;
		}
	}
	displayImage();
}

void rotate(int angle)
{
	float temp_x, temp_y;
	int temp_after_rotate_x, temp_after_rotate_y;
	for (int i = 0; i < inH; i++)
		for (int j = 0; j < inW; j++)
			outImage[i][j] = 0;
	for (int i = 0; i < inH; i++)
	{
		for (int j = 0; j < inW; j++)
		{
			/* 회전변환을 위해 원점을 inW/2, inH/2로 가정 */
			temp_x = j - inW / 2;
			temp_y = inH / 2 - i;
			/* 첨부한 그림 참조 */
			temp_after_rotate_x = (int)(cos(angle * M_PI / 180)* temp_x + sin(angle * M_PI / 180) * temp_y);
			temp_after_rotate_y = (int)(-sin(angle * M_PI / 180) * temp_x + cos(angle * M_PI / 180) * temp_y);
			if (temp_after_rotate_x < inH/2 && temp_after_rotate_x > -inH/2 &&
				temp_after_rotate_y < inW/2 && temp_after_rotate_y > -inW/2)
				/* 영역을 벗어나지 않은 부분에 대해서만 값 입력 */
				outImage[i][j] = inImage[inH / 2 - temp_after_rotate_y][temp_after_rotate_x + inW / 2];
			else
				continue;
		}
	}
	displayImage();
}

void gamma_transformation(double gamma)
{
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			outImage[i][j] = (int)255 * pow(((double)inImage[i][j] / 255), gamma);
			if (outImage[i][j] > 255)
			{
				outImage[i][j] = 255;
			}
		}
	}
	displayImage();
}