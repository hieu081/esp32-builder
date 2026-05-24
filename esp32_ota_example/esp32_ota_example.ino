#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>

const char* ssid = "Hieu T2";
const char* password = "08012004";

#define CURRENT_VERSION "1.0"

String versionURL =
"https://YOUR_USERNAME.github.io/esp32-ota/version.txt";

String firmwareURL =
"https://YOUR_USERNAME.github.io/esp32-ota/firmware.bin";

void checkUpdate(){

  HTTPClient http;

  http.begin(versionURL);

  int httpCode = http.GET();

  if(httpCode == 200){

    String newVersion = http.getString();

    newVersion.trim();

    Serial.println("Current Version: "
                    + String(CURRENT_VERSION));

    Serial.println("New Version: "
                    + newVersion);

    if(newVersion != CURRENT_VERSION){

      Serial.println("Updating Firmware...");

      t_httpUpdate_return ret =
      httpUpdate.update(firmwareURL);

      switch(ret){

        case HTTP_UPDATE_FAILED:

          Serial.printf("Update Failed Error (%d): %s\n",
          httpUpdate.getLastError(),
          httpUpdate.getLastErrorString().c_str());

        break;

        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("No Updates");
        break;

        case HTTP_UPDATE_OK:
          Serial.println("Update Success");
        break;
      }
    }
  }

  http.end();
}

void setup(){

  Serial.begin(115200);

  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  checkUpdate();
}

void loop(){

}
