#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <time.h>
#include <cmath> 

#include "particle.h"
#include "datamanager.h"
#include "filesize.h"

DataManager::DataManager(const std::vector<std::string>& outSteps) : m_NOutSteps(outSteps.size())
{
	assert(m_NOutSteps >= 1);

	// initialize filename list
	for (int i = 0; i < m_NOutSteps; i++) {
		std::string step = outSteps[i];
		padTo(step, 5);
		std::string fileName = "Data\\out_" + step + ".txt";
		m_FileNames.push_back(fileName);
	}

	// get number of particles (from first file);
	m_NParticles = getNumberOfParticles();
	assert(m_NParticles > 0);

	// allocate enough memory to have fast file read
	m_PosSize  = 3 * m_NParticles * m_NOutSteps;
	m_VelSize  = 3 * m_NParticles * m_NOutSteps;
	m_DensSize =     m_NParticles * m_NOutSteps;

	m_Position.reserve(m_PosSize);
	m_Velocity.reserve(m_VelSize);
	m_Density.reserve(m_DensSize);
}

void DataManager::loadAllFiles() 
{
	float total_size = 0.0f;
	clock_t global_start = clock();

	for (int i=0; i < m_NOutSteps; i++) {
		clock_t start = clock();
		std::cout << "Loading... " << m_FileNames[i];
		
		 std::vector<std::vector<float>> data_vec = loadCSVFile(&m_FileNames[i][0]);
		 m_Position.insert(m_Position.end(), data_vec[0].begin(), data_vec[0].end());
		 m_Velocity.insert(m_Velocity.end(), data_vec[1].begin(), data_vec[1].end());
		 m_Density.insert(m_Density.end(), data_vec[2].begin(), data_vec[2].end());

		 clock_t current = clock();
		double time = diffclock(current, start) ;

		std::cout << ", Time: " << current - start << " ms" << std::endl;
	}
	clock_t end = clock();
	
	std::cout << "Loaded " << m_NOutSteps << " files  in " << diffclock(end, global_start) / 1000 << " seconds." << std::endl;

}

std::vector<std::vector<float>> DataManager::loadCSVFile(const char* fileName) {
	
	std::vector<std::vector<float>> data(3);

	std::vector<float> position(3 * m_NParticles);
	std::vector<float> velocity(3 * m_NParticles);
	std::vector<float> density(m_NParticles);
	
	float x, y, z, vx, vy, vz, m, rho;

	int pos_vel_idx = 0;
	int dens_idx = 0;

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
			
			position[pos_vel_idx]     = x;
			position[pos_vel_idx + 1] = y;
			position[pos_vel_idx + 2] = z;

			velocity[pos_vel_idx]     = vx;
			velocity[pos_vel_idx + 1] = vy;
			velocity[pos_vel_idx + 2] = vz;

			density[dens_idx] = rho;

			pos_vel_idx += 3;
			dens_idx ++;
		}
		std::fclose(f);
	}

	data[0] = position;
	data[1] = velocity;
	data[2] = density;

	return data;
}


/* Returns the number of lines in the FIRST file in m_FileNames. */
size_t DataManager::getNumberOfParticles() {

	size_t nParticles = 0;

	float x, y, z, vx, vy, vz, m, rho;

	FILE* f;
	fopen_s(&f, &m_FileNames[0][0], "r");
	if (f == NULL)
	{
		printf("Failed to open.\n");
	}
	else
	{
		while (std::fscanf(f, "%f,%f,%f,%f,%f,%f,%f,%f", &x, &y, &z, &vx, &vy, &vz, &m, &rho) == 8) {
			nParticles++;
		}
		std::fclose(f);
	}

	return nParticles;
}

size_t DataManager::getNTimesteps()
{
	return m_NOutSteps;
}

GLfloat* DataManager::getPositionP(unsigned int timeStep)
{
	return m_Position.data() + 3 * m_NParticles * timeStep;
}

GLfloat* DataManager::getVelocityP(unsigned int timeStep)
{
	return m_Velocity.data() + 3 * m_NParticles * timeStep;
}

GLfloat* DataManager::getDensityP(unsigned int timeStep)
{
	return m_Density.data() + m_NParticles * timeStep;
}

size_t DataManager::getPosSize()
{
	return m_PosSize;
}

size_t DataManager::getVelSize()
{
	return m_VelSize;
}

size_t DataManager::getDensSize()
{
	return m_DensSize;
}


std::vector<float> loadFileInOneVector(const char* fileName) {

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

double diffclock(clock_t clock1, clock_t clock2) {

	double diffticks = clock1 - clock2;
	double diffms = diffticks / (CLOCKS_PER_SEC / 1000);

	return diffms;
}
