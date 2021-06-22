//FPS�̃\�[�X�t�@�C��

//�w�b�_�t�@�C���̓ǂݍ���
#include "FPS.h"

//�O���[�o���ϐ�
FPS  fps;	//FPS�̊Ǘ�
/// <summary>
/// FPS���v�Z���A�l���X�V����
/// </summary>
/// <param name=""></param>
VOID FPSUpdate(VOID)
{
	//��ԍŏ��̏���
	if (FALSE == fps.IsInitFlg)
	{
		//����J�n���Ԃ��}�C�N���b�P�ʂŎ擾
		fps.StartTime = GetNowHiPerformanceCount();	//windous���N�����Ă���o�߂�������(�}�C�N���b)

		fps.IsInitFlg = TRUE;	//�t���O�𗧂Ă�
	}

	//���݂̎������}�C�N���b�P�ʂŎ擾
	fps.NowTime = GetNowHiPerformanceCount();

	//�O��擾�������Ԃ���̌o�ߎ��ԕb�i�����j�ɕϊ����Ă���Z�b�g
	fps.DeltaTime = (fps.NowTime - fps.OldTime) / 1000000.0f;

	//����擾�������Ԃ�ۑ�
	fps.OldTime = fps.NowTime;

	//�P�t���[���ځ`FPS����l�܂ł́A�J�E���g�A�b�v
	if (fps.Count < fps.SampleRate)
	{
		//�J�E���g�𑝂₷
		fps.Count++;
	}
	else
	{
		//FPS�ݒ�l�̃t���[���ŁA����FPS�𑪒�

		//���݂̎�������O�t���[���ڂ̎��Ԃ������AFPS�̐��l�Ŋ���
		fps.DrawValue = 1000000.f / ((fps.NowTime - fps.StartTime) / (float)fps.SampleRate);

		//����J�n�������}�C�N���b�P�ʂŎ擾
		fps.StartTime = GetNowHiPerformanceCount();	//Windows���N�����Ă���o�߂������ԁi�}�C�N���b�j

		//�J�E���g�J�n
		fps.Count = 1;
	}

	return;
}

/// <summary>
/// ����FPS�l��`��
/// </summary>
/// <param name=""></param>
VOID FPSDraw(VOID)
{
	if (GAME_DEBUG ==TRUE)
	{
		//�������`��
		DrawFormatString(0, GAME_HEIGTH - 20, GetColor(0, 0, 0), "FPS:%.1f", fps.DrawValue);
	}
	

	return;
}

/// <summary>
/// FPS�ŏ�����҂�
/// </summary>
/// <param name=""></param>
VOID FPSWeit(VOID)
{
	//���݂̎����A�ŏ��̎��ԂŁA���݂������Ă��鎞�����擾����
	LONGLONG resultTime = fps.NowTime - fps.StartTime;

	//�҂ׂ��~���b���@(1�b�^fps�l�����݂̃t���[����)����A���݂������Ă��鎞��������
	int waitTime = 1000000.0f / fps.Value * fps.Count - resultTime;

	//�}�C�N���b����~���b�ɕϊ�
	waitTime /= 1000.0f;

	//����������������A������҂�
	if (waitTime > 0)
	{
		WaitTimer(waitTime);	//�����~���b�҂�
	}

	//����������OFF�ɂȂ��Ă��邩
	if (GetWaitVSyncFlag() == FALSE)
	{
		//FPS�ő�l�łȂ��Ƃ�
		if (fps.Value < GAME_FPS_MAX)
		{
			//1�b����FPS�l�����A�҂��Ԃ��������Ƃ��́A������FPS�l���グ�Ă�����
			//�҂��ԁ@10�~���@<= 1�b/60fps = 16.666�~���@�������������ł���
			if (waitTime > 0
				&& waitTime <= 1000.0f / fps.Value)
			{
				fps.Value++;
			}
			else
			{
				//FPS�l���ǂ����Ă��炸�A�x�����́AFPS��������
				if (fps.Value = GAME_FPS_MIN)
				{
					fps.Value--;
				}
			}
		}
	}

	return;
}