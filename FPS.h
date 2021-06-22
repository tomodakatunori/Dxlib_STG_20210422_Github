#pragma once

//�w�b�_�t�@�C���̓ǂݍ���]
#include"game.h"	//�Q�[���S�̂̃w�b�_�t�@�C��

//�}�N����`
#define GAME_FPS		60	//FPS�̐��l
#define GAME_FPS_MAX	240	//FPS�̍ő�l
#define GAME_FPS_MIN	60	//FPS�̍ŏ��l

//FPS�\����
struct FPS
{
	BOOL IsInitFlg = FALSE;	//�ŏ��̑���t���O
	LONGLONG StartTime = 0;	//����J�n����
	LONGLONG NowTime = 0;	//���݂̎���
	LONGLONG OldTime = 0;	//�ȑO�̎���

	int Value = GAME_FPS;	//FPS�̒l

	float DeltaTime = 0.000001;	//�O��5�A�o�ߎ���
	int Count = 1;				//���݂̃t���[����
	float DrawValue = 0.0f;		//�v�Z���ʂ�`��
	int SampleRate = GAME_FPS;	//���ς����T���v���l
};

//�O���[�o���ϐ��̊O���Q��
extern FPS fps;	//FPS�̊Ǘ�

//�v���g�^�C�v�錾
VOID FPSUpdate(VOID);	//fps�l���X�V
VOID FPSDraw(VOID);		//fps�l��`��
VOID FPSWeit(VOID);		//fps�l��҂�