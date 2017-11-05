#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>


#include "particle.h"
#include "csvReader.h"

std::vector<std::vector<float>> loadAllFiles(const std::vector<std::string>& outSteps) {

	std::vector<std::vector<float>> allData(outSteps.size());

	/*First get number of points*/
	std::string firstStep = outSteps[0];
	padTo(firstStep, 5);
	std::string firstFileName = "Data\\out_" + firstStep + ".txt";
	std::vector<float> firstDat = loadCSVPos(&firstFileName[0]);
	int nEntriesToReserve = firstDat.size();
	std::cout << "Number of particles : " << nEntriesToReserve / 8 << std::endl;

	for (int i=0; i < outSteps.size(); i++) {
		std::string step = outSteps[i];
		padTo(step, 5);
		std::string fileName = "Data\\out_" + step + ".txt";
		std::cout << "Loading... " << fileName << std::endl;
		std::vector<float> data = loadCSVFile(&fileName[0], nEntriesToReserve);
		allData[i] = data;
	
	}
	
	return allData;
}

std::vector<float> loadCSVFile(const char* fileName, int nEntries) {
	
	std::vector<float> data(nEntries);
	float x, y, z, vx, vy, vz, m, rho;

	int idx = 0;

	FILE* f;
	fopen_s(&f, fileName, "r");
	if (f == NULL)
	{
		printf("Failed to open.\n");
	}
	else
	{
		while (std::fscanf(f, "%f,%f,%f,%f,%f,%f,%f,%f", &x, &y, &z, &vx, &vy, &vz, &m, &rho) == 8) {

			//std::cout << "(x, y, z): (" << x << ", " << y << ", " << z << ")" << std::endl;
			data[idx] = (x);
			data[idx+1] = (y);
			data[idx+2] = (z);
			data[idx+3] = (vx);
			data[idx+4] = (vy);
			data[idx+5] = (vz);
			data[idx+6] = (m);
			data[idx+7] = (rho);
			idx += 8;
		}
		std::fclose(f);
	}

	return data;
}



std::vector<float> loadCSVPos(const char* fileName) {

	std::vector<float> data;

	float x, y, z, vx, vy, vz, m, rho;

	FILE* f;
	fopen_s(&f, fileName, "r");
	if (f == NULL)
	{
		printf("Failed to open.\n");
	}
	else
	{
		printf("File opened.\n");
		while (std::fscanf(f, "%f,%f,%f,%f,%f,%f,%f,%f", &x, &y, &z, &vx, &vy, &vz, &m, &rho) == 8) {

			data.push_back(x); 
			data.push_back(y);
			data.push_back(z);
			data.push_back(vx);
			data.push_back(vy);
			data.push_back(vz);
			data.push_back(m);
			data.push_back(rho);
		}
		std::fclose(f);
	}

	return data;
}

std::vector<std::string> linspace(double min, double max, int n)
{
	std::vector<std::string> result;
	// vector iterator
	int iterator = 0;

	for (int i = 0; i <= n - 2; i++)
	{
		double temp = min + i*(max - min) / (floor((double)n) - 1);
		std::string temps = std::to_string((int)temp);
		result.insert(result.begin() + iterator, temps);
		iterator += 1;
	}

	//iterator += 1;

	result.insert(result.begin() + iterator, std::to_string((int)max));
	
	return result;
}

void padTo(std::string &str, const size_t num, const char paddingChar)
{
	if (num > str.size())
		str.insert(0, num - str.size(), paddingChar);
}

std::vector<std::string> arange(int start, int stop, int step) {
	std::vector<std::string> values;
	for (int value = start; value < stop; value += step)
		values.push_back(std::to_string(value));
	return values;
}