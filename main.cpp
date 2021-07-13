//ヘッダファイルの読み込み
#include "game.h"		//ゲームの基本
#include "keyboard.h"	//キーボードを使うのに必要
#include "FPS.h"		//FPSの処理

#include <math.h>

//マクロ定義
#define	TAMA_DIV_MAX 4			//弾の画像の最大数
#define TAMA_MAX 1024			//弾の総数

#define TEKI_KIND 8				//敵の種類
#define TEKI_MAX 10				//敵の最大の数

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
	RECT coll;			//当たり判定の領域(四角)
};

//動画の構造体
struct MOVIE
{
	int handle = -1;	//動画のハンドル
	char path[255];		//動画のパス

	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ

	int Volume = 255;	//ボリューム　２５５～０
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;		//音楽のハンドル
	char path[255];			//音楽のパス
		
	int Volume = -1;		//音楽のボリューム（０～２５５）
	int playType = -1;
};

//弾の構造体
struct TAMA 
{
	int hundle[TAMA_DIV_MAX];	//画像のハンドル
	char path[255];				//画像のパス

	int DivTate;				//分割数縦
	int DivYoko;				//分割数横
	int DIVMax;					//分割総数

	int AnimeCnt = 0;			//アニメションカウンタ
	int AnimeCntMax = 10;		//アニメションカウンタの上限値

	int NowIndex = 0;		//現在の画像の要素数

	int x;					//x位置
	int y;					//y位置
	int width;				//幅
	int heigth;				//高さ

	int StartX;				//Xの最初の位置
	int StartY;				//Yの最初に位置

	int Speed;				//弾の速さ

	float radius;			//半径
	float degree;			//角度

	RECT coll;				//当たり判定(矩形)

	BOOL IsDraw = FALSE;	//描画できるか？

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

//弾の構造体
struct TAMA tama_moto;		//元
struct TAMA tama[TAMA_MAX];	//実際に使う

//弾の発射カウンタ
int tamaShoCnt = 0;
int tamaShoCntMax = 10;

//プレイヤー
CHARACTOR player;

//背景画像
IMAGE back[2];	//背景は2つの画像

//敵の種類
CHARACTOR teki_moto[TEKI_KIND];

//実際の敵の数
CHARACTOR teki[TEKI_MAX];

//敵データのパス
char tekiPath[TEKI_KIND][255] =
{
	{".\\image\\teki_blue.png" },
	{".\\image\\teki_gray.png" },
	{".\\image\\teki_green.png" },
	{".\\image\\teki_mizu.png" },
	{".\\image\\teki_purple.png" },
	{".\\image\\teki_red.png" },
	{".\\image\\teki_red_big.png" },
	{".\\image\\teki_yellow.png" }
};

//敵が出てくるカウント
int TekiAddCnt = 0;
int tekiAddCntMax = 60;		//60フレーム＝1秒

//ゲームのスコア
int Score = 0;

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
VOID CollUpdateTama(TAMA* tama);			//弾の当たり判定の更新
VOID CollUpdateTeki(CHARACTOR* chara);		//敵の当たり判定

BOOL onCollRect(RECT a, RECT b);	//矩形と矩形の当たり判定

BOOL GameLoad(VOID);	//ゲームのデータの読み込み

BOOL LoadImageMem(IMAGE* image, const char* path);										//ゲームの画像の読み込み
BOOL  LoadimgDivMem(int* handle, const char* path, int bunnkatuYoko, int bunnkatuTate);	//ゲームの画像の分割の読み込み

VOID GameInit(VOID);	//ゲームのデータの初期化

VOID DrawTama(TAMA* tama);	//弾の描画

VOID ShotTama(TAMA* tama, float deg);	//弾の発射

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
		DeleteGraph(tama_moto.hundle[i]);
	}

	//プレイヤー開放
	DeleteGraph(player.img.handle);

	//背景画像開放
	DeleteGraph(back[0].handle);
	DeleteGraph(back[1].handle);
	
	//敵の画像の開放
	for (int i = 0; i < TEKI_KIND; i++)
	{
		DeleteGraph(teki_moto[i].img.handle);
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
	//弾の分割数を設定
	tama_moto.DivYoko = 4;
	tama_moto.DivTate = 1;

	//弾のパス
	strcpyDx(tama_moto.path, ".//image//dia_pink.png");

	//画像を分割して読み込み
	if (LoadimgDivMem(&tama_moto.hundle[0], tama_moto.path,tama_moto.DivYoko,tama_moto.DivTate) == FALSE) { return FALSE; }

	//画面の幅と高さを取得
	GetGraphSize(tama_moto.hundle[0],&tama_moto.width,&tama_moto.heigth );

	//位置を決める
	tama_moto.x = GAME_WIDTH / 2 - tama_moto.width / 2;		//中央揃え
	tama_moto.y = GAME_HEIGTH - tama_moto.heigth;			//画像下

	tama_moto.Speed = 1;	//速度

	//当たり判定の更新
	CollUpdateTama(&tama_moto);

	//画像を表示しない
	tama_moto.IsDraw = FALSE;

	//全ての弾に情報をコピー
	for (int i = 0; i < TAMA_MAX; i++)
	{
		tama[i] = tama_moto;
	}

	//プレイヤーの画像を読み込む
	if (LoadImageMem(&player.img, ".//image//player.png") == FALSE) { return FALSE; }

	//位置を決める
	player.img.x = GAME_WIDTH / 2 - player.img.width;
	player.img.y = GAME_HEIGTH / 2 - player.img.height;

	//当たり判定の更新
	CollUpdatePlayer(&player);

	//描画
	player.img.IsDraw = TRUE;

	//背景の画像を読み込む①
	if (LoadImageMem(&back[0], ".//image//hoshi.png") == FALSE) { return FALSE; }
	back[0].x = 0;					//位置を決める
	back[0].y = -back[0].height;	//位置を決める
	back[0].IsDraw = TRUE;	//描画

	//背景の画像を読み込む②
	if (LoadImageMem(&back[1], ".//image//hoshi_rev.png") == FALSE) { return FALSE; }
	back[1].x = 0;					//位置を決める
	back[1].y = 0;					//位置を決める
	back[1].IsDraw = TRUE;			//描画

	//敵の画像を読み込む
	for (int i = 0; i < TEKI_KIND; i++)
	{
		if (LoadImageMem(&teki_moto[i].img, tekiPath[i]) == FALSE) { return FALSE; }
		teki_moto[i].img.x = GAME_WIDTH / 2 - teki_moto[i].img.width;
		teki_moto[i].img.y = -teki_moto[i].img.height;
		CollUpdatePlayer(&teki_moto[i]);	//当たり判定の更新
		teki_moto[i].img.IsDraw = FALSE;	//描画しない
	}

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
			path,					//本文
			"画像読み込みエラー",	//タイトル
			MB_OK					//ボタン
		);

		return FALSE;	//全て読み込めた
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
/// 画像をメモリに読み込み
/// </summary>
/// <param name="image">画像構造体のアドレス</param>
/// <param name="path">画像のパス</param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//ゴールの画像を読み込み
	strcpyDx(image->path, path);	//パスのコピー
	image->handle = LoadGraph(image->path);	//画像の読み込み

	//画像が読み込めなかったときは、エラー(-1)が入る
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			image->path,			//メッセージ本文
			"画像読み込みエラー！",		//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//画像の幅と高さを取得
	GetGraphSize(image->handle, &image->width, &image->height);

	//読み込めた
	return TRUE;
}

/// <summary>
/// ゲームデータを初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//ゲームスコアの初期化
	Score = 0;

	//プレイヤーの画像
	player.img.x = GAME_WIDTH / 2 - player.img.width;		//位置を決める
	player.img.y = GAME_HEIGTH / 2 - player.img.height;		//位置を決める
	CollUpdatePlayer(&player);	//当たり判定の更新
	player.img.IsDraw = TRUE;	//描画

	//背景の画像の設定①
	back[0].x = 0;					//位置を決める
	back[0].y = -back[0].height;	//位置を決める
	back[0].IsDraw = TRUE;	//描画

	//背景の画像の設定②
	back[1].x = 0;				//位置を決める
	back[1].y = 0;				//位置を決める
	back[1].IsDraw = TRUE;	//描画

	//敵の画像の初期化
	for (int i = 0; i < TEKI_KIND; i++)
	{
		teki_moto[i].img.x = GAME_WIDTH / 2 - teki_moto[i].img.width;
		teki_moto[i].img.y = -teki_moto[i].img.height;
		CollUpdatePlayer(&teki_moto[i]);	//当たり判定の更新
		teki_moto[i].img.IsDraw = FALSE;	//描画しない
	}

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
		//初期化
		GameInit();

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
	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// 弾の描画
/// </summary>
/// <param name="tama">弾の構造体</param>
VOID DrawTama(TAMA* tama)
{
	//弾の描画ができるとき
	if (tama->IsDraw == TRUE)
	{
		//弾の描画
		DrawGraph(tama->x, tama->y, tama->hundle[tama->NowIndex], TRUE);

		//画像を変えるタイミング
		if (tama->AnimeCnt < tama->AnimeCntMax)
		{
			tama->AnimeCnt++;
		}
		else
		{
			//弾の添え字が弾の分割数の最大値より下の時
			if (tama->NowIndex < TAMA_DIV_MAX - 1)
			{
				tama->NowIndex++;	//次の画像
			}
			else
			{
				tama->NowIndex = 0;	//最初に戻す
			}
			tama->AnimeCnt = 0;
		}
	}

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

	//プレイヤーの操作
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		if (player.img.x - player.speed >= 0)
		{
			player.img.x -= player.speed;
		}
	}

	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		if (player.img.x + player.img.width + player.speed <= GAME_WIDTH)
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
		if (player.img.y + player.img.height + player.speed <= GAME_WIDTH)
		{
			player.img.y += player.speed;
		}
	}

	//当たり判定の更新
	CollUpdatePlayer(&player);

	//スペースキーを押した時
	if (KeyDown(KEY_INPUT_SPACE) == TRUE)
	{
		double a = -45.0,b = -90.0,c = -135.0;

		if (tamaShoCnt == 0)
		{
			//弾を発射する（弾を描画する）
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], a);

					//弾を1発出したら、ループを抜ける
					break;
				}	
			}

			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], b);

					//弾を1発出したら、ループを抜ける
					break;
				}
			}

			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], c);

					//弾を1発出したら、ループを抜ける
					break;
				}
			}
		}

		//弾の発射待ち
		if (tamaShoCnt < tamaShoCntMax)
		{
			tamaShoCnt++;
		}
		else
		{
			tamaShoCnt = 0;
		}

	}

	//弾を飛ばす
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//描画されている時
		if (tama[i].IsDraw == TRUE)
		{
			//弾の位置を調整
			tama[i].x = tama[i].StartX + cos(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;
			tama[i].y = tama[i].StartY + sin(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;

			//半径を足す
			tama[i].radius += tama[i].Speed;

			//弾の当たり判定を更新
			CollUpdateTama(&tama[i]);

			//画面の出たら描写しない
			if (tama[i].y + tama[i].heigth < 0 ||	//画面外(上)
				tama[i].y > GAME_HEIGTH ||			//画面外(下)
				tama[i].x + tama[i].width < 0 ||	//画面外(左)
				tama[i].x > GAME_WIDTH)				//画面外(右)
			{
				tama[i].IsDraw = FALSE;
			}
		}
	}

	if (TekiAddCnt < tekiAddCntMax)
	{
		TekiAddCnt++;
	}
	else
	{
		TekiAddCnt = 0;

		//敵を生成
		for (int i = 0; i < TEKI_MAX; i++)
		{
			//描画されていない敵を入れる
			if (teki[i].img.IsDraw == FALSE)
			{
				int Bunkatu = 10;	//画面の横の分割数
				teki[i] = teki_moto[3];

				if (Score < 1000)
				{
					teki[i] = teki_moto[0];
				}
				else if(Score < 2000)
				{
					teki[i] = teki_moto[1];
				}
				else
				{
					teki[i] = teki_moto[GetRand(TEKI_KIND - 1)];
				}

				teki[i].img.x = GetRand(Bunkatu - 1) * GAME_WIDTH / Bunkatu;
				teki[i].img.y = -teki[i].img.height;

				teki[i].img.IsDraw = TRUE;	//描画
				break;
			}
		}
	}

	//敵の処理
	for (int i = 0; i < TEKI_MAX; i++)
	{
		//描画している敵
		if (teki[i].img.IsDraw == TRUE)
		{
			teki[i].img.y += 1;	//とりあえず下に

			//敵の当たり判定の更新
			CollUpdateTeki(&teki[i]);

			if (teki[i].img.y > GAME_HEIGTH)
			{
				teki[i].img.IsDraw = FALSE;
			}
			
			//敵と自分の弾が当たった時
			for (int cnt = 0; i < TAMA_MAX; i++)
			{
				//描画されている時
				if (tama[cnt].IsDraw == TRUE)
				{
					//当たり判定
					if (onCollRect(teki[i].coll, tama[cnt].coll) == TRUE)
					{
						tama[cnt].IsDraw = FALSE;	//弾の描画をしない
						teki[i].img.IsDraw = FALSE; //敵の描画もしない

						Score += 100;	//スコア加算
					}
				}
			}
		}
	}

	//とりあえずスコア加算
	Score++;

	return;
}

/// <summary>
/// 弾を飛ばす
/// </summary>
/// <param name="tama"></param>
VOID ShotTama(TAMA* tama,float deg)
{
		//弾の発射
		tama->IsDraw = TRUE;

		//弾が出る位置
		tama->StartX = player.img.x + player.img.width / 2 - tama->width / 2;
		tama->StartY = player.img.y;

		//弾の速度
		tama->Speed = 6;

		//弾の角度
		tama->degree = deg;

		//弾の半径
		tama->radius = 0.0f;

		//弾の当たり判定を更新
		CollUpdateTama(tama);

}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{

	//背景の描画
	for (int i = 0; i < 2; i++)
	{
		//描画
		DrawGraph(back[i].x, back[i].y, back[i].handle, TRUE);

		//画像が下までいった時
		if (back[i].y > GAME_HEIGTH)
		{
			back[i].y = -back[i].height + 1;	//高さ分、上に戻す
		}

		//画像を下に動かす
		back[i].y++;
	}

	//敵の描画
	for (int i = 0; i < TEKI_MAX; i++)
	{
		if (teki[i].img.IsDraw == TRUE)
		{
			DrawGraph(teki[i].img.x, teki[i].img.y, teki[i].img.handle, TRUE);
		}

		//当たり判定の描画
		if (GAME_DEBUG == TRUE)
		{
			DrawBox(teki[i].coll.left, teki[i].coll.top, teki[i].coll.right, teki[i].coll.bottom, GetColor(0, 0, 255), FALSE);
		}
	}
	
	//プレイヤーの描画
	if (player.img.IsDraw == TRUE)
	{
		//プレイヤーの描画
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		//当たり判定の描画
		if (GAME_DEBUG == TRUE)
		{
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//弾の描画
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//描画されている時
		if (tama[i].IsDraw == TRUE)
		{
			DrawTama(&tama[i]);

			//当たり判定の描画
			if (GAME_DEBUG == TRUE)
			{
				DrawBox(tama[i].coll.left, tama[i].coll.top, tama[i].coll.right, tama[i].coll.bottom, GetColor(255, 0, 0), FALSE);
			}
		}	
	}

	//スコアの描画
	int old = GetFontSize();	//以前のフォントのサイズを取得
	SetFontSize(40);			//フォントを大きくする
	DrawFormatString(0, 100, GetColor(255, 255, 255), "SCORE:%05d", Score);
	SetFontSize (old);			//フォントを元に戻す

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
	chara->coll.left = chara->img.x + 10;						//左側の当たり判定の調整
	chara->coll.top = chara->img.y + 10;						//上側の当たり判定の調整
	chara->coll.right = chara->img.x + chara->img.width - 10;	//右側の当たり判定の調整
	chara->coll.bottom = chara->img.y + chara->img.height -10;	//下側の当たり判定の調整

	return;
}

/// <summary>
/// 当たり判定の領域更新(敵)
/// </summary>
/// <param name="Coll">当たり判定の領域</param>
VOID CollUpdateTeki(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;						//左側の当たり判定
	chara->coll.top = chara->img.y;							//上側の当たり判定
	chara->coll.right = chara->img.x + chara->img.width;	//右側の当たり判定
	chara->coll.bottom = chara->img.y + chara->img.height;	//下側の当たり判定

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
/// 当たり判定の領域更新(弾)
/// </summary>
/// <param name="tama">弾の構造体</param>
VOID CollUpdateTama(TAMA* tama)
{
	tama->coll.left = tama->x;
	tama->coll.top = tama->y;

	tama->coll.right = tama->x + tama->width;
	tama->coll.bottom = tama->y + tama->heigth;

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