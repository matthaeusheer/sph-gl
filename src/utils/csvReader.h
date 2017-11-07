#pragma once

#include <vector>
#include <iostream>

std::vector<std::vector<float>> loadAllFiles(const std::vector<std::string>& outSteps);
std::vector<float> loadCSVFile(const char* fileName, int nEntries);
std::vector<std::string> linspace(double min, double max, int n);
void padTo(std::string &str, const size_t num, const char paddingChar = '0');
std::vector<float> loadCSVPos(const char* fileName);
std::vector<std::string> arange(int start, int stop, int step = 1);

