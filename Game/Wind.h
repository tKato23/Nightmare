//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Wind.cpp
//!
//! @brief  スイッチのヘッダファイル
//!
//! @date   2017/04/10
//!
//! @author 渡邊真人
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

//	多重インクルードの防止
#pragma once

//	ヘッダファイルの読み込み
#include "ObjectBase.h"

//クラス
class Wind : public ObjectBase
{
private:
	static const int M_WIND_GRP = 32;
	static const int M_WIND_GRP2 = 64;
	static const int M_WIND_POS = 32 * 11;
	
public:
	Wind();
	~Wind();
};