/**
 * Copyright (C) 2018 Shanghai YOUWAN Science & Technology Co., Ltd.
 * All rights reserved.
 *
 * This document is the property of Shanghai YOUWAN Science & Technology Co., Ltd.
 * It is considered confidential and proprietary.
 *
 * This document may not be reproduced or transmitted in any form,
 * in whole or in part, without the express written permission of
 * Shanghai YOUWAN Science & Technology Co., Ltd.
 */
#include "WinnerPokerCommon.h"

PAGAMES_WINNER_POKER_BEGIN

#pragma mark - 房间常量
// socket相关
const char *const WINNER_POKER_ROOM_SOCKET_NAME = "WINNER_POKER_ROOM_SOCKET_NAME";
const char *const WINNER_POKER_ROOM_SOCKET_URL  = "http://tst-runfast-stg90.1768.com";

//房间场景
const int TAG_WINNER_POKER_SCENE = 233333;

#pragma mark - 网络请求命令常量
// command response code ok
const char *const RESULT_OK = "200";

//复盘
const char *const CMD_MAIN_REPLAY = "replay";

//发牌
const char *const CMD_MAIN_SEND_CARDS = "dealMj";

//加入房间
const char *const CMD_MAIN_TO_IN = "toIn";

// FIXME: 测试用
//创建房间
const char *const CMD_MAIN_CREATEROOM = "createRoom";
PAGAMES_WINNER_POKER_END
