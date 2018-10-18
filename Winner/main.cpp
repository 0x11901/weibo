#include "Judge.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace WinnerPoker;

std::vector<size_t> test(const std::vector<size_t> &t)
{
    std::stringstream ss;
    ss << "{ ";
    for (auto &&item : t)
    {
        ss << item << ", ";
    }
    ss.seekp(-2, ss.end);
    ss << " }" << std::endl;

    // std::cout << ss.str();

    std::vector<size_t> vector;
    auto                zip = Judge::getInstance().zip(t);
    for (auto &&x : zip)
    {
        for (int i = 1; i <= x.second; ++i)
        {
            vector.push_back((x.first << 8) | i);
        }
    }

    ss.str("");
    ss << "{ ";
    for (auto &&item : vector)
    {
        ss << item << ", ";
    }
    ss.seekp(-2, ss.end);
    ss << " }" << std::endl;

    // std::cout << ss.str();
    // std::cout << std::endl;
    return vector;
}

int main()
{
    // std::vector<size_t> tv = {};
    // Judge::getInstance().setCurrentHandsCategory(test(tv), test(tv));
    //
    // std::vector<size_t> hands;
    // hands = { 3, 3, 3, 4, 5, 5, 5, 6, 6, 7 };
    // hands = { 3, 3, 3, 4, 4, 5, 5, 7 };
    // hands = { 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7 };
    // hands = { 3, 3, 5, 6, 6, 7, 7, 8 };
    // hands = { 3, 4, 4, 5, 5, 6, 6, 7 };
    // hands = { 3, 4, 5, 5, 6, 6, 7 };
    // hands = { 3, 4, 5, 5, 5, 6, 6 };
    // hands = { 3, 4, 5, 5, 6, 6, 7 };
    // hands = { 3, 4, 4, 5, 5, 5, 6, 6, 7 };
    // hands = { 3, 4, 5, 5, 5, 6, 6 };
    // hands = { 3, 3, 3, 4, 4, 5, 5, 5, 6, 6 };
    // hands = { 3, 3, 4, 4, 5, 5, 5, 6 };
    // hands = { 3, 3, 3, 5, 14, 14, 14 };
    // hands = { 3, 3, 3, 14, 14, 14 };
    // hands = { 3, 3, 4, 4, 4, 5, 5, 5, 5, 6 };
    // hands = { 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 8 };
    // hands = { 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 7 };
    // hands = { 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 8 };
    // hands = { 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 7 };
    // hands = { 3, 4, 4, 5, 5, 5, 5, 6 };
    // hands = { 3, 4, 4, 5, 5, 5, 5, 6, 6 };
    // hands = { 4, 5, 6, 6, 7, 8, 8, 8, 9, 9, 9, 12, 12, 12, 12 };
    // hands = { 3, 3, 3, 4, 5, 5, 5, 6, 6, 7 };
    // hands = { 3, 4, 4, 5, 5, 5, 6 };
    // hands = { 3, 4, 4, 5, 5, 5, 5, 6 };
    // hands = { 3, 3, 3, 8, 8, 8, 5, 5, 5, 5, 10, 11, 12, 13, 14, 14, 14 };
    // hands = { 3, 4, 5, 10, 10, 10, 10, 9, 9 };
    // hands = { 13, 12, 11, 11, 11, 10, 10, 10, 10, 8, 7, 6, 5, 5, 4, 3, 3 };
    // hands = { 13, 12, 11, 11, 10, 10, 10, 10, 8, 7, 6, 5, 5, 5, 5, 4, 3, 3 };
    // hands = { 13, 13, 11, 10, 8, 7, 6, 6, 6, 4, 4, 3, 3, 3, 3 };
    // hands = { 6, 6, 6, 11, 4, 4, 4, 4 };
    // hands = { 6, 6, 6, 7, 7, 7, 5, 5, 5, 4 };
    // hands = { 14, 14, 14, 8, 8, 10, 11, 12, 13, 7, 6, 5, 5, 4, 3 };
    // hands = { 13, 13, 11, 10, 8, 7, 6, 6, 6, 4, 4, 3, 3, 3, 3 };
    // hands = { 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 15 };
    // hands = { 9, 9, 8, 8, 8, 8, 11, 11 };
    // hands = { 15, 14, 13, 13, 12, 11, 8, 8, 8, 7, 7, 7, 7, 6, 6, 3 };
    // hands = { 3, 3, 3, 3, 4, 4, 4, 4, 5, 9, 9, 9, 9, 5, 5, 5, 6, 6, 6, 6 };
    //
    // hands = { 3, 4, 5, 6, 8, 8, 8 };
    // hands = { 3, 3, 4, 5, 6, 8, 8, 8 };
    // hands = { 3, 3, 4, 5, 6, 8, 8, 8, 9, 9, 9 };
    // hands = { 3, 3, 4, 5, 6, 7, 8, 8, 8, 9, 9, 9 };
    // hands = { 3, 4, 5, 6, 7, 8, 8, 8, 9, 9, 9 };
    //
    // auto              ret = Judge::getInstance().cardIntentions(test(hands));
    // std::stringstream ss;
    //
    // for (auto &&item : ret)
    // {
    //     ss.str("");
    //
    //     ss << "{ ";
    //     for (auto &&element : item)
    //     {
    //         ss << (element >> 8) << ", ";
    //     }
    //     ss.seekp(-2, ss.end);
    //     ss << " }" << std::endl;
    //
    //     std::cout << ss.str();
    // }

#pragma mark - 跟牌提示
    // std::vector<size_t> tv;
    // tv = { 3, 3, 3, 4, 4 };
    // Judge::getInstance().setCurrentHandsCategory(test(tv), test(tv));
    //
    // std::vector<size_t> hands;
    // hands = { 12, 12, 12, 12, 4, 5, 6, 7, 7, 7 };
    // hands = { 3, 4, 5, 6, 8, 8, 8, 13, 13 };
    //
    // auto              ret = Judge::getInstance().cardHint(test(hands));
    // std::stringstream ss;
    //
    // for (auto &&item : ret)
    // {
    //     ss.str("");
    //
    //     ss << "{ ";
    //     for (auto &&element : item)
    //     {
    //         ss << element << ", ";
    //     }
    //     ss.seekp(-2, ss.end);
    //     ss << " }" << std::endl;
    //
    //     // std::cout << ss.str();
    // }
    //
    // for (auto &&item : ret)
    // {
    //     ss.str("");
    //
    //     ss << "{ ";
    //     for (auto &&element : item)
    //     {
    //         ss << (element >> 8) << ", ";
    //     }
    //     ss.seekp(-2, ss.end);
    //     ss << " }" << std::endl;
    //
    //     std::cout << ss.str();
    // }
    //
    // for (int i = 0; i < 1; ++i)
    // {
    //     const auto &ret = Judge::getInstance().intentions(test(hands));
    //
    //     std::stringstream ss;
    //
    //     ss.str("");
    //     ss << "{ ";
    //     for (auto &&item : ret)
    //     {
    //         ss << (item >> 8) << ", ";
    //     }
    //     ss.seekp(-2, ss.end);
    //     ss << " }" << std::endl;
    //
    //     // std::cout << ss.str();
    // }

#pragma mark - 测试重排手牌
    // // std::stringstream   ss;
    // std::vector<size_t> weight        = {};
    // std::vector<size_t> handsCategory = { 3, 3, 3, 4, 4 };
    // handsCategory                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 7, 8, 9, 7, 9 };
    // handsCategory                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7 };
    // handsCategory                     = {};
    //
    // std::vector<size_t> hand = { 3 };
    // hand                     = { 3, 3 };
    // hand                     = { 3, 3, 3 };
    // hand                     = { 3, 3, 3, 3 };
    // hand                     = { 3, 3, 4, 3 };
    // hand                     = { 6, 7, 6, 4, 6 };
    // hand                     = { 6, 7, 6, 4, 6, 6 };
    // hand                     = { 3, 3, 3, 3, 4, 4, 4, 4 };
    // hand                     = { 8, 8, 8, 8, 4 };
    // hand                     = { 14, 14, 14, 13, 13, 13, 12, 12, 12, 4, 4, 3, 3, 3, 3 };
    // hand                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 5 };
    // hand                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7 };
    // hand                     = { 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 };

    // Judge::getInstance().setCurrentHandsCategory(test(weight), test(handsCategory));
    // auto r = Judge::getInstance().rearrangeHands(test(hand));

    // ss.str("");
    // ss << "{ ";
    // for (auto &&item : r)
    // {
    //     ss << (item >> 8) << ", ";
    // }
    // ss.seekp(-2, ss.end);
    // ss << " }" << std::endl;

    // std::cout << ss.str();

#pragma mark - 测试can play
    std::stringstream   ss;
    std::vector<size_t> weight        = {};
    std::vector<size_t> handsCategory = {};

    std::vector<size_t> hands = { 3, 3, 3, 4, 4, 4, 5, 5, 5,8 };

    Judge::getInstance().setCurrentHandsCategory(test(weight), test(handsCategory));
    auto can = Judge::getInstance().canPlay(test(hands));

    std::cout << can << std::endl;

#pragma mark - 测试is pass
    // std::stringstream   ss;
    // std::vector<size_t> weight        = {};
    // std::vector<size_t> handsCategory = {};
    // weight                            = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9 };
    // handsCategory                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9 };
    // weight                            = { 3, 3, 3, 4, 4 };
    // handsCategory                     = { 3, 3, 3, 4, 4 };
    // weight                            = { 3, 3, 3, 4, 4, 4, 5, 6 };
    // handsCategory                     = { 3, 3, 3, 4, 4, 4, 5, 6 };
    //
    // std::vector<size_t> hands = {};
    // hands                     = { 6, 6, 6, 7, 7, 7, 8, 9, 3, 4 };
    //
    // Judge::getInstance().setCurrentHandsCategory(test(weight), test(handsCategory));
    // auto pass = Judge::getInstance().isPass(test(hands));
    // std::cout << pass << std::endl;

    return 0;
}

// #include <algorithm>
// #include <chrono>
// #include <iomanip>
// #include <iostream>
// #include <set>
// #include <sstream>
// #include <vector>
//
// std::vector<std::vector<size_t>> combination(const std::vector<size_t> &n, ssize_t k);
// void combination(int arr[], int m, int n, int out[], int outL, std::vector<std::vector<int>> &vec);
//
// int main()
// {
//     std::vector<size_t> vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
//
//     std::clock_t c_start, c_end;
//
//     c_start = std::clock();
//     combination(vector, 4);
//     c_end = std::clock();
//     std::cout << std::fixed << std::setprecision(2) << "time used: " << c_end - c_start << std::endl;
//
//     const auto &ret = combination(vector, 2);
//
//     std::stringstream ss;
//     for (auto &&item : ret)
//     {
//         ss << "{ ";
//         for (auto &&n : item)
//         {
//             ss << n << ", ";
//         }
//         ss.seekp(-2, ss.end);
//         ss << " }" << std::endl;
//     }
//
//     std::cout << ss.str() << std::endl;
//
//     auto                          lineL = 2;
//     auto *                        out   = new int[lineL];
//     int                           arr[] = { 1, 2, 3, 4, 1 };
//     std::vector<std::vector<int>> indexVec;
//
//     c_start = std::clock();
//     combination(arr, sizeof(arr) / sizeof(int), lineL, out, lineL, indexVec);
//     c_end = std::clock();
//     std::cout << std::fixed << std::setprecision(2) << "time used: " << c_end - c_start << std::endl;
//
//     ss.str("");
//     for (auto &&item : indexVec)
//     {
//         ss << "{ ";
//         for (auto &&n : item)
//         {
//             ss << n << ", ";
//         }
//         ss.seekp(-2, ss.end);
//         ss << " }" << std::endl;
//     }
//
//     std::cout << ss.str() << std::endl;
//
//     return 0;
// }
//
// std::vector<std::vector<size_t>> combination(const std::vector<size_t> &n, ssize_t k)
// {
//     std::vector<std::vector<size_t>> ret;
//
//     if (n.empty() || k > n.size()) return ret;
//
//     auto copy = n;
//     std::sort(copy.begin(), copy.end());
//
//     std::vector<std::vector<size_t>> node(1);
//     auto                             last = copy[0];
//     ssize_t                          flag = 1;
//
//     for (const auto &i : copy)
//     {
//         if (i != last)
//         {
//             last = i;
//             flag = node.size();
//         }
//
//         ssize_t size = node.size();
//         for (ssize_t j = size - 1; j >= size - flag; j--)
//         {
//             if (node[j].size() <= k)
//             {
//                 node.push_back(node[j]);
//             }
//             else
//             {
//                 continue;
//             }
//
//             node.back().push_back(i);
//             if (node.back().size() == k)
//             {
//                 const auto &temp = node.back();
//                 // OPTIMIZE: 应用回溯法优化
//                 if (std::find_if(ret.begin(), ret.end(), [&temp](std::vector<size_t> i) -> bool { return i ==
// temp;
//                 })
//                     == ret.end())
//                 {
//                     ret.push_back(node.back());
//                 }
//             }
//         }
//     }
//
//     return ret;
// }
//
// void combination(int arr[], int m, int n, int out[], int outL, std::vector<std::vector<int>> &vec)
// {
//     if (n == 0)
//     {
//         std::vector<int> temp;
//         for (int i = 0; i < outL; i++)
//         {
//             temp.push_back(out[i]);
//         }
//         vec.push_back(temp);
//         return;
//     }
//
//     for (int i = m; i >= n; --i)
//     {
//         out[n - 1] = arr[i - 1];
//         combination(arr, i - 1, n - 1, out, outL, vec);
//     }
// }
