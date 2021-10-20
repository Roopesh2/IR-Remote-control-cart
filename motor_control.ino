// pin defintions
const int motors[2][2] = { { 5, 6 }, { 9, 10 } };

// turn offs a single motor
void turnOffMotor(int serialNumber) {
  serialNumber--;
  analogWrite(motors[serialNumber][0], 0);
  analogWrite(motors[serialNumber][1], 0);
}

/**
 * Turns on all motots
 * @param serialNumber the motor number to turn on
 * @param speed the speed to turn on the motor
 * @param anitclockwise whether the motor should turn clockwise or anti-clockwise
 */
void turnMotor(int serialNumber, int speed, boolean anticlockwise) {
  serialNumber--;
  if (anticlockwise) {
    analogWrite(motors[serialNumber][0], 0);
    analogWrite(motors[serialNumber][1], speed);
  } else {
    analogWrite(motors[serialNumber][0], speed);
    analogWrite(motors[serialNumber][1], 0);
  }
}

// without boolean arguments {see turnMotor(int serialNumber, int speed, boolean anticlockwise)}
void turnMotor(int serialNumber, int speed) {
  turnMotor(serialNumber, speed, false);
}

// without speed & boolean arguments
void turnMotor(int serialNumber) {
  turnMotor(serialNumber, 255, false);
}
// stops all motors
void stopAll() {
  turnOffMotor(1);
  turnOffMotor(2);
}

// ------- IR sensor utilities ------- //
#include <IRremote.h>
#define DECODE_NEC        // Includes Apple and Onkyo
#define IR_RECEIVE_PIN 2  // To be compatible with interrupt example, pin 2 is chosen here.
#define ENABLE_LED_FEEDBACK true
#define USE_DEFAULT_FEEDBACK_LED_PIN 0
// convert code to command from "UNE FM Remote"
String codeToCommand(uint16_t code) {
  String cmd = "UNKNOWN";
  switch (code) {
    case 0:
      cmd = "7";
      break;
    case 1:
      cmd = "play";
      break;
    case 2:
      cmd = "skip_back";
      break;
    case 3:
      cmd = "skip_forward";
      break;
    case 4:
      cmd = "equalizer";
      break;
    case 5:
      cmd = "volume-";
      break;
    case 6:
      cmd = "volume+";
      break;
    case 7:
      cmd = "0";
      break;
    case 8:
      cmd = "repeat";
      break;
    case 9:
      cmd = "scan";
      break;
    case 10:
      cmd = "1";
      break;
    case 12:
      cmd = "4";
      break;
    case 13:
      cmd = "5";
      break;
    case 14:
      cmd = "6";
      break;
    case 15:
      cmd = "8";
      break;
    case 18:
      cmd = "power";
      break;
    case 25:
      cmd = "9";
      break;
    case 26:
      cmd = "mode";
      break;
    case 27:
      cmd = "2";
      break;
    case 30:
      cmd = "mute";
      break;
    case 31:
      cmd = "3";
      break;
  }
  return cmd;
}

// reads IR code and returns the command
String readIRCode() {
  if (IrReceiver.decode() && IrReceiver.decodedIRData.protocol != UNKNOWN) {
    String code = codeToCommand(IrReceiver.decodedIRData.command);
    IrReceiver.resume();  // Enable receiving of the next value
    return code;
  }
  return "UNKNOWN";
}

void setup() {
  //Set pins as outputs
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      pinMode(motors[i][j], OUTPUT);
    }
  }
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(9600); //Open serial for debugging
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
  Serial.print("Ready to receive IR signals at pin ");
  Serial.println(IR_RECEIVE_PIN);
}

void loop() {

  // sends potential difference to IR sensor
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);  
  
  String code = readIRCode();
  if (!code.equals("UNKNOWN")) {
    if(code.equals("1")) {
      Serial.println("forward left");
      turnMotor(1, 128);
      turnMotor(2, 255);
      // delay(50);
    } else if(code.equals("2")) {
      Serial.println("forward");
      turnMotor(1, 255);
      turnMotor(2, 255);
      // delay(50);
    } else if(code.equals("3")) {
      Serial.println("forward right");
      turnMotor(1, 255);
      turnMotor(2, 128);
      // delay(50);
    } else if(code.equals("4")) {
      Serial.println("left");
      turnMotor(1, 0);
      turnMotor(2, 255);
      // delay(50);
    } else if(code.equals("5")) {
      Serial.println("stopAll");
      stopAll();
    } else if(code.equals("6")) {
      Serial.println("right");
      turnMotor(1, 255);
      turnMotor(2, 0);
      // delay(50);
    } else if(code.equals("7")) {
      Serial.println("backward left");
      turnMotor(1, 128, true);
      turnMotor(2, 255, true);
      // delay(50);
    } else if(code.equals("8")) {
      Serial.println("backward");
      turnMotor(1, 255, true);
      turnMotor(2, 255, true);
      // delay(50);
    } else if(code.equals("9")) {
      Serial.println("backward right");
      turnMotor(1, 255, true);
      turnMotor(2, 128, true);
      // delay(50);
    } 
    else Serial.println(code);
  }
}
