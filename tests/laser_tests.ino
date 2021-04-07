// test for laser functions
// test_no = 4
void laser_tests(){
  if(test_num > 0 && test_num != 4) return;

  delay(2000);
  Laser laser;
  laser.init();

  unsigned long version = laser.software_version();
  check_num(version, (unsigned long)0x0917, "laser.version");

  byte status = laser.status();
  check_num(status, (byte) 0, "laser.status");

  int voltage = laser.voltage();
  check_in_range(voltage, 0x30, 0x36, "laser.voltage", HEX);
  
  LaserDistance res = laser.single_shot();
  bool r_ok = check_in_range(res.distance, (unsigned long) 20, (unsigned long) 150, "laser.single_shot", DEC);
  if(!r_ok) Serial.println("Try holding hand about 100mm from laser");
}
