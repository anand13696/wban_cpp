#include<bits/stdc++.h>
using namespace std;
#define sf scanf
#define pf printf

class Sensor {//The input of position of node is in mm.
private:
    double posX;
    double posY;
    double energyInitial;
public:
    int priority = 1;
    void setValues() {
        sf("%lf %lf %lf", &posX, &posY, &energyInitial);
    }
    double getXCood() {
        return posX;
    }
    double getYCood() {
        return posY;
    }
    double getEnergy() {
        return energyInitial;
    }
    double calDistance(double x, double y) {
        // Calculates the distance between sensor node and bnc and returns the distance in m2.
        double distance = sqrt(pow(posX - x, 2) + pow(posY - y, 2));
        distance *= pow(10, -3);
        return distance;
    }
};

int main() {
    Sensor eeg, ecg, pulse, ms1, ms2;//ms - motion sensor
    pf("Enter the x and y coordinates of sensor nodes in order EEG, ECG, Pulse, MS1 amd MS2\n");
    eeg.setValues();
    ecg.setValues();
    pulse.setValues();
    ms1.setValues();
    ms2.setValues();
    double bncX, bncY, bncXOld, bncYOld;
    pf("Enter the x and y coordinates of the BNC.\n");
    sf("%lf %lf", &bncX, &bncY);

    double dist[5] = {0}, energyTransmissionOldPos[5] = {0}, energyTransmission[5] = {0}, energyAvailable[5] = {0};
    double utilityFactor[5] = {0}, uf[5] = {0}, xi[5] = {0}, pathLossExpo = 5.9;
    double energyTransmissionAvg = 0, energyTransmissionOldPosAvg = 0;
    int cnt = 0;
    while(1) {
        if(cnt != 0) {
            energyTransmissionOldPosAvg = 0;
            for (int i = 0; i < 5; i++) {
                energyTransmissionOldPos[i] = energyTransmission[i];
                energyTransmissionOldPosAvg += energyTransmissionOldPos[i];
            }
            energyTransmissionOldPosAvg /= 5;
            pf("\n Average Transmission Energy from initial position of BNC %lf", energyTransmissionOldPosAvg);
        }
        dist[0] = eeg.calDistance(bncX, bncY);
        dist[1] = ecg.calDistance(bncX, bncY);
        dist[2] = pulse.calDistance(bncX, bncY);
        dist[3] = ms1.calDistance(bncX, bncY);
        dist[4] = ms2.calDistance(bncX, bncY);

        pf("\n Distance ");// Prints the distance between the sensor node and the BNC
        for (int i = 0; i < 5; ++i) {
            pf("%lf ", dist[i]);
        }

        /*
         * Available Energy = initialEnergyOfTheSensor - energyTransmittedBySensor
         * energyTransmittedBySensor = Etx_elec * k + Eamp * k * dist^pathLossExpo
         * Etx_elec - Energy dissipated by the radio to run the circuitry of the transmitter in nJ/bit (Taken 2.5)
         * Eamp - Energy dissipated by the transmit amplifier in nJ/bit-m^pathLossExpo (Taken 120)
         * k - number of bits (Taken 1 to calculate the energy taken by each bit)
         */
        energyTransmissionAvg = 0;
        pf("\n Transmission energy ");
        for(int i = 0; i < 5; i++) {
            energyTransmission[i] = 2.5 + 120 * pow(dist[i], pathLossExpo);
            pf("%lf ", energyTransmission[i]);
            energyTransmissionAvg += energyTransmission[i];
        }
        energyTransmissionAvg /= 5;
        pf("\n Average Transmission Energy from present position of BNC %lf", energyTransmissionAvg);

        if (cnt != 0) {
            if(energyTransmissionAvg > energyTransmissionOldPosAvg) {
                pf("\n\n The final coordinates of the BNC is %lf %lf.", bncXOld, bncYOld);
                break;
            }
        } else if (cnt == 10) {
            break;
        }
        energyAvailable[0] = eeg.getEnergy() - energyTransmission[0];
        energyAvailable[1] = ecg.getEnergy() - energyTransmission[1];
        energyAvailable[2] = pulse.getEnergy() - energyTransmission[2];
        energyAvailable[3] = ms1.getEnergy() - energyTransmission[3];
        energyAvailable[4] = ms2.getEnergy() - energyTransmission[4];

        pf("\n Energy Available ");//Prints the available energy of each node after the initial handshake
        for (int i = 0; i < 5; ++i) {
            pf("%lf ", energyAvailable[i]);
        }

        pf("\n Utility Factor ");
        for(int i = 0; i < 5; i++) {// Calculates and prints utility Factor of each sensor node
            utilityFactor[i] = energyAvailable[i] / pow(dist[i], pathLossExpo);
            pf("%lf ", utilityFactor[i]);
        }

        double maxUF = utilityFactor[0];
        for (int i = 1; i < 5; i++) {
            if(utilityFactor[i] > maxUF)
                maxUF = utilityFactor[i];
        }
        pf("\n maxUF %lf ", maxUF);

        pf("\n uf ");
        for (int i = 0; i < 5; i++) {// Calculates and prints factor uf
            uf[i] = (maxUF - utilityFactor[i]) / maxUF;
            pf("%lf ", uf[i]);
        }

        double maxuf = uf[0], tmp = 0;
        for (int i = 1; i < 5; i++) {
            if(uf[i] > maxuf)
                maxuf = uf[i];
        }
        pf("\n maxuf %lf ", maxuf);

        pf("\n xi ");
        for (int i = 0; i < 5; i++) {//Calculates and prints the factor xi(Pronounced xye)
            xi[i] = uf[i] / maxuf;
            pf("%lf ", xi[i]);
        }

        bncXOld = bncX;
        bncYOld = bncY;

        tmp += xi[0] * eeg.getXCood();
        tmp += xi[1] * ecg.getXCood();
        tmp += xi[2] * pulse.getXCood();
        tmp += xi[3] * ms1.getXCood();
        tmp += xi[4] * ms2.getXCood();
        bncX = tmp / 5;

        tmp = 0;
        tmp += xi[0] * eeg.getYCood();
        tmp += xi[1] * ecg.getYCood();
        tmp += xi[2] * pulse.getYCood();
        tmp += xi[3] * ms1.getYCood();
        tmp += xi[4] * ms2.getYCood();
        bncY = tmp / 5;

        pf("\nThe new position coordinates of the BNC is %lf %lf.\n", bncX, bncY);
        cnt++;
    }
    return 0;
}
/*
Sample Input -

Enter the x and y coordinates of sensor nodes in order EEG, ECG, Pulse, MS1 amd MS2
200 90 10000
250 530 10000
30 860 10000
130 1550 10000
270 1550 10000
Enter the x and y coordinates of the BNC.
250 870

 Distance 0.781601 0.340000 0.220227 0.690507 0.680294
 Energy Available 9969.459025 9997.293506 9997.484074 9984.001883 9985.137918
 Utility Factor 42663.818546 5809741.804910 75327333.134494 88759.066582 96926.757510
 maxUF 75327333.134494
 uf 0.999434 0.922873 0.000000 0.998822 0.998713
 maxuf 0.999434
 xi 1.000000 0.923396 0.000000 0.999388 0.999279
The new position coordinates of the BNC is 166.114978 735.466771.


Enter the x and y coordinates of sensor nodes in order EEG, ECG, Pulse, MS1 amd MS2
200 90 10000
250 530 10000
30 860 10000
130 1550 10000
270 1550 10000
Enter the x and y coordinates of the BNC.
400 437

 Distance 0.400511 0.176491 0.561987 1.145281 1.120566
 Energy Available 9996.957242 9997.495686 9993.495349 9730.345314 9762.610824
 Utility Factor 2210258.538345 278119285.283952 299456.675806 4370.656766 4987.515037
 maxUF 278119285.283952
 uf 0.992053 0.000000 0.998923 0.999984 0.999982
 maxuf 0.999984
 xi 0.992068 0.000000 0.998939 1.000000 0.999998
The new position coordinates of the BNC is 125.676251 809.674048.


Enter the x and y coordinates of sensor nodes in order EEG, ECG, Pulse, MS1 amd MS2
200 90 10000
250 530 10000
30 860 10000
130 1550 10000
270 1550 10000
Enter the x and y coordinates of the BNC.
250 0


 Distance 0.102956 0.530000 0.887694 1.554638 1.550129
 Energy Available 9997.499821 9994.665941 9938.079871 8376.457665 8404.001887
 Utility Factor 6687119781.672191 423195.121217 20070.127951 620.079376 632.871930
 maxUF 6687119781.672191
 uf 0.000000 0.999937 0.999997 1.000000 1.000000
 maxuf 1.000000
 xi 0.000000 0.999937 0.999997 1.000000 1.000000
The new position coordinates of the BNC is 135.996823 897.992801.

*/




/*
Enter the x and y coordinates of sensor nodes in order EEG, ECG, Pulse, MS1 amd MS2
200 90 10000
250 530 10000
30 860 10000
130 1550 10000
270 1550 10000
Enter the x and y coordinates of the BNC.
400 437

 Distance 0.400511 0.176491 0.561987 1.145281 1.120566
 Average Transmission Energy from present position of BNC 103.819117
 Energy Available 9996.957242 9997.495686 9993.495349 9730.345314 9762.610824
 Utility Factor 2210258.538345 278119285.283952 299456.675806 4370.656766 4987.515037
 maxUF 278119285.283952
 uf 0.992053 0.000000 0.998923 0.999984 0.999982
 maxuf 0.999984
 xi 0.992068 0.000000 0.998939 1.000000 0.999998
The new position coordinates of the BNC is 125.676251 809.674048.

 Average Transmission Energy from initial position of BNC 103.819117
 Distance 0.723502 0.306062 0.108105 0.740339 0.754262
 Average Transmission Energy from present position of BNC 14.695817
 Energy Available 9979.722334 9997.388966 9997.499761 9977.137997 9974.771857
 Utility Factor 67363.550069 10804652.059627 5014252254.535425 58798.566211 52664.778290
 maxUF 5014252254.535425
 uf 0.999987 0.997845 0.000000 0.999988 0.999989
 maxuf 0.999989
 xi 0.999997 0.997856 0.000000 0.999999 1.000000
The new position coordinates of the BNC is 169.892636 743.772271.

 Average Transmission Energy from initial position of BNC 14.695817
 Distance 0.654465 0.228289 0.181876 0.807214 0.812419
 Average Transmission Energy from present position of BNC 18.301479

 The final coordinates of the BNC is 169.892636 743.772271.
*/
