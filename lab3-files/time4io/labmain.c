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

int led_counter = 0;
int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Add your code here for initializing interrupts. */
void labinit(void)
{}


void set_leds(int led_mask){

  volatile int *p = (volatile int *)0x04000000;
  *p = led_mask;

}

void increment_led(){
  set_leds(led_counter);
  led_counter++;
}

void set_displays(int display_number, int value){

  volatile int *display_pointer = (volatile int *)(0x04000050 + display_number * 0x10);

  switch(value){
    case 0:
      *display_pointer = 0b11000000;
      break;
    case 1:
      *display_pointer = 0b11111001;
      break;

    case 2:
      *display_pointer = 0b10100100;
      break;
    
    case 3:
      *display_pointer = 0b10110000;
      break;

    case 4:
      *display_pointer = 0b10011001;
      break;

    case 5:
      *display_pointer = 0b10010010;
      break;

    case 6:
      *display_pointer = 0b10000010;
      break;

    case 7:
      *display_pointer = 0b11111000;
      break;

    case 8:
      *display_pointer = 0b10000000;
      break;

    case 9:
      *display_pointer = 0b10011000;
      break;

    default:
      break;
  }
}

int get_sw(void){

  volatile int *sw_pointer = (volatile int *)0x04000010;

  int sw = *sw_pointer & 0b1111111111;

  return sw;
}

int get_btn(void){

  volatile int *btn_pointer = (volatile int *)0x040000d0;

  int btn = *btn_pointer & 0b1;

  return btn;
}

/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();
  
  // Enter a forever loop
  while (1) {
    if(get_btn()){
      int sw_control = (get_sw() >> 8);
      int sw_values;
      sw_values = get_sw();

      sw_values &= 0b00111111; 

      int sw_values_tens = sw_values / 10;
      int sw_values_ones = sw_values % 10;

      int sw_display_values = (sw_values_tens << 4) | sw_values_ones;
    
      switch(sw_control){
        case 1:
          mytime &= 0xFFFF00;
          mytime = mytime | sw_display_values;
          break;
        
        case 2:
          sw_display_values = sw_display_values << 8;
          mytime &= 0xFF00FF;
          mytime = mytime | sw_display_values;
          break;
        
        case 3:
        sw_display_values = sw_display_values << 16;
          mytime &= 0x00FFFF;
          mytime = mytime | sw_display_values;
          break;

        default:
          break;
      }
    }

    set_displays(0, (mytime & 0xF));
    set_displays(1, (mytime & 0xF0) >> 4);
    set_displays(2, (mytime & 0xF00) >> 8);
    set_displays(3, (mytime & 0xF000) >> 12);

    if((mytime & 0xF0000) >> 16 > 9){
      mytime = mytime & 0xF0FFFF;
      mytime += 0x100000;
    }

    set_displays(4, (mytime & 0xF0000) >> 16);

    if((mytime & 0xF00000) >> 20 > 9){
      mytime &= 0x0FFFFF;
    }

    set_displays(5, (mytime & 0xF00000) >> 20);

    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'
    delay( 2 );          // Delays 1 sec (adjust this value)
    tick( &mytime );     // Ticks the clock once
  }
}