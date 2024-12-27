#include <Wire.h>
#include <MPU6050.h>

// Define the analog pins for the flex sensors
int flexPin[] = {A0, A1, A2, A3, A6};
int flexValue[5];  // To store raw sensor readings
int isBent[5];     // To store whether the sensor is bent (1 for bent, 0 for straight)

// Threshold values for each gesture
int gestureThreshold[] = {1006, 920, 1000, 910, 910}; // Threshold for each sensor

// Initialize the MPU6050 sensor
MPU6050 mpu;

// Variables to store the accelerometer and gyroscope data
int16_t ax, ay, az, gx, gy, gz;

void setup() {
  Serial.begin(9600); // Start serial communication for both monitoring and Bluetooth

  // Initialize the MPU6050 sensor
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
}

void loop() {
  // Collect data from flex sensors
  for (int i = 0; i < 5; i++) {
    // Read the sensor value
    int currentValue = analogRead(flexPin[i]);

    // Store the raw value directly in flexValue
    flexValue[i] = currentValue;

    // Determine whether the sensor is bent or straight based on the threshold
    if (flexValue[i] >= gestureThreshold[i]) {
      isBent[i] = 0; // Sensor is straight
    } else {
      isBent[i] = 1; // Sensor is bent
    }
  }

  // Read the accelerometer and gyroscope data from MPU6050
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Detect the palm orientation
  detectPalmOrientation();

  // Detect gestures based on flex sensor data
  detectGesture();

  delay(2000); // Delay for readability and sensor stability
}

// Function to detect the palm orientation using MPU6050 data
void detectPalmOrientation() {
  // Normalize accelerometer data
  float A_mag = sqrt(ax * ax + ay * ay + az * az);  // Magnitude of the accelerometer data
  float Ax = ax / A_mag;
  float Ay = ay / A_mag;
  float Az = az / A_mag;


}

// Function to detect gestures based on flex sensor data
void detectGesture() {
  
  // High Priority Gestures:

    if (isBent[0] == 1 && isBent[4] == 1 && isBent[1] == 0 && isBent[2] == 0 && isBent[3] == 0) {
  
    sendGestureViaBluetooth("Hello");
    return;
  }
  // Gesture: Fist (all fingers bent) – Gesture 1
  if (isBent[0] == 1 && isBent[1] == 1 && isBent[2] == 1 && isBent[3] == 1 && isBent[4] == 1) {
  
    sendGestureViaBluetooth("Fist");
    return;
  }

  if (isBent[1] == 0 && isBent[2] == 0 && isBent[0] == 0 && isBent[3] == 1 && isBent[4] == 0) {
  
    sendGestureViaBluetooth(" I'm Prateek, here with artham and rishit.");
    return;
  }

  // Gesture: Peace Sign (index and middle finger bent, others straight) – Gesture 2
  if (isBent[1] == 1 && isBent[2] == 0 && isBent[0] == 0 && isBent[3] == 0 && isBent[4] == 0) {
  
    sendGestureViaBluetooth(" How are you?");
    return;
  }

  // Gesture: Rock On (index and pinky bent, others straight) – Gesture 3
  if (isBent[1] == 0 && isBent[2] == 0 && isBent[3] == 0 && isBent[4] == 1 && isBent[0] == 1) {

    sendGestureViaBluetooth("Peace Sign");
    return;
  }

  // Medium Priority Gestures:
  // Gesture: Goodbye (wave away) – Gesture 4
  if (isBent[1] == 0 && isBent[2] == 1 && isBent[3] == 0) {
    
    sendGestureViaBluetooth(" I'm fine,Thank you");
    return;
  }

  // Gesture: Yes (thumb up) – Gesture 5
  if (isBent[0] == 1 && isBent[4] == 0 && isBent[1] == 0 && isBent[2] == 0 && isBent[3] == 0) {
  
    sendGestureViaBluetooth("Yes ");
    return;
  }

  // Gesture: No (thumb down) – Gesture 6
  if (isBent[0] == 0 && isBent[4] == 1 && isBent[1] == 0 && isBent[2] == 0 && isBent[3] == 0) {
  
    sendGestureViaBluetooth("No ");
    return;
  }

  // If no recognized gesture, output nothing
}

// Function to send the detected gesture via Bluetooth
void sendGestureViaBluetooth(String gesture) {
  Serial.println(gesture); // Send the gesture name via Bluetooth
}
