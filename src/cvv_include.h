#ifndef COMM_H
#define COMM_H

#ifndef STASSID
  #define STASSID ""
  #define STAPSK  ""
#endif

String gss;

#define sp Serial.print
#define sp_ Serial.print(" ");
#define spok Serial.print(" ok ");
#define sp1   Serial.print("<====1====>");
#define spn  Serial.print("\r\n");

void sph(const char *h, int i)
{
  Serial.print(h), Serial.print("="), Serial.print(i); Serial.print(" ");
}

void sph(int i){Serial.print(i); Serial.print(" ");}
void sph(String s){Serial.print(s); Serial.print(" ");}


void LED(int pin, int v) {
  pinMode(pin, OUTPUT);  //pinMode(DATA_PIN, OUTPUT);
  if (v) digitalWrite(pin, 0);
  else digitalWrite(pin, 1);
}



#endif
