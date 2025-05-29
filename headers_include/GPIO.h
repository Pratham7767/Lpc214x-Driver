#include "lpc214x.h" 
// PinWrite function 

    // for PORT 0 we are using 0 to 31 as pinnum 
    // for port 1 we are using 100+1 to 100+31 as pinnum just to reduce the parameters to be passed 
    //and they are mapped accordingly
    //For more clarification visit the readme.md section :) 

// this function makes a pin either HIGH or LOW since we dont have pin-selective function on LPC214x

void pinWrite(int pinnum , bool pinval){


if (pinnum>=0 & pinnum<=31){
    //here we consider port 0
    IODIR0 = IODIR0|(0x01<<pinnum); //setting pin as output 
    if (pinval==0)
    {IOCLR0 = IOCLR0|(0x01<<pinnum);}
    else 
    {IOSET0 = IOSET0|(0x01<<pinnum);} 
}

else if (pinnum>=100 & pinnum<=131){
pinnum= pinnum-100;

    IODIR1 = IODIR1|(0x01<<pinnum); //setting pin as output 
    if (pinval==0)
    {IOCLR1 = IOCLR1|(0x01<<pinnum);}
    else 
    {IOSET1 = IOSET1|(0x01<<pinnum);} 

}

else {
    //do nothing bro
}

}


// PortWrite function
/* This can be used to make ports of the lpc214x HIGH or LOW selectively in group of a byte 
Value of portnum
Port 0 :0--> 0-7 bits, 
Port 0 :1--> 8-15 bits, 
Port 0 :2--> 16-23 bits, 
Port 0 :3--> 24-31 bits, some pins are inactive look into datasheet 

Port 1 :10--> 0-7 bits, they are inactive 
Port 1 :11--> 8-15 bits, they are inactive  
Port 1 :12--> 16-23 bits, 
Port 1 :13--> 24-31 bits

portval can range from 0x00 to 0xFF bits values are used at once 
*/
void PortWrite(int portnum , unsigned int portval){
    unsigned int i;
if (portval>=0x00 & portval<=0xFF){
    if (portnum>=0 & portnum<=3) {
        if (portnum==0){
            for(i=0;i<8;i++){
                if ((portval & (0x01<<i))==0){
                    pinWrite(i,0);
                }
                else{
                    pinWrite(i,1);
                }
            }
        }

        else if (portnum==1){
            for(i=0;i<8;i++){
                if ((portval & (0x01<<i))==0){
                    pinWrite(i+8,0);
                }
                else{
                    pinWrite(i+8,1);
                }
            }
        }

        else if (portnum==2){
            for(i=0;i<8;i++){
                if ((portval & (0x01<<i))==0){
                    pinWrite(i+16,0);
                }
                else{
                    pinWrite(i+16,1);
                }
            }
        }

        else if (portnum==3){
            for(i=0;i<8;i++){
                if ((portval & (0x01<<i))==0){
                    pinWrite(i+24,0);
                }
                else{
                    pinWrite(i+24,1);
                }
            }
        }
    } 
if (portnum>=12 & portnum<=13) {
         if (portnum==12){
            for(i=0;i<8;i++){
                if ((portval & (0x01<<i))==0){
                    pinWrite(i+116,0);
                }
                else{
                    pinWrite(i+116,1);
                }
            }
        }
        
            else if (portnum==13){
            for(i=0;i<8;i++){
                if ((portval & (0x01<<i))==0){
                    pinWrite(i+124,0);
                }
                else{
                    pinWrite(i+124,1);
                }
            }
        }
    


}
}
else {
    //do nothing bro
}

}