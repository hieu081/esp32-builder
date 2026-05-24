#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

const char* ssid = "Hieu T2";
const char* password = "08012004";

#define CURRENT_VERSION "1.0"

String versionURL =
"https://hieu081.github.io/esp32-ota/version.txt";

String firmwareURL =
"https://hieu081.github.io/esp32-ota/firmware.bin";

void firmwareUpdate() {

  WiFiClient client;
  HTTPClient http;

  http.begin(client, firmwareURL);

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {

    int contentLength = http.getSize();

    bool canBegin =
      Update.begin(contentLength);

    if (canBegin) {

      WiFiClient * stream =
        http.getStreamPtr();

      size_t written =
        Update.writeStream(*stream);

      if (written == contentLength) {

        Serial.println("Written OK");

      } else {

        Serial.println("Written Fail");
      }

      if (Update.end()) {

        Serial.println("OTA Done");

        if (Update.isFinished()) {

          Serial.println("Restarting...");

          ESP.restart();
        }

      } else {

        Serial.println(Update.errorString());
      }

    } else {

      Serial.println("Not enough space");
    }

  } else {

    Serial.println("HTTP Fail");
  }

  http.end();
}

void checkUpdate() {

  WiFiClient client;
  HTTPClient http;

  http.begin(client, versionURL);

  int httpCode = http.GET();

  if (httpCode == 200) {

    String newVersion =
      http.getString();

    newVersion.trim();

    Serial.println("Current: "
                    + String(CURRENT_VERSION));

    Serial.println("New: "
                    + newVersion);

    if (newVersion != CURRENT_VERSION) {

      Serial.println("Start OTA");

      firmwareUpdate();
    }
  }

  http.end();
}

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  checkUpdate();
}

void loop() {

}
