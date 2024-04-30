#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

int findBin(int x, const std::vector<int>& array)
{
    int ind = 0, left = 0, right = array.size() - 1;
    if (x < array[0] || x > array[array.size() - 1])
        return -1;

    while (left <= right)
    {
        int mid = (left + right) / 2;
        if (array[mid] == x || (array[mid] < x && x < array[mid+1]))
        {
            ind = mid;
            break;
        }
        else if (array[mid] > x)
            right = mid - 1;
        else if (array[mid] < x)
            left = mid + 1;
    }

    return ind;
}

int main()
{
    int n, m;

    std::cin >> n;
    std::set<int> ySort;
    std::set<int> xSort;
    std::vector<std::vector<int>> Points(n, std::vector<int>(4));
    for (int i = 0; i < n; i++)
    {
        std::cin >> Points[i][0] >> Points[i][1] >> Points[i][2] >> Points[i][3];
        xSort.insert(Points[i][0]);
        xSort.insert(Points[i][2]);
        ySort.insert(Points[i][1]);
        ySort.insert(Points[i][3]);
    }

    std::vector<std::vector<int>> matrix(xSort.size(), std::vector<int>(ySort.size()));
    std::vector<int> arr_1(xSort.begin(), xSort.end());
    std::vector<int> arr_2(ySort.begin(), ySort.end());

    std::vector<int>::iterator itr;

    for (int i = 0; i < n; i++)
    {
        for (int j = std::distance(xSort.begin(), std::find(xSort.begin(), xSort.end(), Points[i][0])); j < std::distance(xSort.begin(), std::find(xSort.begin(), xSort.end(), Points[i][2])); j++)
        {
            for (int g = std::distance(ySort.begin(), std::find(ySort.begin(), ySort.end(), Points[i][1])); g < std::distance(ySort.begin(), std::find(ySort.begin(), ySort.end(), Points[i][3])); g++)
            {
                matrix[j][g]++;
            }
        }
    }

    std::cin >> m;
    int x, y, new_x, new_y;
    std::vector<int> result;
    for (int i = 0; i < m; i++)
    {
        std::cin >> x >> y;

        if (n > 0)
        {
            new_x = findBin(x, arr_1);
            new_y = findBin(y, arr_2);
            if (new_x != -1 && new_y != -1)
                result.push_back(matrix[new_x][new_y]);
            else
                result.push_back(0);
        }
        else
            result.push_back(0);
    }

    for (const auto& elem : result)
    {
        std::cout << elem << " ";
    }

    return 0;
}
