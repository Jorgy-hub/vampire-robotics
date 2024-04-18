#include "Pixy2.h"
#include "Robot.h"
#include "Simple_MPU6050.h"

// Variables Globales:
int angulo = 0;
int angle = 0;
int distan = 0;

// Clases:
Pixy2 pixy;
Simple_MPU6050 mpu;
Motor motor_1(6, 5);
Motor motor_2(8, 7);
Motor motor_3(10, 9);
Motor motor_4(12, 11);
Robot Alpharius(&motor_1, &motor_2, &motor_3, &motor_4, 7);

void updateCompass(int16_t *gyro, int16_t *accel, int32_t *quat) {
  Quaternion q;
  VectorFloat gravity;
  float ypr[3] = { 0, 0, 0 };
  float xyz[3] = { 0, 0, 0 };
  mpu.GetQuaternion(&q, quat);
  mpu.GetGravity(&gravity, &q);
  mpu.GetYawPitchRoll(ypr, &q, &gravity);
  mpu.ConvertToDegrees(ypr, xyz);

  angulo = xyz[0];
}

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);

  // Iniciar Motores:
  Alpharius.Iniciar();
  Alpharius.CalibrarLinea();

  //pixy.init();

  mpu.begin();
  mpu.Set_DMP_Output_Rate_Hz(10);
  mpu.CalibrateMPU();
  mpu.load_DMP_Image();
  mpu.on_FIFO(updateCompass);
}

void loop() {
  mpu.dmp_read_fifo(false);
  readOpenMV();

  //Serial.println(angle);
  //Serial.println(angulo);
  
  //Alpharius.Estabilizar(angulo,0);
  Soccer();
  Alpharius.Linea(angulo);
}

void Soccer() {
  int err = 0;
  Serial.println(angle);
  if (!Alpharius.movingLine) {
    if (angle >= 210 || angle <= 100) {
      Alpharius.Atras(angulo, 255, 0);
    } else if (angle > 120 && angle < 155) {
      Alpharius.Izquierda(angulo, 255, 0);
    } else if (angle < 210 && angle > 180) {
      Alpharius.Derecha(angulo, 255, -20);
    } else if (angle >= 150 && angle <= 205) {
      //pixy.ccc.getBlocks();
      if (false) {
        int porteriaX = pixy.ccc.blocks[0].m_x;
        if (porteriaX > 140 && porteriaX < 215) {
          Alpharius.Adelante(angulo, 255, 20);
        } else if (porteriaX >= 215) {
          Alpharius.Derecha(angulo, 255, 0);
        } else if (porteriaX <= 140) {
          Alpharius.Izquierda(angulo, 255, 0);
        } else {
          Alpharius.Adelante(angulo, 200, 0);
        }
      } else
        Alpharius.Adelante(angulo, 255, 0);
        
    }
  }
}

void readOpenMV() {
  String ang = "";
  String dis = "";
  char d = '.';
  if (Serial3.available()) {
    d = char(Serial3.read());
    while (d != ',') {

      if (Serial3.available()) {
        ang = ang + d;
        d = char(Serial3.read());
      }
    }
    d = char(Serial3.read());
    while (d != '\n') {
      if (Serial3.available()) {
        dis = dis + d;
        d = char(Serial3.read());
      }
    }
    dis.remove(0, 1);
    angle = (ang.toInt());
    distan = (dis.toFloat());
  }
}
