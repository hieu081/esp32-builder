#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"
#include <ESP32Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Adafruit_TCS34725 camBienMau = Adafruit_TCS34725(
  TCS34725_INTEGRATIONTIME_50MS,
  TCS34725_GAIN_60X
);

#define CHAN_DT 15
#define CHAN_SCK 2
#define HE_SO_CAN 3972.5f

#define SERVO_DAY 27
#define SERVO_LOAI_1 18
#define SERVO_LOAI_2 19

#define CHAN_BANG_TAI 23

#define CAM_BIEN_1 4
#define CAM_BIEN_2 5

#define NUT_START 14
#define NUT_STOP 26

HX711 can;
Servo servoDay;
Servo servoLoai1;
Servo servoLoai2;

float khoiLuongLoc = 0;
float alpha = 0.15;

String mauSac = "---";
int loaiSanPham = 0;

bool dangXuLy = false;
bool heThongBat = false;

bool startCu;
bool stopCu;

unsigned long lanLCD = 0;
unsigned long lanSerial = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();

  pinMode(NUT_START, INPUT_PULLUP);
  pinMode(NUT_STOP, INPUT_PULLUP);

  pinMode(CHAN_BANG_TAI, OUTPUT);
  digitalWrite(CHAN_BANG_TAI, LOW);

  pinMode(CAM_BIEN_1, INPUT);
  pinMode(CAM_BIEN_2, INPUT);

  servoDay.attach(SERVO_DAY);
  servoLoai1.attach(SERVO_LOAI_1);
  servoLoai2.attach(SERVO_LOAI_2);

  duaServoVeHome();

  can.begin(CHAN_DT, CHAN_SCK);
  can.set_scale(HE_SO_CAN);
  delay(500);
  can.tare();

  if (!camBienMau.begin()) {
    lcd.clear();
    lcd.print("LOI CAM BIEN");
    while (1);
  }

  // luôn STOP khi khởi động
  heThongBat = false;
  dangXuLy = false;

  // lưu trạng thái hiện tại của nút để tránh tự kích hoạt
  startCu = digitalRead(NUT_START);
  stopCu = digitalRead(NUT_STOP);

  lcd.clear();
  lcd.print("DA STOP");
}

void loop() {
  docNut();

  if (!heThongBat) return;

  docKhoiLuong();
  hienThiLCD();
  inSerial();

  if (!dangXuLy && khoiLuongLoc > 10) {
    dangXuLy = true;

    if (!heThongBat) return;

    docKhoiLuong();

    servoDay.write(75);
    delay(1000);
    servoDay.write(0);

    delay(1000);
    if (!heThongBat) return;

    docMau();
    phanLoai();

    batBangTai();
    xuLyPhanLoai();
  }
}

void docNut() {
  bool startMoi = digitalRead(NUT_START);
  bool stopMoi = digitalRead(NUT_STOP);

  // START: chỉ cần đổi trạng thái là chạy
  if (startMoi != startCu) {
    delay(30);

    heThongBat = true;
    dangXuLy = false;

    khoiLuongLoc = 0;
    mauSac = "---";
    loaiSanPham = 0;

    can.tare();
    duaServoVeHome();

    lcd.clear();
  }

  // STOP: chỉ cần đổi trạng thái là dừng
  if (stopMoi != stopCu) {
    delay(30);
    dungHeThong();
  }

  startCu = startMoi;
  stopCu = stopMoi;
}

void dungHeThong() {
  tatBangTai();

  heThongBat = false;
  dangXuLy = false;

  khoiLuongLoc = 0;
  mauSac = "---";
  loaiSanPham = 0;

  can.tare();
  duaServoVeHome();

  lcd.clear();
  lcd.print("DA STOP");
}

void duaServoVeHome() {
  servoDay.write(0);
  servoLoai1.write(55);
  servoLoai2.write(50);
}

void docKhoiLuong() {
  float giaTriRaw = can.get_units(5);

  if (giaTriRaw < 0.5) giaTriRaw = 0;

  khoiLuongLoc = alpha * giaTriRaw + (1 - alpha) * khoiLuongLoc;
  khoiLuongLoc = round(khoiLuongLoc * 10) / 10.0;
}

void docMau() {
  uint16_t r, g, b, c;
  float doMauDo, doMauXanh, doMauDuong;

  camBienMau.getRawData(&r, &g, &b, &c);

  if (c == 0) return;

  doMauDo = ((float)r / c) * 255.0;
  doMauXanh = ((float)g / c) * 255.0;
  doMauDuong = ((float)b / c) * 255.0;

  if (doMauDo > 55 && doMauXanh < 100 && doMauDuong < 90) {
    mauSac = "DO";
  }
  else if (doMauXanh > doMauDo && doMauXanh > doMauDuong && doMauXanh > 100) {
    mauSac = "XANH";
  }
  else {
    mauSac = "KHAC";
  }
}

void phanLoai() {
  if (khoiLuongLoc >= 20 && mauSac == "DO") loaiSanPham = 1;
  else if (khoiLuongLoc <= 20 && mauSac == "DO") loaiSanPham = 2;
  else loaiSanPham = 3;
}

void batBangTai() {
  digitalWrite(CHAN_BANG_TAI, HIGH);
}

void tatBangTai() {
  digitalWrite(CHAN_BANG_TAI, LOW);
}

void xuLyPhanLoai() {
  if (loaiSanPham ==1) {
    while (digitalRead(CAM_BIEN_1) == HIGH) {
      docNut();
      hienThiLCD();
      inSerial();
      if (!heThongBat) return;
    }

    tatBangTai();
    servoLoai1.write(0);
    delay(2000);
    servoLoai1.write(60);
    delay(500);
  }
  else if (loaiSanPham == 2) {
    while (digitalRead(CAM_BIEN_2) == HIGH) {
      docNut();
      hienThiLCD();
      inSerial();
      if (!heThongBat) return;
    }
    tatBangTai();
    servoLoai2.write(0);
    delay(1500);
    servoLoai2.write(50);
    delay(500);
  }
  else {
    delay(3000);
    tatBangTai();
  }

  resetHeThong();
}

void resetHeThong() {
  khoiLuongLoc = 0;
  mauSac = "---";
  loaiSanPham = 0;
  dangXuLy = false;
  can.tare();
}

void hienThiLCD() {
  if (millis() - lanLCD < 200) return;
  lanLCD = millis();

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Loai ");
  lcd.print(loaiSanPham);

  lcd.setCursor(8, 0);
  lcd.print(mauSac);

  lcd.setCursor(0, 1);
  lcd.print("Can:");
  lcd.print(khoiLuongLoc, 1);
  lcd.print("g");
}

void inSerial() {
  if (millis() - lanSerial < 2000) return;
  lanSerial = millis();

  Serial.print("Can: ");
  Serial.print(khoiLuongLoc, 1);
  Serial.print(" g | Mau: ");
  Serial.println(mauSac);
}
