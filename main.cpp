//�w�b�_�t�@�C���̓ǂݍ���
#include "game.h"		//�Q�[���̊�{
#include "keyboard.h"	//�L�[�{�[�h���g���̂ɕK�v
#include "FPS.h"		//FPS�̏���

#include <math.h>

//�}�N����`
#define	TAMA_DIV_MAX 4			//�e�̉摜�̍ő吔
#define TAMA_MAX 1024			//�e�̑���

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
	RECT coll;			//�����蔻��̗̈�(�l�p)
};

//����̍\����
struct MOVIE
{
	int handle = -1;	//����̃n���h��
	char path[255];		//����̃p�X

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	int Volume = 255;	//�{�����[���@�Q�T�T�`�O
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;		//���y�̃n���h��
	char path[255];			//���y�̃p�X
		
	int Volume = -1;		//���y�̃{�����[���i�O�`�Q�T�T�j
	int playType = -1;
};

//�e�̍\����
struct TAMA 
{
	int hundle[TAMA_DIV_MAX];	//�摜�̃n���h��
	char path[255];				//�摜�̃p�X

	int DivTate;				//�������c
	int DivYoko;				//��������

	int AnimeCnt = 0;			//�A�j���V�����J�E���^
	int AnimeCntMax = 10;		//�A�j���V�����J�E���^�̏���l

	int NowIndex = 0;		//���݂̉摜�̗v�f��

	int x;					//x�ʒu
	int y;					//y�ʒu
	int width;				//��
	int heigth;				//����

	int StartX;				//X�̍ŏ��̈ʒu
	int StartY;				//Y�̍ŏ��Ɉʒu

	int Speed;				//�e�̑���

	float radius;			//���a
	float degree;			//�p�x

	RECT coll;				//�����蔻��(��`)

	BOOL IsDraw = FALSE;	//�`��ł��邩�H

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

//�e�̍\����
struct TAMA tama_moto;	//��
struct TAMA tama[TAMA_MAX];	//���ۂɎg��

//�e�̔��˃J�E���^
int tamaShoCnt = 0;
int tamaShoCntMax = 10;

//�v���C���[
CHARACTOR player;

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
VOID CollUpdateTama(TAMA* tama);			//�e�̓����蔻��̍X�V

BOOL onCollRect(RECT a, RECT b);	//��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);	//�Q�[���̃f�[�^�̓ǂݍ���

BOOL LoadImageMem(IMAGE* image, const char* path);										//�Q�[���̉摜�̓ǂݍ���
BOOL  LoadimgDivMem(int* handle, const char* path, int bunnkatuYoko, int bunnkatuTate);	//�Q�[���̉摜�̕����̓ǂݍ���

VOID GameInit(VOID);	//�Q�[���̃f�[�^�̏�����

VOID DrawTama(TAMA* tama);	//�e�̕`��

VOID ShotTama(TAMA* tama, float deg);	//�e�̔���

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
		DeleteGraph(tama_moto.hundle[i]);
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
	//�e�̕�������ݒ�
	tama_moto.DivYoko = 4;
	tama_moto.DivTate = 1;

	//�e�̃p�X
	strcpyDx(tama_moto.path, ".//image//dia_pink.png");

	//�摜�𕪊����ēǂݍ���
	if (LoadimgDivMem(&tama_moto.hundle[0], tama_moto.path,tama_moto.DivYoko,tama_moto.DivTate) == FALSE) { return FALSE; }

	//��ʂ̕��ƍ������擾
	GetGraphSize(tama_moto.hundle[0],&tama_moto.width,&tama_moto.heigth );

	//�ʒu�����߂�
	tama_moto.x = GAME_WIDTH / 2 - tama_moto.width / 2;		//��������
	tama_moto.y = GAME_HEIGTH - tama_moto.heigth;			//�摜��

	tama_moto.Speed = 1;	//���x

	//�����蔻��̍X�V
	CollUpdateTama(&tama_moto);

	//�摜��\�����Ȃ�
	tama_moto.IsDraw = FALSE;

	//�S�Ă̒e�ɏ����R�s�[
	for (int i = 0; i < TAMA_MAX; i++)
	{
		tama[i] = tama_moto;
	}

	//�v���C���[�̉摜��ǂݍ���
	if (LoadImageMem(&player.img, ".//image//player.png") == FALSE) { return FALSE; }

	//�ʒu�����߂�
	player.img.x = GAME_WIDTH / 2 - player.img.width;
	player.img.y = GAME_HEIGTH / 2 - player.img.height;

	//�����蔻��̍X�V
	CollUpdatePlayer(&player);

	//�`��
	player.img.IsDraw = TRUE;

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
/// �摜���������ɓǂݍ���
/// </summary>
/// <param name="image">�摜�\���̂̃A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//�S�[���̉摜��ǂݍ���
	strcpyDx(image->path, path);	//�p�X�̃R�s�[
	image->handle = LoadGraph(image->path);	//�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			image->path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(image->handle, &image->width, &image->height);

	//�ǂݍ��߂�
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
		//������
		GameInit();

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
	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// �e�̕`��
/// </summary>
/// <param name="tama">�e�̍\����</param>
VOID DrawTama(TAMA* tama)
{
	//�e�̕`�悪�ł���Ƃ�
	if (tama->IsDraw == TRUE)
	{
		//�e�̕`��
		DrawGraph(tama->x, tama->y, tama->hundle[tama->NowIndex], TRUE);

		//�摜��ς���^�C�~���O
		if (tama->AnimeCnt < tama->AnimeCntMax)
		{
			tama->AnimeCnt++;
		}
		else
		{
			//�e�̓Y�������e�̕������̍ő�l��艺�̎�
			if (tama->NowIndex < TAMA_DIV_MAX - 1)
			{
				tama->NowIndex++;	//���̉摜
			}
			else
			{
				tama->NowIndex = 0;	//�ŏ��ɖ߂�
			}
			tama->AnimeCnt = 0;
		}
	}

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

	//�v���C���[�̑���
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		if (player.img.x - player.speed >= 0)
		{
			player.img.x -= player.speed;
		}
	}

	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		if (player.img.x + player.speed <= GAME_WIDTH)
		{
			player.img.x += player.speed;
		}
	}

	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		if (player.img.y - player.speed >= 0)
		{
			player.img.y -= player.speed;
		}
	}

	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		if (player.img.y + player.speed <= GAME_WIDTH)
		{
			player.img.y += player.speed;
		}
	}

	//�����蔻��̍X�V
	CollUpdatePlayer(&player);

	//�X�y�[�X�L�[����������
	if (KeyDown(KEY_INPUT_SPACE) == TRUE)
	{
		double a = -45.0,b = -90.0,c = -135.0;

		if (tamaShoCnt == 0)
		{
			//�e�𔭎˂���i�e��`�悷��j
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], a);

					//�e��1���o������A���[�v�𔲂���
					break;
				}	
			}

			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], b);

					//�e��1���o������A���[�v�𔲂���
					break;
				}
			}

			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], c);

					//�e��1���o������A���[�v�𔲂���
					break;
				}
			}
		}

		//�e�̔��ˑ҂�
		if (tamaShoCnt < tamaShoCntMax)
		{
			tamaShoCnt++;
		}
		else
		{
			tamaShoCnt = 0;
		}

	}

	//�e���΂�
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//�`�悳��Ă��鎞
		if (tama[i].IsDraw == TRUE)
		{
			//�e�̈ʒu�𒲐�
			tama[i].x = tama[i].StartX + cos(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;
			tama[i].y = tama[i].StartY + sin(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;

			//���a�𑫂�
			tama[i].radius += tama[i].Speed;

			//��ʂ̏o����`�ʂ��Ȃ�
			if (tama[i].y + tama[i].heigth < 0 ||	//��ʊO(��)
				tama[i].y > GAME_HEIGTH ||			//��ʊO(��)
				tama[i].x + tama[i].width < 0 ||	//��ʊO(��)
				tama[i].x > GAME_WIDTH)				//��ʊO(�E)
			{
				tama[i].IsDraw = FALSE;
			}
		}
	}

	return;
}

/// <summary>
/// �e���΂�
/// </summary>
/// <param name="tama"></param>
VOID ShotTama(TAMA* tama,float deg)
{
		//�e�̔���
		tama->IsDraw = TRUE;

		//�e���o��ʒu
		tama->StartX = player.img.x + player.img.width / 2 - tama->width / 2;
		tama->StartY = player.img.y;

		//�e�̑��x
		tama->Speed = 6;

		//�e�̊p�x
		tama->degree = deg;

		//�e�̔��a
		tama->radius = 0.0f;

		//�e�̓����蔻����X�V
		CollUpdateTama(tama);

}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	
	//�v���C���[�̕`��
	if (player.img.IsDraw == TRUE)
	{
		//�v���C���[�̕`��
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		//�����蔻��̕`��
		if (GAME_DEBUG == TRUE)
		{
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//�e�̕`��
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//�`�悳��Ă��鎞
		if (tama[i].IsDraw == TRUE)
		{
			DrawTama(&tama[i]);

			//�����蔻��̕`��
			if (GAME_DEBUG == TRUE)
			{
				DrawBox(tama[i].coll.left, tama[i].coll.top, tama[i].coll.right, tama[i].coll.bottom, GetColor(255, 0, 0), FALSE);
			}
		}	
	}


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
	chara->coll.left = chara->img.x + 10;						//�����̓����蔻��̒���
	chara->coll.top = chara->img.y + 10;						//�㑤�̓����蔻��̒���
	chara->coll.right = chara->img.x + chara->img.width - 10;	//�E���̓����蔻��̒���
	chara->coll.bottom = chara->img.y + chara->img.height -10;	//�����̓����蔻��̒���

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="Coll">�����蔻��̗̈�</param>
VOID CollUpdateTama(TAMA* rect)
{
	tama->coll.left = tama->x;					
	tama->coll.top = tama->y;						
	tama->coll.right = tama->x + tama->width;
	tama->coll.bottom = tama->y + tama->heigth;

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