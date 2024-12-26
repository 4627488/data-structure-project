#include "sorting_algorithms.h"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

void generateSamples() {
    std::filesystem::create_directory("samples");
    std::vector<int> sample(100000);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    // 正序样本
    for (int i = 0; i < 100000; ++i) {
        sample[i] = i + 1;
    }
    for (int i = 0; i < 10; ++i) {
        std::ofstream outFile("samples/sample" + std::to_string(i) + ".txt");
        if (i == 1) {
            // 逆序样本
            std::reverse(sample.begin(), sample.end());
        } else if (i > 1) {
            // 随机样本
            std::generate(sample.begin(), sample.end(), [&]() { return dis(gen); });
        }
        for (const auto &num : sample) {
            outFile << num << " ";
        }
        outFile.close();
    }
}

void loadSamples(std::vector<std::vector<int>> &samples) {
    for (int i = 0; i < 10; ++i) {
        std::ifstream inFile("samples/sample" + std::to_string(i) + ".txt");
        std::vector<int> sample(100000);
        for (int j = 0; j < 100000; ++j) {
            inFile >> sample[j];
        }
        samples.push_back(sample);
        inFile.close();
    }
}

void measureSortTime(void (*sortFunc)(std::vector<int> &), std::vector<int> sample,
                     const std::string &sortName) {
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(sample);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << sortName << "排序耗时: " << duration.count() << "秒" << std::endl;
}

void countFrequency(const std::vector<int> &sample) {
    std::unordered_map<int, int> frequency;
    for (const auto &num : sample) {
        frequency[num]++;
    }

    std::vector<std::pair<int, int>> freqVec(frequency.begin(), frequency.end());
    std::sort(freqVec.begin(), freqVec.end(),
              [](const auto &a, const auto &b) { return a.second > b.second; });

    std::cout << "数字出现频率：" << std::endl;
    for (const auto &[num, freq] : freqVec) {
        std::cout << "数字: " << num << " 出现次数: " << freq << std::endl;
    }
}

int main() {
    generateSamples();
    std::vector<std::vector<int>> samples;
    loadSamples(samples);
    int sampleCount = 0;
    for (const auto &sample : samples) {
        std::cout << "====样本" << ++sampleCount << "======" << std::endl;
        measureSortTime(insertionSort, sample, "直接插入排序");
        measureSortTime(shellSort, sample, "希尔排序");
        measureSortTime(bubbleSort, sample, "冒泡排序");
        measureSortTime(quickSort, sample, "快速排序");
        measureSortTime(selectionSort, sample, "选择排序");
        measureSortTime(heapSort, sample, "堆排序");
        measureSortTime(mergeSort, sample, "归并排序");
        measureSortTime(radixSort, sample, "基数排序");
    }

    countFrequency(samples[0]);

    return 0;
}
