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

    return vector;
}

int main()
{
    // std::vector<size_t> hands = { 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7 };
    // std::vector<size_t> hands = { 3, 3, 4, 6, 6, 7, 8, 10, 10, 11, 12, 13, 13, 14, 15 };
    // std::vector<size_t> hands = { 3, 3, 4, 4, 5, 5, 7, 7 };
    // std::vector<size_t> hands = { 3, 4, 5, 5, 5, 5, 7, 7, 8, 9, 10 };

    std::vector<size_t> hands = {
        3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 10, 10, 10, 10, 14, 14, 14
    };

    std::vector<size_t> tv = { 4,4,4 };
    Judge::getInstance().setCurrentHandsCategory(test(tv));

    /*
   auto              ret = Judge::getInstance().cardIntentions(test(hands));
   std::stringstream ss;

   for (auto &&item : ret)
   {
       ss.str("");

       ss << "{ ";
       for (auto &&element : item)
       {
           ss << element << ", ";
       }
       ss.seekp(-2, ss.end);
       ss << " }" << std::endl;

       std::cout << ss.str();
   }

   for (auto &&item : ret)
   {
       ss.str("");

       ss << "{ ";
       for (auto &&element : item)
       {
           ss << (element >> 8) << ", ";
       }
       ss.seekp(-2, ss.end);
       ss << " }" << std::endl;

       std::cout << ss.str();
   }
  */

    for (int i = 0; i < 24; ++i)
    {
        const auto &ret = Judge::getInstance().hint(test(hands));

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
