#include <iostream>
#include "tools.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
	/*
  Calculate RMSE
  */
	
  VectorXd rmse(4);
	rmse << 0, 0, 0, 0;

	// Make sure estimation vector size is not zero and,
	// the estimation vector size is equal to the ground truth vector size
  if (estimations.size() != ground_truth.size()
    || estimations.size() == 0){
    std::cout << "Invalid estimation or ground_truth data" << std::endl;
    return rmse;
  }
	
		//accumulate squared residuals
	for(unsigned int i=0; i < estimations.size(); ++i){

		VectorXd residual = estimations[i] - ground_truth[i];

		//coefficient-wise multiplication
		residual = residual.array()*residual.array();
		rmse += residual;
	}

	//calculate the mean
	rmse = rmse/estimations.size();

	//calculate the squared root
	rmse = rmse.array().sqrt();

	//return the result
	return rmse;
	}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
	/*
  Calculate a Jacobian matrix.
  */
	
  MatrixXd Hj(3, 4);
	
  //recover state parameters
	float px = x_state(0);
	float py = x_state(1);
	float vx = x_state(2);
	float vy = x_state(3);

	//pre-compute a set of terms to avoid repeated calculation
	float c1 = px*px + py*py;
	float c2 = sqrt(c1);
	float c3 = (c1*c2);

	//check division by zero
	if (fabs(c1) < 0.00001){
    VectorXd x_temp(4);
    // in case of division by zero, set state to small float value
    x_temp << 0.001, 0.01, 0.001, 0.001;
    std::cout << "CalculateJacobian () - Error - Division by Zero" << std::endl;
    return CalculateJacobian(x_temp);
	}

	//compute the Jacobian matrix
	Hj << (px / c2), (py / c2), 0, 0,
		-(py / c1), (px / c1), 0, 0,
		py*(vx*py - vy*px) / c3, px*(px*vy - py*vx) / c3, px / c2, py / c2;
	return Hj;
}
