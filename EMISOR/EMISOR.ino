////////////**************ESP-32-MASTER*************///////////
////////////**************MAC-9C:9C:1F:EA:F3:E8*****///////////

////////LIBRERIAS
#include "esp_now.h"    ////Comunicacion Unidireccional
#include "WiFi.h"       ////Red WiFi     
#define PINLDR 21
#define LEDB 14
int VALOR_PINLDR = 0;
////Direccion MAC
uint8_t BROADCAST_ADDRESS [] = {0X9C,0X9C,0X1F,0XEA,0XF7,0X20};      ////////Direccion MAC del SLAVE
typedef struct STRUCT_MENSAJE
  {
    int  VALOR;
  }STRUCT_MENSAJE;

STRUCT_MENSAJE MI_DATO;
esp_now_peer_info_t INFORMACION;

void  ENVIO_DE_DATOS(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nESTADO DE ENVIO\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "ENVIO EXITOS" : "FALLA EN EL ENVIO");
}



void setup() 
{
  Serial.begin(115200);
  pinMode(PINLDR, INPUT);
  pinMode(LEDB,OUTPUT);
  WiFi.mode(WIFI_STA);
  /////////Iniciamos WiFi del ESP-NOW
  if (esp_now_init() != ESP_OK)
    {
      Serial.print("ERRO AL INICIAR ESP-NOW");
      return; 
    }
  esp_now_register_send_cb(ENVIO_DE_DATOS);
  
  memcpy(INFORMACION.peer_addr, BROADCAST_ADDRESS, 6);
  INFORMACION.channel = 0;  
  INFORMACION.encrypt = false;

  if (esp_now_add_peer(&INFORMACION) != ESP_OK)
    {
      Serial.println("ERROR AL COMUNICARSE CON EL SLAVE");
      return;
    } 
  delay(20000);   
}

void loop() 
{
  VALOR_PINLDR = digitalRead(PINLDR);
  MI_DATO.VALOR = VALOR_PINLDR;
    if(MI_DATO.VALOR != 0)
    {
      esp_err_t RESULTADO = esp_now_send(BROADCAST_ADDRESS, (uint8_t *) &MI_DATO, sizeof(MI_DATO));
      digitalWrite(LEDB, LOW);
      delay(20000);       
    }
    else if (MI_DATO.VALOR != 1)
    { 
    delay(10);  
    esp_err_t RESULTADO = esp_now_send(BROADCAST_ADDRESS, (uint8_t *) &MI_DATO, sizeof(MI_DATO));
      digitalWrite(LEDB, HIGH);  
    }
}
