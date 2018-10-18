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
#include "Judge.h"
#include "Ruler.h"
#include <algorithm>
#include <numeric>
#include <unordered_set>

PAGAMES_WINNER_POKER_BEGIN
#pragma mark - 常量
std::string duiZi     = "AA";
std::string sanBuDai  = "AAA";
std::string sanDaiYi  = "AAAB";
std::string sanDaiEr1 = "AAABB";
std::string sanDaiEr2 = "AAABC";
std::string zhaDan    = "AAAA";
std::string siDaiYi   = "AAAAB";
std::string siDaiEr1  = "AAAABB";
std::string siDaiEr2  = "AAAABC";

constexpr size_t paiXing3 = 3;
constexpr size_t paiXingA = 14;
constexpr size_t paiXing2 = 15;

constexpr size_t hongTao3 = 771;

#pragma mark - 模版
template <class BidirIt> bool next_combination(BidirIt first1, BidirIt last1, BidirIt first2, BidirIt last2)
{
    if ((first1 == last1) || (first2 == last2)) return false;
    BidirIt m1 = last1;
    BidirIt m2 = last2;
    --m2;
    while (--m1 != first1 && !(*m1 < *m2))
        ;
    bool result = (m1 == first1) && !(*first1 < *m2);
    if (!result)
    {
        while (first2 != m2 && !(*m1 < *first2))
            ++first2;
        first1 = m1;
        std::iter_swap(first1, first2);
        ++first1;
        ++first2;
    }
    if ((first1 != last1) && (first2 != last2))
    {
        m1 = last1;
        m2 = first2;
        while ((m1 != first1) && (m2 != last2))
        {
            std::iter_swap(--m1, m2);
            ++m2;
        }
        std::reverse(first1, m1);
        std::reverse(first1, last1);
        std::reverse(m2, last2);
        std::reverse(first2, last2);
    }
    return !result;
}

template <class BidirIt> bool next_combination(BidirIt first, BidirIt middle, BidirIt last)
{
    return next_combination(first, middle, middle, last);
}

#pragma mark - 函子
struct Functor
{
    template <typename T> bool operator()(const T &$0) const
    {
        T v;
        std::unique_copy($0.begin(), $0.end(), std::back_inserter(v));
        for (const auto &i : v)
        {
            if (std::count($0.begin(), $0.end(), i) % 2 != 0) return true;
        }
        return false;
    }

    template <typename T, typename U> T operator()(T $0, const U &$1) const
    {
        return $0 + $1.second;
    }

    template <typename T> bool operator()(const T &$0, const T &$1) const
    {
        return $0.first < $1.first;
    }
};

#pragma mark - 单例
Judge &Judge::getInstance()
{
    static Judge _instance;
    return _instance;
}

#pragma mark - 判定
HandsCategoryModel Judge::judgeHandsCategory(const std::vector<size_t> &hands) const
{
    HandsCategoryModel model{};
    if (hands.empty())
    {
        model.handsCategory = HandsCategory::illegal;
        return model;
    }

    auto        size   = hands.size();
    const auto &vector = getCardRanks(hands);

    if (size == 1)
    {
        model.handsCategory = HandsCategory::solo;
        model.size          = 1;
        model.weight        = vector.front();
        return model;
    }

    std::unordered_map<size_t, size_t> ranks = zip(vector);

    if (size == 2 && isSame(ranks, duiZi))
    {
        model.handsCategory = HandsCategory::pair;
        model.size          = 2;
        model.weight        = vector.front();
        return model;
    }

    // 下面的判断必须要考虑AAA当💣的特殊情况，所以先筛选出AAA当💣的特殊情况
    if (Ruler::getInstance().isAsTrioAceBomb())
    {
        // OPTIMIZE: 下面某处会修改ranks，先拷贝绕过
        auto copy = ranks;
        if (copy[paiXingA] == 3)
        {
            if (copy.size() == 1)
            {
                model.handsCategory = HandsCategory::bomb;
                model.size          = 3;
                model.weight        = paiXingA;
                return model;
            }

            //当💣不可拆
            if (!Ruler::getInstance().isBombDetachable())
            {
                model.handsCategory = HandsCategory::illegal;
                return model;
            }
        }
    }

    if (size == 3 && isSame(ranks, sanBuDai))
    {
        model.handsCategory = HandsCategory::trio;
        model.size          = 3;
        model.weight        = vector.front();
        return model;
    }

    if (size == 4 && isSame(ranks, sanDaiYi))
    {
        model.handsCategory = HandsCategory::trioWithSolo;
        model.size          = 4;
        size_t weight       = 0;
        for (const auto &rank : ranks)
        {
            if (rank.second == 3)
            {
                weight = rank.first;
                break;
            }
        }
        model.weight = weight;
        return model;
    }

    if (size == 5 && (isSame(ranks, sanDaiEr1) || isSame(ranks, sanDaiEr2)))
    {
        model.handsCategory = HandsCategory::trioWithPair;
        model.size          = 5;
        size_t weight       = 0;
        for (const auto &rank : ranks)
        {
            if (rank.second == 3)
            {
                weight = rank.first;
                break;
            }
        }
        model.weight = weight;
        return model;
    }

    if (size == 4 && isSame(ranks, zhaDan))
    {
        model.handsCategory = HandsCategory::bomb;
        model.size          = 4;
        model.weight        = vector.front();
        return model;
    }

    if (size == 5 && isSame(ranks, siDaiYi))
    {
        model.handsCategory = HandsCategory::fourWithDualSolo;
        model.size          = 5;
        size_t weight       = 0;
        for (const auto &rank : ranks)
        {
            if (rank.second == 4)
            {
                weight = rank.first;
                break;
            }
        }
        model.weight = weight;
        return model;
    }

    if (size == 6 && (isSame(ranks, siDaiEr1) || isSame(ranks, siDaiEr2)))
    {
        model.handsCategory = HandsCategory::fourWithDualPair;
        model.size          = 6;
        size_t weight       = 0;
        for (const auto &rank : ranks)
        {
            if (rank.second == 4)
            {
                weight = rank.first;
                break;
            }
        }
        model.weight = weight;
        return model;
    }

    // 判断顺子
    if (isChain(ranks))
    {
        model.handsCategory = HandsCategory::chain;
        model.weight        = (*std::min_element(ranks.begin(), ranks.end(), Functor())).first;
        model.size          = ranks.size();
        return model;
    }

    // 判断连对
    if (size % 2 == 0 && isPairChain(ranks))
    {
        model.handsCategory = HandsCategory::pairChain;
        model.weight        = (*std::min_element(ranks.begin(), ranks.end(), Functor())).first;
        model.size          = ranks.size();
        return model;
    }

    // 判断三顺
    auto tuple = isTrioChain(ranks);
    bool isTrioChain;
    std::tie(isTrioChain, std::ignore) = tuple;
    if (isTrioChain)
    {
        HandsCategoryModel tempModel{};
        std::tie(std::ignore, tempModel) = tuple;
        model.handsCategory              = tempModel.handsCategory;
        model.weight                     = tempModel.weight;
        model.size                       = tempModel.size;

        // OPTIMIZE: 三顺带一和三顺带二权重有可能会炸裂，总之重算一下
        if (tempModel.handsCategory != HandsCategory::trioChain)
            model.weight = getTrioChainWeight(hands, tempModel.handsCategory);

        return model;
    }

    model.handsCategory = HandsCategory::illegal;
    return model;
}

bool Judge::isPass(const std::vector<size_t> &hands)
{
    return _currentHandsCategory.handsCategory.handsCategory == HandsCategory::anyLegalCategory
               ? false
               : cardHint(hands).empty();
}

bool Judge::canPlay(const std::vector<size_t> &hands, bool isStartingHand) const
{
    // FIXME: 首出♥️3已经由外部处理，其实下面一行已经可以无需判断
    if (isStartingHand && Ruler::getInstance().isThreeOfHeartsFirst() && !isContainsThreeOfHearts(hands)) return false;

    const auto &handsCategoryModel = Judge::getInstance().judgeHandsCategory(hands);
    const auto &handsCategory      = handsCategoryModel.handsCategory;
    // 当💣不可拆时，判断传来的牌中有无💣，如有则无法出牌
    if (!Ruler::getInstance().isBombDetachable() && handsCategory != HandsCategory::bomb)
        if (isContainsBombs(hands)) return false;

    if (_currentHandsCategory.handsCategory.handsCategory == HandsCategory::anyLegalCategory) // 首出
    {
        return canPlay(hands, handsCategoryModel);
    }
    else // 跟出
    {
        return canBeat(hands);
    }
}

bool Judge::isContainsThreeOfHearts(const std::vector<size_t> &hands) const
{
    return std::find(hands.begin(), hands.end(), hongTao3) != hands.end();
}

#pragma mark - 提示
std::vector<size_t> Judge::intentions(const std::vector<size_t> &hands, bool isStartingHand)
{
    if (_currentHandsCategory.handsCategory.handsCategory == HandsCategory::anyLegalCategory)
        return intention(hands, isStartingHand);
    else
        return hint(hands);
}

void Judge::shouldHintTheHighestSingleCard(const std::vector<size_t> &hands)
{
    if (hands.empty()) return;
    if (_currentHandsCategory.handsCategory.handsCategory == HandsCategory::anyLegalCategory)
    {
        cardIntentions(hands, false);
        _iteratorIntentions = _cardIntentions.begin();

        setTheHighestSingleCard(hands, _cardIntentions, _iteratorIntentions);
    }
    else
    {
        setTheHighestSingleCard(hands, _cardHint, _iteratorHint);
    }
}

#pragma mark - 排序 & 重置索引
std::vector<size_t> Judge::rearrangeHands(const std::vector<size_t> &hands) const
{
    if (hands.size() < 2) return hands;

    std::vector<size_t> ret;
    auto                handsCategory = _currentHandsCategory.handsCategory.handsCategory;

    if (handsCategory == HandsCategory::anyLegalCategory)
    {
        auto h        = judgeHandsCategory(hands);
        handsCategory = h.handsCategory;
    }
    if (handsCategory == HandsCategory::illegal) // OPTIMIZE: 程序闪退
        return hands;

    auto copy = hands;
    std::sort(copy.begin(), copy.end());

    auto ranksMultimap = getRanksMultimap(copy, false);
    auto values        = getCardRanks(copy);
    auto ranks         = zip(values);

    if (ranks.size() == 1) return copy;

    if (handsCategory == HandsCategory::trioWithSolo || handsCategory == HandsCategory::trioWithPair)
    {
        std::vector<size_t> temp;
        auto                ranksCopy = ranks;

        for (const auto &rank : ranks)
        {
            if (rank.second > 2)
            {
                for (int i = 0; i < 3; ++i)
                {
                    temp.push_back(rank.first);
                }

                if (rank.second == 3)
                {
                    ranksCopy.erase(rank.first);
                }
                else
                {
                    ranksCopy[rank.first] = 1;
                }
            }
        }

        auto unzipped = unzip(ranksCopy);

        std::sort(temp.begin(), temp.end());
        std::sort(unzipped.begin(), unzipped.end());

        temp.insert(temp.end(), unzipped.begin(), unzipped.end());

        ret = temp;
    }
    else if (handsCategory == HandsCategory::fourWithDualSolo || handsCategory == HandsCategory::fourWithDualPair)
    {
        std::vector<size_t> temp;
        auto                ranksCopy = ranks;

        for (const auto &rank : ranks)
        {
            if (rank.second > 3)
            {
                for (int i = 0; i < 4; ++i)
                {
                    temp.push_back(rank.first);
                }

                ranksCopy.erase(rank.first);
            }
        }

        auto unzipped = unzip(ranksCopy);

        std::sort(temp.begin(), temp.end());
        std::sort(unzipped.begin(), unzipped.end());

        temp.insert(temp.end(), unzipped.begin(), unzipped.end());

        ret = temp;
    }
    // XXX: 当强制三带二时，打出的三顺不带实际上认为是三顺带二
    else if (handsCategory == HandsCategory::trioChainWithSolo || handsCategory == HandsCategory::trioChainWithPair
             || (Ruler::getInstance().isAlwaysWithPair() && handsCategory == HandsCategory::trioChain))
    {
        std::vector<size_t> temp;
        auto                ranksCopy = ranks;

        for (const auto &rank : ranksCopy)
        {
            if (rank.second > 2)
            {
                temp.push_back(rank.first);
            }
        }

        std::sort(temp.begin(), temp.end());
        ssize_t size = temp.size();

        // OPTIMIZE: 为了是实现："3334445555 展示为，444555 3335；以最大的牌型显示"，下面代码开始瞎写了
        std::vector<std::tuple<ssize_t, ssize_t, size_t, size_t>> woyebuzhidaowozaixieshenmele;
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = static_cast<size_t>(j - i + 1);
                if (n < 2) continue;
                if (n * (handsCategory == HandsCategory::trioChainWithSolo ? 4 : 5) != hands.size()) continue;
                if (isContinuous(temp[i], temp[j], n))
                {
                    woyebuzhidaowozaixieshenmele.emplace_back(
                        std::tie<ssize_t, ssize_t, size_t, size_t>(i, j, temp[i], temp[j]));
                }
            }
        }

        if (woyebuzhidaowozaixieshenmele.empty()) return hands;

        const auto &max = std::max_element(woyebuzhidaowozaixieshenmele.begin(),
                                           woyebuzhidaowozaixieshenmele.end(),
                                           [](const std::tuple<ssize_t, ssize_t, size_t, size_t> &$0,
                                              const std::tuple<ssize_t, ssize_t, size_t, size_t> &$1) {
                                               size_t a_1, a_n, b_1, b_n;
                                               std::tie(std::ignore, std::ignore, a_1, a_n) = $0;
                                               std::tie(std::ignore, std::ignore, b_1, b_n) = $1;

                                               return a_1 + a_n < b_1 + b_n;
                                           });

        ssize_t m, n;
        std::tie(m, n, std::ignore, std::ignore) = *max;

        for (ssize_t k = m; k <= n; ++k)
        {
            ret.push_back(temp[k]);
            ret.push_back(temp[k]);
            ret.push_back(temp[k]);

            if (ranksCopy[temp[k]] == 3)
            {
                ranksCopy.erase(temp[k]);
            }
            else
            {
                ranksCopy[temp[k]] = 1;
            }
        }

        auto unzipped = unzip(ranksCopy);

        std::sort(unzipped.begin(), unzipped.end());

        ret.insert(ret.end(), unzipped.begin(), unzipped.end());
    }
    else if (handsCategory == HandsCategory::chain || handsCategory == HandsCategory::pairChain)
    {
        std::sort(values.begin(), values.end(), [](size_t $0, size_t $1) { return $0 > $1; });
        ret.insert(ret.end(), values.begin(), values.end());
    }
    else
    {
        ret = hands;
        std::sort(ret.begin(), ret.end());
        return ret;
    }

    return restoreHands(ret, ranksMultimap);
}

void Judge::reindex()
{
    if (!_cardIntentions.empty())
    {
        _iteratorIntentions = _cardIntentions.begin();
    }

    if (!_cardHint.empty())
    {
        _iteratorHint = _cardHint.begin();
    }
}

std::vector<size_t> Judge::intention(const std::vector<size_t> &hands, bool isStartingHand)
{
    /**
     首出提示：
        1.
        若出牌的时候点击提示，按照最小的牌（除了炸弹以外的最小牌）匹配牌型，按照连对（最大不拆的连对），飞机，三带X，四带X，顺子（顺子导致的拆牌不能超过2个），对子，单张的顺序匹配牌型，只需要提醒一个牌型就行了，一个牌型循环提示，最小的牌不拆333不能拆开3或者33，优先不拆牌；
        2. 未选择三带二强制，提示默认三带二，牌不够就三带一，三个；
        3. 三带二，没有单牌就带小牌；
        4. 3334555667，提示333 47；
        5. 33344557，提示333 44；因为单牌不够；
        6. 33344557,提示33347，不是333 44
        7. 33344455667，提示333444 5566；
        8. 33566778，提示33，因为没有匹配33的其他牌型了；
        9. 34455667，提示3；（顺子导致的拆牌不能超过2个）
        10. 3455667，提示34567；
        11. 344555667，提示555 37；
        12. 3455566，提示555 34；
        13. 3334455566，提示333 44；
        14. 33445556，提示3344；优先不拆牌
        15. 3335AAA提示（炸弹不可拆，三带二强制）给最小的非炸弹牌3
        16. 3335AAA提示（炸弹不可拆）3335
        17. 333AAA提示（炸弹不可拆）333
        18. 3344455556，提示44433 不提示3344（优先不拆牌）
        19. 3344455566668 提示444555 33
        20. 33444455567，提示55533；
        21. 34455556，提示5555 36；
        22. 344555566，提示5555 34；
        23. 344555566，提示3； （炸弹不可拆）
        24. 45667888999QQQQ ，最小牌是4，匹配到飞机，8889994566
     */
    if (_needRecalculateIntentions)
    {
        _cardIntentions     = cardIntentions(hands, isStartingHand);
        _iteratorIntentions = _cardIntentions.begin();
    }
    else
    {
        if (_iteratorIntentions == _cardIntentions.end())
        {
            _iteratorIntentions = _cardIntentions.begin();
            return std::vector<size_t>{};
        }
    }

    if (_cardIntentions.empty())
    {
        return std::vector<size_t>{};
    }
    return *_iteratorIntentions++;
}

std::vector<size_t> Judge::hint(const std::vector<size_t> &hands)
{
    // 给出符合牌型的所有组合，不考虑拆牌情况
    if (_needRecalculateHint)
    {
        _cardHint     = cardHint(hands);
        _iteratorHint = _cardHint.begin();
    }
    else
    {
        if (_iteratorHint == _cardHint.end())
        {
            _iteratorHint = _cardHint.begin();
            return std::vector<size_t>{};
        }
    }

    if (_cardHint.empty())
    {
        return std::vector<size_t>{};
    }
    return *_iteratorHint++;
}

#pragma mark - 转换手牌

#pragma mark - getter & setter
void Judge::setCurrentHandsCategory(const std::vector<size_t> &weight, const std::vector<size_t> &handsCategory)
{
    if (handsCategory.empty())
    {
        CurrentHandsCategory category{};
        HandsCategoryModel   model{};

        category.hands         = handsCategory;
        model.handsCategory    = HandsCategory::anyLegalCategory;
        category.handsCategory = model;

        _currentHandsCategory = category;
    }
    else
    {
        _currentHandsCategory.hands = weight;

        const auto &h = judgeHandsCategory(handsCategory);
        const auto &w = judgeHandsCategory(weight);

        if (w.handsCategory == HandsCategory::bomb)
        {
            _currentHandsCategory.handsCategory.handsCategory = w.handsCategory;
            _currentHandsCategory.handsCategory.weight        = w.weight;
        }
        // 当强制三带二的时候，实际上打出去三顺被认作三顺带二
        else if (Ruler::getInstance().isAlwaysWithPair() && h.handsCategory == HandsCategory::trioChain)
        {
            _currentHandsCategory.handsCategory.size          = h.size;
            _currentHandsCategory.handsCategory.handsCategory = HandsCategory::trioChainWithPair;
            _currentHandsCategory.handsCategory.weight = getTrioChainWeight(weight, HandsCategory::trioChainWithPair);
        }
        else
        {
            _currentHandsCategory.handsCategory.size          = h.size;
            _currentHandsCategory.handsCategory.handsCategory = h.handsCategory;
            _currentHandsCategory.handsCategory.weight        = w.weight;
        }
    }

    _needRecalculateIntentions = true;
    _needRecalculateHint       = true;
}

void Judge::setCurrentHands(const std::vector<size_t> &hands)
{
    _currentHands = hands;
}

#pragma mark - 私有函数
std::multimap<size_t, size_t> Judge::getRanksMultimap(const std::vector<size_t> &hands, bool isThreeOfHeartsFirst) const
{
    std::multimap<size_t, size_t> multimap;
    if (isThreeOfHeartsFirst && isContainsThreeOfHearts(hands))
    {
        multimap.insert(std::pair<size_t, size_t>(paiXing3, hongTao3));
        std::vector<size_t> filter;
        std::remove_copy_if(
            hands.begin(), hands.end(), std::back_inserter(filter), [](const size_t &$0) { return $0 == hongTao3; });
        for (const auto &hand : filter)
        {
            multimap.insert(std::pair<size_t, size_t>((hand >> 8) & 0xff, hand));
        }
        return multimap;
    }
    for (const auto &hand : hands)
    {
        multimap.insert(std::pair<size_t, size_t>((hand >> 8) & 0xff, hand));
    }
    return multimap;
}

std::vector<size_t> Judge::getCardRanks(const std::vector<size_t> &hands) const
{
    std::vector<size_t> t;
    t.reserve(hands.size());
    for (auto &&hand : hands)
    {
        t.push_back((hand >> 8) & 0xff);
    }
    return t;
}

template <typename T> std::unordered_map<size_t, size_t> Judge::zip(const T &t) const
{
    std::vector<size_t>                unique;
    std::unordered_map<size_t, size_t> map;
    std::unique_copy(t.begin(), t.end(), std::back_inserter(unique));
    for (size_t u : unique)
    {
        map[u] = static_cast<size_t>(std::count(t.begin(), t.end(), u));
    }

    return map;
}

std::vector<size_t> Judge::unzip(const std::unordered_map<size_t, size_t> &zipped) const
{
    std::vector<size_t> vector;
    for (const auto &item : zipped)
    {
        for (size_t i = 0; i < item.second; ++i)
        {
            vector.push_back(item.first);
        }
    }
    return vector;
}

std::vector<size_t> Judge::unzip(const std::unordered_map<size_t, size_t> &zipped, size_t ignore) const
{
    std::vector<size_t> vector;

    auto filter = zipped;
    if (filter.find(ignore) != filter.end())
    {
        filter.erase(ignore);
    }

    for (const auto &item : filter)
    {
        for (size_t i = 0; i < item.second; ++i)
        {
            vector.push_back(item.first);
        }
    }

    return vector;
}

std::vector<size_t> Judge::filter3(const std::unordered_map<size_t, size_t> &others, bool canSplit3) const
{
    if (canSplit3)
        return unzip(others);
    else
        return unzip(others, paiXing3);
}

std::unordered_map<size_t, size_t> Judge::filterA(const std::unordered_map<size_t, size_t> &ranks) const
{
    auto ranksCopy = ranks;
    if (!Ruler::getInstance().isBombDetachable() && Ruler::getInstance().isAsTrioAceBomb())
    {
        if (ranks.find(paiXingA) != ranks.end() && ranks.at(paiXingA) == 3)
        {
            ranksCopy.erase(paiXingA);
        }
    }
    return ranksCopy;
}

std::unordered_map<size_t, size_t> Judge::filterConventionalBomb(const std::unordered_map<size_t, size_t> &ranks) const
{
    auto ranksCopy = ranks;
    if (!Ruler::getInstance().isBombDetachable())
    {
        for (const auto &copy : ranks)
        {
            if (copy.second == 4)
            {
                ranksCopy.erase(copy.first);
            }
        }
    }
    return ranksCopy;
}

std::unordered_map<size_t, size_t> Judge::filterBombs(const std::unordered_map<size_t, size_t> &ranks) const
{
    auto copy = filterA(ranks);
    copy      = filterConventionalBomb(copy);
    return copy;
}

std::unordered_map<size_t, size_t> Judge::filterFour(const std::unordered_map<size_t, size_t> &ranks) const
{
    auto copy = ranks;
    for (const auto &item : ranks)
    {
        if (item.second == 4)
        {
            copy.erase(item.first);
        }
    }

    if (Ruler::getInstance().isAsTrioAceBomb())
    {
        if (copy.find(paiXingA) != copy.end() && copy[paiXingA] == 3)
        {
            copy.erase(paiXingA);
        }
    }

    return copy;
}

bool Judge::isContainsTarget(const std::vector<size_t> &temp) const
{
    return std::find(temp.begin(), temp.end(), _target) != temp.end();
}

bool Judge::isContainsBombs(const std::vector<size_t> &hands) const
{
    const auto &values = getCardRanks(_currentHands);
    const auto &ranks  = zip(values);

    std::vector<size_t> bombs;
    auto                isAsTrioAceBomb = Ruler::getInstance().isAsTrioAceBomb();
    for (const auto &rank : ranks)
    {
        if (isAsTrioAceBomb)
        {
            if (rank.first == paiXingA && rank.second == 3)
            {
                bombs.push_back(paiXingA);
                continue;
            }
        }
        if (rank.second == 4)
        {
            bombs.push_back(rank.first);
        }
    }

    const auto &v = getCardRanks(hands);
    for (const auto &item : v)
    {
        if (std::find(bombs.begin(), bombs.end(), item) != bombs.end()) return true;
    }

    return false;
}

bool Judge::canSplit3(const std::unordered_map<size_t, size_t> &others) const
{
    return !(others.find(paiXing3) != others.end() && others.at(paiXing3) > 2);
}

std::vector<std::vector<size_t>> Judge::combination(const std::vector<size_t> &n, ssize_t k) const
{
    std::vector<std::vector<size_t>> ret;

    if (n.empty() || k > static_cast<ssize_t>(n.size())) return ret;

    auto copy = n;
    std::sort(copy.begin(), copy.end());

    std::vector<std::vector<size_t>> node(1);
    auto                             last = copy[0];
    ssize_t                          flag = 1;

    for (const auto &i : copy)
    {
        if (i != last)
        {
            last = i;
            flag = node.size();
        }

        ssize_t size = node.size();
        for (ssize_t j = size - 1; j >= size - flag; j--)
        {
            if (static_cast<ssize_t>(node[j].size()) <= k)
            {
                node.push_back(node[j]);
            }
            else
            {
                continue;
            }

            node.back().push_back(i);
            if (static_cast<ssize_t>(node.back().size()) == k)
            {
                const auto &temp = node.back();
                // OPTIMIZE: 应用回溯法优化
                if (std::find_if(ret.begin(), ret.end(), [&temp](std::vector<size_t> i) -> bool { return i == temp; })
                    == ret.end())
                {
                    ret.push_back(node.back());
                }
            }
        }
    }

    return ret;
}

std::vector<std::vector<size_t>> Judge::combinationN2639(const std::vector<size_t> &n, ssize_t k) const
{
    std::vector<std::vector<size_t>> ret;
    if (n.empty() || static_cast<size_t>(k) > n.size()) return ret;

    auto copy = n;
    std::sort(copy.begin(), copy.end());

    auto it = next(copy.begin(), k);
    do
    {
        std::vector<size_t> t;
        std::copy(copy.begin(), it, std::back_inserter(t));
        ret.push_back(t);
    } while (next_combination(copy.begin(), it, copy.end()));

    return ret;
}

void Judge::withKicker(std::vector<std::vector<size_t>> &ret,
                       const std::vector<size_t> &       combination,
                       const std::vector<size_t> &       primal,
                       ssize_t                           kicker,
                       bool                              filterPairs) const
{
    auto y = this->combination(combination, kicker);

    if (Ruler::getInstance().isKickerAlwaysSameRank() && filterPairs)
    {
        y.erase(std::remove_if(y.begin(), y.end(), Functor()), y.end());
    }

    for (const auto &z : y)
    {
        auto copy = primal;
        copy.insert(copy.end(), z.begin(), z.end());

        ret.push_back(copy);
    }
}

void Judge::withKickerContainsTarget(std::vector<std::vector<size_t>> &ret,
                                     const std::vector<size_t> &       combination,
                                     const std::vector<size_t> &       primal,
                                     ssize_t                           kicker,
                                     bool                              filterPairs) const
{
    auto y = this->combination(combination, kicker);

    if (Ruler::getInstance().isKickerAlwaysSameRank() && filterPairs)
    {
        y.erase(std::remove_if(y.begin(), y.end(), Functor()), y.end());
    }

    for (const auto &z : y)
    {
        auto copy = primal;
        copy.insert(copy.end(), z.begin(), z.end());

        if (isContainsTarget(copy))
        {
            ret.push_back(copy);
        }
    }
}

std::vector<size_t> Judge::restoreHands(const std::vector<size_t> &          ret,
                                        const std::multimap<size_t, size_t> &ranksMultimap) const
{
    std::vector<size_t>        temp;
    std::unordered_set<size_t> unordered_set;

    for (const auto &item : ret)
    {
        auto iterator = ranksMultimap.find(item);
        while (iterator != ranksMultimap.end())
        {
            auto key = iterator->second;
            if (unordered_set.find(key) == unordered_set.end())
            {
                unordered_set.insert(key);
                temp.push_back(key);
                break;
            }

            ++iterator;
        }
    }

    return temp;
}

std::vector<std::vector<size_t>> Judge::restoreHands(const std::vector<std::vector<size_t>> &ret,
                                                     const std::multimap<size_t, size_t> &   ranksMultimap) const
{
    std::vector<std::vector<size_t>> temp1;
    std::vector<size_t>              temp2;
    for (const auto &v : ret)
    {
        std::unordered_set<size_t> unordered_set;
        temp2.clear();
        for (const auto &item : v)
        {
            auto iterator = ranksMultimap.find(item);
            while (iterator != ranksMultimap.end())
            {
                auto key = iterator->second;
                if (unordered_set.find(key) == unordered_set.end())
                {
                    unordered_set.insert(key);
                    temp2.push_back(key);
                    break;
                }

                ++iterator;
            }
        }
        temp1.push_back(temp2);
    }
    return temp1;
}

bool Judge::isSame(const std::unordered_map<size_t, size_t> &ranks, const std::string &category) const
{
    auto t = zip(category);
    if (ranks.size() == t.size())
    {
        std::unordered_set<size_t> s1{};
        std::unordered_set<size_t> s2{};
        for (auto &&rank : ranks)
        {
            s1.insert(rank.second);
        }
        for (auto &&item : t)
        {
            s2.insert(item.second);
        }
        if (s1 == s2)
        {
            return true;
        }
    }
    return false;
}

bool Judge::isContinuous(const std::vector<size_t> &vector) const
{
    /** $$	\ a_n=a_1+(n-1)d $$ */
    return *max_element(vector.begin(), vector.end()) - *min_element(vector.begin(), vector.end())
           == (vector.size() - 1) * 1;
}

bool Judge::isContinuous(size_t a_1, size_t a_n, size_t n) const
{
    return a_n - a_1 == (n - 1) * 1;
}

bool Judge::isChain(const std::unordered_map<size_t, size_t> &ranks) const
{
    // 顺子中不能有牌型2
    if (ranks.find(paiXing2) != ranks.end()) return false;

    if (ranks.size() >= 5)
    {
        std::vector<size_t> vector;
        for (const auto &rank : ranks)
        {
            if (rank.second != 1) return false;
            vector.push_back(rank.first);
        }
        return isContinuous(vector);
    }
    return false;
}

bool Judge::isPairChain(const std::unordered_map<size_t, size_t> &ranks) const
{
    if (ranks.size() >= 2)
    {
        std::vector<size_t> vector;
        for (const auto &rank : ranks)
        {
            if (rank.second != 2) return false;
            vector.push_back(rank.first);
        }
        return isContinuous(vector);
    }
    return false;
}

std::tuple<bool, HandsCategoryModel> Judge::isTrioChain(const std::unordered_map<size_t, size_t> &ranks) const
{
    if (ranks.size() >= 2)
    {
        std::vector<size_t> vector;
        for (const auto &rank : ranks)
        {
            if (rank.second >= 3) vector.push_back(rank.first);
        }
        if (vector.size() > 1)
        {
            std::sort(vector.begin(), vector.end());
            ssize_t count = vector.size();
            for (ssize_t i = 0; i < count - 1; ++i)
            {
                for (ssize_t j = count - 1; j > i; --j)
                {
                    auto n = j - i + 1;
                    if (isContinuous(vector[i], vector[j], static_cast<size_t>(n)))
                    {
                        // FIXME: 三顺的权重以等差数列的首项决定
                        // FIXME: 3334445555 展示为，444555 3335；以最大的牌型显示
                        // FIXME: 这里的权重判断出来会是3，有隐患
                        // OPTIMIZE: 此处并不好修改，所以暂时将三顺带一和三顺带二放在外面再计算一次权重
                        size_t  weight = vector[i];
                        auto    size   = std::accumulate(ranks.begin(), ranks.end(), static_cast<size_t>(0), Functor());
                        ssize_t x      = size - 3 * n;
                        if (x == 0)
                        {
                            return std::make_tuple<bool, HandsCategoryModel>(
                                true, HandsCategoryModel{ HandsCategory::trioChain, weight, size });
                        }
                        else if (x == n)
                        {
                            return std::make_tuple<bool, HandsCategoryModel>(
                                true, HandsCategoryModel{ HandsCategory::trioChainWithSolo, weight, size });
                        }
                        else if (x == 2 * n)
                        {
                            return std::make_tuple<bool, HandsCategoryModel>(
                                true, HandsCategoryModel{ HandsCategory::trioChainWithPair, weight, size });
                        }
                    }
                }
            }
        }
    }
    return std::make_tuple<bool, HandsCategoryModel>(false, HandsCategoryModel{});
}

size_t Judge::getTrioChainWeight(const std::vector<size_t> &hands, HandsCategory handsCategory) const
{
    if (handsCategory != HandsCategory::trioChainWithSolo && handsCategory != HandsCategory::trioChainWithPair)
        return 0;
    std::vector<size_t> temp;
    const auto &        values    = getCardRanks(hands);
    auto                ranksCopy = zip(values);

    for (const auto &rank : ranksCopy)
    {
        if (rank.second > 2)
        {
            temp.push_back(rank.first);
        }
    }

    std::sort(temp.begin(), temp.end());
    ssize_t size = temp.size();

    std::vector<std::tuple<ssize_t, ssize_t, size_t, size_t>> woyebuzhidaowozaixieshenmele;
    for (ssize_t i = 0; i < size - 1; ++i)
    {
        for (ssize_t j = size - 1; j > i; --j)
        {
            auto n = j - i + 1;
            if (n * (handsCategory == HandsCategory::trioChainWithSolo ? 4 : 5) != static_cast<ssize_t>(hands.size()))
                continue;
            if (isContinuous(temp[i], temp[j], static_cast<size_t>(n)))
            {
                woyebuzhidaowozaixieshenmele.emplace_back(
                    std::tie<ssize_t, ssize_t, size_t, size_t>(i, j, temp[i], temp[j]));
            }
        }
    }

    if (woyebuzhidaowozaixieshenmele.empty()) return 0;

    const auto &max = max_element(woyebuzhidaowozaixieshenmele.begin(),
                                  woyebuzhidaowozaixieshenmele.end(),
                                  [](const std::tuple<ssize_t, ssize_t, size_t, size_t> &$0,
                                     const std::tuple<ssize_t, ssize_t, size_t, size_t> &$1) {
                                      size_t a_1, a_n, b_1, b_n;
                                      tie(std::ignore, std::ignore, a_1, a_n) = $0;
                                      tie(std::ignore, std::ignore, b_1, b_n) = $1;

                                      return a_1 + a_n < b_1 + b_n;
                                  });

    ssize_t m;
    tie(m, std::ignore, std::ignore, std::ignore) = *max;
    return temp[m];
}

void Judge::enumerate(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const
{
    // AAA在作为💣且💣不可拆时不能拆开

    // 1. 首先匹配连对
    enumeratePairChain(ret, ranks);
    if (!ret.empty()) return;

    // 2. 匹配三顺
    enumerateTrioChain(ret, ranks);
    if (!ret.empty()) return;

    // 3. 匹配三带X
    enumerateTrio(ret, ranks);
    if (!ret.empty()) return;

    /** 在2018年 9月 2日，产品更改需求不想在首出中提示四带X，故先注释掉
        // 4. 匹配四带X
        enumerateFour(ret, ranks);
        if (!ret.empty()) return;
    */

    // 5. 匹配顺子
    enumerateChain(ret, ranks);
    if (!ret.empty()) return;

    // 6. 匹配对子
    enumeratePair(ret, ranks);
    if (!ret.empty()) return;

    // 7. 匹配单张
    enumerateSolo(ret, ranks);
    if (!ret.empty()) return;

    // 8. 都不匹配，程序闪退
    // exit(1024);
}

void Judge::enumerateSolo(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const
{
    auto                temp = filter3(ranks, canSplit3(ranks));
    std::vector<size_t> vector;

    if (std::find(temp.begin(), temp.end(), _target) != temp.end())
    {
        vector.push_back(_target);
        ret.push_back(vector);
        return;
    }

    if (!temp.empty())
    {
        vector.push_back(*std::min_element(temp.begin(), temp.end()));
        ret.push_back(vector);
    }
}

void Judge::enumeratePair(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const
{
    if (ranks.empty() || !canSplit3(ranks)) return;

    auto ranksCopy = filterA(ranks);

    for (const auto &rank : ranksCopy)
    {
        std::vector<size_t> temp;

        if (rank.second > 1 && rank.second < 4)
        {

            temp.push_back(rank.first);
            temp.push_back(rank.first);

            if (isContainsTarget(temp))
            {
                ret.push_back(temp);
                return;
            }
        }
    }
}

void Judge::enumerateTrio(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const
{
    if (ranks.empty()) return;

    auto ranksCopy        = filterFour(ranks);
    auto isAlwaysWithPair = Ruler::getInstance().isAlwaysWithPair();

    for (const auto &rank : ranksCopy)
    {
        std::vector<size_t> temp;
        // 在2018年9月2日，产品不想在提示三带的时候出现四张的拆牌，以及当AAA算炸时，AAA的拆牌在三带中的"三"或带牌中出现
        if (rank.second == 3)
        {
            //三不带
            temp.clear();
            for (int i = 0; i < 3; ++i)
            {
                temp.push_back(rank.first);
            }

            if (!isAlwaysWithPair && isContainsTarget(temp))
            {
                ret.push_back(temp);
            }

            auto others = ranksCopy;
            others.erase(rank.first);

            const auto &x = filter3(others, canSplit3(others));

            //三带一
            if (!isAlwaysWithPair)
            {
                withKickerContainsTarget(ret, x, temp, 1, false);
            }

            //三带二
            withKickerContainsTarget(ret, x, temp, 2, true);
        }
    }
}

void Judge::enumerateFour(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const
{
    if (ranks.empty()) return;
    if (!Ruler::getInstance().isBombDetachable()) return;

    auto ranksCopy = filterA(ranks);

    auto isAlwaysWithPair = Ruler::getInstance().isAlwaysWithPair();

    for (const auto &rank : ranksCopy)
    {
        std::vector<size_t> temp{};

        if (rank.second == 4)
        {
            // 💣不能单独作为四不带打出去
            temp.clear();
            for (int i = 0; i < 4; ++i)
            {
                temp.push_back(rank.first);
            }

            auto others = ranksCopy;
            others.erase(rank.first);

            const auto &x = filter3(others, canSplit3(others));

            // 四带一
            if (isAlwaysWithPair)
            {
                withKickerContainsTarget(ret, x, temp, 1, false);
            }

            // 四带二
            withKickerContainsTarget(ret, x, temp, 2, false);
        }
    }
}

void Judge::enumerateChain(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const
{
    if (ranks.size() < 5) return;

    // 2不参与连牌
    auto ranksCopy = ranks;
    ranksCopy.erase(paiXing2);

    if (ranksCopy.size() > 4)
    {
        std::vector<size_t> temp;
        std::vector<size_t> t;

        // 由于333、3333牌型不可拆，故先分离3
        // OPTIMIZE: 如果顺子造成的拆牌超过两张后，该顺子不提示。此处通过把结果算出来后再比对是否拆牌过两张。
        // OPTIMIZE: 但正确的做法应是在枚举时发现拆牌多于两张时回溯
        t.reserve(ranksCopy.size());
        for (const auto &rank : ranksCopy)
        {
            t.push_back(rank.first);
        }

        std::sort(t.begin(), t.end());
        if (!isContainsTarget(t)) return;

        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = j - i + 1;
                if (n < 5) continue;

                if (isContinuous(t[i], t[j], static_cast<size_t>(n)))
                {
                    temp.clear();

                    for (ssize_t k = i; k <= j; ++k)
                    {
                        temp.push_back(t[k]);
                    }

                    // OPTIMIZE: 此处通过暴力计算是否包含最小牌型
                    if (!isContainsTarget(temp)) continue;

                    // OPTIMIZE: 此处通过暴力计算是否拆牌超过两张
                    auto count = std::accumulate(
                        temp.begin(), temp.end(), static_cast<size_t>(0), [&ranksCopy](size_t $0, size_t $1) {
                            return $0 + (ranksCopy[$1] - 1);
                        });

                    if (count < 3)
                    {
                        ret.push_back(temp);
                        return;
                    }
                }
            }
        }
    }
}

void Judge::enumeratePairChain(std::vector<std::vector<size_t>> &        ret,
                               const std::unordered_map<size_t, size_t> &ranks) const
{
    // 连对
    // 连对中必须包括牌型中最小的那张牌
    // 仅需要找出一个最大且不拆的连对
    if (ranks.size() > 1)
    {
        std::vector<size_t> temp;

        std::vector<size_t> t;

        for (const auto &rank : ranks)
        {
            if (rank.second == 2)
            {
                t.push_back(rank.first);
            }
        }

        // OPTIMIZE: 连对中必须要有最小牌型，使用一个垃圾办法筛选
        if (!isContainsTarget(t)) return;

        std::sort(t.begin(), t.end());

        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = j - i + 1;
                if (n < 2) continue;

                if (isContinuous(t[i], t[j], static_cast<size_t>(n)))
                {
                    temp.clear();

                    for (ssize_t k = i; k <= j; ++k)
                    {
                        temp.push_back(t[k]);
                        temp.push_back(t[k]);
                    }

                    // OPTIMIZE: 如果排序没有问题t[i]就是最小牌型，但是因不好验证暂时使用保险的方法
                    // OPTIMIZE: 跑得快中只需要找出最长的不可拆连对就行了，所以找到一个就 return
                    if (isContainsTarget(temp))
                    {
                        ret.push_back(temp);
                        return;
                    }
                }
            }
        }
    }
}

void Judge::enumerateTrioChain(std::vector<std::vector<size_t>> &        ret,
                               const std::unordered_map<size_t, size_t> &ranks) const
{
    if (ranks.size() > 2)
    {
        std::vector<size_t> temp;
        std::vector<size_t> t;

        auto isBombDetachable = Ruler::getInstance().isBombDetachable();
        auto ranksCopy        = filterFour(ranks);

        for (const auto &rank : ranksCopy)
        {
            if (isBombDetachable ? rank.second > 2 : rank.second == 3)
            {
                t.push_back(rank.first);
            }
        }

        std::sort(t.begin(), t.end());

        auto    isAlwaysWithPair = Ruler::getInstance().isAlwaysWithPair();
        ssize_t size             = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = j - i + 1;
                if (n < 2) continue;

                if (isContinuous(t[i], t[j], static_cast<size_t>(n)))
                {
                    temp.clear();

                    // FIXME: 此处已经假设数组是有序的
                    // 三顺
                    for (ssize_t k = i; k <= j; ++k)
                    {
                        temp.push_back(t[k]);
                        temp.push_back(t[k]);
                        temp.push_back(t[k]);
                    }

                    // 仅当不强制三带二且三顺中有最小牌时提示
                    if (!isAlwaysWithPair && isContainsTarget(temp))
                    {
                        ret.push_back(temp);
                    }

                    // 三顺带一
                    auto others = ranksCopy;
                    for (ssize_t k = i; k <= j; ++k)
                    {
                        if (isBombDetachable && others[t[k]] == 4)
                        {
                            others[t[k]] = 1;
                        }
                        else
                        {
                            others.erase(t[k]);
                        }
                    }

                    // 如果最小牌是3且有三张3时，3不可拆牌
                    // 所以当3不可拆牌时，组合时排除3
                    auto xx = others;
                    if (!isBombDetachable) xx = filterBombs(xx);
                    const auto &x = filter3(xx, canSplit3(others));

                    if (!isAlwaysWithPair)
                    {
                        withKickerContainsTarget(ret, x, temp, n, false);
                    }

                    // 三顺带二
                    withKickerContainsTarget(ret, x, temp, 2 * n, true);
                }
            }
        }
    }
}

void Judge::exhaustiveSolo(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &copy) const
{
    auto weight = _currentHandsCategory.handsCategory.weight;

    std::vector<size_t> temp;

    for (const auto &item : copy)
    {
        if (item.first > weight)
        {
            temp.clear();
            temp.push_back(item.first);

            ret.push_back(temp);
        }
    }
}

void Judge::exhaustivePair(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &copy) const
{
    auto weight = _currentHandsCategory.handsCategory.weight;

    std::vector<size_t> temp;

    for (const auto &item : copy)
    {
        if (item.first > weight && item.second > 1)
        {
            temp.clear();
            temp.push_back(item.first);
            temp.push_back(item.first);

            ret.push_back(temp);
        }
    }
}

void Judge::exhaustiveTrio(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &copy) const
{
    auto weight = _currentHandsCategory.handsCategory.weight;

    std::vector<size_t> temp;

    for (const auto &item : copy)
    {
        if (item.first > weight && item.second > 2)
        {
            temp.clear();
            temp.push_back(item.first);
            temp.push_back(item.first);
            temp.push_back(item.first);

            ret.push_back(temp);
        }
    }
}

void Judge::exhaustiveTrioWithSolo(std::vector<std::vector<size_t>> &        ret,
                                   const std::unordered_map<size_t, size_t> &copy) const
{
    auto weight = _currentHandsCategory.handsCategory.weight;

    std::vector<size_t> temp;

    for (const auto &item : copy)
    {
        if (item.first > weight && item.second > 2)
        {
            temp.clear();
            temp.push_back(item.first);
            temp.push_back(item.first);
            temp.push_back(item.first);

            auto others = copy;
            others.erase(item.first);

            const auto &x = unzip(others);

            withKicker(ret, x, temp, 1, false);
        }
    }
}

void Judge::exhaustiveTrioWithPair(std::vector<std::vector<size_t>> &        ret,
                                   const std::unordered_map<size_t, size_t> &copy) const
{
    auto weight = _currentHandsCategory.handsCategory.weight;

    std::vector<size_t> temp;

    for (const auto &item : copy)
    {
        if (item.first > weight && item.second > 2)
        {
            temp.clear();
            temp.push_back(item.first);
            temp.push_back(item.first);
            temp.push_back(item.first);

            auto others = copy;
            others.erase(item.first);

            const auto &x = unzip(others);

            withKicker(ret, x, temp, 2, true);

            // 四带一也算是三带二，当炸弹可拆时
            if (item.second == 4)
            {
                const auto &y = this->combination(x, 1);

                for (const auto &z : y)
                {
                    auto copy1 = temp;
                    copy1.insert(copy1.end(), z.begin(), z.end());

                    copy1.push_back(item.first);
                    ret.push_back(copy1);
                }
            }
        }
    }
}

void Judge::exhaustiveChain(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &copy) const
{
    auto length = _currentHandsCategory.handsCategory.size;
    auto weight = _currentHandsCategory.handsCategory.weight;

    if (copy.size() < length) return;

    // 2不参与连牌
    auto ranksCopy = copy;
    ranksCopy.erase(paiXing2);

    if (ranksCopy.size() > length - 1)
    {
        std::vector<size_t> temp;
        std::vector<size_t> t;

        t.reserve(ranksCopy.size());
        for (const auto &rank : ranksCopy)
        {
            t.push_back(rank.first);
        }
        std::sort(t.begin(), t.end());
        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = j - i + 1;
                if (n != static_cast<ssize_t>(length) || t[i] <= weight) continue;

                if (isContinuous(t[i], t[j], static_cast<size_t>(n)))
                {
                    temp.clear();

                    for (ssize_t k = i; k <= j; ++k)
                    {
                        temp.push_back(t[k]);
                    }

                    ret.push_back(temp);
                }
            }
        }
    }
}

void Judge::exhaustivePairChain(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &copy) const
{
    auto length = _currentHandsCategory.handsCategory.size;
    auto weight = _currentHandsCategory.handsCategory.weight;

    if (copy.size() < length) return;

    auto ranksCopy = copy;

    if (ranksCopy.size() > length - 1)
    {
        std::vector<size_t> temp;
        std::vector<size_t> t;

        for (const auto &rank : ranksCopy)
        {
            if (rank.second > 1)
            {
                t.push_back(rank.first);
            }
        }

        std::sort(t.begin(), t.end());

        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = static_cast<size_t>(j - i + 1);
                if (n != length || t[i] <= weight) continue;

                if (isContinuous(t[i], t[j], n))
                {
                    temp.clear();

                    for (ssize_t k = i; k <= j; ++k)
                    {
                        temp.push_back(t[k]);
                        temp.push_back(t[k]);
                    }

                    ret.push_back(temp);
                }
            }
        }
    }
}

void Judge::exhaustiveTrioChain(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const
{
    if (ranks.size() > 2)
    {
        std::vector<size_t> temp;
        std::vector<size_t> t;

        auto length = _currentHandsCategory.handsCategory.size;
        auto weight = _currentHandsCategory.handsCategory.weight;
        length /= 3;

        for (const auto &rank : ranks)
        {
            if (rank.first > weight && rank.second > 2)
            {
                t.push_back(rank.first);
            }
        }
        std::sort(t.begin(), t.end());
        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = static_cast<size_t>(j - i + 1);
                if (n != length) continue;

                if (isContinuous(t[i], t[j], n))
                {
                    temp.clear();

                    // 三顺
                    for (ssize_t k = i; k <= j; ++k)
                    {
                        temp.push_back(t[k]);
                        temp.push_back(t[k]);
                        temp.push_back(t[k]);
                    }

                    ret.push_back(temp);
                }
            }
        }
    }
}

void Judge::exhaustiveTrioChainWithSolo(std::vector<std::vector<size_t>> &        ret,
                                        const std::unordered_map<size_t, size_t> &ranks) const
{
    if (ranks.size() > 2)
    {
        std::vector<size_t> temp;
        std::vector<size_t> t;

        auto length = _currentHandsCategory.handsCategory.size;
        auto weight = _currentHandsCategory.handsCategory.weight;
        length /= 4;

        for (const auto &rank : ranks)
        {
            if (rank.first > weight && rank.second > 2)
            {
                t.push_back(rank.first);
            }
        }
        std::sort(t.begin(), t.end());
        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = static_cast<size_t>(j - i + 1);
                if (n != length) continue;

                if (isContinuous(t[i], t[j], n))
                {
                    temp.clear();

                    // 三顺
                    for (ssize_t k = i; k <= j; ++k)
                    {
                        temp.push_back(t[k]);
                        temp.push_back(t[k]);
                        temp.push_back(t[k]);
                    }

                    auto others = ranks;
                    for (ssize_t k = i; k <= j; ++k)
                    {
                        others.at(t[k]) == 3 ? others.erase(t[k]) : (others.at(t[k]) = 1);
                    }

                    const auto &x = unzip(others);

                    withKicker(ret, x, temp, n, false);
                }
            }
        }
    }
}

void Judge::exhaustiveTrioChainWithPair(std::vector<std::vector<size_t>> &        ret,
                                        const std::unordered_map<size_t, size_t> &ranks) const
{
    if (ranks.size() > 2)
    {
        std::vector<size_t> temp;
        std::vector<size_t> t;

        auto length = _currentHandsCategory.handsCategory.size;
        auto weight = _currentHandsCategory.handsCategory.weight;
        length /= 5;

        for (const auto &rank : ranks)
        {
            if (rank.first > weight && rank.second > 2)
            {
                t.push_back(rank.first);
            }
        }
        std::sort(t.begin(), t.end());
        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = static_cast<size_t>(j - i + 1);
                if (n != length) continue;

                if (isContinuous(t[i], t[j], n))
                {
                    temp.clear();

                    // 三顺
                    for (ssize_t k = i; k <= j; ++k)
                    {
                        temp.push_back(t[k]);
                        temp.push_back(t[k]);
                        temp.push_back(t[k]);
                    }

                    auto others = ranks;
                    for (ssize_t k = i; k <= j; ++k)
                    {
                        others.at(t[k]) == 3 ? others.erase(t[k]) : (others.at(t[k]) = 1);
                    }

                    const auto &x = unzip(others);

                    withKicker(ret, x, temp, 2 * n, true);
                }
            }
        }
    }
}

void Judge::exhaustiveBombs(std::vector<std::vector<size_t>> &        ret,
                            const std::unordered_map<size_t, size_t> &ranks) const
{
    auto                weight = _currentHandsCategory.handsCategory.weight;
    std::vector<size_t> temp;

    for (const auto &rank : ranks)
    {
        if (rank.first > weight && rank.second > 3)
        {
            temp.clear();
            for (int i = 0; i < 4; ++i)
            {
                temp.push_back(rank.first);
            }

            ret.push_back(temp);
        }
    }

    if (Ruler::getInstance().isAsTrioAceBomb() && ranks.find(paiXingA) != ranks.end() && ranks.at(paiXingA) == 3)
    {
        temp.clear();
        for (int i = 0; i < 3; ++i)
        {
            temp.push_back(paiXingA);
        }

        ret.push_back(temp);
    }
}

void Judge::exhaustiveFourWithSolo(std::vector<std::vector<size_t>> &        ret,
                                   const std::unordered_map<size_t, size_t> &ranks) const
{
    auto weight = _currentHandsCategory.handsCategory.weight;

    std::vector<size_t> temp;

    for (const auto &item : ranks)
    {
        if (item.first > weight && item.second > 3)
        {
            temp.clear();
            for (int i = 0; i < 4; ++i)
            {
                temp.push_back(item.first);
            }

            auto others = ranks;
            others.erase(item.first);

            const auto &x = unzip(others);

            withKicker(ret, x, temp, 1, false);
        }
    }
}

void Judge::exhaustiveFourWithPair(std::vector<std::vector<size_t>> &        ret,
                                   const std::unordered_map<size_t, size_t> &ranks) const
{
    auto weight = _currentHandsCategory.handsCategory.weight;

    std::vector<size_t> temp;

    for (const auto &item : ranks)
    {
        if (item.first > weight && item.second > 3)
        {
            temp.clear();
            for (int i = 0; i < 4; ++i)
            {
                temp.push_back(item.first);
            }

            auto others = ranks;
            others.erase(item.first);

            const auto &x = unzip(others);

            withKicker(ret, x, temp, 2, false);
        }
    }
}

void Judge::appendBombs(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const
{
    std::vector<size_t> temp;
    std::vector<size_t> t;

    for (const auto &rank : ranks)
    {
        if (rank.second == 4) t.push_back(rank.first);
    }

    std::sort(t.begin(), t.end());

    for (auto item : t)
    {
        temp.clear();
        for (int i = 0; i < 4; ++i)
            temp.push_back(item);
        ret.push_back(temp);
    }

    if (Ruler::getInstance().isAsTrioAceBomb() && ranks.find(paiXingA) != ranks.end() && ranks.at(paiXingA) == 3)
    {
        temp.clear();
        for (int i = 0; i < 3; ++i)
        {
            temp.push_back(paiXingA);
        }
        ret.push_back(temp);
    }
}

std::vector<std::vector<size_t>> Judge::cardIntentions(const std::vector<size_t> &hands, bool isStartingHand)
{
    if (!_needRecalculateIntentions) return _cardIntentions;

    std::vector<std::vector<size_t>> ret;
    if (hands.empty()) return ret;

    auto isThreeOfHeartsFirst = isStartingHand && Ruler::getInstance().isThreeOfHeartsFirst();

    // key 为牌型，value 为约定的实数（包含牌型与花色）
    auto ranksMultimap = getRanksMultimap(hands, isThreeOfHeartsFirst);

    auto values = getCardRanks(hands);
    auto ranks  = zip(values);

    // 当必出♥️3且手牌中有四个3时，直接提示四个3即可
    if (isThreeOfHeartsFirst && ranks.find(paiXing3) != ranks.end() && ranks[paiXing3] == 4)
    {
        std::vector<size_t> vector{ 3, 3, 3, 3 };
        ret.push_back(vector);
        goto cardIntentionsRestoreHands;
    }

    // 更新提示中必须包含的牌
    // FIXME: 当💣不可拆且当最小牌又是💣的时候，会提示出拆牌后的💣，故最小牌其实不能有💣
    if (isThreeOfHeartsFirst && isContainsThreeOfHearts(hands))
    {
        _target = paiXing3;
    }
    else
    {
        const auto &filter = filterFour(ranks);
        if (filter.empty())
        {
            auto m = *std::min_element(values.begin(), values.end());
            auto c = static_cast<size_t>(std::count(values.begin(), values.end(), m));

            std::vector<size_t> vector;
            vector.reserve(c);
            for (size_t i = 0; i < c; ++i)
            {
                vector.push_back(m);
            }
            ret.push_back(vector);

            goto cardIntentionsRestoreHands;
        }
        else
        {
            _target = (*std::min_element(filter.begin(), filter.end(), Functor())).first;
        }
    }

    // 枚举法
    enumerate(ret, ranks);

    // 根据拆牌多少排序结果，以接近测试要求
    if (ret.size() > 1)
    {
        sortHands(ret, ranks);
    }

cardIntentionsRestoreHands:
    // 将筛选出的组合结果还原为约定的实数
    const auto &temp = restoreHands(ret, ranksMultimap);
    _cardIntentions  = temp;

    _needRecalculateIntentions = false;
    return _cardIntentions;
}

std::vector<std::vector<size_t>> Judge::cardHint(const std::vector<size_t> &hands)
{
    if (!_needRecalculateHint) return _cardHint;

    // 排除特殊情况
    std::vector<std::vector<size_t>> ret;
    if (hands.empty()) return ret;

    auto handsCategory = _currentHandsCategory.handsCategory.handsCategory;

    if (handsCategory == HandsCategory::illegal || handsCategory == HandsCategory::anyLegalCategory) return ret;

    // key 为牌型，value 为约定的实数（包含牌型与花色）
    auto ranksMultimap = getRanksMultimap(hands);
    auto values        = getCardRanks(hands);

    auto ranks = zip(values);
    auto copy  = filterFour(ranks);

    // 枚举法
    switch (_currentHandsCategory.handsCategory.handsCategory)
    {
        case HandsCategory::solo:
            exhaustiveSolo(ret, copy);
            break;
        case HandsCategory::pair:
            exhaustivePair(ret, copy);
            break;
        case HandsCategory::trio:
            exhaustiveTrio(ret, copy);
            break;
        case HandsCategory::trioWithSolo:
            exhaustiveTrioWithSolo(ret, copy);
            break;
        case HandsCategory::trioWithPair:
            exhaustiveTrioWithPair(ret, copy);
            break;
        case HandsCategory::chain:
            exhaustiveChain(ret, copy);
            break;
        case HandsCategory::pairChain:
            exhaustivePairChain(ret, copy);
            break;
        case HandsCategory::trioChain:
            exhaustiveTrioChain(ret, copy);
            break;
        case HandsCategory::trioChainWithSolo:
            exhaustiveTrioChainWithSolo(ret, copy);
            break;
        case HandsCategory::trioChainWithPair:
            exhaustiveTrioChainWithPair(ret, copy);
            break;
        case HandsCategory::bomb:
            exhaustiveBombs(ret, ranks);
            break;
            // 在2018年 9月 3日，测试说当上家出四带一/二时，只提示💣，不提示四带一/二
        case HandsCategory::fourWithDualSolo:
            exhaustiveFourWithSolo(ret, copy);
            break;
        case HandsCategory::fourWithDualPair:
            exhaustiveFourWithPair(ret, copy);
            break;
        default:
            return ret;
    }

    // 根据拆牌多少排序结果，以接近测试要求
    if (ret.size() > 1 && handsCategory != HandsCategory::bomb)
    {
        sortHands(ret, ranks);
    }

    // 💣放在提示数组最后
    if (handsCategory != HandsCategory::bomb)
    {
        appendBombs(ret, ranks);
    }
    else
    {
        std::sort(ret.begin(), ret.end());
    }

    // 将筛选出的组合结果还原为约定的实数
    const auto &temp = restoreHands(ret, ranksMultimap);
    _cardHint        = temp;
    // FIXME: 这里这样写不好
    _iteratorHint = _cardHint.begin();

    _needRecalculateHint = false;
    return _cardHint;
}

size_t Judge::getSplitCount(const std::vector<size_t> &hands, const std::unordered_map<size_t, size_t> &ranks) const
{
    const auto &zipped = zip(hands);
    return std::accumulate(zipped.begin(),
                           zipped.end(),
                           static_cast<size_t>(0),
                           [&ranks](size_t $0, const std::unordered_map<size_t, size_t>::value_type &$1) {
                               return $0 + ranks.at($1.first) - $1.second;
                           });
}

void Judge::sortHands(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const
{
    sort(ret.begin(), ret.end(), [&](const std::vector<size_t> &x, const std::vector<size_t> &y) {
        auto n = getSplitCount(x, ranks);
        auto m = getSplitCount(y, ranks);
        if (n == m)
        {
            if (x.size() != y.size())
            {
                return x.size() > y.size();
            }
            else
            {
                std::vector<size_t> uniqueX, uniqueY;
                std::unique_copy(x.begin(), x.end(), std::back_inserter(uniqueX));
                std::unique_copy(y.begin(), y.end(), std::back_inserter(uniqueY));
                if (uniqueX.size() != uniqueY.size())
                {
                    return uniqueX.size() > uniqueY.size();
                }
                else
                {
                    return std::accumulate(x.begin(), x.end(), static_cast<size_t>(0))
                           < std::accumulate(y.begin(), y.end(), static_cast<size_t>(0));
                }
            }
        }
        return n < m;
    });
}

void Judge::setTheHighestSingleCard(const std::vector<size_t> &                 hands,
                                    std::vector<std::vector<size_t>> &          vector,
                                    std::vector<std::vector<size_t>>::iterator &iterator)
{
    if (!vector.empty() && vector.front().size() == 1)
    {
        vector.clear();

        std::vector<size_t> ret;

        auto ranksMultimap = getRanksMultimap(hands, false);
        auto values        = getCardRanks(hands);

        auto max = *(max_element(values.begin(), values.end()));
        if (Ruler::getInstance().isAsTrioAceBomb() && max == paiXingA
            && std::count(values.begin(), values.end(), paiXingA) == 3)
        {
            ret.emplace_back(paiXingA);
            ret.emplace_back(paiXingA);
            ret.emplace_back(paiXingA);
        }
        else if (std::count(values.begin(), values.end(), max) == 4)
        {
            ret.emplace_back(max);
            ret.emplace_back(max);
            ret.emplace_back(max);
            ret.emplace_back(max);
        }
        else
        {
            ret.emplace_back(max);
        }

        vector.push_back(restoreHands(ret, ranksMultimap));
        iterator = vector.begin();
    }
}

bool Judge::canPlay(const std::vector<size_t> &hands, const HandsCategoryModel &handsCategoryModel) const
{
    const auto &handsCategory          = handsCategoryModel.handsCategory;
    const auto &ranks                  = zip(getCardRanks(hands));
    const auto  isKickerAlwaysSameRank = Ruler::getInstance().isKickerAlwaysSameRank();

    // 当强制三带二时，所有的带牌不满两张都无法出牌
    if (Ruler::getInstance().isAlwaysWithPair())
    {
        if (handsCategory == HandsCategory::trio || handsCategory == HandsCategory::trioWithSolo) return false;

        if (handsCategory == HandsCategory::trioChain || handsCategory == HandsCategory::trioChainWithSolo)
        {
            std::vector<size_t> vector;
            for (const auto &rank : ranks)
            {
                if (rank.second >= 3) vector.push_back(rank.first);
            }
            if (vector.size() > 1)
            {
                std::sort(vector.begin(), vector.end());
                ssize_t count = vector.size();
                for (ssize_t i = 0; i < count - 1; ++i)
                {
                    for (ssize_t j = count - 1; j > i; --j)
                    {
                        auto n = static_cast<size_t>(j - i + 1);
                        if (isContinuous(vector[i], vector[j], n))
                        {
                            auto size = std::accumulate(ranks.begin(), ranks.end(), static_cast<size_t>(0), Functor());
                            if (size == 5 * n)
                            {
                                if (isKickerAlwaysSameRank)
                                {
                                    auto copy = ranks;
                                    for (size_t k = vector[i]; k <= vector[j]; ++k)
                                    {
                                        copy[k] == 4 ? (copy[k] = 1) : copy.erase(k);
                                    }
                                    for (const auto &item : copy)
                                    {
                                        if (item.second % 2 != 0) return false;
                                    }
                                }
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        }
    }

    if (isKickerAlwaysSameRank)
    {
        if (handsCategory == HandsCategory::trioWithPair)
        {
            return isSame(ranks, sanDaiEr1);
        }
        else if (handsCategory == HandsCategory::trioChainWithPair)
        {
            // 三顺可能会因为必带一对而造成权重偏移
            std::vector<size_t> vector;
            for (const auto &rank : ranks)
            {
                if (rank.second >= 3) vector.push_back(rank.first);
            }
            if (vector.size() > 1)
            {
                std::sort(vector.begin(), vector.end());
                ssize_t count = vector.size();
                for (ssize_t i = 0; i < count - 1; ++i)
                {
                    for (ssize_t j = count - 1; j > i; --j)
                    {
                        auto n = static_cast<size_t>(j - i + 1);
                        if (isContinuous(vector[i], vector[j], n))
                        {
                            auto size = std::accumulate(ranks.begin(), ranks.end(), static_cast<size_t>(0), Functor());
                            if (size == 5 * n)
                            {
                                auto copy = ranks;
                                for (size_t k = vector[i]; k <= vector[j]; ++k)
                                {
                                    copy[k] == 4 ? (copy[k] = 1) : copy.erase(k);
                                }
                                for (const auto &item : copy)
                                {
                                    if (item.second % 2 != 0) goto trioChainWithPairKickerAlwaysSameRankContinue;
                                }
                                return true;
                            trioChainWithPairKickerAlwaysSameRankContinue:
                                continue;
                            }
                        }
                    }
                }
            }
            return false;
        }
    }

    return !(handsCategory == HandsCategory::illegal);
}

bool Judge::canBeat(const std::vector<size_t> &hands) const
{
    const auto &x                      = judgeHandsCategory(hands);
    const auto &y                      = _currentHandsCategory.handsCategory;
    const auto &ranks                  = zip(getCardRanks(hands));
    const auto  isKickerAlwaysSameRank = Ruler::getInstance().isKickerAlwaysSameRank();

    if (x.handsCategory == HandsCategory::bomb)
    {
        if (y.handsCategory == HandsCategory::bomb)
        {
            return x.weight > y.weight;
        }
        return true;
    }

    if (x.size != y.size) return false;

    if (isKickerAlwaysSameRank)
    {
        if (x.handsCategory == HandsCategory::trioWithPair)
        {
            if (!isSame(ranks, sanDaiEr1)) return false;
        }
        else if (x.handsCategory == HandsCategory::trioChainWithPair)
        {
            if (isKickerRankUnpaired(x, ranks)) return false;
        }
    }
    else
    {
        // 当炸弹可拆时且不强制带二张时，玩家出三带二，跟牌者出四带一也能出牌，总之特殊处理一下
        if (Ruler::getInstance().isBombDetachable())
        {
            if (x.handsCategory == HandsCategory::fourWithDualSolo && y.handsCategory == HandsCategory::trioWithPair)
                return x.weight > y.weight;
        }
    }

    // 当强制三带二时，玩家出出来的三顺实际上可能是三顺带二
    if (Ruler::getInstance().isAlwaysWithPair() && y.handsCategory == HandsCategory::trioChain
        && (x.handsCategory == HandsCategory::trioChain || x.handsCategory == HandsCategory::trioChainWithSolo
            || x.handsCategory == HandsCategory::trioChainWithPair))
    {
        auto weight = getTrioChainWeight(hands, HandsCategory::trioChainWithPair);
        if (isKickerAlwaysSameRank)
        {
            if (isKickerRankUnpaired(x, ranks)) return false;
        }
        return weight > getTrioChainWeight(_currentHandsCategory.hands, HandsCategory::trioChainWithPair);
    }

    if (y.handsCategory == HandsCategory::trioChainWithSolo && x.handsCategory == HandsCategory::trioChain)
    {
        return getTrioChainWeight(hands, HandsCategory::trioChainWithSolo) > y.weight;
    }

    if (y.handsCategory == HandsCategory::trioChainWithPair
        && (x.handsCategory == HandsCategory::trioChain || x.handsCategory == HandsCategory::trioChainWithSolo))
    {
        auto weight = getTrioChainWeight(hands, HandsCategory::trioChainWithPair);
        auto copy   = x;
        copy.weight = weight;
        if (isKickerAlwaysSameRank)
        {
            if (isKickerRankUnpaired(copy, ranks)) return false;
        }
        return getTrioChainWeight(hands, HandsCategory::trioChainWithPair) > y.weight;
    }

    return x.handsCategory == y.handsCategory && x.weight > y.weight;
}

bool Judge::isKickerRankUnpaired(const HandsCategoryModel &                handsCategoryModel,
                                 const std::unordered_map<size_t, size_t> &ranks) const
{
    auto copy   = ranks;
    auto weight = handsCategoryModel.weight;
    auto size   = handsCategoryModel.size;
    auto j      = weight + size / 5;
    for (size_t i = weight; i < j; ++i)
    {
        copy[i] == 4 ? (copy[i] = 1) : copy.erase(i);
    }
    for (const auto &item : copy)
    {
        if (item.second % 2 != 0) return true;
    }
    return false;
}

PAGAMES_WINNER_POKER_END
