#include <esp_now.h>
#include <WiFi.h>
#define LEDRED 2
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    bool d;
} struct_message;


struct_message myData;


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  Serial.print("Float: ");
  Serial.println(myData.c);
  Serial.print("Bool: ");
  Serial.println(myData.d);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(LEDRED, OUTPUT);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ERRO INICIALIZAR ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() 
{

  if(myData.b == 8)
  {
    digitalWrite(LEDRED, HIGH);
  }
  else 
  {
    digitalWrite(LEDRED, LOW);
  }

}
