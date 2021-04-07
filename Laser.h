// Library for JRT Laser Rangefinding module
// Author: C. R. Drane, Philomaths Pty Limited.
#ifndef LASER_H
#define LASER_H
#include <Wire.h>
#include <Arduino.h>

struct LaserDistance {
  unsigned long distance, signalQuality;
};

class Laser{

  static const int MaxBytes = 13;

  struct LaserBuffer {
    byte e[13] = {0};
  };

public:

    int laser_pin;
    long int timeout;
    bool debug;

    Laser(int laser_reset_pin = -1, bool laser_debug = false, long int laser_timeout = 1000) {
      laser_pin = laser_reset_pin;
      timeout = laser_timeout;
      debug = laser_debug;
    }

    void init() const;

    byte status() const;

    byte voltage() const;

    unsigned long software_version() const;

    byte laser_on() const;

    byte laser_off() const;

    LaserDistance single_shot() const;

    byte autobaud() const;

    void test() const;

    LaserBuffer read(int nBytes) const;

    void flush(char txt[20] = "") const;

  private:

    void checkNBytes(int nBytes)  const;

    bool check_response(LaserBuffer buffer, int nBytesAnswer, String callerName) const;

    void buffer_print(LaserBuffer buffer, int nBytes = MaxBytes) const;

    bool check_num_bytes(int n_bytes, int n_bytes_received) const;

};
#endif

