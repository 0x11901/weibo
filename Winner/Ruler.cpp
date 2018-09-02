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
#include "Ruler.h"
PAGAMES_WINNER_POKER_BEGIN

Ruler::Ruler()
{
    _isAlwaysWithPair     = true;
    _isBombDetachable     = false;
    _isAsTrioAceBomb      = true;
    _isThreeOfHeartsFirst = false;
}

Ruler &Ruler::getInstance()
{
    static Ruler _instance;
    return _instance;
}

bool Ruler::isAlwaysWithPair()
{
    return _isAlwaysWithPair;
}

bool Ruler::isBombDetachable()
{
    return _isBombDetachable;
}

bool Ruler::isAsTrioAceBomb()
{
    return _isAsTrioAceBomb;
}

bool Ruler::isThreeOfHeartsFirst()
{
    return _isThreeOfHeartsFirst;
}

PAGAMES_WINNER_POKER_END
