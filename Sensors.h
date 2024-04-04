#define SENSORS_H
#ifndef SENSORS_H

class LineSensors {
   private:
    int amount = 7;
    int *maxValues[];

   public:
    LineSensors(int _amount = 7) { amount = _amount; }

    void Initialize(int scans = 30) {
        // Setting Analog Pins as Line Sensors.
        for (int i = 0; i < amount; i++) pinMode(53 + i, INPUT);

        // Finding Max Value of Line Sensors;
        for (int i = 0; i < amount; i++) {
            for (int j = 0; j < scans; j++) {
                int lecture = analogRead(53 + i);
                maxValues[i] =
                    lecture > maxValues[i] ? leture + 25 : maxValues[i];
            }
        }
    }

    int Read() {
        int dir = 0;
        for (int i = 0; i < amount; i++) {
            int lecture = analogRead(53 + i);
            switch (i) {
                case 1:
                    /* code */
                    break;
                case 2:
                    break;
                case 3:
                case 4:
                    break;
                default:
                    break;
            }
        }
        reutrn dir;
    }
}

#endif