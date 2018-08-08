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
#ifndef PAGAMES_WINNER_RULER_H
#define PAGAMES_WINNER_RULER_H
#include "WinnerPokerCommon.h"
PAGAMES_WINNER_POKER_BEGIN
class Ruler
{
public:
    Ruler();
    Ruler(const Ruler &) = delete;
    Ruler &operator=(const Ruler &) = delete;

    static Ruler &getInstance();

    /**
     * 是否总是带两张牌
     * 如果用户选择三带二（强制）那么不能打出三带一和三不带，三顺也只能带2n张牌
     * @return 是否总是带两张牌
     */
    bool isAlwaysWithPair();

    /**
     * 炸弹是否可以拆开，如不能在用户点击炸弹牌时，四张要同时提起
     * @return
     */
    bool isBombDetachable();

    /**
     * 是否把三张A作为炸弹
     * @return
     */
    bool isAsTrioAceBomb();

    /**
     * 是不是♥️3必出。
     * 仅当玩家选择了♥️3先出并且♥️3必出时，返回true
     * @return
     */
    bool isThreeOfHeartsFirst();

private:
    bool _isAlwaysWithPair;
    bool _isBombDetachable;
    bool _isAsTrioAceBomb;
    bool _isThreeOfHeartsFirst;
};
PAGAMES_WINNER_POKER_END

#endif // PAGAMES_WINNER_RULER_H
