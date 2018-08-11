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
const char *const WINNER_POKER_ROOM_SOCKET_URL  = "http://tst-runfast-stg41.1768.com";

//房间场景
const int TAG_WINNER_POKER_SCENE = 233333;

#pragma mark - 网络请求命令常量
// command response code ok
const char *const RESULT_OK = "200";

// 复盘
const char *const CMD_MAIN_REPLAY = "replay";
// 房间复盘
const char *const CMD_MAIN_REPLAY_ROOM = "roomReplay";
// 发牌
const char *const CMD_MAIN_SEND_CARDS = "dealMj";
// 加入房间
const char *const CMD_MAIN_TO_IN = "toIn";
// 出牌
const char *const CMD_MAIN_HIT = "hit";
// 不要（要不起）
const char *const CMD_MAIN_PASS = "pass";
// 轮到谁
const char *const CMD_MAIN_TURNS = "turns";
// 创建房间
const char *const CMD_MAIN_CREATEROOM = "createRoom";
PAGAMES_WINNER_POKER_END
