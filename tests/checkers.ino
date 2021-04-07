// functions to check test results

void check_scalar(double s, double sCheck, String test_name, double toler = 0.00001){
  double sDiff = s - sCheck;
  bool ok = abs(s - sCheck) < toler;
  if(ok){
    Serial.print('.');
  } else {
    Serial.println("");
    Serial.print(test_name);Serial.println(" test failed, expected");
    Serial.println(sCheck);
    Serial.println("but result was ");Serial.println(s);
  }
}

void check_bool(bool b, bool b_check, String test_name){
  bool ok = (b == b_check);
  if(ok){
    Serial.print('.');
  } else {
    Serial.println("");
    Serial.print(test_name);Serial.println(" test failed, expected");
    Serial.println(b_check);
    Serial.println("but result was ");Serial.println(b);
  }
}

void check_int(int i, int i_check, String test_name){
  bool ok = (i == i_check);
  if(ok){
    Serial.print('.');
  } else {
    Serial.println("");
    Serial.print(test_name);Serial.println(" test failed, expected");
    Serial.println(i_check, HEX);
    Serial.println("but result was ");Serial.println(i, HEX);
  }
}
template <typename T>
bool check_in_range(T i, T i_low, T i_high, String test_name, int fmt){
  // int fmt = HEX;
  bool ok = ( i >= i_low && i <= i_high );
  if(ok){
    Serial.print('.');
  } else {
    Serial.println("");
    Serial.print(test_name);Serial.println(" test failed, expected between ");
    Serial.print(i_low, fmt);Serial.print(" to ");Serial.println(i_high, fmt);
    Serial.println("but result was ");Serial.println(i, fmt);
  }
  return ok;
}

template <typename T> 
void check_num(T i, T i_check, String test_name){
  bool ok = (i == i_check);
  if(ok){
    Serial.print('.');
  } else {
    Serial.println("");
    Serial.print(test_name);Serial.println(" test failed, expected");
    Serial.println(i_check, HEX);
    Serial.println("but result was ");Serial.println(i, HEX);
  }
}