#include <esp_now.h>
#include <WiFi.h>
#define LEDRED 2
#define LEDAMBAR 4
int tiempo, i;

/* HECTOR */

typedef struct STRUCT_MENSAJE 
  {
    int  VALOR;
  } STRUCT_MENSAJE;


STRUCT_MENSAJE MI_DATO;


void RECIBO_DE_DATOS(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&MI_DATO, incomingData, sizeof(MI_DATO));
  Serial.print("Int: ");
  Serial.println(MI_DATO.VALOR);

}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDAMBAR, OUTPUT);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ERRO INICIALIZAR ESP-NOW");
    return;
  }

}
void ALARMA_LUZ()
{   
  /////////////////////////////////AMBAR 10s///////////////////////////
   for(int i = 0; i < 10; i++)
    {
      digitalWrite(LEDAMBAR, HIGH);
      delay(500);
      digitalWrite(LEDAMBAR, LOW);
      delay(500);
    }
/////////////////////////////////AMBAR RED 5s///////////////////////////
  for(int j = 0; j < 5; j++)
    {
      digitalWrite(LEDRED, HIGH);
      delay(500);
      digitalWrite(LEDRED, LOW);
      delay(500);
    }

///////////////////////////////// RED 5s///////////////////////////

  digitalWrite(LEDRED, HIGH);
  delay(5000);
  digitalWrite(LEDRED, LOW);
  delay(10);

  
}



 
void loop() 
{

  esp_now_register_recv_cb(RECIBO_DE_DATOS);
  if(MI_DATO.VALOR == 1)
  {
    delay(10);
    ALARMA_LUZ();
    delay(10);
   
  }
  else if (MI_DATO.VALOR == 0)
  {
    digitalWrite(LEDRED, LOW);
    digitalWrite(LEDAMBAR, LOW);
  }

}
