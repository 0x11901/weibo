#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

std::vector<std::vector<size_t>> combination(const std::vector<size_t> &n, ssize_t k);
void combination(int arr[], int m, int n, int out[], int outL, std::vector<std::vector<int>> &vec);

int main()
{
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