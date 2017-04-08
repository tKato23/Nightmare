//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameTitle.cpp
//!
//! @brief  タイトル処理のソースファイル
//!
//! @date   2017/04/07		
//!
//! @author 加藤 竜哉
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================
#include "GameMain.h"
#include "GameTitle.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
Title::Title()
{
	//	タイトル画像の初期化
	m_title_image = new Texture(L"Resources\\Images\\title.png");
	m_title_image2 = new Texture(L"Resources\\Images\\title2.png");

	//	フェード用変数の初期化
	m_fade_flag = false;
	m_alfa = 1.0f;

	//	カウントの初期化
	m_count = 0;
}

//----------------------------------------------------------------------
//! @brief デストラクタ
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
Title::~Title()
{

}

//----------------------------------------------------------------------
//! @brief タイトルの更新処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Title::Update()
{
	//シーンの最初に一回だけ行う初期化処理
	if (g_init == 0)
	{
		g_init = 1;
	}

	//	Zキーを押したらプレイシーンへ
	if (g_keyTracker->pressed.Z)
	{
		ADX2Le::Play(CRI_CUESHEET_1_SE);
		m_fade_flag = true;
	}

	//	フェードフラグがtrueなら透明度を上げる
	if (m_fade_flag == true)
	{
		m_alfa -= 0.01f;
	}
	else
	{
		//	カウントを進める
		m_count++;
	}

	//	カウントが40を超えたら0に戻す
	if (m_count > 40)
	{
		m_count = 0;
	}

	//	透明ならシーンを切り替える
	if (m_alfa <= 0.0f)
	{
		g_NextScene = PLAY;
	}

}

//----------------------------------------------------------------------
//! @brief タイトルの描画処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Title::Render()
{
	if (m_count <= 20)
	{
		//	画像の描画
		g_spriteBatch->Draw(m_title_image->m_pTexture, Vector2(0, 0), Color(1.0f, 1.0f, 1.0f, m_alfa));
	}
	else
	{
		//	画像の描画
		g_spriteBatch->Draw(m_title_image2->m_pTexture, Vector2(0, 0), Color(1.0f, 1.0f, 1.0f, m_alfa));
	}
}
