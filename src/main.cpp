/*
 PMS7003 датчик пыли отдельный в ESP-12
*/
#include "esp_OTA.h"
#include "pms.h"
#include "Net.h"

int pinLB = 2;   // led board 2-ESP12
int qq;

void setup() 
{
    OTA_setup();
    pmsInit();
    setupNet(); //Blynk.begin(auth, STASSID, STAPSK, "192.168.1.147", 8080);
    TerminalP("pmsON");
}


int c = 0, pmscount, pmsInterval = 120, re = 0, SendCount = 0, csend = 0, stat = 99;
unsigned long tt;
void loop() 
{
    OTA_loop();
    Blynk.run();

    if ((millis() - tt) > 200) // 0.2s
    {
        tt = millis();
        if(re == 2) { LED(pinLB, 1); } //прогрев
        if (++c >= 5) // 1s
        {   
            c = 0;  //TerminalP("*");
            re = pmsProc(pmsInterval);
            if(re != 2) LED(pinLB, 1);
            if(re == 1)
            { 
                //Send(VIRT_PMS_C, ++SendCount);
                int p3 = pmsGetData(VIRT_PMS_3); //Send(VIRT_PMS_3, p3);
                int p5 = pmsGetData(VIRT_PMS_5); //Send(VIRT_PMS_5, p5);
                int p10 = pmsGetData(VIRT_PMS_10); //Send(VIRT_PMS_10, p10);
                int p25 = pmsGetData(VIRT_PMS_25); //Send(VIRT_PMS_25, p25);
                String msg = "pms" + String(SendCount, DEC) + ":" + String(p3, DEC) + "," + String(p5, DEC) + "," + String(p10, DEC) + "," + String(p25, DEC);
                if(++SendCount == 50){pmsInterval = 600; msg += "#"; } //потом реже

                TerminalP(msg);
                stat = 0;
            } 
            else 
            {
                //нужно периодически отправлять - если редко то на графике пропуски
                if (++csend > 122) //60 норм
                {
                    csend = 0;
                    Send(VIRT_PMS_C, SendCount);
                }
            }

            switch(stat)
            {
                // case 1:  Send(VIRT_PMS_C, SendCount++); break;
                case 2:  Send(VIRT_PMS_3, pmsGetData(VIRT_PMS_3)); break;
                case 3:  Send(VIRT_PMS_5, pmsGetData(VIRT_PMS_5)); break;
                case 4:  Send(VIRT_PMS_10, pmsGetData(VIRT_PMS_10));  break;
                case 5:  Send(VIRT_PMS_25, pmsGetData(VIRT_PMS_25));  break;
            }
            stat++;
        }
    }
    delay(11); LED(pinLB, 0); delay(11);
}


