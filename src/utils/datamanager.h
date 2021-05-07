#pragma once

#include <vector>
#include <iostream>
#include <ctime>

#include "GLAD/glad.h"
#include "glm/glm.hpp"

class DataManager
{
private:
	size_t m_NOutSteps;
	size_t m_NParticles;

	size_t m_PosSize;
	size_t m_VelSize;
	size_t m_DensSize;

	std::vector<std::string> m_FileNames;
	
public:
	std::vector<float> m_Position;
	std::vector<float> m_Velocity;
	std::vector<float> m_Density;

	DataManager(const std::vector<std::string>& outSteps);
	void loadAllFiles();

	GLfloat* getPositionP(unsigned int timestep);
	GLfloat* getVelocityP(unsigned int timestep);
	GLfloat* getDensityP(unsigned int timestep);
	size_t getPosSize();
	size_t getVelSize();
	size_t getDensSize();
	size_t getNumberOfParticles();
	size_t getNTimesteps();

private:
	std::vector<std::vector<float>> loadCSVFile(const char* fileName);
};


std::vector<std::string> linspace(double min, double max, int n);
void padTo(std::string &str, const size_t num, const char paddingChar = '0');
std::vector<float> loadFileInOneVector(const char* fileName);
std::vector<std::string> arange(int start, int stop, int step = 1);
double diffclock(clock_t clock1, clock_t clock2);

