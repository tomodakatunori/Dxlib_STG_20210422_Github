#pragma once

//�Q�[���S�̂̃w�b�_�t�@�C��
// 
//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"		//DxLib���g���̂ɕK�v

//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��"	//�Q�[���^�C�g��
#define GAME_WIDTH 1280				//�Q�[����ʂ̕�
#define GAME_HEIGTH 720				//�Q�[����ʂ̍���
#define GAME_COLOR 32				//�Q�[���̐F��
#define GAME_ICON_ID 333			//�Q�[���̃A�C�R��
#define GAME_WINDOWBER 0			//�E�B���h�E�o�[�̎��

#define GAME_DEBUG TRUE				//�f�o�b�O���[�h

//�񋓌^
enum GAME_SCENE
{
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
};	//�Q�[���̃V�[��
