
#pragma once
#pragma warning(disable:4201)
#pragma warning(disable:4505)

// standard includes
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>

// external dependencies
#include "glm.hpp"

// internal util
#include "Random.h"
#include "SimMath.h"

// macros
#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(x)x
#endif

#define DRAWS_VISION_CONES	0
#define USES_PREDATOR_PREY	1
#define USES_NEAT			1

#define MAX_TIMESCALE 15

#define PI		3.14159265359f
#define TWOPI	6.28318530718f

#define PERCEPTION_NUM_VISION_CONES	8

#define NETWORK_MAX_NODES	10000
#define NETWORK_MAX_IN		27
#define NETWORK_MIN_IN		11

#define NETWORK_NUM_IN_STATUS	3
#define NETWORK_NUM_HIDDEN		15
#define NETWORK_NUM_OUT			3

#define PREDATOR_COLOR			glm::vec3(1.0f, 0.5f, 0.5f)
#define PREDATOR_FITNESS_FILE	"pred_fitness.csv"
#define PREDATOR_NUM_INPUTS		19

#define PREY_COLOR			glm::vec3(1.0f, 1.0f, 0.5f)
#define PREY_FITNESS_FILE	"prey_fitness.csv"
#define PREY_NUM_INPUTS		27

#define AGENT_COLOR			glm::vec3(0.85f, 0.85f, 0.85f)
#define AGENT_FITNESS_FILE	"fitness.csv"
#define AGENT_NUM_INPUTS	27

#if USES_NEAT
#if USES_PREDATOR_PREY
#define DIR_GENOMES	"assets\\config_neat\\genomes_predprey\\"
#else
#define DIR_GENOMES	"assets\\config_neat\\genomes_agents\\"
#endif
#else
#if USES_PREDATOR_PREY
#define DIR_GENOMES	"assets\\config_static\\genomes_predprey\\"
#else
#define DIR_GENOMES	"assets\\config_static\\genomes_agents\\"
#endif
#endif
