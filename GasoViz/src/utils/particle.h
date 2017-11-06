#pragma once

#include <string>

struct Particle
{
	double x;
	double y;
	double z;
	double vx;
	double vy;
	double vz;
	double m;
	double rho;
	double T;
	int mat;
};

struct ParticleStr
{
	std::string x;
	std::string y;
	std::string z;
	std::string vx;
	std::string vy;
	std::string vz;
	std::string m;
	std::string rho;


};