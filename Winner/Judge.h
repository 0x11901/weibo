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
     * 是否在传入的手牌中包含♥️3
     * @param hands 用户当前的整个手牌
     * @return 是否包含♥️3
     */
    bool isContainsThreeOfHearts(const std::vector<size_t> &hands) const;

#pragma mark - 提示
    /**
     * 智能提示，根据给 CurrentHandsCategory 传入的向量判断是想要首出提示还是跟牌提示
     * 当 setCurrentHandsCategory 传入空向量时，认为当前玩家首出，传入非空认为玩家跟牌
     * @param hands 玩家整个手牌
     * @param isStartingHand 是不是起手牌，如果是整个牌局出的第一手牌，需要判定♥️3必出
     * @return 手牌中符合出牌意图的某个组合，也有可能是空
     */
    std::vector<size_t> intentions(const std::vector<size_t> &hands, bool isStartingHand = false);

    /**
     * 应该提示最大单牌
     * 在2018年 8月27日 星期一，测试提出想要修改下家报单的逻辑
     * 当下家报单，如果首出提示是提示的单牌，那么应该提示最大的单牌，而不是最小的单牌
     * @param hands 玩家整个手牌
     */
    void shouldHintTheHighestSingleCard(const std::vector<size_t> &hands);

#pragma mark - 排序 & 重置索引
    /**
     * 重新排列打出的牌。
     *
     * @param hands 打出的牌。
     * @return 排列好的按照牌型排列的牌。
     */
    std::vector<size_t> rearrangeHands(const std::vector<size_t> &hands) const;

    /**
     * 在2018年 8月27日 星期一，测试提出想在把提示的牌点下去之后重置提示索引，想从头开始提示
     * 故开放本方法重置提示索引，调用后点击提示会从头开始
     */
    void reindex();

#pragma mark - 转换手牌
    /**
     * 转换手牌，将服务端传回的手牌数据结构转换为跑得快客户端约定的手牌数据结构
     * @param hands 服务端手牌的数据结构
     * @return 客户端约定的手牌数据结构
     */

    /**
     * 恢复手牌，将跑得快客户端约定的手牌数据结构恢复为服务端使用的手牌数据结构
     * @param hands 客户端约定的手牌数据结构
     * @return 服务端手牌的数据结构
     */

#pragma mark - getter & setter
    /**
     * 设置决定当前这一圈牌型的牌。
     * 如果设置为空的话说明当前用户拥有决定权，可以出符合牌型的任意牌。
     *
     * @param weight 决定出牌权重的牌向量（上家出牌）。
     * @param handsCategory 决定出牌及展示牌型的牌向量（本小轮首家出牌）。
     */
    void setCurrentHandsCategory(const std::vector<size_t> &weight, const std::vector<size_t> &handsCategory);

    /**
     * 设置当前的手牌，当玩家手牌发生变化时，应将手牌传给Judge，用在判断出牌是是否含有💣
     * 用以当💣不可拆，作出正确的能否出牌判定
     * @param hands
     */
    void setCurrentHands(const std::vector<size_t> &hands);

private:
#pragma mark - 私有变量
    /** 持有这一圈的牌型，用以判断能否跟牌和提示跟牌 */
    CurrentHandsCategory _currentHandsCategory{};

    /** 持有这一圈玩家的手牌，用以当💣不可拆，作出正确的能否出牌判定 */
    std::vector<size_t> _currentHands{};

    /** 提示中必须包含的牌，可能是♥️3，也有可能是用户传入的手牌中最小的牌型 */
    size_t _target = 0;

    /** 持有分析好的首出提示向量 */
    std::vector<std::vector<size_t>> _cardIntentions{};

    /** 持有首出提示向量的迭代器，记录上一次提示的位置 */
    std::vector<std::vector<size_t>>::iterator _iteratorIntentions{};

    /** 是否需要重新计算首出提示 */
    bool _needRecalculateIntentions = true;

    /** 持有分析好的跟牌提示向量 */
    std::vector<std::vector<size_t>> _cardHint{};

    /** 持有跟牌提示向量的迭代器，记录上一次提示的位置 */
    std::vector<std::vector<size_t>>::iterator _iteratorHint{};

    /** 是否需要重新计算跟牌提示 */
    bool _needRecalculateHint = true;

private:
#pragma mark - 私有函数
    /**
     * 传入未处理的手牌，获得手牌对应的大小到未处理手牌多重映射
     * @param hands 未处理过的手牌
     * @param isThreeOfHeartsFirst 是否是♥️3首出
     * @return 手牌对应的大小到未处理手牌多重映射
     */
    std::multimap<size_t, size_t> getRanksMultimap(const std::vector<size_t> &hands,
                                                   bool                       isThreeOfHeartsFirst = false) const;

    /**
     * 传入未处理的手牌，得到手牌对应的大小的向量
     * @param hands 未处理过的手牌
     * @return 手牌对应大小的向量
     */
    std::vector<size_t> getCardRanks(const std::vector<size_t> &hands) const;

public:
    /**
     * 传入装有手牌对应大小的标准容器，得到手牌对应大小到手牌个数的映射
     * @tparam T 标准容器
     * @param t 不传一般就是int了
     * @return 手牌对应大小到手牌个数的映射
     * @example [1,1,1,2,2,3] -> {{1,3},{2,2},{3,1}}
     */
    template <typename T> std::unordered_map<size_t, size_t> zip(const T &t) const;

private:
    /**
     * 传入手牌对应大小到手牌个数的映射，返回手牌对应大小的向量
     * @param zipped 手牌对应大小到手牌个数的映射
     * @return 手牌对应大小的向量
     * @example {{1,3},{2,2},{3,1}} -> [1,1,1,2,2,3]
     */
    std::vector<size_t> unzip(const std::unordered_map<size_t, size_t> &zipped) const;

    /**
     * 同上，增加忽略某个key的功能
     * @param zipped 手牌对应大小到手牌个数的映射
     * @param ignore 想要忽略掉的key
     * @return 手牌对应大小的向量
     * @example {{1,3},{2,2},{3,1}},1 -> [2,2,3]
     */
    std::vector<size_t> unzip(const std::unordered_map<size_t, size_t> &zipped, size_t ignore) const;

    /**
     * 忽略掉牌型3并调用unzip
     * @param others 手牌对应大小到手牌个数的映射
     * @param canSplit3 能否拆牌牌型3，如果不能拆，那么unzip出来的向量里不会包含牌型3
     * @return 手牌对应大小的向量
     */
    std::vector<size_t> filter3(const std::unordered_map<size_t, size_t> &others, bool canSplit3) const;

    /**
     * 根据牌局规则选在是否要删除映射中的牌型A
     * @param ranks 手牌对应大小到手牌个数的映射
     * @return 处理之后的映射，其中牌型A是否存在和薛定谔的猫一样
     */
    std::unordered_map<size_t, size_t> filterA(const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 删除映射中的普通💣，即四张相同大小的牌
     * @param ranks 手牌对应大小到手牌个数的映射
     * @return 处理之后的映射
     */
    std::unordered_map<size_t, size_t> filterConventionalBomb(const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 根据牌局规则删除映射中的💣，即当三张A算💣且有三张A时，将A也从映射中删除
     * @param ranks 手牌对应大小到手牌个数的映射
     * @return 处理之后的映射
     */
    std::unordered_map<size_t, size_t> filterBombs(const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 删除映射中所有💣，不考虑规则
     * @param ranks 手牌对应大小到手牌个数的映射
     * @return 处理之后的映射
     */
    std::unordered_map<size_t, size_t> filterFour(const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 是否在向量中包含提示中必须包含的牌，提示中必须包含的牌是一个私有成员变量
     * @param temp 装有手牌对应大小的向量
     * @return 是否包含
     */
    bool isContainsTarget(const std::vector<size_t> &temp) const;

    /**
     * 是否传入的手牌向量中含有💣，即四张相同牌或者当3张A算💣时有3张A
     * @param hands 装有手牌对应大小的向量
     * @return 是否包含
     */
    bool isContainsBombs(const std::vector<size_t> &hands) const;

    /**
     * 根据产品要求，当有三张以上牌型3时，3不可被拆牌
     * @param others 手牌对应大小到手牌个数的映射
     * @return 是否可拆
     */
    bool canSplit3(const std::unordered_map<size_t, size_t> &others) const;

    /**
     * 从给定个数的元素中取出指定个数的元素，进行组合
     * @param n 参与组合的元素的一维向量
     * @param k 参与选择的元素个数
     * @return 所有组合结果的二维向量
     */
    std::vector<std::vector<size_t>> combination(const std::vector<size_t> &n, ssize_t k) const;

    /**
     * 从给定个数的元素中取出指定个数的元素，进行组合
     * @param n 参与组合的元素的一维向量
     * @param k 参与选择的元素个数
     * @return 所有组合结果的二维向量
     */
    std::vector<std::vector<size_t>> combinationN2639(const std::vector<size_t> &n, ssize_t k) const;

    /**
     * 追加带牌
     * @param ret 装有追加后结果的二维向量
     * @param combination 参与组合的元素的一维向量
     * @param primal 带牌的牌，即三带二中的三张相同牌，四带同理
     * @param kicker 要带的个数
     * @example [[]], [1,2], [AAA], 1 -> [[AAA1], [AAA2]]
     */
    void withKicker(std::vector<std::vector<size_t>> &ret,
                    const std::vector<size_t> &       combination,
                    const std::vector<size_t> &       primal,
                    ssize_t                           kicker,
                    bool                              filterPairs) const;

    /**
     * 追加带牌，且包含提示中必须包含的牌
     * @param ret 装有追加后结果的二维向量
     * @param combination 参与组合的元素的一维向量
     * @param primal 带牌的牌，即三带二中的三张相同牌，四带同理
     * @param kicker 要带的个数
     */
    void withKickerContainsTarget(std::vector<std::vector<size_t>> &ret,
                                  const std::vector<size_t> &       combination,
                                  const std::vector<size_t> &       primal,
                                  ssize_t                           kicker,
                                  bool                              filterPairs) const;

    /**
     * 将处理过的手牌向量，恢复为未处理过的手牌
     * @param ret 处理过的手牌向量
     * @param ranksMultimap 手牌对应的大小到未处理手牌多重映射
     * @return 未处理过的手牌向量
     */
    std::vector<size_t> restoreHands(const std::vector<size_t> &          ret,
                                     const std::multimap<size_t, size_t> &ranksMultimap) const;

    /**
     * 将处理过的手牌二维向量，恢复为未处理过的手牌二维向量
     * @param ret 处理过的手牌二维向量
     * @param ranksMultimap 手牌对应的大小到未处理手牌多重映射
     * @return 未处理过的手牌二维向量
     */
    std::vector<std::vector<size_t>> restoreHands(const std::vector<std::vector<size_t>> &ret,
                                                  const std::multimap<size_t, size_t> &   ranksMultimap) const;

    /**
     * 是否是同一种牌型
     * @param ranks 手牌对应大小到手牌个数的映射
     * @param category 牌型字符串
     * @return 是否是同一种牌型
     */
    bool isSame(const std::unordered_map<size_t, size_t> &ranks, const std::string &category) const;

    /**
     * 判断传入向量是否连续，向量中元素必须先去重
     * @param vector 待判定向量，装有牌型大小
     * @return 是否连续
     */
    bool isContinuous(const std::vector<size_t> &vector) const;

    /**
     * 是否连续，即满足公差为1的等差数列通项公式
     * @param a_1 首项
     * @param a_n 末项
     * @param n 项数
     * @return 是否连续
     */
    bool isContinuous(size_t a_1, size_t a_n, size_t n) const;

    /**
     * 是否是顺子
     * @param ranks 手牌对应大小到手牌个数的映射
     * @return 是否是顺子
     */
    bool isChain(const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 是否是连对
     * @param ranks 手牌对应大小到手牌个数的映射
     * @return 是否是连对
     */
    bool isPairChain(const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 是否是三顺
     * @param ranks 手牌对应大小到手牌个数的映射
     * @return (是否是三顺，三顺的牌型模型)
     */
    std::tuple<bool, HandsCategoryModel> isTrioChain(const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 获取三顺的权重
     * @param hands 三顺的牌大小组成的向量
     * @param handsCategory 三顺的具体牌型
     * @return 权重
     */
    size_t getTrioChainWeight(const std::vector<size_t> &hands, HandsCategory handsCategory) const;

    /**
     * 使用枚举法计算所有可能的首出组合，符合产品意图
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void enumerate(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的首出单牌，符合产品意图
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void enumerateSolo(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的首出对牌，符合产品意图
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void enumeratePair(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的首出三不带、三带一和三带二，符合产品意图
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void enumerateTrio(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的首出四不带、四带一和四带二，符合产品意图
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void enumerateFour(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的首出顺子，符合产品意图
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void enumerateChain(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的首出连对，符合产品意图
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void enumeratePairChain(std::vector<std::vector<size_t>> &        ret,
                            const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的首出三顺，三顺带一和三顺带二，符合产品意图
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void enumerateTrioChain(std::vector<std::vector<size_t>> &        ret,
                            const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出单牌
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustiveSolo(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出对牌
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustivePair(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出三不带
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustiveTrio(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出三带一
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustiveTrioWithSolo(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出三带二
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustiveTrioWithPair(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出顺子
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustiveChain(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出连对
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustivePairChain(std::vector<std::vector<size_t>> &        ret,
                             const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出三顺不带
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustiveTrioChain(std::vector<std::vector<size_t>> &        ret,
                             const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出三顺带一
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustiveTrioChainWithSolo(std::vector<std::vector<size_t>> &        ret,
                                     const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出三顺带二
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustiveTrioChainWithPair(std::vector<std::vector<size_t>> &        ret,
                                     const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出💣
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustiveBombs(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出四带一
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustiveFourWithSolo(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 使用枚举法计算所有可能的跟出四带二
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void exhaustiveFourWithPair(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 在存放结果的二维向量中追加所有的💣组合
     * @param ret 存放结果的二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void appendBombs(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

public:
    /**
     * 获得首出提示中的某一个组合
     * @param hands 玩家整个手牌
     * @param isStartingHand 是不是起手牌，如果是整个牌局出的第一手牌，需要判定♥️3必出
     * @return 手牌中符合出牌意图的某个组合，也有可能是空
     */
    std::vector<size_t> intention(const std::vector<size_t> &hands, bool isStartingHand = false);

    /**
     * 计算首出提示，获取所有组合
     * @param hands 玩家整个手牌
     * @param isStartingHand 是不是起手牌，如果是整个牌局出的第一手牌，需要判定♥️3必出
     * @return 所有首出提示组合的二维向量
     */
    std::vector<std::vector<size_t>> cardIntentions(const std::vector<size_t> &hands, bool isStartingHand = false);

    /**
     * 获得跟出提示中的某一个组合
     * @param hands 玩家整个手牌
     * @return 手牌中符合出牌意图的某个组合，也有可能是空
     */
    std::vector<size_t> hint(const std::vector<size_t> &hands);

    /**
     * 计算跟出提示，获取所有组合
     * @param hands 玩家整个手牌
     * @return 所有跟出提示组合的二维向量
     */
    std::vector<std::vector<size_t>> cardHint(const std::vector<size_t> &hands);

    /**
     * 计算某一种手牌组合的拆牌数量
     * @param hands 某一种手牌组合
     * @param ranks 手牌对应大小到手牌个数的映射
     * @return 拆牌组合
     */
    size_t getSplitCount(const std::vector<size_t> &hands, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 传入未排序的提示出牌二维向量，根据产品和测试的各种要求进行排序，优先提示那些拆牌少的，带的多的
     * @param ret 排序的提示出牌二维向量
     * @param ranks 手牌对应大小到手牌个数的映射
     */
    void sortHands(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    /**
     * 将装有提示出牌组合的向量设置为仅提示最大的单牌/💣
     * @param hands 玩家的整个手牌
     * @param vector 装有提示出牌组合的二维向量
     * @param iterator 该二维向量对应的迭代器的私有变量
     */
    void setTheHighestSingleCard(const std::vector<size_t> &                 hands,
                                 std::vector<std::vector<size_t>> &          vector,
                                 std::vector<std::vector<size_t>>::iterator &iterator);

    /**
     * 能否出牌
     * @param hands 用户想要打出的牌
     * @param handsCategoryModel 想要打出的牌的具体牌型信息
     * @return 能否打出
     */
    bool canPlay(const std::vector<size_t> &hands, const HandsCategoryModel &handsCategoryModel) const;

    /**
     * 能否跟牌
     * @param hands 用户想跟的牌
     * @return 能否跟牌
     */
    bool canBeat(const std::vector<size_t> &hands) const;

    /**
     * 带牌是否不成对
     * @param handsCategoryModel 牌型模型
     * @param ranks 带判断牌型与数量的映射
     * @return 是否不成对
     */
    bool isKickerRankUnpaired(const HandsCategoryModel &                handsCategoryModel,
                              const std::unordered_map<size_t, size_t> &ranks) const;
};
PAGAMES_WINNER_POKER_END
#endif // PAGAMES_WINNER_JUDGE_H
