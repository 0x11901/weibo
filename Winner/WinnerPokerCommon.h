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
#ifndef WINNER_POKER_WINNERPOKERCOMMON_H
#define WINNER_POKER_WINNERPOKERCOMMON_H
#include <iostream>

#pragma mark - 命名空间
#define PAGAMES_WINNER_POKER_BEGIN namespace WinnerPoker {
#define PAGAMES_WINNER_POKER_END }
#define USING_PAGAMES_WINNER_POKER using namespace WinnerPoker

#pragma mark - 宏

#define PA_RETAIN(varType, varName, funName)  \
protected:                                    \
    varType *varName;                         \
                                              \
public:                                       \
    virtual varType *get##funName(void) const \
    {                                         \
        return varName;                       \
    }                                         \
                                              \
public:                                       \
    virtual void set##funName(varType *var)   \
    {                                         \
        if (varName != var)                   \
        {                                     \
            CC_SAFE_RETAIN(var);              \
            CC_SAFE_RELEASE(varName);         \
            varName = var;                    \
        }                                     \
    }

#define PA_CREATOR_ORIGIN(varType)                         \
protected:                                                 \
    virtual varType *create##varType(const ValueMap &data) \
    {                                                      \
        return varType::create(data);                      \
    }

#define PA_CREATOR_SUBCLASS(varType, newVarType)           \
protected:                                                 \
    virtual varType *create##varType(const ValueMap &data) \
    {                                                      \
        return newVarType::create(data);                   \
    }

#define PA_RETAIN_WRITE_ONLY(varType, varName, funName) \
protected:                                              \
    varType varName;                                    \
                                                        \
protected:                                              \
    virtual void set##funName(varType var)              \
    {                                                   \
        if (varName != var)                             \
        {                                               \
            CC_SAFE_RETAIN(var);                        \
            CC_SAFE_RELEASE(varName);                   \
            varName = var;                              \
        }                                               \
    }

PAGAMES_WINNER_POKER_BEGIN

#pragma mark - 枚举
/** 玩家逻辑方位 */
enum class UserDirection
{
    self,
    left,
    right,
    illegal
};

/** 玩家物理方位 */
enum class GeoDirection
{
    east,
    south,
    west,
    illegal
};

/** 牌型 */
enum class HandsCategory
{
    illegal,
    solo,
    pair,
    trio,
    trioWithSolo,
    trioWithPair,
    chain,
    pairChain,
    trioChain,
    trioChainWithSolo,
    trioChainWithPair,
    bomb,
    fourWithDualSolo,
    fourWithDualPair,
    anyLegalCategory
};

enum class SpecialCardsType
{
    pairChain,
    chain,
    airplane,
    bomb,
    trioWithSolo,
    trioWithPair,
    pass
};

enum class RoundType
{
    round,
    game
};

#pragma mark - 房间常量
// socket相关
extern const char *const WINNER_POKER_ROOM_SOCKET_NAME;
extern const char *const WINNER_POKER_ROOM_SOCKET_URL;

// 房间场景
extern const int TAG_WINNER_POKER_SCENE;

#pragma mark - 网络请求命令常量

// command response code ok
extern const char *const RESULT_OK;

// 复盘
extern const char *const CMD_MAIN_REPLAY;
// 房间复盘
extern const char *const CMD_MAIN_REPLAY_ROOM;
// 发牌
extern const char *const CMD_MAIN_SEND_CARDS;
// 加入房间
extern const char *const CMD_MAIN_TO_IN;
// 出牌
extern const char *const CMD_MAIN_HIT;
// 不要（要不起）
extern const char *const CMD_MAIN_PASS;
// 轮到谁
extern const char *const CMD_MAIN_TURNS;
// 创建房间
extern const char *const CMD_MAIN_CREATEROOM;

#pragma mark - 工具
template <typename E>
using enable_enum_t = typename std::enable_if<std::is_enum<E>::value, typename std::underlying_type<E>::type>::type;

template <typename E> constexpr inline enable_enum_t<E> underlying_value(E e) noexcept
{
    return static_cast<typename std::underlying_type<E>::type>(e);
}

template <typename E, typename T>
constexpr inline typename std::enable_if<std::is_enum<E>::value && std::is_integral<T>::value, E>::type
to_enum(T value) noexcept
{
    return static_cast<E>(value);
}

PAGAMES_WINNER_POKER_END
#endif // WINNER_POKER_WINNERPOKERCOMMON_H
