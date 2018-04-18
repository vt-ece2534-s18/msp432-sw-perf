#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>

unsigned TimerLap() {
    static unsigned int previousSnap;
    unsigned int currentSnap, ret;
    currentSnap = Timer32_getValue(TIMER32_0_BASE);
    ret = (previousSnap - currentSnap);
    previousSnap = currentSnap;
    return ret;
}

int sumArray(unsigned int A[16]) {
    unsigned i, s=0;
    for (i=0; i<16; i++)
        s += A[i];
    return s;
}

int main(void) {
  // Stop WDT
  WDT_A_holdTimer();

  // Timer Initialization
  Timer32_initModule(TIMER32_0_BASE,
                     TIMER32_PRESCALER_1,
                     TIMER32_32BIT,
                     TIMER32_FREE_RUN_MODE);
  Timer32_startTimer(TIMER32_BASE, false);

  // RED LED
  GPIO_setAsOutputPin    (GPIO_PORT_P2,    GPIO_PIN6);
  GPIO_setOutputLowOnPin (GPIO_PORT_P2,    GPIO_PIN6);

  // BUTTON S1
  GPIO_setAsInputPin (GPIO_PORT_P5, GPIO_PIN1);

  volatile unsigned c;
  unsigned i, thisA[16];
  unsigned k = 0, spin = 0;

  TimerLap();
  while (1) {
      k += TimerLap();
      spin++;
      for (i=0; i<16; i++) {
          thisA[i] = rand();
      }
      for (i=0; i<10; i++) {
          c = sumArray(thisA);
      }
      if (spin > 1000) {
          printf("%4d\n", k/1000);
          spin = 0;
          k    = 0;
          TimerLap();
      }
  }

}
