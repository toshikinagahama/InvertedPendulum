#include <M5Atom.h>

// TC78H653FTGの制御ピンをM5AtomのGPIOピンに割り当てる
// GPIO22とGPIO19を使用するとのことなので、以下のように割り当てます
const int IN1_PIN = 22;
const int IN2_PIN = 19;
const int IN3_PIN = 33;
const int IN4_PIN = 23;

// PWM設定
const int PWM_FREQ = 2000;    // PWM周波数 (Hz)
const int PWM_RESOLUTION = 8; // PWM分解能 (8bit = 0-255)

const int PWM_CHANNEL_IN1 = 0; // IN1用のPWMチャンネル
const int PWM_CHANNEL_IN2 = 1; // IN2用のPWMチャンネル
const int PWM_CHANNEL_IN3 = 0; // IN3用のPWMチャンネル
const int PWM_CHANNEL_IN4 = 1; // IN4用のPWMチャンネル
//
void setup()
{
  M5.begin(true, false, true);
  delay(50);
  M5.dis.drawpix(0, 0xf00000);
  // ピンモード設定
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  // PWM設定
  ledcSetup(PWM_CHANNEL_IN1, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN1_PIN, PWM_CHANNEL_IN1);

  ledcSetup(PWM_CHANNEL_IN2, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN2_PIN, PWM_CHANNEL_IN2);

  ledcSetup(PWM_CHANNEL_IN3, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN3_PIN, PWM_CHANNEL_IN1);

  ledcSetup(PWM_CHANNEL_IN3, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN4_PIN, PWM_CHANNEL_IN2);

  // 初期状態: モーター停止 (ENをLOWにしておくのが安全)
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  ledcWrite(PWM_CHANNEL_IN1, 0); // PWMも0 (停止)
  ledcWrite(PWM_CHANNEL_IN2, 0); // PWMも0 (停止)
  ledcWrite(PWM_CHANNEL_IN3, 0); // PWMも0 (停止)
  ledcWrite(PWM_CHANNEL_IN4, 0); // PWMも0 (停止)
}

uint8_t FSM = 0;

void loop()
{
  if (M5.Btn.wasPressed())
  {

    switch (FSM)
    {
    case 0:
      M5.dis.drawpix(0, 0x707070);
      ledcWrite(PWM_CHANNEL_IN1, 0);
      ledcWrite(PWM_CHANNEL_IN2, 0);
      ledcWrite(PWM_CHANNEL_IN3, 0);
      ledcWrite(PWM_CHANNEL_IN4, 0);
      break;
    case 1:
      M5.dis.drawpix(0, 0xf00000);
      ledcWrite(PWM_CHANNEL_IN2, 0); // IN2はLOW固定
      ledcWrite(PWM_CHANNEL_IN4, 0); // IN4はLOW固定
      for (int dutyCycle = 0; dutyCycle <= 200; dutyCycle += 10)
      {
        ledcWrite(PWM_CHANNEL_IN1, dutyCycle); // IN1にPWM信号
        ledcWrite(PWM_CHANNEL_IN3, dutyCycle); // IN3にPWM信号
        delay(1);
      }
      break;
    case 2:
      M5.dis.drawpix(0, 0x00f000);
      ledcWrite(PWM_CHANNEL_IN1, 0); // IN1はLOW固定
      ledcWrite(PWM_CHANNEL_IN3, 0); // IN3はLOW固定
      for (int dutyCycle = 0; dutyCycle <= 200; dutyCycle += 10)
      {
        ledcWrite(PWM_CHANNEL_IN2, dutyCycle); // IN2にPWM信号
        ledcWrite(PWM_CHANNEL_IN4, dutyCycle); // IN4にPWM信号
        delay(1);
      }
      break;
    case 3:
      M5.dis.drawpix(0, 0x0000f0);
      ledcWrite(PWM_CHANNEL_IN1, 0);
      ledcWrite(PWM_CHANNEL_IN2, 0);
      ledcWrite(PWM_CHANNEL_IN3, 0);
      ledcWrite(PWM_CHANNEL_IN4, 0);
      break;
    default:
      break;
    }
    FSM++;
    if (FSM >= 4)
    {
      FSM = 0;
    }
  }

  delay(50);
  M5.update();
}