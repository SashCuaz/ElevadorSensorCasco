////////////**************ESP-32-MASTER*************///////////
////////////**************MAC-9C:9C:1F:EA:F3:E8*****///////////
////////LIBRERIAS
#include "esp_now.h"    ////Comunicacion Unidireccional
#include "WiFi.h"       ////Red WiFi     
#include "Wire.h"
#include "TFMPI2C.h"
TFMPI2C tfmP;           
///////////////////////Pines OUTPUT
#define PIN_SDA 21
#define PIN_SCL 22
#define LEDB 14
int VALOR_PINLDR = 0;
int16_t tfDist = 0;      
int16_t tfFlux = 0;      
int16_t tfTemp = 0;       
////Direccion MAC/////////////////////////////////////////////////////////////////////////////////////
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
  pinMode(LEDB,OUTPUT);
  WiFi.mode(WIFI_STA);
  /////////Iniciamos WiFi del ESP-NOW
  if (esp_now_init() != ESP_OK)
    {
      Serial.print("ERRO AL INICIAR ESP-NOW");
      return; 
    }
//////////////////////////////////////////////////////////////////////////////
  tfmP.recoverI2CBus(PIN_SDA,PIN_SCL);  
    printf( "SYSTEM RESET: ");
    if( tfmP.sendCommand( SOFT_RESET, 0))
    {
        printf( "PASSED.\r\n");
    }
    else tfmP.printReply();
/////////////////////////////////////////////////////////////////////////////
printf( "FIRMWARE VERSION: ");
    if( tfmP.sendCommand( GET_FIRMWARE_VERSION, 0))
    {
        printf( "%1u.",  tfmP.version[ 0]); 
        printf( "%1u.",  tfmP.version[ 1]); 
        printf( "%1u\n", tfmP.version[ 2]);
    }
    else tfmP.printReply();

    printf( "DATA-FRAME RATE: ");
    if( tfmP.sendCommand( SET_FRAME_RATE, FRAME_20))
    {
        printf( "%2uHz.\n", FRAME_20);
    }
    else tfmP.printReply();
/////////////////////////////////////////////////////////////////////////////
 esp_now_register_send_cb(ENVIO_DE_DATOS);
  memcpy(INFORMACION.peer_addr, BROADCAST_ADDRESS, 6);
  INFORMACION.channel = 0;  
  INFORMACION.encrypt = false;

  if (esp_now_add_peer(&INFORMACION) != ESP_OK)
    {
      Serial.println("ERROR AL COMUNICARSE CON EL SLAVE");
      return;
    } 
}

void loop() 
{
  tfmP.getData( tfDist, tfFlux, tfTemp);
      Serial.printf( "Dist:%04icm ", tfDist);   
      delay(10);
      if (tfDist < 100)
        {
          VALOR_PINLDR = 1;
        }
      else 
        {
          VALOR_PINLDR = 0;
        }
    Serial.println(VALOR_PINLDR); 

  
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
