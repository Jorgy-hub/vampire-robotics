#include <stdint.h>
#ifndef ROBOT_H
#define ROBOT_H

class Motor {
private:
  int pin_a;
  int pin_b;
  int previous_speed = 0;

public:
  Motor(int a, int b) {
    Motor::pin_a = a;
    Motor::pin_b = b;
  }
  void Iniciar() {
    pinMode(Motor::pin_a, OUTPUT);
    pinMode(Motor::pin_b, OUTPUT);
  }
  void Mover(int vel, bool inverted = false) {
    vel = vel * (inverted ? -1 : 1);
    vel = constrain(vel, -255, 255);
    analogWrite(Motor::pin_a, vel < 0 ? 0 : vel);
    analogWrite(Motor::pin_b, vel < 0 ? abs(vel) : 0);
  }
};

class Robot {
private:
  // Linea
  int Lecturas[7];
  int LecturasMaximas[7];
  int num_sensores;
  int L1, L2, L3, L4, L5, L6, L7, LS1, LS2, LS3, LS4, LS5, LS6, LS7, S;

  // Motores
  int kp = 6;
  Motor *ad_izq;
  Motor *ad_der;
  Motor *at_izq;
  Motor *at_der;
  // Movimiento
  bool moviendo = false;
  int previous_movement = 0;
public:

  int lastMovement = 0;
  int timerMovement = 0;
  bool movingLine = false;
  Robot(Motor *motor_01, Motor *motor_02, Motor *motor_03, Motor *motor_04, int _num_sensores) {
    Robot::num_sensores = _num_sensores;
    Robot::ad_der = motor_01;
    Robot::ad_izq = motor_02;
    Robot::at_der = motor_04;
    Robot::at_izq = motor_03;
  }

  void Iniciar() {
    for (int i = 0; i < num_sensores; i++) {
      Lecturas[i] = 0;
      LecturasMaximas[i] = 0;
      pinMode((54 + i), INPUT);
    }

    Robot::ad_izq->Iniciar();
    Robot::ad_der->Iniciar();
    Robot::at_izq->Iniciar();
    Robot::at_der->Iniciar();
  }

  void CalibrarLinea() {
    Serial.println("");
    Serial.println("Calibrando Sensores de Linea");

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);

    int i = 0;
    while (i < 150) {
      i++;
      L1 = analogRead(A0);
      L2 = analogRead(A1);
      L3 = analogRead(A2);
      L4 = analogRead(A3);
      LS1 = (L1 > LS1) ? L1 : LS1;
      LS2 = (L2 > LS2) ? L2 : LS2;
      LS3 = (L3 > LS3) ? L3 : LS3;
      LS4 = (L4 > LS4) ? L4 : LS4;
    }

    LS1 += 25;
    LS2 += 25;
    LS3 += 25;
    LS4 += 25;
    Serial.println("Sensores de Linea Calibrados");
  }

  void Linea(int angulo) {
    L1 = analogRead(A0);
    L2 = analogRead(A1);
    L3 = analogRead(A2);
    L4 = analogRead(A3);

    if (!movingLine) {
      if (L2 > LS2 && L1 > LS1) {
        //Robot::Diagonal_Izquierda(angulo, -255);
        Robot::Freno();
        lastMovement = 5;
        movingLine = true;
      } else if (L4 > LS4 && L1 > LS1) {
        //Robot::Diagonal_Derecha(angulo, -255);
        Robot::Freno();
        lastMovement = 6;
        movingLine = true;
      }
      if (L2 > LS2 && L3 > LS3) {
        //Robot::Diagonal_Derecha(angulo, 255);
        Robot::Freno();
        lastMovement = 7;
        movingLine = true;
      } else if (L4 > LS4 && L3 > LS3) {
        //Robot::Diagonal_Izquierda(angulo, 255);
        Robot::Freno();
        lastMovement = 8;
        movingLine = true;
      } else if (L2 > LS2) {
        Serial.println("Izquierda");
        //Robot::Derecha(angulo, 255, 0);
        Robot::Freno();
        lastMovement = 2;
        movingLine = true;
      } else if (L4 > LS4) {
        Serial.println("Derecha");
        //Robot::Izquierda(angulo, 255, 0);
        Robot::Freno();
        lastMovement = 4;
        movingLine = true;
      } else if (L1 > LS1) {
        Serial.println("Adelante");
        //Robot::Atras(angulo, 255, 0);
        Robot::Freno();
        lastMovement = 1;
        movingLine = true;
      } else if (L3 > LS3) {
        Serial.println("Atras");
        //Robot::Adelante(angulo, 255, 0);
        Robot::Freno();
        lastMovement = 3;
        movingLine = true;
      }
    } else {
      if (timerMovement <= 40) {
        (lastMovement == 1) ? Robot::Atras(angulo, 255, 0) : (lastMovement == 2) ? Robot::Derecha(angulo, 255, 0)
                                                           : (lastMovement == 3) ? Robot::Adelante(angulo, 255, 0)
                                                           : (lastMovement == 5) ? Robot::Diagonal_Izquierda(angulo, -255)
                                                           : (lastMovement == 6) ? Robot::Diagonal_Derecha(angulo, -255)
                                                           : (lastMovement == 7) ? Robot::Diagonal_Derecha(angulo, 255)
                                                                                 : Robot::Diagonal_Izquierda(angulo, 255);

        timerMovement++;
      } else {
        timerMovement = 0;
        movingLine = false;
      }
    }
  }

  void Estabilizar(float angulo, int inclinacion) {
    int a = 4;
    int offset = ((angulo + inclinacion) * a);
    Robot::ad_izq->Mover(offset, true);
    Robot::ad_der->Mover(offset);
    Robot::at_izq->Mover(offset, true);
    Robot::at_der->Mover(offset);
  }

  void Derecha(int angulo, int vel, int inclinacion) {
    int mov = ((angulo + inclinacion) * kp);
    Robot::ad_izq->Mover(-vel + mov, true);
    Robot::ad_der->Mover(vel + 100 + mov);
    Robot::at_izq->Mover(vel + 100 + mov, true);
    Robot::at_der->Mover(vel + mov);
  }

  void Izquierda(int angulo, int vel, int inclinacion) {
    int mov = ((angulo + inclinacion) * kp);
    Robot::ad_izq->Mover(vel + 150 + mov, true);
    Robot::ad_der->Mover(-vel + mov);
    Robot::at_izq->Mover(-vel + 100 + mov, true);
    Robot::at_der->Mover(-vel - 100 + mov);
  }

  void Atras(int angulo, int vel, int inclinacion) {
    int mov = ((angulo + inclinacion) * kp);
    Robot::ad_izq->Mover(vel + mov, true);
    Robot::ad_der->Mover(vel + mov);
    Robot::at_izq->Mover(vel + mov, true);
    Robot::at_der->Mover(-vel + mov);
  }

  void Adelante(int angulo, int vel, int inclinacion) {
    //Serial.println("Moviendo Atras");
    int mov = ((angulo + inclinacion) * kp);
    Robot::ad_izq->Mover(-vel + 100 + mov, true);
    Robot::ad_der->Mover(-vel + 100 + mov);
    Robot::at_izq->Mover(-vel - 40 + mov, true);
    Robot::at_der->Mover(vel - 100 + mov);
  }

  void Diagonal_Izquierda(int angulo, int vel) {
    int mov = (angulo * kp);
    Robot::ad_izq->Mover(mov);
    Robot::ad_der->Mover(-vel + mov, true);
    Robot::at_izq->Mover(-vel + mov, true);
    Robot::at_der->Mover(mov);
  }

  void Diagonal_Derecha(int angulo, int vel) {
    int mov = (angulo * kp);
    Robot::ad_izq->Mover(-vel + mov, true);
    Robot::ad_der->Mover(mov);
    Robot::at_izq->Mover(mov);
    Robot::at_der->Mover(-vel + mov, true);
  }

  void Freno() {
    Robot::ad_izq->Mover(0);
    Robot::ad_der->Mover(0);
    Robot::at_izq->Mover(0);
    Robot::at_der->Mover(0);
  }
};

#endif
