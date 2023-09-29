#pragma once

#include <algorithm>
#include <cmath>

double pv(double notional, int cp, double K, double spot, double fwd, double vol, double T, double sqrt_T);

double NormCDF(double x);


