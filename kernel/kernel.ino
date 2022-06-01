#define d1 13
#define d2 12
#define d3 11
#define d4 10

/* 4 Tasks:
 *     T1 - T = 100ms   -> Led d1 toggle
 *     T2 - T = 200ms   -> Led d2 toggle
 *     T3 - T = 600ms   -> Led d3 toggle
 *     T4 - T = 100ms   -> Led d4 copied from button A1
 */


void toggle(void) {digitalWrite(d4, !digitalRead(d4));}

void t0(void) {
  static int A1_old;
  int A1_new = digitalRead(A1); 
  
  if (A1_new != A1_old)
    Sched_AddT(toggle, 2000 /* delay */, 0 /* period */,4);
    
  A1_old = A1_new;
}

void t1(void) {digitalWrite(d1, !digitalRead(d1)); delay(700);}
void t2(void) {digitalWrite(d2, !digitalRead(d2));}
void t3(void) {digitalWrite(d3, !digitalRead(d3));}
void t4(void) {digitalWrite(d4,  digitalRead(A1));}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(d4, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d1, OUTPUT);
  Sched_Init();

  Sched_AddT(t0, 1 /* delay */,   10 /* period */, 0);
  Sched_AddT(t3, 1 /* delay */,  200 /* period */, 3);
  Sched_AddT(t2, 1 /* delay */,  500 /* period */, 2);

  /* add a bunch of one-shot tasks, that will temporarily take up space in the TCB array */
  /* This forces task t1 to have a lower priority, and leave empty TCB entries for the 
   *  toggle task added by t0.
   */
  Sched_AddT(toggle, 10 /* delay */,  0 /* period */,4);
  Sched_AddT(toggle, 20 /* delay */,  0 /* period */,4);
  Sched_AddT(toggle, 30 /* delay */,  0 /* period */,4);
  Sched_AddT(toggle, 40 /* delay */,  0 /* period */,4);
  Sched_AddT(toggle, 50 /* delay */,  0 /* period */,4);
  Sched_AddT(toggle, 60 /* delay */,  0 /* period */,4);
  Sched_AddT(toggle, 70 /* delay */,  0 /* period */,4);
  Sched_AddT(toggle, 80 /* delay */,  0 /* period */,4);
  
  Sched_AddT(t1, 1 /* delay */, 1000 /* period */,1);
}


ISR(TIMER1_COMPA_vect){//timer1 interrupt
  Sched_Schedule();
  Sched_Dispatch();
}


// the loop function runs over and over again forever
void loop() {
  /* nothing to do */
}
