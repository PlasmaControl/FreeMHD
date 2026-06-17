const int kCacheSize = 16;

double Factorial(int x) {
  static const double factorial_cache[kCacheSize] = {1, 1, 2, 6, 24, 120, 720, 5040,
											  40320, 362880, 3628800, 39916800,
											  479001600, 6227020800,
											  87178291200, 1307674368000};

  if (x < kCacheSize) {
	return factorial_cache[x];
  } else {
	double s = factorial_cache[kCacheSize - 1];
	for (int n = kCacheSize; n <= x; n++) {
	  s *= n;
	}
	return s;
  }
}
double DoubleFactorial(int x) {
  static const double dbl_factorial_cache[kCacheSize] = {1, 1, 2, 3, 8, 15, 48, 105,
												  384, 945, 3840, 10395, 46080,
												  135135, 645120, 2027025};

  if (x < kCacheSize) {
	return dbl_factorial_cache[x];
  } else {
	double s = dbl_factorial_cache[kCacheSize - (x % 2 == 0 ? 2 : 1)];
	double n = x;
	while (n >= kCacheSize) {
	  s *= n;
	  n -= 2.0;
	}
	return s;
  }
}
double EvalLegendrePolynomial(int l, int m, double x) {
  double pmm = 1.0;
  if (m > 0) {
	double sign = (m % 2 == 0 ? 1 : -1);
	pmm = sign * DoubleFactorial(2 * m - 1) * Foam::pow(1 - x * x, m / 2.0);
  }
  if (l == m) {
	return pmm;
  }
  double pmm1 = x * (2 * m + 1) * pmm;
  if (l == m + 1) {
	return pmm1;
  }
  for (int n = m + 2; n <= l; n++) {
	double pmn = (x * (2 * n - 1) * pmm1 - (n + m - 1) * pmm) / (n - m);
	pmm = pmm1;
	pmm1 = pmn;
  }
  return pmm1;
}
double EvalLegendrePolynomial_dTheta(int l, int m, double theta) {
  const double x = Foam::cos(theta);
  const double s = Foam::sin(theta);
  const double Plm = EvalLegendrePolynomial(l, m, x);
  const double Plm1 = (l > m) ? EvalLegendrePolynomial(l - 1, m, x) : 0.0;
  const double eps = 1e-14;
  if (Foam::mag(s) < eps) {
	const double h = 1e-7;
	const double thp = theta + h;
	const double thm = theta - h;
	const double Pp = EvalLegendrePolynomial(l, m, Foam::cos(thp));
	const double Pm = EvalLegendrePolynomial(l, m, Foam::cos(thm));
	return (Pp - Pm) / (2.0 * h);
  }
  return ((l + m) * Plm1 - l * x * Plm) / s;
}
scalar EvalSHSlow_Re(int l, int m, scalar phi, scalar theta) {
  const int am = abs(m);
  const double x = Foam::cos(theta);
  const double P = EvalLegendrePolynomial(l, am, x);
  double kml = Foam::sqrt((2.0 * l + 1) * Factorial(l - abs(m)) /
					(4.0 * M_PI * Factorial(l + abs(m))));
  const double c = Foam::cos(am * phi);
  if (m >= 0) {
	return kml * P * c;
  } else {
	const double sign = (am % 2 == 0) ? 1.0 : -1.0;
	return sign * kml * P * c;
  }
}
  scalar EvalSHSlow_Im(int l, int m, scalar phi, scalar theta) {
	const int am = abs(m);
	const double x = Foam::cos(theta);
	const double P = EvalLegendrePolynomial(l, am, x);
	double kml = Foam::sqrt((2.0 * l + 1) * Factorial(l - abs(m)) /
					  (4.0 * M_PI * Factorial(l + abs(m))));
	const double s = Foam::sin(am * phi);
	if (m >= 0) {
	  return kml * P * s;
	} else {
	  const double sign = (am % 2 == 0) ? 1.0 : -1.0;
	  return -sign * kml * P * s;
	}
}
scalar EvalSHSlow_dTheta_Re(int l, int m, scalar phi, scalar theta) {
  const int am = abs(m);
  const double x = Foam::cos(theta);

  double kml = Foam::sqrt((2.0 * l + 1) * Factorial(l - am) /
					(4.0 * M_PI * Factorial(l + am)));

  const double dP_dtheta = EvalLegendrePolynomial_dTheta(l, am, theta);
  const double c = Foam::cos(am * phi);
  if (m >= 0){
	return kml * dP_dtheta * c;
  } else {
	const double sign = (am % 2 == 0) ? 1.0 : -1.0;
	return sign * kml * dP_dtheta * c;
  }
}

scalar EvalSHSlow_dTheta_Im(int l, int m, scalar phi, scalar theta) {
  const int am = abs(m);
  const double x = Foam::cos(theta);

  double kml = Foam::sqrt((2.0 * l + 1) * Factorial(l - am) /
					(4.0 * M_PI * Factorial(l + am)));

  const double dP_dtheta = EvalLegendrePolynomial_dTheta(l, am, theta);
  const double s = std::sin(am * phi);
  if (m >= 0){
	return kml * dP_dtheta * s;
  } else {
	const double sign = (am % 2 == 0) ? 1.0 : -1.0;
	return -sign * kml * dP_dtheta * s;
  }
}
