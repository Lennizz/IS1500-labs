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

int mytime = 0x005959;
char textstring[] = "text, more text, and even more text!";

volatile int *timeout_pointer = (volatile int*)0x04000020;


int clamp(int value, int min, int max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}


void set_leds(int led_mask) {
  volatile int* led_toggle = (volatile int*) 0x04000000;
  *led_toggle = led_mask & 0x3FF;
}


void set_displays(int display_number, int value, int dot){
  int start_address = 0x04000050;
  int offset = 0x10 * (clamp(display_number, 0, 5));

  volatile int* segment_pointer = (volatile int*) (start_address + offset);

  int display_bits;
  switch (value){
    case 0:
      display_bits = 0b11000000;
      break;
    case 1:
      display_bits = 0b11111001;
      break;
    case 2:
      display_bits = 0b10100100;
      break;
    case 3:
      display_bits = 0b10110000;
      break;
    case 4:
      display_bits = 0b10011001;
      break;
    case 5:
      display_bits = 0b10010010;
      break;
    case 6:
      display_bits = 0b10000010;
      break;
    case 7:
      display_bits = 0b11111000;
      break;
    case 8:
      display_bits = 0b10000000;
      break;
    case 9:
      display_bits = 0b10011000;
      break;
    default:
      display_bits = 0b11000000;
      break;
  }
  if (dot)
    display_bits = display_bits & 0b01111111;

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


/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause){

}

/* Add your code here for initializing interrupts. */
void labinit(void){

  volatile int* control_pointer = (volatile int *)0x04000024;

  volatile int* periodh_pointer = (volatile int *)0x0400002c;

  volatile int* periodl_pointer = (volatile int *)0x04000028;

  *periodh_pointer = 0b0000000000101101;

  *periodl_pointer = 0b1100011011000000;

  *control_pointer = 0b110;
}

int check_timeout(){
  
  int status = *timeout_pointer;

  status &= 0b1;

  if(status)
    *timeout_pointer &= 0xFFFFFFFE;

  return status;

}

/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();


  int counter = 0;
  while (counter <= 15) { 
    if(check_timeout()){
      set_leds(counter);
      counter++;
    }
  }

  // Enter a forever loop
  while (1) {
    if (get_btn()) {
      int sw_values = get_sw();
      int msb2_sw = sw_values  >> 8;
      int lsb6_sw = (sw_values & 0b111111);

      int tens = lsb6_sw / 10;
      int ones = lsb6_sw % 10;
      int display_value = (tens << 4) | ones;

      int reset_sw = (sw_values & 0b1000000);

      int masked_time;
      switch (msb2_sw){
        case 1:
          masked_time = mytime & 0xFFFF00;
          mytime = masked_time | display_value;
          break;
        case 2:
          masked_time = mytime & 0xFF00FF;
          mytime = masked_time | (display_value << 8);
          break; 
        case 3:
          masked_time = mytime & 0x00FFFF;
          mytime = masked_time | (display_value << 16);
          break;     
        default:
          break;
      }

      if (reset_sw)
        mytime = 0x0;
    }
    
    
    if(check_timeout()){

    set_displays(0, (mytime & 0x00000F), 0);
    set_displays(1, (mytime & 0x0000F0) >> 4, 0);
    set_displays(2, (mytime & 0x000F00) >> 8, 1);
    set_displays(3, (mytime & 0x00F000) >> 12, 0);
    
    if (((mytime & 0x0F0000) >> 16) > 9){
      mytime &= 0xF0FFFF;
      mytime += 0x100000;
    }

    set_displays(4, (mytime & 0x0F0000) >> 16, 1);

    if (((mytime & 0xF00000) >> 20) > 9)
      mytime &= 0x0FFFFF;
  
    set_displays(5, (mytime & 0xF00000) >> 20, 0);
      time2string( textstring, mytime ); // Converts mytime to string
      display_string( textstring ); //Print out the string 'textstring'
      tick( &mytime );     // Ticks the clock once
    }

  }
}