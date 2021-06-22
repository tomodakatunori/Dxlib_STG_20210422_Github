//�L�[�{�[�h�̃\�[�X�t�@�C��

//�w�b�_�[�t�@�C���̓ǂݍ���
#include "keyboard.h"

//�O���[�o���ϐ�
KEYBOARD keyboard;

//�֐�

/// <summary>
/// �L�[�̓��͏�Ԃ��X�V����
/// </summary>
VOID AllKeyUpdate(VOID)
{
	//���O�̃L�[���͂��Ƃ��Ă���
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{
		keyboard.OldKeyState[i] = keyboard.AllKeyState[i];
	}

	//���ׂẴL�[�̓��͏�Ԃ𓾂�
	GetHitKeyStateAll(keyboard.TempKeyState);

	// ������Ă���L�[�̎��Ԃ��X�V����
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{
		//�L�[�R�[�h�������Ă���Ƃ�
		if (keyboard.TempKeyState[i] != 0)
		{
			keyboard.AllKeyState[i]++;
		}
		else
		{
			keyboard.AllKeyState[i] = 0;
		}
	}

	return;
}

/// <summary>
/// �L�[�������Ă��邩�A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT">�L�[�R�[�h</param>
/// <returns>�����Ă�����TRUE</returns>
BOOL KeyDown(int KEY_INPUT_)
{
	if (keyboard.AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// �L�[���グ�Ă��邩�A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT">�L�[�R�[�h</param>
/// <returns>�グ�Ă�����TRUE</returns>
BOOL KeyUp(int KEY_INPUT_)
{
	if (
		keyboard.OldKeyState[KEY_INPUT_] != 0	//�ȑO�͉����Ă�
		&& keyboard.AllKeyState[KEY_INPUT_] == 0	//���݂͉����ĂȂ�
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// �L�[���N���b�N�������A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT">�L�[�R�[�h</param>
/// <returns>�N���b�N������TRUE</returns>
BOOL KeyClick(int KEY_INPUT_)
{
	if (
		keyboard.OldKeyState[KEY_INPUT_] != 0	//�ȑO�͉����Ă�
		&& keyboard.AllKeyState[KEY_INPUT_] == 0	//���݂͉����ĂȂ�
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/// <summary>
/// �L�[�����������Ă��邩�A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT">�L�[�R�[�h</param>
/// <param name="MillitTime">�L�[�����������Ă���~���b</param>
/// <returns></returns>
BOOL KeyDownKeep(int KEY_INPUT, int MillitTime)
{
	//1�b��1000�~���b
	float MillSec = 1000.0f;

	//���������Ă��鎞�Ԃ́A�~���b�~FPS�l
	int UpdateTime = (MillitTime / MillSec) * 60;

	if (keyboard.AllKeyState[ KEY_INPUT ] > UpdateTime)
	{
		return TRUE;	//���������Ă���
	}
	else
	{
		return FALSE;	//���������Ă��Ȃ�
	}
}