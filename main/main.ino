#define L_DIR 8
#define L_PWM 6
#define R_DIR 7
#define R_PWM 5

//#define MOTOR_TEST
#define SERIAL_SPEED 115200

static int l_motor_value = 0;
static int r_motor_value = 0;

void init_motor_test() {
  pinMode(L_DIR, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_DIR, OUTPUT);
  pinMode(R_PWM, OUTPUT);

  digitalWrite(L_DIR, LOW);
  digitalWrite(L_PWM, LOW);
  digitalWrite(R_DIR, LOW);
  digitalWrite(R_PWM, LOW);
}

void motor_test() {
  digitalWrite(L_DIR, LOW);
  digitalWrite(L_PWM, LOW);

  digitalWrite(L_DIR, HIGH);
  digitalWrite(L_PWM, LOW);
  delay(3000);                       // wait for a second

  digitalWrite(L_DIR, LOW);
  digitalWrite(L_PWM, LOW);
  delay(3000);                       // wait for a second

  digitalWrite(L_DIR, LOW);
  digitalWrite(L_PWM, HIGH);
  delay(3000);                       // wait for a second

  digitalWrite(L_DIR, LOW);
  digitalWrite(L_PWM, LOW);


  digitalWrite(R_DIR, HIGH);
  digitalWrite(R_PWM, LOW);
  delay(3000);                       // wait for a second

  digitalWrite(R_DIR, LOW);
  digitalWrite(R_PWM, LOW);
  delay(3000);                       // wait for a second

  digitalWrite(R_DIR, LOW);
  digitalWrite(R_PWM, HIGH);
  delay(3000);                       // wait for a second

  digitalWrite(R_DIR, LOW);
  digitalWrite(R_PWM, LOW);
}

void setup() {
  #ifdef MOTOR_TEST
    init_motor_test();
  #endif
  Serial.begin(SERIAL_SPEED);
  
}

void process_serial() {
  static int ch;
  static int state = 0;
  static bool space_detected = false;
  static String string;
  static int motor;

//  Serial.println("-1. ch:");
  if (Serial.available() > 0) {
    ch = Serial.read();

    switch(state) {
      //begin L/R
      case 0: {
        if (ch == 'L') {
          motor = 0;  
          state = 1;
        } else if (ch == 'R') {
          motor = 1;  
          state = 1;
        }
        break;
      }
      //space detected
      case 1: {
        if (ch == ' ' || ch == '\t') {
          space_detected = true;
          break;
        } else if (!space_detected){
          state = 0;
          space_detected = false;
          break;
        }
        state = 2;
      }
      //get digit
      case 2: {
        if (isDigit(ch) || char(ch) == '+' || char(ch) == '-') {
          string += char(ch);
        } else {
          if (string.length()) {
            int value = string.toInt();
            if (!motor) {
              l_motor_value = value;
            } else {
              r_motor_value = value;
            }
//            Serial.println(String("Motor:") + motor + " value:" + value);
          }
          space_detected = false;
          state = 0;
          string = "";
        }
        break;
      }
    }
  }
}

void loop() {
  process_serial();
  #ifdef MOTOR_TEST
    motor_test();
  #endif
  Serial.println(String("l:") + l_motor_value + " r:" + r_motor_value);
}
