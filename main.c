#include "msp.h"
#include "lcdLib_432.h"

int period = 60000;                             //20ms Period wanted from handout
int enable = 0;                                     //Variable to control state
int sec = 0;
int left = 3045;                        //arbitrary points chosen for left and right position (0 and 180 degrees)
int right = 8456;

void update()
{
    lcdSetText("Time->",0,1);
    lcdSetInt(sec,6,1);
}

void reset()
{
    lcdClear();
    P2->OUT &= ~0x07;
    TIMER_A0->CCR[1] = 0;           //This is off mode
    lcdSetText("Mach 1 Wash!", 0, 0);               //writes into LCD

}


void NormWash()
{
    SysTick->VAL = 0x00;
    sec = 1;
    update();
    while(sec > 0)
    {
        lcdClear();
        lcdSetText("Opening Valve", 0, 0);
        update();
        TIMER_A0->CCR[1] = right;
        delay_ms(250);
    }
    sec = 5;
    update();
    while(sec > 0)
    {
        lcdClear();
        lcdSetText("Cycling", 0, 0);
        update();
        TIMER_A0->CCR[1] = right;
        delay_ms(250);
        TIMER_A0->CCR[1] = left;
        delay_ms(250);
    }
    TIMER_A0->CCR[1] = 0;           //This is off mode
    sec = 1;
    update();
    while(sec > 0)
    {
        lcdClear();
        lcdSetText("Release Water", 0, 0);
        update();
        TIMER_A0->CCR[1] = right;
        delay_ms(1500);
    }
    sec = 1;
    update();
    while(sec > 0)
    {
        lcdClear();
        lcdSetText("Opening Supply", 0, 0);
        update();
        TIMER_A0->CCR[1] = left;
        delay_ms(250);
        delay_ms(1500);
    }
    sec = 2;
    update();
    while(sec > 0)
    {
        lcdClear();
        lcdSetText("Rinsing!", 0, 0);
        update();
        TIMER_A0->CCR[1] = right;
        delay_ms(250);
        TIMER_A0->CCR[1] = left;
        delay_ms(1500);
    }
    sec = 1;
    update();
    while(sec > 0)
    {
        lcdClear();
        lcdSetText("Release Water", 0, 0);
        update();
        TIMER_A0->CCR[1] = right;
        delay_ms(1500);

    }
    P2->OUT ^= BIT1;
    lcdClear();
    lcdSetText("Clothes Cleaned!",0,0);
    delay_ms(3000);
    reset();
}



void SuperWash()
{
    SysTick->VAL = 0x00;
    sec = 2;
    update();
    while(sec > 0)
    {
        lcdClear();
        lcdSetText("Opening Valve", 0, 0);
        update();
        TIMER_A0->CCR[1] = right;
        delay_ms(250);
    }
    sec = 10;
    update();
    while(sec > 0)
    {
        lcdClear();
        lcdSetText("Cycling", 0, 0);
        update();
        TIMER_A0->CCR[1] = right;
        delay_ms(500);
        TIMER_A0->CCR[1] = left;
        delay_ms(500);
    }
    TIMER_A0->CCR[1] = 0;           //This is off mode
    sec = 2;
    update();
    while(sec > 0)
    {
        lcdClear();
        lcdSetText("Release Water", 0, 0);
        update();
        TIMER_A0->CCR[1] = right;
        delay_ms(1500);
    }
    sec = 2;
    update();
    while(sec > 0)
    {
        lcdClear();
        lcdSetText("Opening Supply", 0, 0);
        update();
        TIMER_A0->CCR[1] = left;
        delay_ms(250);
        delay_ms(1500);
    }
    sec = 4;
    update();
    while(sec > 0)
    {
        lcdClear();
        lcdSetText("Rinsing!", 0, 0);
        update();
        TIMER_A0->CCR[1] = right;
        delay_ms(250);
        TIMER_A0->CCR[1] = left;
        delay_ms(1500);
    }
    sec = 2;
    update();
    while(sec > 0)
    {
        lcdClear();
        lcdSetText("Release Water", 0, 0);
        update();
        TIMER_A0->CCR[1] = left;
        delay_ms(1500);

    }
    P2->OUT ^= BIT2;
    lcdClear();
    lcdSetText("Clothes Cleaned!",0,0);
    delay_ms(3000);
    reset();
}
void STOP()
{
    lcdClear();
    lcdSetText("Lid Opened!", 0, 0);
    reset();

    return;
}


void PORT5_IRQHandler()                         //function called when interrupt is called
{

    if(P5->IFG & BIT0)                          //Off switch
    {
        P2->OUT |= BIT0;
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk);
    }
    else
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;


    P5->IFG = 0;                                //Clears Flags
}


void PWMInit()                                  //Initalization of Ports used for PWM
{

    P2->DIR |= BIT4;
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~BIT4;

    TIMER_A0->CCR[0] = period;                 //Setting period into CCR[0], this controls the period of all TIMERA0.X Ports

    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;//puts into reset/set mode

    TIMER_A0->CCR[1] = 0;                       //Initial point to put servo motor at

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // This is setting up TimerA0; System main clock
                    TIMER_A_CTL_MC__UP |      // Up mode
                    TIMER_A_CTL_CLR;          // Clear TAR
}
void SysTickInit()
{
    /*SysTick Timer*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = 30000 - 1;
    SysTick->VAL = 0x01;

}
void SysTick_Handler()
{
    if(sec > 0)
        sec--;
    update();
}

void GPIOInit()                                 //Initialization function
{
        /*PORTS*/
    P2->DIR |= 0x07;
    P2->REN |= 0xF8;
    P2->OUT |= 0xFF;
    P2->OUT &= ~0x07;
    /*Interrupt*/
    P5->DIR &= ~BIT0;
    P5->OUT &= ~BIT0;
    P5->REN |= BIT0;
    P5->IES |= BIT0;
    P5->IE |= BIT0;
    P5->IFG = 0;
    NVIC->ISER[1] |= BIT7;
}



int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    GPIOInit();                               //Calls all functions created and enabled
    PWMInit();
    SysTickInit();
    lcdInit();
    lcdClear();
    __enable_irq();

    lcdSetText("Mach 1 Wash!", 0, 0);               //writes into LCD


    while(1)
    {
        if(P2->IN & BIT3){}
        else
        {
            P2->OUT |= BIT1;
            NormWash();

        }

        if(P2->IN & BIT5){}
        else
        {
            P2->OUT |= BIT2;
            SuperWash();
        }

    }
}

