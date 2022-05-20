//timer interrupts


//timer setup for timer1.

#define ledPin 13


void setup(){
  //set pin as output
  pinMode(ledPin, OUTPUT);

  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
 
  OCR1A = 31250; // compare match register 16MHz/256/2Hz
//OCR1A = 31;    // compare match register 16MHz/256/2kHz
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts
}//end setup


ISR(TIMER1_COMPA_vect){//timer1 interrupt
digitalWrite(ledPin, digitalRead(ledPin) ^ 1); // toggle LED pin
}
  


void loop(){
  //do other things here
}
