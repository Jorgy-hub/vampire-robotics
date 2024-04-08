#include <iostream>
#include <cmath>
#include <thread> // Para manejar el tiempo de espera entre movimientos

using namespace std;

class Robot {
private:
    // Velocidades de las cuatro ruedas
    double velocidad_rueda_1, velocidad_rueda_2, velocidad_rueda_3, velocidad_rueda_4;

public:
    // Constructor
    Robot() : velocidad_rueda_1(0), velocidad_rueda_2(0), velocidad_rueda_3(0), velocidad_rueda_4(0) {}

    // Método para cambiar la velocidad de las ruedas gradualmente
    void cambiarVelocidad(double vel_rueda_1, double vel_rueda_2, double vel_rueda_3, double vel_rueda_4) {
        // Tiempo total para el cambio gradual (en segundos)
        const double tiempo_total = 2.0;
        // Número de pasos para el cambio gradual
        const int num_pasos = 20;
        // Calculamos la diferencia de velocidad por paso
        double delta_1 = (vel_rueda_1 - velocidad_rueda_1) / num_pasos;
        double delta_2 = (vel_rueda_2 - velocidad_rueda_2) / num_pasos;
        double delta_3 = (vel_rueda_3 - velocidad_rueda_3) / num_pasos;
        double delta_4 = (vel_rueda_4 - velocidad_rueda_4) / num_pasos;

        // Realizamos el cambio gradual de velocidad
        for (int i = 0; i < num_pasos; ++i) {
            velocidad_rueda_1 += delta_1;
            velocidad_rueda_2 += delta_2;
            velocidad_rueda_3 += delta_3;
            velocidad_rueda_4 += delta_4;

            // Simulamos el cambio gradual durante un tiempo específico
            // (esto puede ser más complejo dependiendo de cómo manejes el hardware)
            // En este ejemplo, simplemente esperamos un tiempo entre pasos.
            this_thread::sleep_for(chrono::milliseconds(static_cast<int>(tiempo_total * 1000 / num_pasos)));

            // Aquí iría el código para enviar las velocidades a las ruedas del robot
            // Por simplicidad, aquí solo mostramos las velocidades
            cout << "Velocidades: " << velocidad_rueda_1 << ", " << velocidad_rueda_2 << ", " 
                 << velocidad_rueda_3 << ", " << velocidad_rueda_4 << endl;
        }

        // Asignamos las velocidades finales
        velocidad_rueda_1 = vel_rueda_1;
        velocidad_rueda_2 = vel_rueda_2;
        velocidad_rueda_3 = vel_rueda_3;
        velocidad_rueda_4 = vel_rueda_4;
    }
};

int main() {
    Robot mi_robot;

    // Cambiamos gradualmente las velocidades
    mi_robot.cambiarVelocidad(1.0, 1.0, 1.0, 1.0);

    return 0;
}
