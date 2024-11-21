# App Gui MPU Kalman Filter
# 

This is a desktop application to illustrate the operation of the IMU - MPU6050 3-axis accelerometer with gyroscope and thermometer. In addition, it has been extended to include a Kalman filter in the X and Y axes. In addition to imaging the aforementioned data, the program also allows you to adjust the data transmission settings via the serial port and the settings of the IMU itself.

The application allows not only to visualize the sensor's behavior in space on two-dimensional graphs, but also to show in real time in a window the data received from the sensor, along with the exact time of its receipt. The user has the option to save this data as txt and csv files. In addition, he has the option to save the image of the window showing the behavior of the sensor in the X, Y and Z axes.
An additional window of the application allows a three-dimensional visualization of the sensor's behavior in space.


### Libraries & Dependencies:

The graphical user interface was developed based on the Qt 6.2 environment and Arduino. It was developed in C++ and QML using QSerialPort serial port handling classes. The GUI itself was developed using the QCustomPlot widget.


### Images from working app


<img src="https://github.com/user-attachments/assets/2c54a43d-36db-4793-933e-f234e7923402" width=75% height=75%>


<img src="https://github.com/user-attachments/assets/3da8aab5-e505-4143-ad2a-799d9ae2b83f" width=75% height=75%>



Below are images showing the representation of the sensors' work and saved with the help of this application.

<img src="https://github.com/user-attachments/assets/672952cb-fc3e-44f3-bd60-3304b7051192" width=75% height=75%>

<img src="https://github.com/user-attachments/assets/1e2ed53f-d3a0-49c6-8ea7-6ff9b58b482c" width=75% height=75%>



### Resources

  The following scientific resources were used in the development of the above project:
  
   [Qt Documentation](https://doc.qt.io/)
   
   [Qt QML Documentation](https://doc.qt.io/qt-6/qmlapplications.html)
   
   [QCustomPlot documentation](https://www.qcustomplot.com/index.php/introduction)

### License
  [MIT License](https://github.com/ragane/ImageProcessingApp/blob/master/LICENSE)
