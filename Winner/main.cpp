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

enum class E : int
{
    a = 1,
    b = 2,
    c = 3,
    d = 4,
    e = 5,
    f,
    g
};

int main()
{
    auto e = E(-1);
    auto e1 = E(1);

    std::cout << static_cast<int>(e) << std::endl;
    
    /*
    // std::vector<size_t> hands = { 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7 };
    // std::vector<size_t> hands = { 3, 3, 4, 6, 6, 7, 8, 10, 10, 11, 12, 13, 13, 14, 15 };
    // std::vector<size_t> hands = { 3, 3, 4, 4, 5, 5, 7, 7 };
    // std::vector<size_t> hands = { 3, 4, 5, 5, 5, 5, 7, 7, 8, 9, 10 };

    // std::vector<size_t> hands = { 3, 3, 3, 3, 4,  4,  4,  4,  5,  5,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7,
    //                               8, 8, 9, 9, 10, 10, 10, 10, 11, 11, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15 };

    std::vector<size_t> tv = { 3, 3, 3, 4, 4, 4, 4, 5 };
    // std::vector<size_t> tv = { 3, 3, 3, 4, 4, 4 };
    tv = {};
    Judge::getInstance().setCurrentHandsCategory(test(tv));

    // std::cout << Judge::getInstance().isPass(test(hands)) << std::endl;

    std::vector<size_t> hands;
    hands = { 3, 3, 3, 4, 5, 5, 5, 6, 6, 7 };
    hands = { 3, 3, 3, 4, 4, 5, 5, 7 };
    hands = { 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7 };
    hands = { 3, 3, 5, 6, 6, 7, 7, 8 };
    hands = { 3, 4, 4, 5, 5, 6, 6, 7 };
    hands = { 3, 4, 5, 5, 6, 6, 7 };
    hands = { 3, 4, 5, 5, 5, 6, 6 };
    hands = { 3, 4, 5, 5, 6, 6, 7 };
    hands = { 3, 4, 4, 5, 5, 5, 6, 6, 7 };
    hands = { 3, 4, 5, 5, 5, 6, 6 };
    hands = { 3, 3, 3, 4, 4, 5, 5, 5, 6, 6 };
    hands = { 3, 3, 4, 4, 5, 5, 5, 6 };
    hands = { 3, 3, 3, 5, 14, 14, 14 };
    hands = { 3, 3, 3, 14, 14, 14 };
    hands = { 3, 3, 4, 4, 4, 5, 5, 5, 5, 6 };
    hands = { 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 8 };
    hands = { 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 7 };
    hands = { 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 8 };
    hands = { 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 7 };
    hands = { 3, 4, 4, 5, 5, 5, 5, 6 };
    hands = { 3, 4, 4, 5, 5, 5, 5, 6, 6 };
    hands = { 4, 5, 6, 6, 7, 8, 8, 8, 9, 9, 9, 12, 12, 12, 12 };
*/

#pragma mark - 跟牌提示
    std::vector<size_t> hands;
    hands = { 4, 4, 4, 5, 6, 6, 7, 7, 7, 15 };
    hands = { 6, 6, 6, 6, 7, 7, 8, 8 };
    hands = { 5, 5, 5, 6, 6, 7, 7, 8, 8, 8, 15 };
    hands = { 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 15 };
    hands = { 3, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8 };
    hands = { 3, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8 };
    hands = { 4, 5, 6, 6, 7, 8, 9, 10, 10, 11, 11, 12 };
    hands = { 5, 5, 5, 5, 3 };
    hands = { 12, 12, 12, 12 };

    std::vector<size_t> tv;
    tv = { 3 };
    tv = { 3, 3, 3, 4, 5 };
    tv = { 3, 3, 3, 4, 5 };
    tv = { 3, 3, 4, 4 };
    tv = { 3 };
    tv = { 3, 3, 3, 4, 4, 4, 5, 12, 12, 12 };
    tv = { 3, 4, 5, 6, 7 };
    tv = { 3, 3, 3, 4 };
    tv = { 14 };

    Judge::getInstance().setCurrentHandsCategory(test(tv));

    // auto              ret = Judge::getInstance().intentions(test(hands));
    // std::stringstream ss;

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

    for (int i = 0; i < 1; ++i)
    {
        const auto &ret = Judge::getInstance().intentions(test(hands));

        std::stringstream ss;

        ss.str("");
        ss << "{ ";
        for (auto &&item : ret)
        {
            ss << (item >> 8) << ", ";
        }
        ss.seekp(-2, ss.end);
        ss << " }" << std::endl;

        std::cout << ss.str();
    }

    return 0;
}
