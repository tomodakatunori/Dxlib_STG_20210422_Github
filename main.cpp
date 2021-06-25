//ヘッダファイルの読み込み
#include "game.h"		//ゲームの基本
#include "keyboard.h"	//キーボードを使うのに必要
#include"FPS.h"			//FPSの処理

//マクロ定義
#define	TAMA_DIV_MAX	4

//構造体の定義

//画像の構造体
struct IMAGE
{
	int handle = -1;	//画像のハンドル
	char path[255];		//画像の場所（パス）
	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ

	RECT coll;				//当たり判定の領域(四角)
	BOOL IsDraw = FALSE;	//画像が描画できるか
};

//キャラクタの構造体
struct CHARACTOR
{
    IMAGE img;
	int speed = 5;		//移動速度
	RECT coll;				//当たり判定の領域(四角)
};

//動画の構造体
struct MOVIE
{
	int handle = -1;	//動画のハンドル
	char path[255];		//動画のパス

	int x;		//X位置
	int y;		//Y位置
	int width;	//幅
	int height;	//高さ

	int Volume = 255;	//ボリューム　２５５～０
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;		//音楽のハンドル
	char path[255];		//音楽のパス

	int Volume = -1;	//音楽のボリューム（０～２５５）
	int playType = -1;
};

//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームシーン
GAME_SCENE OldGameScene;	//前回のゲームシーン
GAME_SCENE NextGameScene;	//次のゲームシーン

//画面の切り替え
BOOL IsfadeOut = FALSE;	//フェードアウト
BOOL IsfadeIn = FALSE;	//フェードイン

//シーン切り替え
int fadeTimeMill = 2000;					//切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//ミリ秒をフレームに変換

//フェードアウト
int fadeOutCntInit = 0;				//初期値
int fadeOutCnt = fadeOutCntInit;	//フェードアウトのカウント
int fadeOutCntMax = fadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int fadeInCntInit = fadeTimeMax;	//初期値
int fadeInCnt = fadeInCntInit;		//フェードインのカウント
int fadeInCntMax = 0;				//フェードインのカウンタMAX

//弾の画像のハンドル
int Tama[TAMA_DIV_MAX];
int TamaIndex = 0;			//画像の添え字
int TamaChangeCnt = 0;		//画像を変えるタイミング
int TamaChangeCntMax = 60;	//画像を変えるタイミングMax

//プロトタイプ宣言
VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面(処理)
VOID TitleDraw(VOID);	//タイトル画面(描画)

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面(処理)
VOID PlayDraw(VOID);	//プレイ画面(描画)

VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面(処理)
VOID EndDraw(VOID);		//エンド画面(描画)

VOID Change(VOID);		//切り替え画面
VOID ChangeProc(VOID);	//切り替え画面(処理)
VOID ChangeDraw(VOID);	//切り替え画面(描画)

VOID ChangeScene(GAME_SCENE scene);	//シーン切り替え

VOID CollUpdatePlayer(CHARACTOR* chara);	//当たり判定の領域を更新
VOID CollUpdate(CHARACTOR* chara);			//当たり判定

BOOL onCollRect(RECT a, RECT b);	//矩形と矩形の当たり判定

BOOL GameLoad(VOID);	//ゲームのデータの読み込み

BOOL  LoadimgDivMem(int* handle, const char* path, int bunnkatuYoko, int bunnkatuTate);	//ゲームの画像の分割の読み込み

VOID GameInit(VOID);	//ゲームのデータの初期化

// プログラムは WinMain から始まります
//windousのプログラミング＝winAPIで動いている
//DxLibはDirectxというゲームプログラムを簡単に使える仕組み
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txtを表示しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);						//ウィンドウのタイトル文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGTH, GAME_COLOR);	//ウィンドウの解像度
	SetWindowSize(GAME_WIDTH, GAME_HEIGTH);				//ウィンドウの大きさ
	SetBackgroundColor(255, 255, 255);					//背景の色
	SetWindowIconID(GAME_ICON_ID);						//ウィンドウのアイコン
	SetWindowStyleMode(GAME_WINDOWBER);					//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期の有効
	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする	

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	//ダブルバッファリングの有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//最初のシーンはタイトル画面だから
	GameScene = GAME_SCENE_TITLE;

	//ゲーム全体の初期化

	//ゲームの読み込み.
	if (!GameLoad())
	{
		//データの読み込みに失敗した時
		DxLib_End;	//Dxlib終了
		return -1;	//異状終了
	}

	//ゲームの初期化
	GameInit();

	//無限ループ
	while (1)
	{

		if (ProcessMessage() != 0) { break; }	//メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; }	//画面の消去

		//キーボードの入力の更新
		AllKeyUpdate();

		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシ－ンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとの処理
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();	//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();		//プレイ画面
			break;
		case GAME_SCENE_END:
			End();		//エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();	//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		//FPS値を描画
		FPSDraw();

		//FPS値を待つ
		FPSWeit();

		ScreenFlip();	//ダブルバッファリングした画面を描画

	}

	//終わる時の処理

	//読み込んだ画像の開放
	for (int i = 0; i < TAMA_DIV_MAX; i++)
	{
		DeleteGraph(Tama[i]);
	}
	
	DxLib_End();	//ＤＸライブラリ使用の終了処理

	return 0;		//ソフトの終了 
}

/// <summary>
/// ゲームのデータを読み込み
/// </summary>
/// <returns>読み込めたらTRUE、読み込めなかったならFARSE</returns>
BOOL GameLoad(VOID)
{
	//画像を分割して読み込み
	if (LoadimgDivMem(&Tama[0], ".\\image\\tama.png", 4, 1) == FALSE) { return FALSE; }

	return TRUE;	//すべて読み込めた
}

/// <summary>
/// 画像を分割してメモリに読み込み
/// </summary>
/// <param name="handle">ハンドル配列の先頭アドレス</param>
/// <param name="path">画像のパス</param>
/// <param name="bunnkatuYoko">分割するときの横の数</param>
/// <param name="bunnkatuTate">分割するときの縦の数</param>
/// <returns></returns>
BOOL LoadimgDivMem(int* handle, const char* path, int bunnkatuYoko, int bunnkatuTate)
{

	//弾の読み込み
	int isTamaLoad = -1;	//画像が読み込めたか？

	//一時的に使うハンドル
	int TamaHandle = LoadGraph(path);

	//読み込みエラー
	if (TamaHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウィンドウハンドル
			path,				//本文
			"画像読み込みエラー",	//タイトル
			MB_OK					//ボタン
		);

		return FALSE;
	}

	//画像の幅と高さを取得
	int TamaWidth = 1;	//幅
	int TamaHeight = 1;	//高さ
	GetGraphSize(TamaHandle, &TamaWidth, &TamaHeight);

	//画像の分割
	isTamaLoad = LoadDivGraph(
		path,													//画像のパス
		TAMA_DIV_MAX,											//分割総数
		bunnkatuYoko, bunnkatuTate,								//横、縦の分割
		TamaWidth / bunnkatuYoko, TamaHeight / bunnkatuTate,	//画像1つ分の幅、高さ
		handle													//連続で管理する配列の先頭アドレス
	);

	//分割エラー
	if (isTamaLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウィンドウハンドル
			path,					//本文
			"画像分割エラー",		//タイトル
			MB_OK					//ボタン
		);

		return FALSE;
	}

	//一時的に読み込んだハンドルを介抱
	DeleteGraph(TamaHandle);

	return TRUE;
}

/// <summary>
/// ゲームデータを初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	

}

/// <summary>
/// シーンを切り替える
/// </summary>
/// <param name="">シーン</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;				//シーンに切り替える
	IsfadeIn = FALSE;				//フェードインしない
	IsfadeOut = TRUE;				//フェードアウトする

	return;
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//処理
	TitleDraw();	//描画

	return;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
VOID TitleProc(VOID)
{
	//プレイヤーの操作
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);

		//処理を強制終了
		return;
	}
	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
VOID TitleDraw(VOID)
{

	//弾の描画
	DrawGraph(0, 0, Tama[TamaIndex], TRUE);

	//画像を変えるタイミング
	if (TamaChangeCnt < TamaChangeCntMax)
	{
		TamaChangeCnt++;
	}
	else
	{
		//弾の添え字が弾の分割数より下の時
		if (TamaIndex < TAMA_DIV_MAX - 1)
		{
			TamaIndex++;	//次の画像
		}
		else
		{
			TamaIndex = 0;
		}
		TamaChangeCnt = 0;
	}
	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();		//処理
	PlayDraw();		//描画

	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
VOID PlayProc(VOID)
{
	
	//プレイヤーの操作
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);

		//処理を強制終了
		return;
	}

	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{
	

	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();		//処理
	EndDraw();		//描画

	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
VOID EndProc(VOID)
{
	//プレイヤーの操作
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{

		//スタート画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);

		//処理を強制終了
		return;
	}
	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();		//処理
	ChangeDraw();		//描画

	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
VOID ChangeProc(VOID)
{
	//フェードイン
	if (IsfadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//カウントを減らす
		}
		else
		{
			//フェードイン処理が終わった
			fadeInCnt = fadeInCntInit;	//カウント初期化
			IsfadeIn = FALSE;			//フェードイン処理完了
		}
	}

	//フェードアウト
	if (IsfadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//カウントを減らす
		}
		else
		{
			//フェードイン処理が終わった
			fadeOutCnt = fadeOutCntInit;	//カウント初期化
			IsfadeOut = FALSE;			//フェードイン処理完了
		}
	}

	//切り替え処理終了？
	if (IsfadeIn == FALSE && IsfadeOut == FALSE)
	{
		//フェードインしてない、フェードアウトもしていない時
		GameScene = NextGameScene;	//次のシーンに切り替え
		OldGameScene = GameScene;	//以前のゲームシーン更新
	}

	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();		//エンド画面の描画
		break;
	case GAME_SCENE_CHANGE:
		break;
	default:
		break;
	}

	//フェードイン
	if (IsfadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (IsfadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//四角を描写
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGTH,GetColor(0,0,0),TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 当たり判定の領域更新(プレイヤー)
/// </summary>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 70;
	chara->coll.top = chara->img.y + 80;
	chara->coll.right = chara->img.x + chara->img.width - 70;
	chara->coll.bottom = chara->img.y + chara->img.height - 150;

	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;					
	chara->coll.top = chara->img.y;						
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 矩形と矩形の当たり判定
/// </summary>
/// <param name="a">矩形A</param>
/// <param name="b">矩形B</param>
/// <returns>当たったらTRUE、当たらなかったらFALSE</returns>
BOOL onCollRect(RECT a, RECT b)
{
	if (a.left < b.right &&		//aの左辺X座標 < bの右辺X座標かつ
		a.right > b.left &&		//aの右辺X座標 > bの左辺X座標かつ
		a.top < b.bottom &&		//aの上辺Y座標 < bの下辺Y座標かつ
		a.bottom > b.top		//aの下辺Y座標 > bの上辺Y座標
		)
	{
		//当たっている時
		return TRUE;
	}
	else
	{
		//当たっていない時
		return FALSE;
	}
}