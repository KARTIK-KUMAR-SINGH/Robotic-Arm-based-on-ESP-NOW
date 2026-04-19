#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Servo.h>

Servo s1,s2,s3,s4,s5;

typedef struct {
  int j1x,j1y,j2x,j2y,j3;
  bool reset;
  int s1,s2,s3,s4,s5;
} Data;

Data data;
uint8_t senderMAC[6];

int pos1=90,pos2=90,pos3=90,pos4=90,pos5=90;

int speedDelay=5;
int deadZone=30;

void onReceive(uint8_t *mac,uint8_t *incomingData,uint8_t len){
  memcpy(senderMAC,mac,6);
  memcpy(&data,incomingData,sizeof(data));
}

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // 🔥 SERVO PINS (UNCHANGED)
  s1.attach(D1,500,2400);
  s2.attach(D2,500,2400);
  s3.attach(D5,500,2400);
  s4.attach(D6,500,2400);
  s5.attach(D7,500,2400);

  esp_now_init();
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(onReceive);
}

void loop(){

  // 🔘 RESET SEQUENCE
  if(data.reset){
    int d=300;

    pos1=90; s1.write(pos1); delay(d);
    pos2=90; s2.write(pos2); delay(d);
    pos3=90; s3.write(pos3); delay(d);
    pos4=90; s4.write(pos4); delay(d);
    pos5=90; s5.write(pos5); delay(d);

    data.reset=false;
    return;
  }

  // ▶ EXECUTE SAVED POSE (SEQUENTIAL)
  if (data.j1x == -999) {

    int d = 500;   // 🔥 REQUIRED DELAY

    pos1 = data.s1; s1.write(pos1); delay(d);
    pos2 = data.s2; s2.write(pos2); delay(d);
    pos3 = data.s3; s3.write(pos3); delay(d);
    pos4 = data.s4; s4.write(pos4); delay(d);
    pos5 = data.s5; s5.write(pos5); delay(d);

    data.j1x = 0; // clear flag
    return;
  }

  // 🎮 NORMAL CONTROL
  int d1x=data.j1x-512;
  int d1y=data.j1y-512;
  int d2x=data.j2x-512;
  int d2y=data.j2y-512;
  int d3=data.j3-512;

  if(abs(d1x)>deadZone) pos1+=d1x/100;
  if(abs(d1y)>deadZone) pos2+=d1y/100;
  if(abs(d2x)>deadZone) pos3+=d2x/100;
  if(abs(d2y)>deadZone) pos4+=d2y/100;
  if(abs(d3)>deadZone) pos5+=d3/100;

  pos1=constrain(pos1,0,180);
  pos2=constrain(pos2,0,180);
  pos3=constrain(pos3,0,180);
  pos4=constrain(pos4,0,180);
  pos5=constrain(pos5,0,180);

  s1.write(pos1);
  s2.write(pos2);
  s3.write(pos3);
  s4.write(pos4);
  s5.write(pos5);

  // 🔁 SEND BACK FEEDBACK
  data.s1=pos1;
  data.s2=pos2;
  data.s3=pos3;
  data.s4=pos4;
  data.s5=pos5;

  esp_now_send(senderMAC,(uint8_t*)&data,sizeof(data));

  delay(speedDelay);
}

