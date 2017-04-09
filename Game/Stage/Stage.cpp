//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Stage.cpp
//!
//! @brief  ステージ処理のソースファイル
//!
//! @date   2017/04/07
//!
//! @author 佐久間尚輝
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Stage.h"
#include <fstream>
#include <sstream>
#include <SimpleMath.h>

using namespace std;
using namespace DirectX::SimpleMath;
using namespace DirectX;

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
Stage::Stage()
	:m_press_count(0)	//プレス機のカウント
	,m_burner_count(0)	//バーナーのカウント
	,m_side_press_count(0)//横のプレス機のカウント
{
	m_map_image = new Texture(L"Resources\\Images\\MapChip.png");		//マップの画像
	ImportData("Resources\\map\\map.csv");	//マップの読み込み
	//オブジェクト用
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			switch (m_map[i][j])
			{
				//プレス機の生成
			case PRESS:
				m_press[m_press_count] = new Press(j * CHIPSIZE, i* CHIPSIZE);
				m_press_count++;
				break;
				//横のプレス機の生成
			case SIDEPRESS:
				m_side_press[m_side_press_count] = new SidePress(j * CHIPSIZE, i* CHIPSIZE);
				m_side_press[m_side_press_count]->SetState(m_side_press_count);
				m_side_press_count++;
				break;
				//バーナーの生成
			case BURNER:
				m_burner[m_burner_count] = new Burner(j * CHIPSIZE, i * CHIPSIZE);
				m_burner_count++;
				break;
				//プレイヤーの生成
			case PLAYER:
				m_player = new Player(j * CHIPSIZE, i * CHIPSIZE);
				break;
			default:
				break;
			}
		}
	}
	//カメラ
	m_camera = new Camera();
}

//----------------------------------------------------------------------
//! @brief デストラクタ
//----------------------------------------------------------------------
Stage::~Stage()
{
	delete m_map_image;		//画像
	m_map_image = nullptr;
	for (int i = 0; i < m_press_count; i++)
	{
		delete m_press[i];	//プレス機
		m_press[i] = nullptr;
	}
	for (int i = 0; i < m_side_press_count; i++)
	{
		delete m_side_press[i];	//横のプレス機
		m_side_press[i] = nullptr;
	}
	for (int i = 0; i < m_burner_count; i++)
	{
		delete m_burner[i];	//バーナー
		m_burner[i] = nullptr;
	}
	delete m_player;	//プレイヤー
	m_player = nullptr;
	delete m_camera;	//カメラ
	m_camera = nullptr;
}

//----------------------------------------------------------------------
//! @brief ステージの描画
//!
//! @param[in] ファイルの名前
//!
//! @return なし
//----------------------------------------------------------------------
void Stage::DrawStage()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			//壁の描画
			switch (m_map[i][j])
			{
			case BACKGROUND :
				DrawSprite(0, 0, CHIPSIZE, CHIPSIZE, i, j);
				break;
				//壁
			case WALL:
				DrawSprite(CHIPSIZE, 0 , CHIPSIZE * WALL, CHIPSIZE, i, j);
				break;
				//横長の壁
			case HORIZONTALWALL:
				DrawSprite(CHIPSIZE * WALL, 0 ,
					CHIPSIZE * HORIZONTALWALL, CHIPSIZE, i, j);
				break;
				//縦長の壁
			case VERTICALWALL:
				DrawSprite(CHIPSIZE * HORIZONTALWALL, 0,
					CHIPSIZE * VERTICALWALL , CHIPSIZE , i, j);
				break;
				//水
			case WATER:
				break;
				//プレス機
			case PRESS:
				DrawSprite(0, 0, CHIPSIZE, CHIPSIZE, i, j);
				break;
				//横のプレス機
			case SIDEPRESS:
				DrawSprite(0, 0, CHIPSIZE, CHIPSIZE, i, j);
				break;
				//バーナー
			case BURNER:
				DrawSprite(0, 0, CHIPSIZE, CHIPSIZE, i, j);
				break;
				//プレイヤー
			case PLAYER:
				DrawSprite(0, 0, CHIPSIZE, CHIPSIZE, i, j);
				break;
			default:
				break;
			}

		}
	}

}

//----------------------------------------------------------------------
//! @brief アップデート
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Stage::Update()
{
	//プレス機
	for (int i = 0; i < m_press_count; i++)
	{
		m_press[i]->Update();	//座標変更
		m_press[i]->Move();		//移動
	}
	//バーナー
	for (int i = 0; i < m_burner_count; i++)
	{
		m_burner[i]->Ignition();	//火をつける
	}
	//横のプレス機
	for (int i = 0; i < m_side_press_count; i++)
	{
		m_side_press[i]->Update();	//座標変更
		m_side_press[i]->Move();	//移動
	}
	//カメラ
	m_camera->Coordinate(m_player->GetPosX(),  m_player->GetGrpW());	//座標変更
	m_camera->CameraMaxPos();	//スクロールの限界
	//プレイヤー
	m_player->Move();	//移動
	m_player->Update(); //座標変更
	MapDownDecison();	//マップチップとの下の当たり判定
	MapSideDecison();	//マップチップとの横の当たり判定
	MapJumpDecison();	//マップチップとの上の当たり判定
}

//----------------------------------------------------------------------
//! @brief オブジェクトの描画
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Stage::ObjectDraw()
{
	//プレス機
	for (int i = 0; i < m_press_count; i++)
		m_press[i]->Render(m_camera->GetPosX());
	//横のプレス機
	for (int i = 0; i < m_side_press_count; i++)
		m_side_press[i]->Render(m_camera->GetPosX());

	//バーナー
	for (int i = 0; i < m_burner_count; i++)
	{
		//バーナーが生きている時
		if (m_burner[i]->GetState() == true)
			m_burner[i]->Render(m_camera->GetPosX());
	}
	//プレイヤー
	m_player->Render(m_camera->GetPosX());

}


//----------------------------------------------------------------------
//! @brief csvファイルの読み込み
//!
//! @param[in] ファイルの名前
//!
//! @return なし
//----------------------------------------------------------------------
void Stage::ImportData(string filename)
{
	ifstream ifs(filename);
	string str;
	int i;
	if (!ifs)
	{
		for (i = 0; i < MAX_TIP; i++)
		{
			m_map[i / MAP_WIDTH][i % MAP_WIDTH] = 0;
		}
	}

	i = 0;
	while (getline(ifs, str))
	{
		string token;
		istringstream stream(str);

		//一行のうち、文字列とコンマを分割する
		while (getline(stream, token, ','))
		{
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			m_map[i / MAP_WIDTH][i % MAP_WIDTH] = atoi(token.c_str());
			i++;
		}
	}

}

//----------------------------------------------------------------------
//! @brief スプライトの描画
//!
//! @param[in] 画像のAABB、高さ、幅
//!
//! @return なし
//----------------------------------------------------------------------
void Stage::DrawSprite(int grp_x, int grp_y, int grp_w, int grp_h, int i,int j)
{
	RECT rect = { grp_x,grp_y,grp_w,grp_h };
	//スクロールに対応
	g_spriteBatch->Draw(m_map_image->m_pTexture, Vector2(CHIPSIZE * j - (m_camera->GetPosX() - SCREEN_WIDTH / 2), CHIPSIZE * i),
		&rect, Colors::White, 0.0f, Vector2(0, 0), Vector2(1, 1));

}

//----------------------------------------------------------------------
//! @brief マップチップとの下の当たり判定
//!
//! @param[in] 
//!
//! @return なし
//----------------------------------------------------------------------

void Stage::MapDownDecison()
{
	int map_x;
	int map_y;
	//左下
	map_x = floor((m_player->GetPosX() + SHIFTED_POS) / CHIPSIZE);
	map_y = floor((m_player->GetPosY() + m_player->GetGrpH()) / CHIPSIZE);
	if ((map_x >= 0 && map_x < MAP_WIDTH) && (map_y >= 0 && map_y < MAP_HEIGHT))
	{
		//マップチップが壁の時
		if (m_map[map_y][map_x] == WALL)
		{
			if (m_player->GetPosY() + m_player->GetGrpH() > map_y * CHIPSIZE)
			{
				m_player->SetPosY((map_y - 1)*CHIPSIZE);
				m_player->SetSpdY(0);
				m_player->Ground();
			}
		}

	}
	//右下
	map_x = floor((m_player->GetPosX() + m_player->GetGrpW() - SHIFTED_POS) / CHIPSIZE);
	map_y = floor((m_player->GetPosY() + m_player->GetGrpH()) / CHIPSIZE);
	if ((map_x >= 0 && map_x < MAP_WIDTH) && (map_y >= 0 && map_y < MAP_HEIGHT))
	{
		//マップチップが壁の時
		if (m_map[map_y][map_x] == WALL)
		{
			if (m_player->GetPosY() + m_player->GetGrpH() > map_y * CHIPSIZE)
			{
				m_player->SetPosY((map_y - 1)*CHIPSIZE);
				m_player->SetSpdY(0);
				m_player->Ground();
			}

		}
	}
}
//----------------------------------------------------------------------
//! @brief マップチップとの横の当たり判定
//!
//! @param[in] m_player
//!
//! @return なし
//----------------------------------------------------------------------

void Stage::MapSideDecison()
{
	int map_x;
	int map_y;
	//左中心
	map_x = (int)floor((m_player->GetPosX() + m_player->GetGrpW()) / CHIPSIZE);
	map_y = (int)floor((m_player->GetPosY() + m_player->GetGrpH() / 2) / CHIPSIZE);
	if ((map_x >= 0 && map_x < MAP_WIDTH) && (map_y >= 0 && map_y < MAP_HEIGHT))
	{
		//マップチップが壁の時
		if (m_map[map_y][map_x] == WALL)
		{
			if (m_player->GetPosX() + m_player->GetGrpW() >= map_x* CHIPSIZE)
			{
				m_player->SetPosX((map_x - 1)* CHIPSIZE);
				m_player->SetSpdX(0);
			}
		}
	}
	//右中心
	map_x = (int)floor((m_player->GetPosX()) / CHIPSIZE);
	map_y = (int)floor((m_player->GetPosY() + m_player->GetGrpH() / 2) / CHIPSIZE);
	if ((map_x >= 0 && map_x < MAP_WIDTH) && (map_y >= 0 && map_y < MAP_HEIGHT))
	{
		//マップチップが壁の時
		if (m_map[map_y][map_x] == WALL)
		{
			if (m_player->GetPosX() <= map_x* CHIPSIZE + CHIPSIZE)
			{
				m_player->SetPosX((map_x + 1)* CHIPSIZE);
				m_player->SetSpdX(0);
			}
		}
	}
}
//----------------------------------------------------------------------
//! @brief マップチップとの上の当たり判定
//!
//! @param[in] m_player
//!
//! @return なし
//----------------------------------------------------------------------

void Stage::MapJumpDecison()
{
	int map_x;
	int map_y;
	//左上
	map_x = ((int)floor((m_player->GetPosX() + SHIFTED_POS / 2) / CHIPSIZE));
	map_y = ((int)floor(m_player->GetPosY() / CHIPSIZE));
	//配列内かチェック
	if ((map_x >= 0 && map_x < MAP_WIDTH && map_y >= 0 && map_y < MAP_HEIGHT))
	{
		//マップチップが壁の時
		if (m_map[map_y][map_x] == WALL)
		{
			//判定
			if ((m_player->GetPosY()) >(map_y * CHIPSIZE))
			{
				m_player->SetPosY((map_y + 1) * CHIPSIZE);
				m_player->SetSpdY(0);
			}
		}

	}
	//右上
	map_x = ((int)floor((m_player->GetPosX() + m_player->GetGrpW() - SHIFTED_POS / 2) / CHIPSIZE));
	map_y = ((int)floor(m_player->GetPosY() / CHIPSIZE));
	//配列内かチェック
	if ((map_x >= 0 && map_x < MAP_WIDTH && map_y >= 0 && map_y < MAP_HEIGHT))
	{
		//マップチップが壁の時
		if (m_map[map_y][map_x] == WALL)
		{
			//判定
			if ((m_player->GetPosY()) >(map_y * CHIPSIZE))
			{
				m_player->SetPosY((map_y + 1)* CHIPSIZE);
				m_player->SetSpdY(0);
			}
		}
	}
}