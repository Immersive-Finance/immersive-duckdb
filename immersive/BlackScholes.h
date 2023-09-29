#pragma once
#include <algorithm>
#include <cmath>


double NormCDF(double x) {
	return std::erfc(-x / std::sqrt(2)) / 2.0;
}

double pv(double notional, int cp, double K, double spot, double fwd, double vol, double T, double sqrt_T) {
	if (T <= 0.0)
		return notional * std::max(cp * (spot - K), 0.0);
	double sqrt_var = vol * sqrt_T;
	double var = sqrt_var * sqrt_var;
	double d_p = (log(fwd / K) + 0.5 * var) / sqrt_var;
	double d_m = d_p - sqrt_var;
	double pv = notional * cp * (fwd * NormCDF(cp * d_p) - K * NormCDF(cp * d_m));

	return pv;
};


