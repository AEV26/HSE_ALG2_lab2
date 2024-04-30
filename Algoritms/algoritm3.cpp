#include <iostream>
#include <algorithm>
#include <vector>
#include <set>


struct Node
{
    int lNum, rNum;
    int modification = 0;
    Node* left = nullptr;
    Node* right = nullptr;
};

void createTree(Node*& head, size_t L, size_t R)
{
    if (L == R)
    {
        Node* newNode = new Node;
        head = newNode;
        head->lNum = L;
        head->rNum = R;
        return;
    }
    else
    {
        Node* newNode = new Node;
        head = newNode;
        head->lNum = L;
        head->rNum = R;

        int mid = (L + R)/2;
        createTree(head->left, L, mid);
        createTree(head->right, mid+1, R);
        return;
    }
}

void changeTree(Node* head, size_t L, size_t R, int operation)
{
    if ((L <= head->lNum && head->lNum < R) && (L <= head->rNum && head->rNum < R)) // Так было: L <= head->lNum && head->rNum < R
    {
        head->modification += operation;
        return;
    }
    else
    {
        if (head->right != nullptr && !(R <= head->right->lNum))
        {
            Node *newNode = new Node;
            newNode->rNum = head->right->rNum;
            newNode->lNum = head->right->lNum;
            newNode->modification = head->right->modification;
            newNode->left = head->right->left;
            newNode->right = head->right->right;
            head->right = newNode;
            changeTree(head->right, L, R, operation);
        }
        if (head->left != nullptr && !(L > head->left->rNum))
        {
            Node *newNode = new Node;
            newNode->rNum = head->left->rNum;
            newNode->lNum = head->left->lNum;
            newNode->modification = head->left->modification;
            newNode->left = head->left->left;
            newNode->right = head->left->right;
            head->left = newNode;
            changeTree(head->left, L, R, operation);
        }
        return;
    }
}

int findCountRectangle(Node* head, int elem)
{
    int countMod = 0;
    countMod += head->modification;
    while (head->rNum != elem || head->lNum != elem)
    {
        if ((head->rNum + head->lNum)/2 < elem)
        {
            head = head->right;
            countMod += head->modification;
        }
        else
        {
            head = head->left;
            countMod += head->modification;
        }
    }
    return countMod;
}

void sorted(std::vector<std::pair<int, int>>& array)
{
    for (int i = 0; i < array.size()-1; i++)
    {
        for (int j = i+1; j < array.size(); j++)
        {
            if (array[i].first > array[j].first)
            {
                std::pair<int, int> temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}

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


int main(int argc, const char * argv[])
{
    int n, m;
    std::vector<std::vector<std::pair<int, int>>> Points;
    std::vector<std::pair<int, int>> Pairs;
    std::pair<int, int> flag(0, 0);
    std::cin >> n;
    if (n > 0) {
        std::pair<int, int> temp_1, temp_2;
        std::set<int> yArr, xArr;
        for (int i = 0; i < n; i++) {
            std::cin >> temp_1.first >> temp_1.second;
            std::cin >> temp_2.first >> temp_2.second;
            yArr.insert(temp_1.second);
            xArr.insert(temp_1.first);
            yArr.insert(temp_2.second);
            xArr.insert(temp_2.first);
            Pairs.push_back(temp_1);
            Pairs.push_back(temp_2);
            Points.push_back({temp_1, temp_2, flag});
        }

        std::vector<int> xVec(xArr.begin(), xArr.end());
        std::vector<int> yVec(yArr.begin(), yArr.end());

        sorted(Pairs);
        Node *head;
        createTree(head, 0, yVec.size() - 2); // -2 потому что мы начинаем отсчёт с нуля

        std::vector<Node *> Trees(xVec.size() + 1); // Сколько различных Х столько и будет различных деревьев
        Trees[0] = head;

        for (auto &Pair: Pairs) {
            for (int i = 0; i < Points.size(); i++){
                if (Points[i][0].first == Pair.first && Points[i][0].second == Pair.second && Points[i][2].first != 1) {
                    Points[i][2].first = 1;
                    size_t ind = std::distance(xVec.begin(), std::find(xVec.begin(), xVec.end(),
                                                                       Pair.first)); // находим позицию элемента
                    if (Trees[ind + 1] != nullptr) {

                        changeTree(Trees[ind + 1],
                                   std::distance(yVec.begin(), std::find(yVec.begin(), yVec.end(), Points[i][0].second)),
                                   std::distance(yVec.begin(), std::find(yVec.begin(), yVec.end(), Points[i][1].second)), 1);
                    } else {
                        Node *newTree = new Node;
                        newTree->rNum = Trees[ind]->rNum;
                        newTree->modification = Trees[ind]->modification;
                        newTree->lNum = Trees[ind]->lNum;
                        newTree->left = Trees[ind]->left;
                        newTree->right = Trees[ind]->right;
                        changeTree(newTree,
                                   std::distance(yVec.begin(), std::find(yVec.begin(), yVec.end(), Points[i][0].second)),
                                   std::distance(yVec.begin(), std::find(yVec.begin(), yVec.end(), Points[i][1].second)), 1);
                        Trees[ind + 1] = newTree;

                    }
                    break;
                } else if (Points[i][1].first == Pair.first && Points[i][1].second == Pair.second && Points[i][2].second != 1) {
                    Points[i][2].second = 1;
                    size_t ind = std::distance(xVec.begin(), std::find(xVec.begin(), xVec.end(),
                                                                       Pair.first)); // находим позицию элемента
                    if (Trees[ind + 1] != nullptr) {

                        changeTree(Trees[ind + 1],
                                   std::distance(yVec.begin(), std::find(yVec.begin(), yVec.end(), Points[i][0].second)),
                                   std::distance(yVec.begin(), std::find(yVec.begin(), yVec.end(), Points[i][1].second)),
                                   -1);
                    } else {
                        Node *newTree = new Node;
                        newTree->rNum = Trees[ind]->rNum;
                        newTree->modification = Trees[ind]->modification;
                        newTree->lNum = Trees[ind]->lNum;
                        newTree->left = Trees[ind]->left;
                        newTree->right = Trees[ind]->right;

                        changeTree(newTree,
                                   std::distance(yVec.begin(), std::find(yVec.begin(), yVec.end(), Points[i][0].second)),
                                   std::distance(yVec.begin(), std::find(yVec.begin(), yVec.end(), Points[i][1].second)),
                                   -1);
                        Trees[ind + 1] = newTree;
                    }
                    break;
                }
            }
        }

        std::cin >> m;

        std::vector<int> result;
        std::pair<int, int> point;
        int new_x, new_y;
        for (int i = 0; i < m; i++) {
            std::cin >> point.first >> point.second;

            new_x = findBin(point.first, xVec);
            new_y = findBin(point.second, yVec);
            if (new_x != -1 && new_y != -1 && new_y != (yVec.size() - 1))
            {
                result.push_back(findCountRectangle(Trees[new_x + 1], new_y));
            }
            else
                result.push_back(0);
        }

        for (int i = 0; i < m; i++)
        {
            std::cout << result[i] << " ";
        }
    }
    else
    {
        std::cin >> m;
        std::vector<int> result;
        std::pair<int, int> point;
        for (int i = 0; i < m; i++)
        {
            std::cin >> point.first >> point.second;
            result.push_back(0);
        }

        for (int i = 0; i < m; i++)
        {
            std::cout << result[i] << " ";
        }
    }

    return 0;
}
