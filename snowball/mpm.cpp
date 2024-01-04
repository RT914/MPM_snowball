#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

#include "Draw.h"
#include "Object.h"
#include "mpm.h"

using namespace std;

vector<vector<Eigen::Vector2f>> grid_v(n_grid, vector<Eigen::Vector2f>(n_grid, Eigen::Vector2f(0, 0)));
vector<vector<float>> grid_m(n_grid, vector<float>(n_grid, 0.0f));

//----------------------------------------------------
// MPMの関数
//----------------------------------------------------
Object mpm(int SimulationTime, Object object)
{
	// 格子点の物理量の初期化
	for (int i = 0; i < n_grid; i++) {
		for (int j = 0; j < n_grid; j++) {
			grid_v[i][j] = { 0.0, 0.0 };
			grid_m[i][j] = 0.0;
		}
	}

    // Particle state updateand scatter to grid(P2G)
    for (int p = 0; p < n_particles; p++) {
        Eigen::Vector2i base = (object.points[p].position.array() * inv_dx - 0.5).cast<int>();
        Eigen::Vector2f fx = object.points[p].position.array() * inv_dx - base.array().cast<float>();
        
        // Quadratic kernels[http://mpm.graphics   Eqn. 123, with x=fx, fx-1,fx-2]
        Eigen::Matrix<float, 2, 3> w;
        w.row(0) << 0.5f * (1.5f - fx.x()) * (1.5f - fx.x()),
            0.75f - (fx.x() - 1.0f) * (fx.x() - 1.0f),
            0.5f * (fx.x() - 0.5f) * (fx.x() - 0.5f);
        w.row(1) << 0.5f * (1.5f - fx.y()) * (1.5f - fx.y()),
            0.75f - (fx.y() - 1.0f) * (fx.y() - 1.0f),
            0.5f * (fx.y() - 0.5f) * (fx.y() - 0.5f);
        
        // deformation gradient update
        object.points[p].F = (Eigen::Matrix2f::Identity() + dt * object.points[p].C) * object.points[p].F;
                
        // Hardening coefficient : snow gets harder when compressed
        float h = max(0.1, min(5.0, exp(10 * (1.0 - object.points[p].Jp))));

        float mu = mu_0 * h;
        float lambda = lambda_0 * h;

        // 特異値分解
        Eigen::JacobiSVD<Eigen::MatrixXf> svd(object.points[p].F, Eigen::ComputeFullU | Eigen::ComputeFullV);
        Eigen::Matrix2f U = svd.matrixU();
        Eigen::Matrix2f Sigma = svd.singularValues().asDiagonal();
        Eigen::Matrix2f V = svd.matrixV();

        float J = 1.0;

        for (int d = 0; d < 2; d++) {
            float new_sig = min(max(Sigma(d, d), 1.0f - 2.5e-2f), 1.0f + 7.5e-3f);  // Plasticity
            object.points[p].Jp *= Sigma(d, d) / new_sig;
            Sigma(d, d) = new_sig;
            J *= new_sig;
        }

        // 塑性変形後の弾性変形勾配を再構築する
        object.points[p].F = U * Sigma * V.transpose();

        Eigen::Matrix2f stress = 2 * mu * (object.points[p].F - U * V.transpose()) * object.points[p].F.transpose() + Eigen::Matrix2f::Identity() * lambda * J * (J - 1);
        stress = (-dt * p_vol * 4 * inv_dx * inv_dx) * stress;
        Eigen::Matrix2f affine = stress + p_mass * object.points[p].C;

        
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                // 3x3グリッドのノード近傍をループする
                Eigen::Vector2i offset = { i, j };
                Eigen::Vector2f dpos = (offset.cast<float>() - fx) * dx;
                float weight = w(0, i) * w(1, j);

                // グリッドノードの座標を計算
                Eigen::Vector2i node = base + offset;

                // grid_v と grid_m の更新
                if (node.x() >= 0 && node.x() < n_grid && node.y() >= 0 && node.y() < n_grid) {
                    Eigen::Vector2f affinetransform = affine * dpos;
                    grid_v[node.x()][node.y()] += weight * (p_mass * object.points[p].velocity + affinetransform);
                    grid_m[node.x()][node.y()] += weight * p_mass;
                }

            }
        }
        

    }

    for (int i = 0; i < n_grid; i++) {
        for (int j = 0; j < n_grid; j++) {
            if (grid_m[i][j] > 0.0) {
                // Momentum to velocity
                grid_v[i][j] /= grid_m[i][j];
                grid_v[i][j] += dt * gravity * 30;

                // Boundary conditions
                if (i < 3 && grid_v[i][j].x() > 0) {
                    grid_v[i][j].x() = 0;
                }
                if (i > n_grid - 3 && grid_v[i][j].x() > 0) {
                    grid_v[i][j].x() = 0;
                }
                if (j < 3 && grid_v[i][j].y() < 0) {
                    grid_v[i][j].y() = 0;
                }
                if (j > n_grid - 3 && grid_v[i][j].y() > 0) {
                    grid_v[i][j].y() = 0;
                }
            }
        }
    }

    // grid to particle(G2P)
    for (int p = 0; p < n_particles; p++) {
        Eigen::Vector2i base = (object.points[p].position * inv_dx - Eigen::Vector2f(0.5, 0.5)).cast<int>();
        if (base.x() >= n_grid || base.y() >= n_grid) {
            cout << SimulationTime << endl;
            cout << p << " : " << object.points[p].position.x() << " " << object.points[p].position.y() << endl;
        }
        else if (base.x() < 0 || base.y() < 0) {
            cout << SimulationTime << endl;
            cout << p << " : " << object.points[p].position.x() << " " << object.points[p].position.y() << endl;
        }
        

        Eigen::Vector2f fx = object.points[p].position * inv_dx - base.cast<float>();

        // Quadratic kernels[http://mpm.graphics   Eqn. 123, with x=fx, fx-1,fx-2]
        Eigen::Matrix<float, 2, 3> w;
        w.row(0) << 0.5f * (1.5f - fx.x()) * (1.5f - fx.x()),
            0.75f - (fx.x() - 1.0f) * (fx.x() - 1.0f),
            0.5f * (fx.x() - 0.5f) * (fx.x() - 0.5f);
        w.row(1) << 0.5f * (1.5f - fx.y()) * (1.5f - fx.y()),
            0.75f - (fx.y() - 1.0f) * (fx.y() - 1.0f),
            0.5f * (fx.y() - 0.5f) * (fx.y() - 0.5f);

        Eigen::Vector2f new_v = Eigen::Vector2f::Zero();
        Eigen::Matrix2f new_C = Eigen::Matrix2f::Zero();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

                int grid_x = base.x() + i;
                int grid_y = base.y() + j;
                
                Eigen::Vector2f dpos = Eigen::Vector2f(i, j).cast<float>() - fx;
                Eigen::Vector2f g_v = grid_v[grid_x][grid_y];
                float weight = w(0, i) * w(1, j);
                new_v += weight * g_v;
                new_C += 4 * inv_dx * weight * g_v * dpos.transpose();                
                
            }
        }
        
        object.points[p].velocity = new_v;
        object.points[p].C = new_C;
        object.points[p].position += dt * object.points[p].velocity;  // advection
        
    }

    return object;
};