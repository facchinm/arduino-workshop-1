#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!

// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5
// You can also use software SPI
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_SCK, LSM9DS1_MISO, LSM9DS1_MOSI, LSM9DS1_XGCS, LSM9DS1_MCS);
// Or hardware SPI! In this case, only CS pins are passed in
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_XGCS, LSM9DS1_MCS);


void setupSensor()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
}


void setup() 
{
  Serial.begin(9600);

  while (!Serial) {
    delay(1); // will pause Zero, Leonardo, etc until serial console opens
  }
  
  Serial.println("LSM9DS1 data read demo");
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS1 9DOF");

  // helper to just set the default scaling we want, see above!
  setupSensor();
}

void loop() 
{
  lsm.read();  /* ask it to read in the data */ 

  /* Get a new sensor event */ 
  sensors_event_t a, m, g, temp;

  lsm.getEvent(&a, &m, &g, &temp); 

  //uint8_t a_cmpr = a.acceleration*100;
  int a_compr;
  a_compr = 5.64*100;
  Serial.println(a_compr);

  uint8_t payload[7] = {0};

  uint8_t sgnd_mask = B00000000;
  uint8_t drvd_mask = B00000001;

  int a_accl_x_cmpr = a.acceleration.x*100;
  if (a_accl_x_cmpr < 0) {
    sgnd_mask = sgnd_mask | drvd_mask;
    a_accl_x_cmpr = a_accl_x_cmpr*(-1);
  }
  Serial.print("Accel X: "); Serial.print(a_accl_x_cmpr); Serial.print(" m/s^2");
  
  int a_accl_y_cmpr = a.acceleration.y*100;
  if (a_accl_y_cmpr < 0) {
    drvd_mask = B00000001 << 1;
    sgnd_mask = sgnd_mask | drvd_mask;
    a_accl_y_cmpr = a_accl_y_cmpr*(-1);
  }
  Serial.print("\tY: "); Serial.print(a_accl_y_cmpr);     Serial.print(" m/s^2 ");
  int a_accl_z_cmpr = a.acceleration.z*100;
  if (a_accl_z_cmpr < 0) {
    drvd_mask = B00000001 << 2;
    sgnd_mask = sgnd_mask | drvd_mask;
    a_accl_z_cmpr = a_accl_z_cmpr*(-1);
  }
  Serial.print("\tZ: "); Serial.print(a_accl_z_cmpr);     Serial.println(" m/s^2 ");

  memset(payload, sgnd_mask, 1 );
  memset(payload+1, a_accl_x_cmpr, 2);
  memset(payload+3, a_accl_y_cmpr, 2);
  memset(payload+5, a_accl_z_cmpr, 2);
  
  for (int i = 0; i < 7; i++){
      static char tmp[4] = {};
      sprintf(tmp, "%02X", payload[i]);
      Serial.print(String(tmp));
    }
  Serial.println();

  int m_mag_x_cmpr = m.magnetic.x*100;
  Serial.print("Mag X: "); Serial.print(m_mag_x_cmpr);   Serial.print(" gauss");
  int m_mag_y_cmpr = m.magnetic.y*100;
  Serial.print("\tY: "); Serial.print(m_mag_y_cmpr);     Serial.print(" gauss");
  int m_mag_z_cmpr = m.magnetic.z*100;
  Serial.print("\tZ: "); Serial.print(m_mag_z_cmpr);     Serial.println(" gauss");

  int g_gyr_x_cmpr = g.gyro.x*100;
  Serial.print("Gyro X: "); Serial.print(g_gyr_x_cmpr);   Serial.print(" dps");
  int g_gyr_y_cmpr = g.gyro.y*100;
  Serial.print("\tY: "); Serial.print(g_gyr_y_cmpr);      Serial.print(" dps");
  int g_gyr_z_cmpr = g.gyro.z*100;
  Serial.print("\tZ: "); Serial.print(g_gyr_z_cmpr);      Serial.println(" dps");

  Serial.println();
  delay(200);
}
