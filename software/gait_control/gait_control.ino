// __________________________________________________________//
// Arduino code for vibration of SenS-Device during gait     //
// using an IMU and pressure sensor. The Arduino controls    //
// the vibration based on shank movement and heel contact    //
// to control the start and stop timing.                     //
// ~ 80 Hz vibration fs = duty cycle 200 decimal
//                                                           //
// Start vibration: Sagittal shank angular velocity (Gy)     //
//                  is taken from the gyroscope. This data   //
//                  is filtered using a 30-sample moving     //
//                  average. The peak extension after        //
//                  toe-off is detected and used to send     //
//                  the onset signal.                        //
//                                                           //
// Stop vibration:  The same data is used to detect the peak //
//                  flexion during ground contact, which     //
//                  happens shortly before the actual toe-   //
//                  off. This then sends the offset signal.  //
//                                                           //
// Code by Kevin Soter, last updated 15.11.2024              //
// __________________________________________________________//

#include <Wire.h>
#include <Adafruit_LSM6DSOX.h>

// FILL IN BY HAND //

u_int32_t vibr = 200;       // Duty Cycle (Decimals: 0-255)

//____END________//

Adafruit_LSM6DSOX sox;      // Defining the handle for the IMU

u_int32_t motor_pin = 2;    // Pin used for vibration (PWM signal to open/close transistor)
u_int32_t anOutput = 4;     // Pin sends analog output to DAQ
u_int32_t heel_pin = A3;    // Pin for FSR reading of heel contact

float heel_reading;         // Variable to store FSR value
float peak_vel;
float threshold_wait = 400; // Only initial value
float threshold_vel = 75;   // Only initial value

u_int32_t timer_heel;       // Variable to store the time since the last heel contact
u_int32_t vibr_timer;       // Variable to store time since vibration started

bool firstContact = false;    // Bool for first heel contact in gait cycle
bool vibr_status = false;     // Bool for status of vibration
bool start_searching = false; // Bool for the start of searching conditions for vibration start
bool start_searching2 = false;
bool stop_searching = false;
bool stop_searching2 = false;
bool peak_vel_flag = false;

// Variables needed for gyroscope calibration
int RateCalibrationNumber;
float RateCalibrationY;

float Gx, Gy, Gz;
float Gy_avg, Gy_avg_old;

float Gy_old = 1000;
float stop_val = 0;

// Function to retrieve the IMU value
void IMU_signals(void) {

  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;

  sox.getEvent(&accel, &gyro, &temp);

  Gy = gyro.gyro.y * 1 / (3.142 / 180);
}

float movingAverage(float value) {
  const byte nvalues = 30;             // Moving average window size

  static byte current = 0;             // Index for current value
  static byte cvalues = 0;             // Count of values read (<= nvalues)
  static float sum = 0;                // Rolling sum
  static float values[nvalues];

  sum += value;

  if (cvalues == nvalues)
    sum -= values[current];

  values[current] = value;

  if (++current >= nvalues)
    current = 0;

  if (cvalues < nvalues)
    cvalues += 1;

  return sum / cvalues;
}

void setup(void) {

  if (!sox.begin_I2C()) {
    while (1);
  }

  sox.setAccelRange(LSM6DS_ACCEL_RANGE_8_G);
  sox.setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS);
  sox.setAccelDataRate(LSM6DS_RATE_6_66K_HZ);
  sox.setGyroDataRate(LSM6DS_RATE_6_66K_HZ);

  for (RateCalibrationNumber = 0; RateCalibrationNumber < 2000; RateCalibrationNumber++) {
    IMU_signals();
    RateCalibrationY += Gy;
    delay(1);
  }

  RateCalibrationY /= 2000;

  pinMode(heel_pin, INPUT);
  pinMode(anOutput, OUTPUT);
  pinMode(motor_pin, OUTPUT);

  analogWrite(motor_pin, 0);
}

void loop() {

  IMU_signals();
  heel_reading = analogRead(heel_pin);

  Gy_avg_old = Gy_avg;

  Gy -= RateCalibrationY;
  Gy_avg = movingAverage(Gy);

  //____Find Vibration Onset___//

  if (vibr_status == false &&
      start_searching == false &&
      firstContact == false &&
      Gy > threshold_vel) {

    start_searching = true;
  }

  if (start_searching == true &&
      start_searching2 == false &&
      Gy < -1.0) {

    start_searching2 = true;
    start_searching = false;
  }

  if (start_searching2 == true &&
      Gy_avg_old - Gy_avg < -0.3) {

    vibr_status = true;
    start_searching2 = false;
    peak_vel_flag = true;
    vibr_timer = micros();
  }

  if (peak_vel_flag == true) {

    peak_vel = abs(Gy_avg);
    threshold_wait = 1000 / peak_vel * 100;
    threshold_vel = peak_vel / 3.5;

    peak_vel_flag = false;
  }

  //____Find Vibration Offset___//

  if (vibr_status == true &&
      stop_searching == false &&
      firstContact == true &&
      micros() - timer_heel >= threshold_wait * 1000) {

    stop_searching = true;
  }

  if (stop_searching == true &&
      stop_searching2 == false &&
      Gy > threshold_vel) {

    stop_searching2 = true;
    stop_searching = false;
  }

  if (stop_searching2 == true &&
      Gy_avg - Gy_avg_old < -0.3) {

    vibr_status = false;
    stop_searching2 = false;
  }

  if (vibr_status == true) {

    analogWrite(motor_pin, vibr);
    analogWrite(anOutput, 170);

  } else {

    analogWrite(motor_pin, 0);
    analogWrite(anOutput, 85);
  }

  //____Maximum vibration duration___//

  if (vibr_status == true &&
      micros() - vibr_timer >= 3000 * 1000) {

    vibr_status = false;
  }

  //____Heel Contact___//

  if (heel_reading > 950 &&
      firstContact == false) {

    firstContact = true;
    timer_heel = micros();
  }

  if (micros() - timer_heel >= threshold_wait * 1000 &&
      heel_reading < 400) {

    firstContact = false;
  }
}
