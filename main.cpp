//�w�b�_�t�@�C���̓ǂݍ���
#include "game.h"		//�Q�[���̊�{
#include "keyboard.h"	//�L�[�{�[�h���g���̂ɕK�v
#include"FPS.h"			//FPS�̏���

//�}�N����`
#define	TAMA_DIV_MAX	4

//�\���̂̒�`

//�摜�̍\����
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h��
	char path[255];		//�摜�̏ꏊ�i�p�X�j
	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	RECT coll;				//�����蔻��̗̈�(�l�p)
	BOOL IsDraw = FALSE;	//�摜���`��ł��邩
};

//�L�����N�^�̍\����
struct CHARACTOR
{
    IMAGE img;
	int speed = 5;		//�ړ����x
	RECT coll;				//�����蔻��̗̈�(�l�p)
};

//����̍\����
struct MOVIE
{
	int handle = -1;	//����̃n���h��
	char path[255];		//����̃p�X

	int x;		//X�ʒu
	int y;		//Y�ʒu
	int width;	//��
	int height;	//����

	int Volume = 255;	//�{�����[���@�Q�T�T�`�O
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;		//���y�̃n���h��
	char path[255];		//���y�̃p�X

	int Volume = -1;	//���y�̃{�����[���i�O�`�Q�T�T�j
	int playType = -1;
};

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���V�[��
GAME_SCENE OldGameScene;	//�O��̃Q�[���V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���V�[��

//��ʂ̐؂�ւ�
BOOL IsfadeOut = FALSE;	//�t�F�[�h�A�E�g
BOOL IsfadeIn = FALSE;	//�t�F�[�h�C��

//�V�[���؂�ւ�
int fadeTimeMill = 2000;					//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//�~���b���t���[���ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���g
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;	//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�C���̃J�E���g
int fadeInCntMax = 0;				//�t�F�[�h�C���̃J�E���^MAX

//�e�̉摜�̃n���h��
int Tama[TAMA_DIV_MAX];
int TamaIndex = 0;			//�摜�̓Y����
int TamaChangeCnt = 0;		//�摜��ς���^�C�~���O
int TamaChangeCntMax = 60;	//�摜��ς���^�C�~���OMax

//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g�����(����)
VOID TitleDraw(VOID);	//�^�C�g�����(�`��)

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C���(����)
VOID PlayDraw(VOID);	//�v���C���(�`��)

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h���(����)
VOID EndDraw(VOID);		//�G���h���(�`��)

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ����(����)
VOID ChangeDraw(VOID);	//�؂�ւ����(�`��)

VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V
VOID CollUpdate(CHARACTOR* chara);			//�����蔻��

BOOL onCollRect(RECT a, RECT b);	//��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);	//�Q�[���̃f�[�^�̓ǂݍ���

BOOL  LoadimgDivMem(int* handle, const char* path, int bunnkatuYoko, int bunnkatuTate);	//�Q�[���̉摜�̕����̓ǂݍ���

VOID GameInit(VOID);	//�Q�[���̃f�[�^�̏�����

// �v���O������ WinMain ����n�܂�܂�
//windous�̃v���O���~���O��winAPI�œ����Ă���
//DxLib��Directx�Ƃ����Q�[���v���O�������ȒP�Ɏg����d�g��
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txt��\�����Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);						//�E�B���h�E�̃^�C�g������
	SetGraphMode(GAME_WIDTH, GAME_HEIGTH, GAME_COLOR);	//�E�B���h�E�̉𑜓x
	SetWindowSize(GAME_WIDTH, GAME_HEIGTH);				//�E�B���h�E�̑傫��
	SetBackgroundColor(255, 255, 255);					//�w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						//�E�B���h�E�̃A�C�R��
	SetWindowStyleMode(GAME_WINDOWBER);					//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐��������̗L��
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���	

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	//�_�u���o�b�t�@�����O�̗L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�ŏ��̃V�[���̓^�C�g����ʂ�����
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���̓ǂݍ���.
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ��݂Ɏ��s������
		DxLib_End;	//Dxlib�I��
		return -1;	//�ُ�I��
	}

	//�Q�[���̏�����
	GameInit();

	//�������[�v
	while (1)
	{

		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	//��ʂ̏���

		//�L�[�{�[�h�̓��͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�|�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃ̏���
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();	//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();		//�v���C���
			break;
		case GAME_SCENE_END:
			End();		//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();	//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWeit();

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��

	}

	//�I��鎞�̏���

	//�ǂݍ��񂾉摜�̊J��
	for (int i = 0; i < TAMA_DIV_MAX; i++)
	{
		DeleteGraph(Tama[i]);
	}
	
	DxLib_End();	//�c�w���C�u�����g�p�̏I������

	return 0;		//�\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE�A�ǂݍ��߂Ȃ������Ȃ�FARSE</returns>
BOOL GameLoad(VOID)
{
	//�摜�𕪊����ēǂݍ���
	if (LoadimgDivMem(&Tama[0], ".\\image\\tama.png", 4, 1) == FALSE) { return FALSE; }

	return TRUE;	//���ׂēǂݍ��߂�
}

/// <summary>
/// �摜�𕪊����ă������ɓǂݍ���
/// </summary>
/// <param name="handle">�n���h���z��̐擪�A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <param name="bunnkatuYoko">��������Ƃ��̉��̐�</param>
/// <param name="bunnkatuTate">��������Ƃ��̏c�̐�</param>
/// <returns></returns>
BOOL LoadimgDivMem(int* handle, const char* path, int bunnkatuYoko, int bunnkatuTate)
{

	//�e�̓ǂݍ���
	int isTamaLoad = -1;	//�摜���ǂݍ��߂����H

	//�ꎞ�I�Ɏg���n���h��
	int TamaHandle = LoadGraph(path);

	//�ǂݍ��݃G���[
	if (TamaHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,				//�{��
			"�摜�ǂݍ��݃G���[",	//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;
	}

	//�摜�̕��ƍ������擾
	int TamaWidth = 1;	//��
	int TamaHeight = 1;	//����
	GetGraphSize(TamaHandle, &TamaWidth, &TamaHeight);

	//�摜�̕���
	isTamaLoad = LoadDivGraph(
		path,													//�摜�̃p�X
		TAMA_DIV_MAX,											//��������
		bunnkatuYoko, bunnkatuTate,								//���A�c�̕���
		TamaWidth / bunnkatuYoko, TamaHeight / bunnkatuTate,	//�摜1���̕��A����
		handle													//�A���ŊǗ�����z��̐擪�A�h���X
	);

	//�����G���[
	if (isTamaLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,					//�{��
			"�摜�����G���[",		//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;
	}

	//�ꎞ�I�ɓǂݍ��񂾃n���h�������
	DeleteGraph(TamaHandle);

	return TRUE;
}

/// <summary>
/// �Q�[���f�[�^��������
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	

}

/// <summary>
/// �V�[����؂�ւ���
/// </summary>
/// <param name="">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;				//�V�[���ɐ؂�ւ���
	IsfadeIn = FALSE;				//�t�F�[�h�C�����Ȃ�
	IsfadeOut = TRUE;				//�t�F�[�h�A�E�g����

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{
	//�v���C���[�̑���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		//�����������I��
		return;
	}
	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{

	//�e�̕`��
	DrawGraph(0, 0, Tama[TamaIndex], TRUE);

	//�摜��ς���^�C�~���O
	if (TamaChangeCnt < TamaChangeCntMax)
	{
		TamaChangeCnt++;
	}
	else
	{
		//�e�̓Y�������e�̕�������艺�̎�
		if (TamaIndex < TAMA_DIV_MAX - 1)
		{
			TamaIndex++;	//���̉摜
		}
		else
		{
			TamaIndex = 0;
		}
		TamaChangeCnt = 0;
	}
	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();		//����
	PlayDraw();		//�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	
	//�v���C���[�̑���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

		//�����������I��
		return;
	}

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	

	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();		//����
	EndDraw();		//�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	//�v���C���[�̑���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{

		//�X�^�[�g��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		//�����������I��
		return;
	}
	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();		//����
	ChangeDraw();		//�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsfadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//�J�E���g�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����
			fadeInCnt = fadeInCntInit;	//�J�E���g������
			IsfadeIn = FALSE;			//�t�F�[�h�C����������
		}
	}

	//�t�F�[�h�A�E�g
	if (IsfadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//�J�E���g�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����
			fadeOutCnt = fadeOutCntInit;	//�J�E���g������
			IsfadeOut = FALSE;			//�t�F�[�h�C����������
		}
	}

	//�؂�ւ������I���H
	if (IsfadeIn == FALSE && IsfadeOut == FALSE)
	{
		//�t�F�[�h�C�����ĂȂ��A�t�F�[�h�A�E�g�����Ă��Ȃ���
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃Q�[���V�[���X�V
	}

	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h��ʂ̕`��
		break;
	case GAME_SCENE_CHANGE:
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsfadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsfadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGTH,GetColor(0,0,0),TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �����蔻��̗̈�X�V(�v���C���[)
/// </summary>
/// <param name="Coll">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 70;
	chara->coll.top = chara->img.y + 80;
	chara->coll.right = chara->img.x + chara->img.width - 70;
	chara->coll.bottom = chara->img.y + chara->img.height - 150;

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="Coll">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;					
	chara->coll.top = chara->img.y;						
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// ��`�Ƌ�`�̓����蔻��
/// </summary>
/// <param name="a">��`A</param>
/// <param name="b">��`B</param>
/// <returns>����������TRUE�A������Ȃ�������FALSE</returns>
BOOL onCollRect(RECT a, RECT b)
{
	if (a.left < b.right &&		//a�̍���X���W < b�̉E��X���W����
		a.right > b.left &&		//a�̉E��X���W > b�̍���X���W����
		a.top < b.bottom &&		//a�̏��Y���W < b�̉���Y���W����
		a.bottom > b.top		//a�̉���Y���W > b�̏��Y���W
		)
	{
		//�������Ă��鎞
		return TRUE;
	}
	else
	{
		//�������Ă��Ȃ���
		return FALSE;
	}
}