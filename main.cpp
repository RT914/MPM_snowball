#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#include "Draw.h"
#include "mpm.h"
#include "Window.h"

using namespace std;

#define _USE_MATH_DEFINES

int SimulationTime = 0;
Object object = CreateObject(n_particles);

//----------------------------------------------------
// 関数プロトタイプ（後に呼び出す関数名と引数の宣言）
//----------------------------------------------------
void Initialize(void);
void Display(void);
void Idle();

//----------------------------------------------------
// メイン関数
//----------------------------------------------------
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);//環境の初期化
	glutInitWindowPosition(WindowPositionX, WindowPositionY); //ウィンドウの位置の指定
	glutInitWindowSize(WindowWidth, WindowHeight); //ウィンドウサイズの指定
	glutInitDisplayMode(GLUT_RGBA); //ディスプレイモードの指定

	//ウィンドウの作成
	if (!glutCreateWindow(WindowTitle)) {
		std::cerr << "ウィンドウの作成に失敗しました。" << std::endl;
		return -1;
	}

	glutIdleFunc(Idle); //プログラムアイドル状態時(暇な時)に呼び出される関数
	glutDisplayFunc(Display); //描画時に呼び出される関数を指定する
	Initialize(); //初期設定の関数を呼び出す
	glutMainLoop();

	return 0;
}

//----------------------------------------------------
// 初期設定の関数
//----------------------------------------------------
void Initialize(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0); //背景色
	glEnable(GL_DEPTH_TEST);//デプスバッファを使用：glutInitDisplayMode() で GLUT_DEPTH を指定する
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WindowWidth, 0, WindowHeight);
	glMatrixMode(GL_MODELVIEW);
}

//----------------------------------------------------
// 描画の関数
//----------------------------------------------------
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去

	for (int s = 0; s < static_cast<int>(2e-3 / dt); ++s) {
		object = mpm(SimulationTime++, object);
	}

	cout << "Simulation Time : " << SimulationTime << endl;

	DrawObject(object);

	glFlush();
};

void Idle() {
	glutPostRedisplay(); //glutDisplayFunc()を１回実行する
}

