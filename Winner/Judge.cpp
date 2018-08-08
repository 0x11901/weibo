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
#include <unordered_set>

PAGAMES_WINNER_POKER_BEGIN

std::string 对子    = "AA";
std::string 三不带  = "AAA";
std::string 三带一  = "AAAB";
std::string 三带二1 = "AAABB";
std::string 三带二2 = "AAABC";
std::string 炸弹    = "AAAA";
std::string 四带一  = "AAAAB";
std::string 四带二1 = "AAAABB";
std::string 四带二2 = "AAAABC";

constexpr size_t 牌型3 = 3;
constexpr size_t 牌型A = 14;
constexpr size_t 牌型2 = 15;

constexpr size_t 红桃3 = 771;

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
        model.handsCategory = HandsCategory::不成牌型;
        return model;
    }

    auto vector = getCardRanks(hands);

    if (hands.size() == 1)
    {
        model.handsCategory = HandsCategory::单张;
        model.size          = 1;
        model.weight        = vector.front();
        return model;
    }

    std::unordered_map<size_t, size_t> ranks = zip(vector);
    for (auto &&item : ranks) {
        std::cout << item.first << std::endl;
        std::cout << item.second << std::endl;
    }

    if (isSame(ranks, 对子))
    {
        model.handsCategory = HandsCategory::对子;
        model.size          = 2;
        model.weight        = vector.front();
        return model;
    }

    // 下面的判断必须要考虑AAA当💣的特殊情况，所以先筛选出AAA当💣的特殊情况
    if (Ruler::getInstance().isAsTrioAceBomb())
    {
        if (ranks[牌型A] == 3)
        {
            if (ranks.size() == 1)
            {
                model.handsCategory = HandsCategory::炸弹;
                model.size          = 3;
                model.weight        = 牌型A;
                return model;
            }

            //当💣不可拆
            if (!Ruler::getInstance().isBombDetachable())
            {
                model.handsCategory = HandsCategory::不成牌型;
                return model;
            }
        }
    }

    if (isSame(ranks, 三不带))
    {
        model.handsCategory = HandsCategory::三不带;
        model.size          = 3;
        model.weight        = vector.front();
        return model;
    }

    if (isSame(ranks, 三带一))
    {
        model.handsCategory = HandsCategory::三带一;
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

    if (isSame(ranks, 三带二1) || isSame(ranks, 三带二2))
    {
        model.handsCategory = HandsCategory::三带二;
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

    if (isSame(ranks, 炸弹))
    {
        model.handsCategory = HandsCategory::炸弹;
        model.size          = 4;
        model.weight        = vector.front();
        return model;
    }

    if (isSame(ranks, 四带一))
    {
        model.handsCategory = HandsCategory::四带一;
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

    if (isSame(ranks, 四带二1) || isSame(ranks, 四带二2))
    {
        model.handsCategory = HandsCategory::四带二;
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

    // FIXME: 目前以顺子中最小的牌作为牌型的权重
    //判断顺子
    if (isChain(ranks))
    {
        model.handsCategory = HandsCategory::顺子;
        // OPTIMIZE: 待优化
        std::vector<size_t> v;
        v.reserve(ranks.size());
        for (const auto &rank : ranks)
        {
            v.push_back(rank.first);
        }
        model.weight = *std::min_element(v.begin(), v.end());
        model.size   = ranks.size();
        return model;
    }

    //判断连对
    if (isPairChain(ranks))
    {
        model.handsCategory = HandsCategory::连对;
        // OPTIMIZE: 待优化
        std::vector<size_t> v;
        v.reserve(ranks.size());
        for (const auto &rank : ranks)
        {
            v.push_back(rank.first);
        }
        model.weight = *std::min_element(v.begin(), v.end());
        model.size   = ranks.size();
        return model;
    }

    //判断三顺
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
        return model;
    }

    model.handsCategory = HandsCategory::不成牌型;
    return model;
}

bool Judge::isPass(const std::vector<size_t> &hands)
{
    return _currentHandsCategory.handsCategory.handsCategory == HandsCategory::可以出任意成牌牌型
               ? !cardIntentions(hands).empty()
               : !cardHint(hands).empty();
}

bool Judge::canPlay(const std::vector<size_t> &hands, bool isStartingHand) const
{
    if (isStartingHand && Ruler::getInstance().isThreeOfHeartsFirst() && !isContainsThreeOfHearts(hands))
    {
        return false;
    }

    if (_currentHandsCategory.handsCategory.handsCategory == HandsCategory::可以出任意成牌牌型)
    {
        return !(judgeHandsCategory(hands).handsCategory == HandsCategory::不成牌型);
    }
    else
    {
        const auto &x = judgeHandsCategory(hands);
        const auto &y = _currentHandsCategory.handsCategory;

        if (y.handsCategory == HandsCategory::炸弹)
        {
            if (x.handsCategory == HandsCategory::炸弹)
            {
                return x.weight > y.weight;
            }
            return false;
        }

        return x.handsCategory == y.handsCategory && x.size == y.size && x.weight > y.weight;
    }
}

bool Judge::isTheHighestSingleCard(const std::vector<size_t> &hands, size_t singleCard) const
{
    std::vector<size_t> filter;
    std::remove_copy_if(hands.begin(), hands.end(), std::back_inserter(filter), [&](size_t $0) {
        return std::count(hands.begin(), hands.end(), $0) != 1;
    });
    return *std::max_element(filter.begin(), filter.end()) == singleCard;
}

bool Judge::isContainsThreeOfHearts(const std::vector<size_t> &hands) const
{
    return std::find(hands.begin(), hands.end(), 红桃3) != hands.end();
}

#pragma mark - 提示
std::vector<size_t> Judge::intentions(const std::vector<size_t> &hands, bool isStartingHand)
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
    if (needRecalculate(hands, _handsIntentions, true))
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
    if (needRecalculate(hands, _handsHint, false))
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
void Judge::setCurrentHandsCategory(const std::vector<size_t> &currentHandsCategory)
{
    CurrentHandsCategory category{};
    category.hands = currentHandsCategory;
    if (currentHandsCategory.empty())
    {
        HandsCategoryModel model{};
        model.handsCategory    = HandsCategory::可以出任意成牌牌型;
        category.handsCategory = model;
    }
    else
    {
        category.handsCategory = judgeHandsCategory(currentHandsCategory);
    }
    Judge::_currentHandsCategory = category;
}

#pragma mark - 私有函数
std::multimap<size_t, size_t> Judge::getRanksMultimap(const std::vector<size_t> &hands, bool isThreeOfHeartsFirst) const
{
    std::multimap<size_t, size_t> multimap;
    if (isThreeOfHeartsFirst && isContainsThreeOfHearts(hands))
    {
        multimap.insert(std::pair<size_t, size_t>(牌型3, 红桃3));
        std::vector<size_t> filter;
        std::remove_copy_if(
            hands.begin(), hands.end(), std::back_inserter(filter), [](const size_t &$0) { return $0 == 红桃3; });
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
    // FIXME: 多次循环，可优化
    std::unordered_set<size_t> set{};
    for (const auto &t1 : t)
    {
        set.insert(t1);
    }

    std::unordered_map<size_t, size_t> map;
    for (size_t s : set)
    {
        map[s] = static_cast<size_t>(std::count(t.begin(), t.end(), s));
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
    {
        return unzip(others);
    }
    else
    {
        return unzip(others, 牌型3);
    }
}

std::unordered_map<size_t, size_t> Judge::filterA(const std::unordered_map<size_t, size_t> &ranks) const
{
    auto ranksCopy = ranks;
    if (!Ruler::getInstance().isBombDetachable() && Ruler::getInstance().isAsTrioAceBomb())
    {
        if (ranks.find(牌型A) != ranks.end() && ranks.at(牌型A) == 3)
        {
            ranksCopy.erase(牌型A);
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

bool Judge::isContainsTarget(const std::vector<size_t> &temp) const
{
    return std::find(temp.begin(), temp.end(), _target) != temp.end();
}

bool Judge::canSplit3(const std::unordered_map<size_t, size_t> &others) const
{
    return !(others.find(牌型3) != others.end() && others.at(牌型3) > 2);
}

std::vector<std::vector<size_t>> Judge::combination(const std::vector<size_t> &n, ssize_t k) const
{
    std::vector<std::vector<size_t>> ret;

    if (n.empty() || k > n.size()) return ret;

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
            // OPTIMIZE: 应用回溯算法优化
            node.push_back(node[j]);
            node.back().push_back(i);
            if (node.back().size() == k)
            {
                ret.push_back(node.back());
            }
        }
    }

    return ret;
}

void Judge::withKicker(std::vector<std::vector<size_t>> &ret,
                       const std::vector<size_t> &       combination,
                       const std::vector<size_t> &       primal,
                       ssize_t                           kicker) const
{
    const auto &y = this->combination(combination, kicker);

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
                                     ssize_t                           kicker) const
{
    const auto &y = this->combination(combination, kicker);

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

std::vector<std::vector<size_t>> Judge::restoreHands(const std::vector<std::vector<size_t>> &ret,
                                                     const std::multimap<size_t, size_t> &   ranksMultimap) const
{
    // ranksMultimap
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
    for (auto &&item2 : t) {
        std::cout << item2.first << std::endl;
        std::cout << item2.second << std::endl;
    }
    std::cout << ranks.size() << std::endl;
    std::cout << t.size() << std::endl;

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

bool Judge::isContinuous(size_t a_1, size_t a_n, ssize_t n) const
{
    return a_n - a_1 == (n - 1) * 1;
}

bool Judge::isChain(const std::unordered_map<size_t, size_t> &ranks) const
{
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
            auto count = vector.size();
            for (ssize_t i = 0; i < count - 1; ++i)
            {
                for (ssize_t j = count - 1; j > i; --j)
                {
                    ssize_t n = j - i + 1;
                    if (isContinuous(vector[i], vector[j], n))
                    {
                        // FIXME: 三顺的权重以等差数列的首项决定
                        size_t weight = vector[i];
                        size_t size   = 0;
                        for (const auto &rank : ranks)
                        {
                            size += rank.second;
                        }
                        auto x = size - 3 * n;
                        if (x == 0)
                        {
                            return std::make_tuple<bool, HandsCategoryModel>(
                                true, HandsCategoryModel{ HandsCategory::三顺, weight, size });
                        }
                        else if (x == n)
                        {
                            return std::make_tuple<bool, HandsCategoryModel>(
                                false, HandsCategoryModel{ HandsCategory::三顺带一, weight, size });
                        }
                        else if (x == 2 * n)
                        {
                            return std::make_tuple<bool, HandsCategoryModel>(
                                false, HandsCategoryModel{ HandsCategory::三顺带二, weight, size });
                        }
                    }
                }
            }
        }
    }
    return std::make_tuple<bool, HandsCategoryModel>(false, HandsCategoryModel{});
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

    // 4. 匹配四带X
    enumerateFour(ret, ranks);
    if (!ret.empty()) return;

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
    auto temp = filter3(ranks, canSplit3(ranks));
    if (!temp.empty())
    {
        std::vector<size_t> vector;
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

    auto ranksCopy = filterA(ranks);

    auto canSplitBomb     = Ruler::getInstance().isBombDetachable();
    auto isAlwaysWithPair = Ruler::getInstance().isAlwaysWithPair();

    for (const auto &rank : ranksCopy)
    {
        std::vector<size_t> temp;
        // FIXME: 如果可以四带自然也能三带，此处不知道如何操作
        if (canSplitBomb ? rank.second > 2 : rank.second == 3)
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
                withKickerContainsTarget(ret, x, temp, 1);
            }

            //三带二
            withKickerContainsTarget(ret, x, temp, 2);
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
                withKickerContainsTarget(ret, x, temp, 1);
            }

            // 四带二
            withKickerContainsTarget(ret, x, temp, 2);
        }
    }
}

void Judge::enumerateChain(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const
{
    if (ranks.size() < 5) return;

    // 2不参与连牌
    auto ranksCopy = ranks;
    ranksCopy.erase(牌型2);

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

                if (isContinuous(t[i], t[j], n))
                {
                    temp.clear();

                    for (ssize_t k = i; k <= j; ++k)
                    {
                        temp.push_back(t[k]);
                    }

                    // OPTIMIZE: 此处通过暴力计算是否包含最小牌型
                    if (!isContainsTarget(temp)) continue;

                    // OPTIMIZE: 此处通过暴力计算是否拆牌超过两张
                    size_t count = 0;
                    for (const auto &item : temp)
                    {
                        count += (ranksCopy[item] - 1);
                    }

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

        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = j - i + 1;
                if (n < 2) continue;

                if (isContinuous(t[i], t[j], n))
                {
                    temp.clear();

                    // FIXME: 此处已经假设数组是有序的
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

        auto ranksCopy = filterA(ranks);

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

                if (isContinuous(t[i], t[j], n))
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
                    const auto &x = filter3(others, canSplit3(others));

                    if (!isAlwaysWithPair)
                    {
                        withKickerContainsTarget(ret, x, temp, n);
                    }

                    // 三顺带二
                    withKickerContainsTarget(ret, x, temp, 2 * n);
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

            withKicker(ret, x, temp, 1);
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

            withKicker(ret, x, temp, 2);

            // 四带一也算是三带二，当炸弹可拆时
            if (item.second == 4)
            {
                const auto &y = this->combination(x, 1);

                for (const auto &z : y)
                {
                    auto copy1 = temp;
                    copy1.insert(copy1.end(), z.begin(), z.end());

                    // FIXME: 特意把最后一个"四"放在最后
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
    ranksCopy.erase(牌型2);

    if (ranksCopy.size() > length - 1)
    {
        std::vector<size_t> temp;
        std::vector<size_t> t;

        t.reserve(ranksCopy.size());
        for (const auto &rank : ranksCopy)
        {
            t.push_back(rank.first);
        }

        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = j - i + 1;
                if (n != length || t[i] <= weight) continue;

                if (isContinuous(t[i], t[j], n))
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

        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = j - i + 1;
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

        for (const auto &rank : ranks)
        {
            if (rank.first > weight && rank.second > 2)
            {
                t.push_back(rank.first);
            }
        }

        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = j - i + 1;
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

        for (const auto &rank : ranks)
        {
            if (rank.first > weight && rank.second > 2)
            {
                t.push_back(rank.first);
            }
        }

        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = j - i + 1;
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

                    withKicker(ret, x, temp, n);
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

        for (const auto &rank : ranks)
        {
            if (rank.first > weight && rank.second > 2)
            {
                t.push_back(rank.first);
            }
        }

        ssize_t size = t.size();
        for (ssize_t i = 0; i < size - 1; ++i)
        {
            for (ssize_t j = size - 1; j > i; --j)
            {
                auto n = j - i + 1;
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

                    withKicker(ret, x, temp, 2 * n);
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

    if (Ruler::getInstance().isAsTrioAceBomb() && ranks.find(牌型A) != ranks.end() && ranks.at(牌型A) == 3)
    {
        temp.clear();
        for (int i = 0; i < 3; ++i)
        {
            temp.push_back(牌型A);
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

            withKicker(ret, x, temp, 1);
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

            withKicker(ret, x, temp, 2);
        }
    }
}

void Judge::appendBombs(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const
{
    std::vector<size_t> temp;
    for (const auto &rank : ranks)
    {
        if (rank.second == 4)
        {
            temp.clear();
            for (int i = 0; i < 4; ++i)
            {
                temp.push_back(rank.first);
            }

            ret.push_back(temp);
        }
    }
    if (Ruler::getInstance().isAsTrioAceBomb() && ranks.find(牌型A) != ranks.end() && ranks.at(牌型A) == 3)
    {
        temp.clear();
        for (int i = 0; i < 3; ++i)
        {
            temp.push_back(牌型A);
        }
        ret.push_back(temp);
    }
}

bool Judge::needRecalculate(const std::vector<size_t> &newer, std::vector<size_t> &older, bool isIgnoreHandsCategory)
{
    const auto &x = _lastHandsCategory.handsCategory;
    const auto &y = _currentHandsCategory.handsCategory;
    auto        b = true;

    // OPTIMIZE: 待优化
    if (isIgnoreHandsCategory)
    {
        auto copy = newer;
        std::sort(copy.begin(), copy.end());
        b = copy != older;
        if (b)
        {
            older = copy;
        }
    }
    else
    {
        if (x.handsCategory != y.handsCategory || x.size != y.size || x.weight != y.weight)
        {
            Judge::_lastHandsCategory = Judge::_currentHandsCategory;
            auto copy                 = newer;
            std::sort(copy.begin(), copy.end());
            older = copy;
        }
        else
        {
            auto copy = newer;
            std::sort(copy.begin(), copy.end());
            b = copy != older;
            if (b)
            {
                older = copy;
            }
        }
    }

    return b;
}

std::vector<std::vector<size_t>> Judge::cardIntentions(const std::vector<size_t> &hands, bool isStartingHand)
{
    std::vector<std::vector<size_t>> ret;
    if (hands.empty()) return ret;

    auto isThreeOfHeartsFirst = false;
    if (isStartingHand && Ruler::getInstance().isThreeOfHeartsFirst())
    {
        isThreeOfHeartsFirst = true;
    }

    // key 为牌型，value 为约定的实数（包含牌型与花色）
    auto ranksMultimap = getRanksMultimap(hands, isThreeOfHeartsFirst);

    auto values = getCardRanks(hands);
    // 更新提示中必须包含的牌
    _target = isThreeOfHeartsFirst && isContainsThreeOfHearts(hands) ? 牌型3
                                                                     : *std::min_element(values.begin(), values.end());

    // FIXME:  按默认方式排序，可能和摆牌方式冲突，待摆牌完成后看是否需要处理
    std::sort(values.begin(), values.end());

    auto ranks = zip(values);

    //枚举法
    enumerate(ret, ranks);

    // 将筛选出的组合结果还原为约定的实数
    const auto &temp = restoreHands(ret, ranksMultimap);
    ret              = temp;

    return ret;
}

std::vector<std::vector<size_t>> Judge::cardHint(const std::vector<size_t> &hands)
{
    // 排除特殊情况
    std::vector<std::vector<size_t>> ret;
    if (hands.empty()) return ret;

    auto handsCategory = _currentHandsCategory.handsCategory.handsCategory;

    if (handsCategory == HandsCategory::不成牌型 || handsCategory == HandsCategory::可以出任意成牌牌型) return ret;

    // key 为牌型，value 为约定的实数（包含牌型与花色）
    auto ranksMultimap = getRanksMultimap(hands);

    auto values = getCardRanks(hands);

    // FIXME:  按默认方式排序，可能和摆牌方式冲突，待摆牌完成后看是否需要处理
    std::sort(values.begin(), values.end());

    auto ranks = zip(values);
    auto copy  = filterBombs(ranks);

    // 枚举法
    switch (_currentHandsCategory.handsCategory.handsCategory)
    {
        case HandsCategory::单张:
            exhaustiveSolo(ret, copy);
            break;
        case HandsCategory::对子:
            exhaustivePair(ret, copy);
            break;
        case HandsCategory::三不带:
            exhaustiveTrio(ret, copy);
            break;
        case HandsCategory::三带一:
            exhaustiveTrioWithSolo(ret, copy);
            break;
        case HandsCategory::三带二:
            exhaustiveTrioWithPair(ret, copy);
            break;
        case HandsCategory::顺子:
            exhaustiveChain(ret, copy);
            break;
        case HandsCategory::连对:
            exhaustivePairChain(ret, copy);
            break;
        case HandsCategory::三顺:
            exhaustiveTrioChain(ret, copy);
            break;
        case HandsCategory::三顺带一:
            exhaustiveTrioChainWithSolo(ret, copy);
            break;
        case HandsCategory::三顺带二:
            exhaustiveTrioChainWithPair(ret, copy);
            break;
        case HandsCategory::炸弹:
            exhaustiveBombs(ret, ranks);
            break;
        case HandsCategory::四带一:
            exhaustiveFourWithSolo(ret, ranks);
            break;
        case HandsCategory::四带二:
            exhaustiveFourWithPair(ret, ranks);
            break;
        default:
            return ret;
    }

    if (handsCategory != HandsCategory::炸弹)
    {
        appendBombs(ret, ranks);
    }

    // 将筛选出的组合结果还原为约定的实数
    const auto &temp = restoreHands(ret, ranksMultimap);
    ret              = temp;

    return ret;
}

PAGAMES_WINNER_POKER_END
