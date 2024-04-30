#include <iostream>
#include <vector>

int findCountRectangles(int x, int y, const std::vector<std::vector<int>>& array)
{
    int count = 0;
    for (const auto & elem : array)
    {
        if ((elem[0] <= x && x < elem[2]) && (elem[1] <= y && y < elem[3]))
        {
            count++;
        }
    }
    return count;
}


int main()
{
    int n, m;
    std::cin >> n;
    std::vector<std::vector<int>> Points(n, std::vector<int>(4));
    for (int i = 0; i < n; i++)
    {
        std::cin >> Points[i][0] >> Points[i][1] >> Points[i][2] >> Points[i][3];
    }

    std::cin >> m;
    int x, y;
    std::vector<int> result;
    for (int i = 0; i < m; i++)
    {
        std::cin >> x >> y;

        result.push_back(findCountRectangles(x, y, Points));
    }

    for (const auto& elem : result)
    {
        std::cout << elem << " ";
    }
    return 0;
}
