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
#ifndef PAGAMES_WINNER_JUDGE_H
#define PAGAMES_WINNER_JUDGE_H

#include "WinnerPokerCommon.h"
#include <map>
#include <unordered_map>
#include <vector>
PAGAMES_WINNER_POKER_BEGIN

struct HandsCategoryModel
{
    HandsCategory handsCategory; //牌型
    size_t        weight;        //权重
    size_t        size;          //数量
};

class Judge
{
private:
    /** 这一圈的牌型 */
    struct CurrentHandsCategory
    {
        HandsCategoryModel  handsCategory; //牌型
        std::vector<size_t> hands;         //这一圈第一个人打出的牌，决定了本圈牌型
    };

public:
    Judge()              = default;
    Judge(const Judge &) = delete;
    Judge &operator=(const Judge &) = delete;

#pragma mark - 单例
    static Judge &getInstance();

#pragma mark - 判定
    /**
     * 根据传入的手牌，判定是什么牌型
     * @param hands 待判定手牌
     * @return 牌型
     */
    HandsCategoryModel judgeHandsCategory(const std::vector<size_t> &hands) const;

    /**
     * 根据传入的手牌，判定用户是否已经走投无路，只能选择要不起
     * @param hands 待判定手牌
     * @return 是否只能要不起
     */
    bool isPass(const std::vector<size_t> &hands);

    /**
     * 根据传入的手牌，判定用户是否可以出牌
     * @param hands 想要出的牌
     * @param isStartingHand 是不是起手牌，如果是整个牌局出的第一手牌，需要判定♥️3必出
     * @return 是否可以出牌
     */
    bool canPlay(const std::vector<size_t> &hands, bool isStartingHand = false) const;

    /**
     * 是否是传入手牌中的最大单牌
     * @param hands 用户当前的整个手牌
     * @param singleCard 待判定的单牌
     * @return 是否是最大单张
     */
    bool isTheHighestSingleCard(const std::vector<size_t> &hands, size_t singleCard) const;

    /**
     * 是否在传入的手牌中包含♥️3
     * @param hands 用户当前的整个手牌
     * @return 是否包含♥️3
     */
    bool isContainsThreeOfHearts(const std::vector<size_t> &hands) const;

#pragma mark - 提示
    /**
     * 首出提示，通过分析用户传入的手牌，分析用户意图，给出一组符合出牌规则的牌型
     * @param hands 用户传入的手牌
     * @param isStartingHand 是不是起手牌，如果是整个牌局出的第一手牌，需要判定♥️3必出
     * @return 手牌中符合出牌意图的某个组合，也有可能是空
     */
    std::vector<size_t> intentions(const std::vector<size_t> &hands, bool isStartingHand = false);

    /**
     * 跟牌提示，依据持有的当前圈的牌型以及传入的手牌，给出一组能跟牌的牌型
     * @param hands 用户传入的手牌
     * @return 暗示用户可以出的牌，可能为空
     */
    std::vector<size_t> hint(const std::vector<size_t> &hands);

#pragma mark - 转换手牌

#pragma mark - getter & setter
    /**
     * 设置决定当前这一圈牌型的牌。
     * 如果设置为空的话说明当前用户拥有决定权，可以出符合牌型的任意牌。
     *
     * @param currentHandsCategory 决定当前这一圈牌型的牌数组
     */
    void setCurrentHandsCategory(const std::vector<size_t> &currentHandsCategory);

private:
#pragma mark - 私有变量
    /** 持有上一圈的牌型，用以判断能否跟牌和提示跟牌 */
    CurrentHandsCategory _lastHandsCategory{};
    /** 持有这一圈的牌型，用以判断能否跟牌和提示跟牌 */
    CurrentHandsCategory _currentHandsCategory{};

    /** 提示中必须包含的牌，可能是♥️3，也有可能是用户传入的手牌中最小的牌型 */
    size_t _target = 0;

    /** 持有用户上一次传入首出提示的手牌 */
    std::vector<size_t> _handsIntentions{};

    /** 持有分析好的首出提示数组 */
    std::vector<std::vector<size_t>> _cardIntentions{};

    /** 持有首出提示数组的迭代器，记录上一次提示的位置 */
    std::__wrap_iter<std::vector<size_t, std::allocator<size_t>> *> _iteratorIntentions;

    /** 持有用户上一次传入首出提示的手牌 */
    std::vector<size_t> _handsHint{};

    /** 持有分析好的首出提示数组 */
    std::vector<std::vector<size_t>> _cardHint{};

    /** 持有首出提示数组的迭代器，记录上一次提示的位置 */
    std::__wrap_iter<std::vector<size_t, std::allocator<size_t>> *> _iteratorHint;

private:
#pragma mark - 私有函数
    std::multimap<size_t, size_t> getRanksMultimap(const std::vector<size_t> &hands,
                                                   bool                       isThreeOfHeartsFirst = false) const;

    std::vector<size_t> getCardRanks(const std::vector<size_t> &hands) const;

public:
    template <typename T> std::unordered_map<size_t, size_t> zip(const T &t) const;

private:
    std::vector<size_t> unzip(const std::unordered_map<size_t, size_t> &zipped) const;

    std::vector<size_t> unzip(const std::unordered_map<size_t, size_t> &zipped, size_t ignore) const;

    std::vector<size_t> filter3(const std::unordered_map<size_t, size_t> &others, bool canSplit3) const;

    std::unordered_map<size_t, size_t> filterA(const std::unordered_map<size_t, size_t> &ranks) const;

    std::unordered_map<size_t, size_t> filterConventionalBomb(const std::unordered_map<size_t, size_t> &ranks) const;

    std::unordered_map<size_t, size_t> filterBombs(const std::unordered_map<size_t, size_t> &ranks) const;

    bool isContainsTarget(const std::vector<size_t> &temp) const;

    bool canSplit3(const std::unordered_map<size_t, size_t> &others) const;

    std::vector<std::vector<size_t>> combination(const std::vector<size_t> &n, ssize_t k) const;

    void withKicker(std::vector<std::vector<size_t>> &ret,
                    const std::vector<size_t> &       combination,
                    const std::vector<size_t> &       primal,
                    ssize_t                           kicker) const;

    void withKickerContainsTarget(std::vector<std::vector<size_t>> &ret,
                                  const std::vector<size_t> &       combination,
                                  const std::vector<size_t> &       primal,
                                  ssize_t                           kicker) const;

    std::vector<std::vector<size_t>> restoreHands(const std::vector<std::vector<size_t>> &ret,
                                                  const std::multimap<size_t, size_t> &   ranksMultimap) const;

    bool isSame(const std::unordered_map<size_t, size_t> &ranks, const std::string &category) const;

    bool isContinuous(const std::vector<size_t> &vector) const;

    bool isContinuous(size_t a_1, size_t a_n, ssize_t n) const;

    bool isChain(const std::unordered_map<size_t, size_t> &ranks) const;

    bool isPairChain(const std::unordered_map<size_t, size_t> &ranks) const;

    std::tuple<bool, HandsCategoryModel> isTrioChain(const std::unordered_map<size_t, size_t> &ranks) const;

    void enumerate(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void enumerateSolo(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void enumeratePair(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void enumerateTrio(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void enumerateFour(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void enumerateChain(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void enumeratePairChain(std::vector<std::vector<size_t>> &        ret,
                            const std::unordered_map<size_t, size_t> &ranks) const;

    void enumerateTrioChain(std::vector<std::vector<size_t>> &        ret,
                            const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveSolo(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustivePair(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveTrio(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveTrioWithSolo(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveTrioWithPair(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveChain(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustivePairChain(std::vector<std::vector<size_t>> &        ret,
                             const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveTrioChain(std::vector<std::vector<size_t>> &        ret,
                             const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveTrioChainWithSolo(std::vector<std::vector<size_t>> &        ret,
                                     const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveTrioChainWithPair(std::vector<std::vector<size_t>> &        ret,
                                     const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveBombs(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveFourWithSolo(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveFourWithPair(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const;

    void appendBombs(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    bool needRecalculate(const std::vector<size_t> &newer, std::vector<size_t> &older, bool isIgnoreHandsCategory);

public:
    std::vector<std::vector<size_t>> cardIntentions(const std::vector<size_t> &hands, bool isStartingHand = false);

    std::vector<std::vector<size_t>> cardHint(const std::vector<size_t> &hands);
};
PAGAMES_WINNER_POKER_END
#endif // PAGAMES_WINNER_JUDGE_H
