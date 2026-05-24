# ESP32 OTA GitHub Pages

## Cấu trúc

- index.html
- firmware.bin
- version.txt

## Deploy

1. Tạo repository GitHub:
   esp32-ota

2. Upload toàn bộ file lên repository.

3. Vào:
   Settings -> Pages

4. Chọn:
   Deploy from branch

5. Branch:
   main

6. Save

Sau vài phút web sẽ có dạng:

https://YOUR_USERNAME.github.io/esp32-ota/

## OTA URLs

Version:
https://YOUR_USERNAME.github.io/esp32-ota/version.txt

Firmware:
https://YOUR_USERNAME.github.io/esp32-ota/firmware.bin

## Update firmware

- Build firmware .bin từ Arduino IDE
- Đổi tên thành firmware.bin
- Upload đè lên file cũ
- Sửa version.txt
- Commit changes

ESP32 sẽ tự update.
