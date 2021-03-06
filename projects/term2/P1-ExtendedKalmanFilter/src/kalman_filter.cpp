#include "kalman_filter.h"

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() 
{
  x_ = F_ * x_;
  P_ = (F_ * P_ * F_.transpose()) + Q_;
}

void KalmanFilter::Update(const VectorXd &z) 
{
  VectorXd y = z - (H_ * x_);
  MatrixXd Ht = H_.transpose();
  MatrixXd S = (H_ * P_ * Ht) + R_;
  MatrixXd K = P_ * Ht * S.inverse();

  x_ = x_ + (K * y);
  P_ = (MatrixXd::Identity(x_.size(), x_.size()) - (K * H_)) * P_;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) 
{
  VectorXd y = z - ProjectToMeasurmentSpace();
  MatrixXd Ht = H_.transpose();
  MatrixXd S = (H_ * P_ * Ht) + R_;
  MatrixXd K = P_ * Ht * S.inverse();

  x_ = x_ + (K * y);
  P_ = (MatrixXd::Identity(x_.size(), x_.size()) - (K * H_)) * P_;
}

VectorXd KalmanFilter::ProjectToMeasurmentSpace() 
{
    VectorXd measurement_vector(3u);

    float px = x_(0u);
    float py = x_(1u);
    float vx = x_(2u);
    float vy = x_(3u);
    float norm = sqrt((px * px) + (py * py));

    measurement_vector(0) = norm;
    measurement_vector(1) = atan2(py, px);
    measurement_vector(2) = ((px * vx) + (py * vy)) / norm;

    return measurement_vector;
}
