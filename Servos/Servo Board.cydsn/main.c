
#include <project.h>
#include <stdlib.h> 
#include <stdio.h>
#include <math.h>

#define USBUART_BUFFER_SIZE 64

char msg[USBUART_BUFFER_SIZE];    
   
//variables UART
char8* parity[] = {"None", "Odd", "Even", "Mark", "Space"};
char8* stopm[]   = {"1", "1.5", "2"};
uint16 count;
uint8 buffer[USBUART_BUFFER_SIZE];
void UARTconfig ();
//Servo Signal param//20ms - 64000 COUNTS
//Potentiometer Angle 265  Servo Range (200Deg Right Angle),(265 Straight) 
//PWM COMPARE VALUES
#define PWMMIN 3250 //1ms
#define PWMMID 4800 //1.5ms
#define PWMMAX 6350 //2ms

//--- anglelimit for joint type
//angle limits
#define Servo_MIN -100
#define Servo_MAX 100
#define ang_lim(x) (x<Servo_MIN ? Servo_MIN : (x>Servo_MAX ? Servo_MAX : x))

///----LEGS
#define servo1(x) PWMA_WriteCompare1(x); //Front Left
#define servo2(x) PWMA_WriteCompare2(x); //Front Right
//#define servo3(x) PWMB_WriteCompare1(x); //Back Left
//#define servo4(x) PWMB_WriteCompare2(x); //Back Right

int anglegs[] ={0,0,0,0},a=1,pwmlegs[] ={0,0,0,0};
///----SHOULDER


///----ARMS


void degtopwm() {
   int i;
    //for(i=0;i<4;i++){
    i=0;
        anglegs[i]=ang_lim(anglegs[i]);
        pwmlegs[i]=(anglegs[i]*0.01)*1550+PWMMID;
    //}
    
}
void servooutput(){
    degtopwm();
    servo1(pwmlegs[0]);
    servo2(pwmlegs[1]);
    //servo3(anglegs[1]);
   // servo4(anglegs[1]);
}
int main(){  
    CyGlobalIntEnable;/* Enable global interrupts */
    UARTconfig();
    USBUART_PutString("Spider");  
    PWMA_Start();  
    for(;;)
    {
        if(anglegs[0]>95) a=-1; 
        else if(anglegs[0]<-95)a=1;
        anglegs[0]=anglegs[0]+a*5;  
        sprintf(msg,"%i\r\n",pwmlegs[0]);
        USBUART_PutString(msg); 
        servooutput();
        CyDelay(50);
    }
}

void UARTconfig (){
    if (0u != USBUART_IsConfigurationChanged()) {//Initialize IN endpoints when device is configured.             
        if (0u != USBUART_GetConfiguration()) {// Enumeration is done, enable OUT endpoint to receive data from host.
            USBUART_CDC_Init();
        }
    }

       //Service USB CDC when device is configured. 
        if (0u != USBUART_GetConfiguration()){ // Check for input data from host. 
           
            if (0u != USBUART_DataIsReady()){// Read received data and re-enable OUT endpoint. 
                count = USBUART_GetAll(buffer);
                if (0u != count){//Wait until component is ready to send data to host.
                    while (0u == USBUART_CDCIsReady()){}
                    //Send data back to host.
                    USBUART_PutString("and");                   
                    if (USBUART_BUFFER_SIZE == count){// Wait until component is ready to send data to PC. 
                        while (0u == USBUART_CDCIsReady()){}                        
                        USBUART_PutChar('\r');  
                        USBUART_PutChar('\n');  
                    }
                }
            }  
        }
}