// functions for JRT Laser Rangefinding module
#include <Laser.h>
#include <Arduino.h>
#include <Wire.h>

byte Laser::status() const{
  LaserBuffer buffer;
  int nBytesAnswer = 9;
  byte commandStr[] = {0xAA, 0x80, 0x00, 0x00, 0x80};
  Serial3.write(commandStr, sizeof(commandStr));
  Serial3.flush();
  buffer = read(nBytesAnswer);
  bool ok = check_response(buffer, nBytesAnswer, "rangerStatus");
  return buffer.e[7];
}

byte Laser::voltage() const{
  LaserBuffer buffer;
  int nBytesAnswer = 9;
  byte commandStr[] = {0xAA, 0x80, 0x00, 0x06, 0x86};
  Serial3.write(commandStr, sizeof(commandStr));
  Serial3.flush();
  buffer = read(nBytesAnswer);
  bool ok = check_response(buffer, nBytesAnswer, "rangerVoltage");
  return buffer.e[6];
}

unsigned long Laser::software_version() const{
  LaserBuffer buffer;
  unsigned long version;
  int nBytesAnswer = 9;
  byte commandStr[] = {0xAA, 0x80, 0x00, 0x0C, 0x8C};
  Serial3.write(commandStr, sizeof(commandStr));
  Serial3.flush();
  buffer = read(nBytesAnswer);
  bool ok = check_response(buffer, nBytesAnswer, "rangerVersion");
  version = (unsigned long) buffer.e[6] << 8
      | (unsigned long)  buffer.e[7];
  return version;
}

byte Laser::laser_on() const{
  LaserBuffer buffer;
  int nBytesAnswer = 9;
  byte commandStr[] = {0xAA, 0x00, 0x01, 0xBE, 0x00, 0x01, 0x00, 0x01, 0xC1};
  Serial3.write(commandStr, sizeof(commandStr));
  Serial3.flush();
  buffer = read(nBytesAnswer);
  bool ok = check_response(buffer, nBytesAnswer, "rangerLaserOn");
  return buffer.e[7];
}


byte Laser::laser_off() const{
  LaserBuffer buffer;
  int nBytesAnswer = 9;
  byte commandStr[] = {0xAA, 0x00, 0x01, 0xBE, 0x00, 0x01, 0x00, 0x00, 0xC0};
  Serial3.write(commandStr, sizeof(commandStr));
  Serial3.flush();
  buffer = read(nBytesAnswer);
  bool ok = check_response(buffer, nBytesAnswer, "rangerLaserOff");
  return buffer.e[7];
}

LaserDistance Laser::single_shot() const{
  LaserBuffer buffer;
  LaserDistance results;
  int nBytesAnswer = 13;
  byte commandStr[] = {0xAA, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x00, 0x21};
  Serial3.write(commandStr, sizeof(commandStr));
  Serial3.flush();
  delay(150); // wait for measurement
  buffer = read(nBytesAnswer);
  bool ok = check_response(buffer, nBytesAnswer, "rangerSingleShot");
  results.distance = (unsigned long) buffer.e[6] << 24
    | (unsigned long)  buffer.e[7] << 16
    | (unsigned long)  buffer.e[8] << 8
    | (unsigned long)  buffer.e[9];
  results.signalQuality = (unsigned long) buffer.e[10] << 8
    | (unsigned long)  buffer.e[11];
  return results;
}


byte Laser::autobaud() const{
  LaserBuffer buffer;
  int nBytesAnswer = 1;
  byte commandStr[] = {0x55};
  Serial3.write(commandStr, sizeof(commandStr));
  Serial3.flush();
  buffer = read(nBytesAnswer);
  bool ok = check_response(buffer, nBytesAnswer, "rangerAutobaud");
  return buffer.e[7];
}

void Laser::test() const{
  byte status = Laser::status();
  if(status == 0) {
    Serial.println("Status OK");
  }
  else {
    Serial.print("Status check failed with: ");
    Serial.println(status, HEX);
  }
  int voltage = Laser::voltage();
  if(voltage > 0x30 && voltage < 0x36) {
    Serial.print("Voltage OK: ");Serial.println(voltage, HEX);
  }
  else {Serial.println("Voltage check failed with: ");Serial.println(voltage, HEX);}
  Laser::laser_on();
  LaserDistance res = Laser::single_shot();
  Serial.print("Ranger Distance, SQ: ");Serial.print(res.distance);Serial.print(", ");Serial.println(res.signalQuality);
}

void Laser::flush(char txt[20]) const{
  unsigned long n_bytes = 0;
  byte b;
  while (Serial3.available() > 0) {
    n_bytes += 1;
    b = Serial3.read();
    if(n_bytes == 1) {
      Serial.print(txt);
      Serial.print("laser.flush found data: ");
    }
    Serial.print(b, HEX);
  }
  if(n_bytes > 0) Serial.println("");
}

Laser::LaserBuffer Laser::read(int n_bytes) const{
  LaserBuffer laser_buffer;
  byte buffer[MaxBytes] = {0xFF};
  int n_bytes_read;
  checkNBytes(n_bytes);
  n_bytes_read = Serial3.readBytes(buffer, n_bytes);
  check_num_bytes(n_bytes, n_bytes_read);
  if(debug)Serial.print("Start debug: ");
  for (int i = 0; i < n_bytes_read ; i++){
    laser_buffer.e[i] = buffer[i];
    if (debug) {
      Serial.print(laser_buffer.e[i], HEX);Serial.print(' ');
    }
  }
  if (debug) Serial.println(" End of debug");
  return laser_buffer;
}

void Laser::checkNBytes(int nBytes) const{
  if(nBytes <= MaxBytes){ return; }
  Serial.print("GME - fatal - nBytes larger than MaxBytes: ");
  Serial.println(nBytes);
}

bool Laser::check_response(LaserBuffer buffer, int nBytesAnswer, String callerName) const{
  if (buffer.e[0] == 0xAA) { return true; }
  Serial.print("GME - fatal - Incorrect answer from ");
  Serial.print(callerName);Serial.print(": ");
  buffer_print(buffer, nBytesAnswer);
  return false;
}

bool Laser::check_num_bytes(int n_bytes, int n_bytes_read) const{
  if (n_bytes_read == n_bytes) { return true; }
  Serial.print("GME - warning - insufficient bytes, expected: ");
  Serial.print(n_bytes);Serial.print(", but only received ");
  Serial.println(n_bytes_read);
  return false;
}

void Laser::buffer_print(LaserBuffer buffer, int nBytes) const{
  for (int i = 0; i <= (nBytes - 1); i++){
    Serial.print(buffer.e[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
}

void Laser::init() const{
  Serial3.begin(19200);
  Serial3.setTimeout(timeout);
  delay(2000);
  if(laser_pin > -1){
    pinMode(laser_pin, OUTPUT);
    digitalWrite(laser_pin, LOW);
    delay(200);
    digitalWrite(laser_pin, HIGH);
    delay(200);
    Laser::autobaud();
  }
}

