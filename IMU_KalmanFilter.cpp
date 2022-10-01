#include "IMU_KalmanFilter.h"

#if (ARDUINO >= 100)
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


KalmanFilter::KalmanFilter()
{
// INITIAL ESTIMATE

    // set the best values for process and measurment noise variances (Q_Angle, Q_Bias, R_Measure)
	Q_Angle = 0.001f;
	Q_Bias = 0.003f;
	R_Measure = 0.03f;

    // initial values of Angle and Bias
	KF_Angle = 0.0f;
	KF_Bias = 0.0f;

    // initial values for Kalman Gain (K)
	for (int i = 0; i < 2; i++)
		K[i] = 0.0f;

    // initial values for Error Covariance Matrix (P)
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			P[i][j] = 0.0f;

	// a priori time set just once during construct class
	pastTime = (float)micros();
};

float KalmanFilter::setQ_Angle(float Q_Angle)
{
	this->Q_Angle = Q_Angle;
	return Q_Angle;
}

float KalmanFilter::setQ_Bias(float Q_Bias)
{
	this->Q_Bias = Q_Bias;
	return Q_Bias;
}

float KalmanFilter::setR_Meas(float R_Measure)
{
	this->R_Measure = R_Measure;
	return R_Measure;
}

float KalmanFilter::compute(float Angle, float Rate)
{
	// a posteriori time
	currTime = (float)micros();
	dT = (currTime - pastTime) / 1000000;

    // TIME UPDATE ("PREDICT STEP")
    prediction(Angle, Rate);
    // MEASURMENT UPDATE ("CORRECT")
	correction(Angle);

    // a priori time
	pastTime = (float)micros();

	return KF_Angle;
}

void KalmanFilter::prediction(float Angle, float Rate)
{

    // TIME UPDATE ("PREDICT STEP")
        // 1. Extrapolate the state
        newRate = Rate - KF_Bias;
        KF_Angle += dT * newRate;

        // 2. Extrapolate uncertainty
        P[0][0] += (P[0][1] + P[1][1]) * dT + Q_Angle * dT;
        P[0][1] -= P[1][1] * dT;
        P[1][0] -= P[1][1] * dT;
        P[1][1] += Q_Bias * dT;
}

void KalmanFilter::correction(float Angle)
{
    // MEASURMENT UPDATE ("CORRECT")
        // 1. Compute the Kalman Gain

        S = P[0][0] + R_Measure;
        K[0] = P[0][0] / S;
        K[1] = P[1][0] / S;

        // 2. Update estimate with measurment

        // delta between angles
        z = Angle - KF_Angle;

        // update Angle and Bias with Kalman Gain
        KF_Angle += K[0] * z;
        KF_Bias += K[1] * z;

        // 3. Update the estimate uncertainty

        P[0][0] -= P[0][0] * K[0];
        P[0][1] -= P[0][1] * K[0];
        P[1][0] -= P[1][0] * K[1];
        P[1][1] -= P[1][1] * K[1];
}
