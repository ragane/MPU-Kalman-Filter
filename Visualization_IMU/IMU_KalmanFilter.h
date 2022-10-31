#ifndef _IMU_KalmanFilter_h_
#define _IMU_KalmanFilter_h_

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif



class KalmanFilter
{
    public:
    
        // Constructor
        KalmanFilter();
        // KF body
        float compute(float Angle, float Rate);
        // tune value of Process Noise Variance (ACC)
        float setQ_Angle(float Q_Angle);
        // tune value of Process Noise Variance (GYRO)
        float setQ_Bias(float Q_Bias);
        // tune value of Measurment Noise Variance
        float setR_Meas(float R_Measure);

    private:
    
        // Angle and Bias computed by KF
        float KF_Angle, KF_Bias;
        // variable for calculation KF_Angle
        float newRate;
        // Proccess noise variance (ACC, GY) and Measurment Noise Variance
        float Q_Angle, Q_Bias, R_Measure;
        // Kalman Gain for Angle and Bias is a 2x1 vector
        float K[2];
        // Error estimation
        float S;              
        // Error covariance matrix
        float P[2][2];
        // Estimate error 
        float z;
        // variables for estimate time
        float pastTime, currTime, dT;
        // prediction part of KF
        void prediction(float Angle, float Rate);
        // correction part of KF
        void correction(float Angle);
};                 
#endif                       

