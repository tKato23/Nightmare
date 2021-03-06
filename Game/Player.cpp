//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Player.cpp
//!
//! @brief  プレイヤー処理のソースファイル
//!
//! @date   2017/04/09
//!
//! @author 佐久間尚輝
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Player.h"

//定数
const float Player::PLAYER_HALF_GRAVITY = 0.5;

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
Player::Player(int pos_x, int pos_y)
{
	m_handle = new Texture(L"Resources\\Images\\runa.png");	//画像
	m_pos_x = pos_x;	//x座標
	m_pos_y = pos_y;	//y座標
	m_grp_x = 0;		//元画像のgrp_x
	m_grp_y = 0;		//元画像のgrp_y
	m_grp_w = PLAYER_GRP;		//元画像のgrp_w
	m_grp_h = PLAYER_GRP;		//元画像のgrp_h
	m_state = 0;
	m_right_flag = true;
	m_left_flag = false;
}

//----------------------------------------------------------------------
//! @brief デストラクタ
//----------------------------------------------------------------------
Player::~Player()
{
	delete m_handle;
	m_handle = nullptr;
}

//----------------------------------------------------------------------
//! @brief 移動
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::Move()
{
	//オートで進まないように
	m_spd_x = 0;
	//右移動
	if (g_key.Right)
	{
		m_spd_x = PLAYER_MOVE_POW;
		m_right_flag = true;
		m_left_flag = false;
	}
	//左移動
	if (g_key.Left)
	{
		m_spd_x = -PLAYER_MOVE_POW;
		m_right_flag = false;
		m_left_flag = true;
	}
	//ジャンプ
	if (g_keyTracker->pressed.Up && !m_jump_flag && m_grp_h == PLAYER_GRP)
	{
		m_spd_y = PLAYER_JUMP_POW;
		m_jump_flag = true;
	}
	//重力

	m_spd_y += PLAYER_GRAVITY;
	//エリア外に行かないようにする
	if (m_pos_x <= 0)
	{
		m_pos_x = 0;
	}
	
	if (m_right_flag == true && m_grp_h == 32 && m_grp_w == 32)
	{
		m_grp_x = 0;
		m_grp_y = 0;
	}
	if (m_left_flag == true && m_grp_h == 32 && m_grp_w == 32)
	{
		m_grp_x = 80;
		m_grp_y = 0;
	}
	if (m_right_flag == true && m_grp_h == 16 && m_grp_w == 32)
	{
		m_grp_x = 48;
		m_grp_y = 16;
	}
	if (m_left_flag == true && m_grp_h == 16 && m_grp_w == 32)
	{
		m_grp_x = 48;
		m_grp_y = 16;
	}
	if (m_right_flag == true && m_grp_h == 32 && m_grp_w == 16)
	{
		m_grp_x = 32;
		m_grp_y = 0;
	}
	if (m_left_flag == true && m_grp_h == 32 && m_grp_w == 16)
	{
		m_grp_x = 80 + 32;
		m_grp_y = 0;
	}

}

//----------------------------------------------------------------------
//! @brief 地面についている
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::Ground()
{
	m_jump_flag = false;
}

//----------------------------------------------------------------------
//! @brief プレイヤーの高さを変える
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::ChangePlayerH()
{

	if (m_grp_h == PLAYER_GRP)
	{
		m_grp_h = PLAYER_GRP / 2;
		m_grp_w = PLAYER_GRP;
	}

}

//----------------------------------------------------------------------
//! @brief プレイヤーの幅を変える
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::ChangePlayerW()
{
	if (m_grp_w == PLAYER_GRP)
	{
		m_grp_w = PLAYER_GRP / 2;
		m_grp_h = PLAYER_GRP;
	}
}

//----------------------------------------------------------------------
//! @brief プレイヤーがジャンプしているかどうかを返す
//!
//! @param[in] なし
//!
//! @return true:ジャンプしている　false:ジャンプしていない
//----------------------------------------------------------------------
bool Player::GetJump()
{
	return m_jump_flag;
}

