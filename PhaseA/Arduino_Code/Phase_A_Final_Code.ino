#define BUTTON_1  2
#define BUTTON_2  3
#define GREEN_LED 4
#define RED_LED   5
//#define BUZZER    6

#define DATA      9
#define LATCH     8
#define CLOCK     7

#define DIGIT_4   10
#define DIGIT_3   11
#define DIGIT_2   12
#define DIGIT_1   13
 
unsigned char table[]=
{ 0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
  };

unsigned char count = 30;
unsigned char initialValue = 30;
unsigned int buttonState = 0;  // variable for reading the pushbutton status
int countdown = 10;

volatile unsigned char isr_1_flag = 0;
volatile unsigned char isr_2_flag = 0;
volatile unsigned char isr_3_flag = 0;

void setup() {
 // LEDs Pins
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Button Pins
  pinMode(BUTTON_1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), Button_1_ISR, CHANGE);
  pinMode(BUTTON_2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2), Button_2_ISR, CHANGE);

  // 7-Seg Display
  pinMode(DIGIT_1, OUTPUT);
  pinMode(DIGIT_2, OUTPUT);
  pinMode(DIGIT_3, OUTPUT);
  pinMode(DIGIT_4, OUTPUT);

  // Shift Register Pins
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);

  //disp_on();

  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 62500 / 1000;            // compare match register 16MHz/256
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

void Display_Numbers()
{
  disp_on1();
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, table[0]);
  digitalWrite(LATCH, HIGH);
  delay(250);
  disp_on2();
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, table[0]);
  digitalWrite(LATCH, HIGH);
  delay(250);
  disp_on3();
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, table[countdown / 10]);
  digitalWrite(LATCH, HIGH);
  delay(250);
  disp_on4();
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, table[countdown % 10]);
  digitalWrite(LATCH, HIGH);
  delay(250);
  countdown = countdown-0.000001;
  
}

// void Display_Numbers()
// {
//   Display(count);
// if(countdown == 0)
//    {
//      countdown = initialValue;
//    }
//   // Reset Counter

// }

void disp_on1()
{
   digitalWrite(DIGIT_1, LOW);
   digitalWrite(DIGIT_2, HIGH);
   digitalWrite(DIGIT_3, HIGH);
   digitalWrite(DIGIT_4, HIGH);
}
void disp_on2()
{
   digitalWrite(DIGIT_1, HIGH);
   digitalWrite(DIGIT_2, LOW);
   digitalWrite(DIGIT_3, HIGH);
   digitalWrite(DIGIT_4, HIGH);
}
void disp_on3()
{
   digitalWrite(DIGIT_1, HIGH);
   digitalWrite(DIGIT_2, HIGH);
   digitalWrite(DIGIT_3, LOW);
   digitalWrite(DIGIT_4, HIGH);
}
void disp_on4()
{
   digitalWrite(DIGIT_1, HIGH);
   digitalWrite(DIGIT_2, HIGH);
   digitalWrite(DIGIT_3, HIGH);
   digitalWrite(DIGIT_4, LOW);
}

void Button_1_ISR()
{
  unsigned int buttonState = 0;  // variable for reading the pushbutton status
    
  buttonState = digitalRead(BUTTON_1);
  digitalWrite(RED_LED, buttonState);

  count++;
  Display_Numbers();
}

void Button_2_ISR()
{ 
  unsigned int buttonState = 0;  // variable for reading the pushbutton status
   
  buttonState = digitalRead(BUTTON_2);
  digitalWrite(GREEN_LED, buttonState);
}

ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
{
  // Set ISR Flag
  isr_3_flag = 1;
}

void loop() {
   Display_Numbers();
  // Attend Button 1
  if(isr_1_flag == 1)
  {
    // Reset ISR Flag
    isr_1_flag = 0;

    // Code
    count++;
    if (count > initialValue){
      count = 0;
    }
    buttonState = digitalRead(BUTTON_1);
    digitalWrite(RED_LED, buttonState);
  }

  // Attend Button 2
  if(isr_2_flag == 1)
  {
    // Reset ISR Flag
    isr_2_flag = 0;
    
    // Code
    buttonState = digitalRead(BUTTON_2);
    digitalWrite(GREEN_LED, buttonState);
  }

  // Attend 7-Seg Display
  if(isr_3_flag == 1)
  {
    // Reset ISR Flag
    isr_3_flag = 0;

    // Code
    //count--;
  }
}