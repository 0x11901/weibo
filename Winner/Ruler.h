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
     * 通过设置 RoomConfigModel 更新Ruler，Ruler不持有 RoomConfigModel，也不管理传入 RoomConfigModel 的生命周期
     * @param model
     */

    /**
     * 是否总是带两张牌
     * 如果用户选择三带二（强制）那么不能打出三带一和三不带，三顺也只能带2n张牌
     * @bug 没去看产品需求文档导致理解错误，认为当选择这个选项时，带牌总是得带两张，所以无法出四带一，但是实际上需求是四
     * 带一 不受此影响。在加入三带二必须带相同牌这个玩法之后，出现了问题。但是代码已经积重难返了，为了减少测试回归量，故
     * 在三 带二必须带相同牌的相关处理中特殊处理
     * @return 是否总是带两张牌
     */
    bool isAlwaysWithPair();

    /**
     * 当发生三带二时，带牌是否总是同一牌型
     * @return
     */
    bool isKickerAlwaysSameRank();

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
    bool _isKickerAlwaysSameRank;
    bool _isBombDetachable;
    bool _isAsTrioAceBomb;
    bool _isThreeOfHeartsFirst;
};
PAGAMES_WINNER_POKER_END

#endif // PAGAMES_WINNER_RULER_H
