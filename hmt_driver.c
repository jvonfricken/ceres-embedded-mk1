//
// Created by Joshua VonFricken on 10/5/19.
//

#include "hmt_driver.h"
#include "wiringPi.h"
#include <stdio.h>
#include "time.h"
#include "sys/time.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define DHT_DATA_PIN 8
#define MAX_TIMINGS 82

void read_sensor() {
    while (TRUE) {
        pinMode(DHT_DATA_PIN, OUTPUT);
        digitalWrite(DHT_DATA_PIN, LOW);
        delay(18);
        pinMode(DHT_DATA_PIN, INPUT);
        //pullUpDnControl(DHT_DATA_PIN, PUD_UP);

        int dht_val = 1;
        int read_count = 0;
        double diff_time = 0;
        time_t start_time = time(NULL);

        struct timeval pulse_times[MAX_TIMINGS];

        while (diff_time < 0.25 && read_count < MAX_TIMINGS) {
            int dht_pin_val = digitalRead(DHT_DATA_PIN);
            if (dht_pin_val != dht_val) {
                // Pin toggled
                dht_val = !dht_val;
                struct timeval pulse_time;
                gettimeofday(&pulse_time, NULL);
                pulse_times[read_count] = pulse_time;
                read_count++;
            }

            diff_time = difftime(time(NULL), start_time);
        }

        int bits[40];
        int bit_count = 0;
        //Skipping first two pulses ~ notification pulses
        for (int i = 2; i < MAX_TIMINGS; i++) {
            double pulse_diff = pulse_times[i + 1].tv_usec - pulse_times[i].tv_usec;
            if (pulse_diff <= 40) {
                bits[bit_count] = 0;
                bit_count++;
            } else if (pulse_diff >= 65) {
                bits[bit_count] = 1;
                bit_count++;
            }
        }

        int byte = 0;
        int byte_i = 0;
        int bytes[5];
        for (int i = 0; i < 50; i++) {
            byte = byte << 1 | bits[i];
            if ((i + 1) % 8 == 0) {
                bytes[byte_i] = byte;
                byte = 0;
                byte_i++;
            }
        }

        int check_sum = 0;
        for (int i = 0; i < 4; i++) {
            check_sum += bytes[i];
        }

        //Failed checksum validation
        if (check_sum != bytes[4]) {
            printf("Failed to validate checksum\n");
            delay(2000);
            continue;
        }

        int temp_C = bytes[2];
        int humidity = bytes[0];
        printf("Temp: %d Humidity: %d\n", temp_C, humidity);

        delay(2000);
    }
}


#pragma clang diagnostic pop