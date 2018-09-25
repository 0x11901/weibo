// #include "Judge.h"
// #include <iostream>
// #include <sstream>
// #include <unordered_map>
// #include <unordered_set>
// #include <vector>
//
// using namespace WinnerPoker;
//
// std::vector<size_t> test(const std::vector<size_t> &t)
// {
//     std::stringstream ss;
//     ss << "{ ";
//     for (auto &&item : t)
//     {
//         ss << item << ", ";
//     }
//     ss.seekp(-2, ss.end);
//     ss << " }" << std::endl;
//
//     // std::cout << ss.str();
//
//     std::vector<size_t> vector;
//     auto                zip = Judge::getInstance().zip(t);
//     for (auto &&x : zip)
//     {
//         for (int i = 1; i <= x.second; ++i)
//         {
//             vector.push_back((x.first << 8) | i);
//         }
//     }
//
//     ss.str("");
//     ss << "{ ";
//     for (auto &&item : vector)
//     {
//         ss << item << ", ";
//     }
//     ss.seekp(-2, ss.end);
//     ss << " }" << std::endl;
//
//     // std::cout << ss.str();
//     // std::cout << std::endl;
//     return vector;
// }
//
// int main()
// {
//
//     // std::vector<size_t> hands = { 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7 };
//     // std::vector<size_t> hands = { 3, 3, 4, 6, 6, 7, 8, 10, 10, 11, 12, 13, 13, 14, 15 };
//     // std::vector<size_t> hands = { 3, 3, 4, 4, 5, 5, 7, 7 };
//     // std::vector<size_t> hands = { 3, 4, 5, 5, 5, 5, 7, 7, 8, 9, 10 };
//
//     // std::vector<size_t> hands = { 3, 3, 3, 3, 4,  4,  4,  4,  5,  5,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7,
//     //                               8, 8, 9, 9, 10, 10, 10, 10, 11, 11, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15 };
//
//     std::vector<size_t> tv = { 3, 3, 3, 4, 4, 4, 4, 5 };
//     tv                     = { 3, 3, 3, 5, 4 };
//     tv                     = { 4, 4, 4, 3, 3, 3, 9, 9 };
//     tv                     = {};
//
//     Judge::getInstance().setCurrentHandsCategory(test(tv), test(tv));
//
//     // std::cout << Judge::getInstance().isPass(test(hands)) << std::endl;
//
//     std::vector<size_t> hands;
//     hands = { 3, 3, 3, 4, 5, 5, 5, 6, 6, 7 };
//     hands = { 3, 3, 3, 4, 4, 5, 5, 7 };
//     hands = { 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7 };
//     hands = { 3, 3, 5, 6, 6, 7, 7, 8 };
//     hands = { 3, 4, 4, 5, 5, 6, 6, 7 };
//     hands = { 3, 4, 5, 5, 6, 6, 7 };
//     hands = { 3, 4, 5, 5, 5, 6, 6 };
//     hands = { 3, 4, 5, 5, 6, 6, 7 };
//     hands = { 3, 4, 4, 5, 5, 5, 6, 6, 7 };
//     hands = { 3, 4, 5, 5, 5, 6, 6 };
//     hands = { 3, 3, 3, 4, 4, 5, 5, 5, 6, 6 };
//     hands = { 3, 3, 4, 4, 5, 5, 5, 6 };
//     hands = { 3, 3, 3, 5, 14, 14, 14 };
//     hands = { 3, 3, 3, 14, 14, 14 };
//     hands = { 3, 3, 4, 4, 4, 5, 5, 5, 5, 6 };
//     hands = { 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 8 };
//     hands = { 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 7 };
//     hands = { 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 8 };
//     hands = { 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 7 };
//     hands = { 3, 4, 4, 5, 5, 5, 5, 6 };
//     hands = { 3, 4, 4, 5, 5, 5, 5, 6, 6 };
//     hands = { 4, 5, 6, 6, 7, 8, 8, 8, 9, 9, 9, 12, 12, 12, 12 };
//     hands = { 3, 3, 3, 4, 5, 5, 5, 6, 6, 7 };
//     hands = { 3, 4, 4, 5, 5, 5, 6 };
//     hands = { 3, 4, 4, 5, 5, 5, 5, 6 };
//     hands = { 3, 3, 3, 8, 8, 8, 5, 5, 5, 5, 10, 11, 12, 13, 14, 14, 14 };
//     hands = { 3, 4, 5, 10, 10, 10, 10, 9, 9 };
//     hands = { 13, 12, 11, 11, 11, 10, 10, 10, 10, 8, 7, 6, 5, 5, 4, 3, 3 };
//     hands = { 13, 12, 11, 11, 10, 10, 10, 10, 8, 7, 6, 5, 5, 5, 5, 4, 3, 3 };
//     hands = { 13, 13, 11, 10, 8, 7, 6, 6, 6, 4, 4, 3, 3, 3, 3 };
//     hands = { 6, 6, 6, 11, 4, 4, 4, 4 };
//     hands = { 6, 6, 6, 7, 7, 7, 5, 5, 5, 4 };
//     hands = { 14, 14, 14, 8, 8, 10, 11, 12, 13, 7, 6, 5, 5, 4, 3 };
//     hands = { 13, 13, 11, 10, 8, 7, 6, 6, 6, 4, 4, 3, 3, 3, 3 };
//     hands = { 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 15 };
//     hands = { 9, 9, 8, 8, 8, 8, 11, 11 };
//     hands = { 15, 14, 13, 13, 12, 11, 8, 8, 8, 7, 7, 7, 7, 6, 6, 3 };
//     hands = { 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6 };
//
// #pragma mark - 跟牌提示
//     // std::vector<size_t> hands;
//     // hands = { 4, 4, 4, 5, 6, 6, 7, 7, 7, 15 };
//     // hands = { 6, 6, 6, 6, 7, 7, 8, 8 };
//     // hands = { 5, 5, 5, 6, 6, 7, 7, 8, 8, 8, 15 };
//     // hands = { 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 15 };
//     // hands = { 3, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8 };
//     // hands = { 3, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8 };
//     // hands = { 4, 5, 6, 6, 7, 8, 9, 10, 10, 11, 11, 12 };
//     // hands = { 5, 5, 5, 5, 3 };
//     // hands = { 12, 12, 12, 12 };
//     //
//     // std::vector<size_t> tv;
//     // tv = { 3 };
//     // tv = { 3, 3, 3, 4, 5 };
//     // tv = { 3, 3, 3, 4, 5 };
//     // tv = { 3, 3, 4, 4 };
//     // tv = { 3 };
//     // tv = { 3, 3, 3, 4, 4, 4, 5, 12, 12, 12 };
//     // tv = { 3, 4, 5, 6, 7 };
//     // tv = { 3, 3, 3, 4 };
//     // tv = { 14 };
//     // Judge::getInstance().setCurrentHandsCategory(test(tv));
//     //玩家A出33344，玩家B手上有345888899点击提示，88883，提示不了8888
//     auto              ret = Judge::getInstance().cardIntentions(test(hands));
//     std::stringstream ss;
//
//     // for (auto &&item : ret)
//     // {
//     //     ss.str("");
//     //
//     //     ss << "{ ";
//     //     for (auto &&element : item)
//     //     {
//     //         ss << element << ", ";
//     //     }
//     //     ss.seekp(-2, ss.end);
//     //     ss << " }" << std::endl;
//     //
//     //     std::cout << ss.str();
//     // }
//
//     for (auto &&item : ret)
//     {
//         ss.str("");
//
//         ss << "{ ";
//         for (auto &&element : item)
//         {
//             ss << (element >> 8) << ", ";
//         }
//         ss.seekp(-2, ss.end);
//         ss << " }" << std::endl;
//
//         std::cout << ss.str();
//     }
//
//     // for (int i = 0; i < 1; ++i)
//     // {
//     //     const auto &ret = Judge::getInstance().intentions(test(hands));
//     //
//     //     std::stringstream ss;
//     //
//     //     ss.str("");
//     //     ss << "{ ";
//     //     for (auto &&item : ret)
//     //     {
//     //         ss << (item >> 8) << ", ";
//     //     }
//     //     ss.seekp(-2, ss.end);
//     //     ss << " }" << std::endl;
//     //
//     //     std::cout << ss.str();
//     // }
//
// #pragma mark - 测试重排手牌
//     // // std::stringstream   ss;
//     // std::vector<size_t> weight        = {};
//     // std::vector<size_t> handsCategory = { 3, 3, 3, 4, 4 };
//     // handsCategory                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 7, 8, 9, 7, 9 };
//     // handsCategory                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7 };
//     // handsCategory                     = {};
//     //
//     // std::vector<size_t> hand = { 3 };
//     // hand                     = { 3, 3 };
//     // hand                     = { 3, 3, 3 };
//     // hand                     = { 3, 3, 3, 3 };
//     // hand                     = { 3, 3, 4, 3 };
//     // hand                     = { 6, 7, 6, 4, 6 };
//     // hand                     = { 6, 7, 6, 4, 6, 6 };
//     // hand                     = { 3, 3, 3, 3, 4, 4, 4, 4 };
//     // hand                     = { 8, 8, 8, 8, 4 };
//     // hand                     = { 14, 14, 14, 13, 13, 13, 12, 12, 12, 4, 4, 3, 3, 3, 3 };
//     // hand                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 5 };
//     // hand                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7 };
//     // hand                     = { 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 };
//
//     // Judge::getInstance().setCurrentHandsCategory(test(weight), test(handsCategory));
//     // auto r = Judge::getInstance().rearrangeHands(test(hand));
//
//     // ss.str("");
//     // ss << "{ ";
//     // for (auto &&item : r)
//     // {
//     //     ss << (item >> 8) << ", ";
//     // }
//     // ss.seekp(-2, ss.end);
//     // ss << " }" << std::endl;
//
//     // std::cout << ss.str();
//
// #pragma mark - 测试can play
//     // std::stringstream   ss;
//     // std::vector<size_t> weight        = {};
//     // std::vector<size_t> handsCategory = {};
//     // weight                            = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9 };
//     // handsCategory                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9 };
//     // weight                            = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 11, 12, 13 };
//     // handsCategory                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 11, 12, 13 };
//     //
//     // std::vector<size_t> hands = {};
//     // hands                     = { 3, 3 };
//     // hands                     = { 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 13 };
//     // hands                     = { 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10 };
//     //
//     // Judge::getInstance().setCurrentHandsCategory(test(weight), test(handsCategory));
//     // auto can = Judge::getInstance().canPlay(test(hands));
//     //
//     // std::cout << can << std::endl;
//
// #pragma mark - 测试is pass
//     // std::stringstream   ss;
//     // std::vector<size_t> weight        = {};
//     // std::vector<size_t> handsCategory = {};
//     // weight                            = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9 };
//     // handsCategory                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9 };
//     // weight                            = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 11, 12, 13 };
//     // handsCategory                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 11, 12, 13 };
//     // weight                            = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7 };
//     // handsCategory                     = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7 };
//     //
//     // std::vector<size_t> hands = {};
//     // hands                     = { 3, 3 };
//     // hands                     = { 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 13 };
//     // hands                     = { 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10 };
//     // hands                     = { 3, 4, 5, 6, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 12 };
//     //
//     // Judge::getInstance().setCurrentHandsCategory(test(weight), test(handsCategory));
//     // auto pass = Judge::getInstance().isPass(test(hands));
//     // std::cout << pass << std::endl;
//
//     return 0;
// }
//
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

std::vector<std::vector<size_t>> combination(const std::vector<size_t> &n, ssize_t k);
void combination(int arr[], int m, int n, int out[], int outL, std::vector<std::vector<int>> &vec);
long GetCombinations(std::vector<size_t> nums);
std::vector<std::vector<size_t>> newCombination(const std::vector<size_t> &n, ssize_t k);
template <class RandIt> bool     next_k_permutation(RandIt first, RandIt mid, RandIt last)
{
    typedef typename std::iterator_traits<RandIt>::value_type value_type;

    std::sort(mid, last, std::greater<value_type>());
    return std::next_permutation(first, last);
}

void t()
{
    std::string sentence1 = " 2 ";
    std::string sentence2 = " 2 ";
    std::string sentence3 = " 3 ";
    std::string sentence4 = " 4 ";

    // Store all the elements in a container ( here a std::vector)
    std::vector<std::string> myVectorOfStrings;
    // In the vector we add all the sentences.
    // Note : It is possible to do myVectorOfStrings.push_back("Some sentence");
    myVectorOfStrings.push_back(sentence1);
    myVectorOfStrings.push_back(sentence2);
    myVectorOfStrings.push_back(sentence3);
    myVectorOfStrings.push_back(sentence4);

    // The elements must be sorted to output all the combinations
    std::sort(myVectorOfStrings.begin(), myVectorOfStrings.end());

    auto r = myVectorOfStrings.begin() + 2;

    std::cout << "The 4! possible permutations with 4 elements:\n";
    do
    {
        // This printing can be improved to handle any number of sentences, not only four.
        std::cout << myVectorOfStrings[0] << ' ' << myVectorOfStrings[1] << ' ' << myVectorOfStrings[2] << ' '
                  << myVectorOfStrings[3] << '\n';
    } while (next_k_permutation(myVectorOfStrings.begin(), r, myVectorOfStrings.end()));

    std::cout << "After loop: " << myVectorOfStrings[0] << ' ' << myVectorOfStrings[1] << ' ' << myVectorOfStrings[2]
              << ' ' << myVectorOfStrings[3] << '\n';
}

int main()
{
    t();
    std::vector<size_t> vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    std::clock_t c_start, c_end;

    c_start = std::clock();
    combination(vector, 4);
    c_end = std::clock();
    std::cout << std::fixed << std::setprecision(2) << "time used: " << c_end - c_start << std::endl;

    const auto &ret = combination(vector, 2);

    std::stringstream ss;
    for (auto &&item : ret)
    {
        ss << "{ ";
        for (auto &&n : item)
        {
            ss << n << ", ";
        }
        ss.seekp(-2, ss.end);
        ss << " }" << std::endl;
    }

    std::cout << ss.str() << std::endl;

    auto                          lineL = 2;
    auto *                        out   = new int[lineL];
    int                           arr[] = { 1, 2, 3, 4, 1 };
    std::vector<std::vector<int>> indexVec;

    c_start = std::clock();
    combination(arr, sizeof(arr) / sizeof(int), lineL, out, lineL, indexVec);
    c_end = std::clock();
    std::cout << std::fixed << std::setprecision(2) << "time used: " << c_end - c_start << std::endl;

    ss.str("");
    for (auto &&item : indexVec)
    {
        ss << "{ ";
        for (auto &&n : item)
        {
            ss << n << ", ";
        }
        ss.seekp(-2, ss.end);
        ss << " }" << std::endl;
    }

    std::cout << ss.str() << std::endl;

    auto count = GetCombinations(vector);
    std::cout << count << std::endl;
    return 0;
}

std::vector<std::vector<size_t>> combination(const std::vector<size_t> &n, ssize_t k)
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
            if (node[j].size() <= k)
            {
                node.push_back(node[j]);
            }
            else
            {
                continue;
            }

            node.back().push_back(i);
            if (node.back().size() == k)
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

void combination(int arr[], int m, int n, int out[], int outL, std::vector<std::vector<int>> &vec)
{
    if (n == 0)
    {
        std::vector<int> temp;
        for (int i = 0; i < outL; i++)
        {
            temp.push_back(out[i]);
        }
        vec.push_back(temp);
        return;
    }

    for (int i = m; i >= n; --i)
    {
        out[n - 1] = arr[i - 1];
        combination(arr, i - 1, n - 1, out, outL, vec);
    }
}

long GetCombinations(std::vector<size_t> nums)
{
    unsigned long int n         = nums.size();
    unsigned long int n2        = n / 2;
    unsigned long int numUnique = 1;
    unsigned long int numCombinations;

    std::sort(nums.begin(), nums.end());
    std::vector<int> numReps;

    size_t testVal = nums[0];
    numReps.push_back(1);

    for (std::size_t i = 1; i < n; ++i)
    {
        if (nums[i] != testVal)
        {
            numReps.push_back(1);
            testVal = nums[i];
            ++numUnique;
        }
        else
        {
            ++numReps[numUnique - 1];
        }
    }

    int                 myMax, r = n2 + 1;
    std::vector<size_t> triangleVec(r);
    std::vector<size_t> temp(r);
    size_t              tempSum;

    myMax = r;
    if (myMax > numReps[0] + 1) myMax = numReps[0] + 1;

    for (int i = 0; i < myMax; ++i)
        triangleVec[i] = 1;

    temp = triangleVec;

    for (std::size_t k = 1; k < numUnique; ++k)
    {
        for (int i = n2; i > 0; --i)
        {
            myMax = i - numReps[k];
            if (myMax < 0) myMax = 0;

            tempSum = 0;
            for (int j = myMax; j <= i; ++j)
                tempSum += triangleVec[j];

            temp[i] = tempSum;
        }
        triangleVec = temp;
    }

    numCombinations = (size_t)triangleVec[n2];

    return numCombinations;
}