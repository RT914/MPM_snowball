#ifndef __MPM_H__
#define __MPM_H__

#include <Eigen/Dense>
#include <vector>
#include <utility>
#include <cmath>
#include "Object.h"

using namespace std;

const int quality = 1;
const int n_particles = 3000 * quality * quality;  // number of particle
const int n_grid = 128 * quality; // number of cells

const float dx = 1.0 / n_grid;
const float inv_dx = static_cast<float>(n_grid);
const float dt = 1e-4 / quality;
const float p_vol = std::pow(dx * 0.5, 2);
const float p_rho = 1;
const float p_mass = p_vol * p_rho;

const float E = 5e3; // Young's modulus
const float nu = 0.2; // Poisson's ratio
const float mu_0 = E / (2 * (1 + nu));
const float lambda_0 = E * nu / ((1 + nu) * (1 - 2 * nu));

const Eigen::Vector2f gravity = {0.0, -1.0};

Object mpm(int SimulationTime, Object object);

#endif
