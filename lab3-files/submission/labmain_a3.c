/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );

extern void enable_interrupt();

int mytime = 0x005959;
char textstring[] = "text, more text, and even more text!";

volatile int *timeout_pointer = (volatile int*)0x04000020;

int timeoutcount = 0;
int prime = 1234567;

int clamp(int value, int min, int max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}


void set_leds(int led_mask) {
  volatile int* led_toggle = (volatile int*) 0x04000000;
  *led_toggle = led_mask & 0x3FF;
}


void set_displays(int display_number, int value){
  int start_address = 0x04000050;
  int offset = 0x10 * (clamp(display_number, 0, 5));

  volatile int* segment_pointer = (volatile int*) (start_address + offset);

  int display_bits;
  switch (value){
    case 0:
      display_bits = 0b01000000;
      break;
    case 1:
      display_bits = 0b01111001;
      break;
    case 2:
      display_bits = 0b00100100;
      break;
    case 3:
      display_bits = 0b00110000;
      break;
    case 4:
      display_bits = 0b00011001;
      break;
    case 5:
      display_bits = 0b00010010;
      break;
    case 6:
      display_bits = 0b00000010;
      break;
    case 7:
      display_bits = 0b01111000;
      break;
    case 8:
      display_bits = 0b00000000;
      break;
    case 9:
      display_bits = 0b00011000;
      break;
    default:
      display_bits = 0b01000000;
      break;
  }

  *segment_pointer = display_bits; 
}

 int get_sw(void){
  volatile int* switch_values = (volatile int*) 0x04000010;  
  return (*switch_values & 0x3FF);
 }

 int get_btn(void){
  volatile int* button_value = (volatile int*) 0x040000d0;  
  return (*button_value & 0x1); 
 }


void display_time(){
  set_displays(0, (mytime & 0x00000F));

  if (((mytime & 0x0000F0) >> 4) > 5){
    mytime &= 0xFFFF0F;
    mytime += 0x000100;
  }

  set_displays(1, (mytime & 0x0000F0) >> 4);
  set_displays(2, (mytime & 0x000F00) >> 8);

  if (((mytime & 0x00F000) >> 12) > 5){
    mytime &= 0xFF0FFF;
    mytime += 0x010000;
  }

  set_displays(3, (mytime & 0x00F000) >> 12);
  
  if (((mytime & 0x0F0000) >> 16) > 9){
    mytime &= 0xF0FFFF;
    mytime += 0x100000;
  }

  set_displays(4, (mytime & 0x0F0000) >> 16);

  if (((mytime & 0xF00000) >> 20) > 9)
    mytime &= 0x0FFFFF;

  set_displays(5, (mytime & 0xF00000) >> 20);  
}

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt ( unsigned cause ) {
  switch (cause){
    case (0x00000010): // Timer interupt
      *timeout_pointer &= 0xFFFFFFFE;
      timeoutcount++;

      if (timeoutcount < 10)
        return;

      timeoutcount = 0;

      display_time();
      tick( &mytime );

      break;
    default:
      return;
  }
}

/* Add your code here for initializing interrupts. */
void labinit(void){
  enable_interrupt();

  volatile int* control_pointer = (volatile int *)0x04000024;
  volatile int* periodh_pointer = (volatile int *)0x0400002c;
  volatile int* periodl_pointer = (volatile int *)0x04000028;

  *periodh_pointer = 0b0000000000101101;
  *periodl_pointer = 0b1100011011000000;

  *control_pointer = 0b111;
}

/* Your code goes into main as well as any needed functions. */
int main ( void ) {
  labinit();
  while (1) {
    print("Prime: ");
    prime = nextprime( prime );
    print_dec( prime );
    print("\n");
  }
}