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
// �֐��v���g�^�C�v�i��ɌĂяo���֐����ƈ����̐錾�j
//----------------------------------------------------
void Initialize(void);
void Display(void);
void Idle();

//----------------------------------------------------
// ���C���֐�
//----------------------------------------------------
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);//���̏�����
	glutInitWindowPosition(WindowPositionX, WindowPositionY); //�E�B���h�E�̈ʒu�̎w��
	glutInitWindowSize(WindowWidth, WindowHeight); //�E�B���h�E�T�C�Y�̎w��
	glutInitDisplayMode(GLUT_RGBA); //�f�B�X�v���C���[�h�̎w��

	//�E�B���h�E�̍쐬
	if (!glutCreateWindow(WindowTitle)) {
		std::cerr << "�E�B���h�E�̍쐬�Ɏ��s���܂����B" << std::endl;
		return -1;
	}

	glutIdleFunc(Idle); //�v���O�����A�C�h����Ԏ�(�ɂȎ�)�ɌĂяo�����֐�
	glutDisplayFunc(Display); //�`�掞�ɌĂяo�����֐����w�肷��
	Initialize(); //�����ݒ�̊֐����Ăяo��
	glutMainLoop();

	return 0;
}

//----------------------------------------------------
// �����ݒ�̊֐�
//----------------------------------------------------
void Initialize(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0); //�w�i�F
	glEnable(GL_DEPTH_TEST);//�f�v�X�o�b�t�@���g�p�FglutInitDisplayMode() �� GLUT_DEPTH ���w�肷��
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WindowWidth, 0, WindowHeight);
	glMatrixMode(GL_MODELVIEW);
}

//----------------------------------------------------
// �`��̊֐�
//----------------------------------------------------
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�o�b�t�@�̏���

	for (int s = 0; s < static_cast<int>(2e-3 / dt); ++s) {
		object = mpm(SimulationTime++, object);
	}

	cout << "Simulation Time : " << SimulationTime << endl;

	DrawObject(object);

	glFlush();
};

void Idle() {
	glutPostRedisplay(); //glutDisplayFunc()���P����s����
}

