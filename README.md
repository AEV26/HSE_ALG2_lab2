_Выполнил: Асташонок Егор_
_Группа 22ПИ-2_
Логин в контесте: evastashonok@edu.hse.ru

### Цель лабораторной работы

Целью данной лабораторной работы является в реализации и исследовании различных алгоритмов для того чтобы решить проблему нахождения количества прямоугольников, которым принадлежит точка. 

### Формулы

Формула для генерации точек прямоугольника:
$\{(10 * i, 10 * i), (10*(2N-i), 10*(2N-i))\}$

Формула для генерации одной из координат точки:
(p * i) ^ 31 % (20 * N)
где, $p$ - большое простое, разное для $x$ и $y$ 

### Алгоритм перебора

Алгоритм основан на поочередном прохождении координат каждого прямоугольника и определении принадлежности заданной точки к данному прямоугольнику. 

__Сложность алгоритма:__ подготовка - $O(1)$, поиск - $O(N)$

Генерация каждого прямоугольника:
```cpp
int n, m;
std::cin >> n;
std::vector<std::vector<int>> Points(n, std::vector<int>(4));
for (int i = 0; i < n; i++)
{
    Points[i] = {10 * i, 10 * i, 10 * (2 * n - i), 10 * (2 * n - i)};
}
```

Перебор прямоугольников для каждой точки. Функция проверяет, принадлежит ли точка прямоугольнику или нет:
```cpp
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
```

Генерация координат происходит с помощью вызова вышеописанной функции, после чего вызывается функция нахождения количества прямоугольников которым принадлежит точка для каждой пары координат. А в заключении результат выводится с помощью цикла на экран:
```cpp
std::cin >> m;
std::vector<int> result(m);
for (int i = 0; i < m; i++)
{
	result[i] = findCountRectangles(getRandomPoint(i, arrPrimeNumbers, n), getRandomPoint(i, arrPrimeNumbers, n), Points);
}

for (const auto& elem : result)
{
    std::cout << elem << " ";
}
```
### Алгоритм на карте

Суть алгоритма заключается в сжатии координат и последующей работы с ними, используя бинарный поиск. Карта - матрица размерности по $x$ и $y$ сжатых координат. Заполнение матрицы - это проход по всем прямоугольникам и увеличение элементов матрицы в соответствии с полученными сжатыми координатами. В итоге, имея заполненную матрицу и координаты точки, сжимаем координаты точки с помощью бинарного поиска и после этого можем сделать запрос к матрице и получить ответ за константу.

**Сложность алгоритма:** подготовка - $O(N^3)$, поиск - $O(log(N))$

Функция сжатия координат, которая основана на бинарном поиске:
```cpp
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
```

Занесение координат прямоугольника, создание массивов, помогающих в сжатии:
```cpp
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
```

Карта заполняется посредством поиска индекса координат каждого прямоугольника в отсортированных массивах координат:
```cpp
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
```

Генерация координат, их сжатие при помощи бинарного поиска и работа с заполненной картой. Сжатые координаты поступают в качестве индексов, а в самой матрице находится соответствующее число и вносится в массив результатов:
```cpp
std::vector<int> arrPrimeNumbers = primeNumbers();
std::cin >> m;
int x, y, new_x, new_y;
std::vector<int> result;
for (int i = 0; i < m; i++)
{
    x = getRandomPoint(i, arrPrimeNumbers, n);
    y = getRandomPoint(i, arrPrimeNumbers, n);

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
```
### Алгоритм на дереве

Данный алгоритм заключается в построении персистентного дерева отрезков. 
Дерево работает со сжатыми координатами по тому же принципу, что и во втором алгоритме. 
Обновление дерева происходит в процессе прохода по событиям открытия и закрытия имеющихся прямоугольников. 
На отрезке открытия (по y координатам) мы увеличиваем на 1 все значения дерева в соответствии с заданной логикой, а на отрезке закрытия - уменьшаем. 
X координаты будут отвечать за индекс дерева, который мы возьмём в специально созданном массиве. 
Запрос состоит в получении индекса по x координате поданной точки, а спуск по дереву представляет собой проход до конкретного y листа (или корня, в который входит весь интервал заданных точек. 
В процессе прохода по дереву необходимо собирать значения с каждого узла (соответствующий модификатор). Итоговый результат будет суммой собранных модификаторов на пути до точки назначения.

**Сложность алгоритма:** подготовка - $O(N*log(N))$, поиск - $O(log(N))$

Занесение координат прямоугольника, создание массивов, помогающих в сжатии:
```cpp
std::pair<int, int> temp_1, temp_2;
std::set<int> yArr, xArr;
for (int i = 0; i < n; i++) {
	temp_1.first = 10 * i;
	temp_1.second = 10 * i;
	temp_2.first = 10 * (2 * n - i);
	temp_2.second = 10 * (2 * n - i);
    
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
```

Создаем пустое дерево отрезков:
```cpp
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
```

Функция для обновления дерева отрезков, на вход подается скопированный корень старого дерева:
```cpp

void changeTree(Node* head, size_t L, size_t R, int operation)
{
    if ((L <= head->lNum && head->lNum < R) && (L <= head->rNum && head->rNum < R))
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
```

Функция для нахождения количества прямоугольников, которым принадлежит точка. В функции происходит спуск по дереву и сбор соответствующих модификаторов:
```cpp
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
```

Обработка координат прямоугольников, выбор соответствующего дерева отрезков и вызов функции по изменению имеющегося дерева или для создания нового:
```cpp
for (auto &Pair: Pairs) {
    for (int i = 0; i < Points.size(); i++){
        if (Points[i][0].first == Pair.first && Points[i][0].second == Pair.second && Points[i][2].first != 1) {
            Points[i][2].first = 1;
            size_t ind = std::distance(xVec.begin(), std::find(xVec.begin(), xVec.end(),
                                                               Pair.first));
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
```

Обработка введенных координат и вызов функции для поиска количества прямоугольников, которым принадлежит точка:
```cpp
std::vector<int> arrPrimeNumbers = primeNumbers();
std::vector<int> result;
std::pair<int, int> point;
int new_x, new_y;
for (int i = 0; i < m; i++) {
    point.first = getRandomPoint(i, arrPrimeNumbers, n);
    point.second = getRandomPoint(i, arrPrimeNumbers, n);

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
```


### Измерения

![[Data preparation.png|500]]

![[Data search.png|500]]

![[Total time.png|500]]

### Графики

#### Подготовка данных

![[Pasted image 20240430142350.png]]
![[Pasted image 20240430142405.png]]
![[Pasted image 20240430142418.png]]
#### Поиск данных

![[Pasted image 20240430142446.png]]
![[Pasted image 20240430142457.png]]
![[Pasted image 20240430142508.png]]
#### Общее время

![[Pasted image 20240430142549.png]]
![[Pasted image 20240430142602.png]]
![[Pasted image 20240430142612.png]]
### Выводы

В процессе работы над алгоритмами были выполнены 3 различных замера:
- время на построение
- время на поиск
- общее время работы алгоритма

Данные замеры дали возможность лучше понять сильные и слабые стороны алгоритма. 

Ниже будут изложены мысли по поводу каждого измерения:
1. Построение:
   - Лучший результат показал **алгоритм перебора**, так как он не требует построение дополнительных структур данных, не считая массива (сложность **O(N)**).
   - Средний результат продемонстрировал **алгоритм на дереве отрезков**, из-за того, что построение дерева проигрывает обычному заполнению массива, зато превосходит построение двумерной матрицы (сложность **O(N*log(N))**)
   - Худший результат показал **алгоритм на карте**, так как построение двумерной матрицы довольно продолжительный процесс, поэтому на большом массиве данных данный алгоритм будет серьезно уступать по времени (сложность **O(N^3)**)
2. Поиск:
   - Время работы **алгоритма на карте** и **алгоритма на дереве** довольно похожи, разница в их работе незначительная. 
   - Хуже себя показал **алгоритм перебора**, его лучше не использовать на большом объеме данных. 
3. Общее время:
   - В результате анализа всех трех алгоритмов худший результат показал **алгоритм на карте**.
   - Средний результат показал **алгоритм перебора**.
   - Лучший результат показал **алгоритм на дереве**, особенно это заметно, если увеличить количество точек - запросом, а количество прямоугольников оставить без изменения. 
   
