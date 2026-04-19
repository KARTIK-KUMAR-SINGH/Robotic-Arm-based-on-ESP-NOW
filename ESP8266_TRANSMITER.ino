#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t receiverMAC[] = {0x08, 0xF9, 0xE0, 0x63, 0xE5, 0x6B};
typedef struct {
  int j1x,j1y,j2x,j2y,j3;
  bool reset;
  int s1,s2,s3,s4,s5; // feedback + EXEC data
} Data;

Data data;

// 🔁 RECEIVE FROM RECEIVER → SEND TO MEGA
void onReceive(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&data, incomingData, sizeof(data));

  Serial.print("FB,");
  Serial.print(data.s1); Serial.print(",");
  Serial.print(data.s2); Serial.print(",");
  Serial.print(data.s3); Serial.print(",");
  Serial.print(data.s4); Serial.print(",");
  Serial.println(data.s5);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  esp_now_init();

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_add_peer(receiverMAC, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  esp_now_register_recv_cb(onReceive);
}

void loop() {

  if (Serial.available()) {

    String input = Serial.readStringUntil('\n');
    input.trim();

    // 🔘 RESET
    if (input == "RESET") {
      data.reset = true;
      esp_now_send(receiverMAC,(uint8_t*)&data,sizeof(data));
      return;
    }

    // ▶ EXECUTE SAVED POSE
    if (input.startsWith("EXEC")) {

      int a,b,c,d,e;

      sscanf(input.c_str(),"EXEC,%d,%d,%d,%d,%d",&a,&b,&c,&d,&e);

      data.s1=a;
      data.s2=b;
      data.s3=c;
      data.s4=d;
      data.s5=e;

      data.j1x = -999;   // 🔥 EXEC FLAG
      data.reset = false;

      esp_now_send(receiverMAC,(uint8_t*)&data,sizeof(data));
      return;
    }

    // 🎮 NORMAL JOYSTICK DATA
    int v1,v2,v3,v4,v5;

    if (sscanf(input.c_str(),"%d,%d,%d,%d,%d",&v1,&v2,&v3,&v4,&v5)==5) {
      data.j1x=v1;
      data.j1y=v2;
      data.j2x=v3;
      data.j2y=v4;
      data.j3=v5;
      data.reset=false;

      esp_now_send(receiverMAC,(uint8_t*)&data,sizeof(data));
    }
  }
}
