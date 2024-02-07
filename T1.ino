#include <Arduino_FreeRTOS.h>
#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);


const float BETA = 3950; // should match the Beta Coefficient of the thermistor

void setup() {

  dht.begin();
  Serial.begin(115200);

  Serial.println();

  // Create a task for reading temperature

  xTaskCreate(readTemperatureTask, "ReadTemp", 200, NULL, 1, NULL);
  
}

void loop() {
  // The scheduler will handle the tasks, so the loop is empty
}

void readTemperatureTask(void *pvParameters) {
  (void)pvParameters;

  while (1) {
    // Read temperature from ntcPin sensor
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

    t = t +1.5;

    // Delay for a while
    vTaskDelay(pdMS_TO_TICKS(1000)); // 1-second delay
  }
}


int flag=0; 
static char *ptr=NULL;
int ind=0; //for index

// Task: Perform some mundane task
void testTask(void *parameter) {
  unsigned char bytes='0';
  while (1) {
    

      if(Serial.available())
      {
            ptr = (char*)pvPortMalloc(20 * sizeof(char));
            // One way to prevent heap overflow is to check the malloc output
            if (ptr == NULL) {
              Serial.println("Not enough heap.");
              vPortFree(NULL);
            } 
            else {
                    bytes=Serial.read();
                    if(bytes=='\n')
                    {
                      
                      Serial.println("Turning on flag");
                      //*(ptr+(ind))='\0';                     
                      /*for(int i=0;i<ind;i++)
                      {
                        Serial.println(*(ptr+i));
                      }*/
                      flag=1;
                      Serial.println("After turning flag");
                      vTaskDelay(3000 / portTICK_PERIOD_MS);         
                     }
                    else{                      
                          //*(ptr+ind)=bytes;
                          ptr[ind]=bytes;                        
                          //Serial.println(ptr[ind]);
                          Serial.println(*(ptr+ind));                    
                          ind++;                   
                        }
                    }
     }

    // Wait for a while
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void printTask(void *parameter) {

  int i=0;
  while(1){
    if(flag==1)
    {
      Serial.println("In receiving task");
      Serial.println(ind);
      for(int i=0;i<ind;i++)
       {
        Serial.println(*(ptr+i));
       }

      flag=0;
      ind=0;
      vTaskDelay(100 / portTICK_PERIOD_MS);
      vPortFree(ptr);
      ptr=NULL; 
    }
    vTaskDelay(100 / portTICK_PERIOD_MS); 
   
  }
}


