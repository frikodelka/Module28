#include <future>
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>

#define SIZE 10
const int STOP = 10;
int threshold = 50;
bool sorted_win = true;

void generate_arr(std::vector<int>& arr)
{
    for (int i = 0; i < SIZE; i++)
    {
        std::random_device rd;   
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> dist(0, threshold);
        arr[i] = dist(gen);
    }
}

void merge(std::vector<int>& arr, int l, int m, int r)
{
    int nl = m - l + 1;
    int nr = r - m;

    std::vector<int> left;
    std::vector<int> right;

    for (int i = 0; i < nl; i++)
    {
        left.push_back(arr[l + i]);
    }

    for (int j = 0; j < nr; j++)
    {
        right.push_back(arr[m + 1 + j]);
    }
    
    int i = 0, j = 0;
    int k = l; 

    while (i < nl && j < nr)
    {
        if (left[i] <= right[j])
        {
            arr[k] = left[i];
            i++;
        }
        else
        {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < nl)
    {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < nr)
    {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<int>& arr, int l, int r)
{
    if (r <= l)
        return;
    auto m = l + (r - l) / 2;
    for (int i = 0; i < SIZE; i++)
    {
        auto sort_in_thread = std::async(std::launch::async, [&]() { mergeSort(arr, l, m); mergeSort(arr, m + 1, r); });
        merge(arr, l, m, r);
    }
}

void show(std::vector<int>& arr)
{
    std::cout << "Unsorted: " << std::endl;
    for (auto i = 0; i < SIZE; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<int> arr(SIZE);
    generate_arr(arr);
    for (auto i = 0; i < SIZE; i++) { 
        std::cout << arr[i] << " ";
    }

    std::cout << std::endl;

    show(arr);

    unsigned int start_time = clock();

    mergeSort(arr, 0, SIZE - 1);

    show(arr);

    unsigned int end_time = clock();
    double seconds = difftime(end_time, start_time) / 1000;
   
    std::cout << "Runtime using multithreading = " << seconds << std::endl;
    arr.clear();

    return 0;
}