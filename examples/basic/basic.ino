/**
 * This example shows the basics of using EasyESP32Camera library
 */
#include <EasyESP32Camera.h>

/**
 * Firstly, create camera class instance 
 */
EasyESP32Camera camera;

uint32_t lastTime = 0;

void setup()
{
  delay(5000);
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial.println("Welcome to EasyESP32Camera basic example");

  Serial.println("Initializing the camera module");
  /**
   * Call init function to initialize the camera module
   * It takes up to 3 arguments:
   *  1. Camera model (required) 
   *  2. Camera settings (optional)
   *  3. Shot settings (optional)
   * 
   * Available camera models:
   *  WROVER_KIT,
   *  M5STACK_PSRAM,
   *  M5STACK_WITHOUT_PSRAM,
   *  AI_THINKER
   * 
   *  Init function returns true or false depending on init success
   */
  bool success = camera.init(BoardModel::AI_THINKER);
  Serial.print("Initialization ");
  Serial.println(success ? "success" : "fail");

  lastTime = millis();
}

void loop()
{
  /**
   * Take a shot every 5 seconds
   */
  if (millis() - lastTime >= 5000) {
    Serial.println("Taking a picture");

    /**
     * To take a picture call takePicture function
     * it takes up to 3 arguments:
     *  1. enable flash LED
     *  2. Callback on success
     *  3. Callback on fail
     */
    camera.takePicture(
        false,
        [](camera_fb_t* frameBuffer) {
          Serial.println("Picture has been taken successfully, frame buffer length: " + String(frameBuffer->len));
        },
        []() {
          Serial.println("Something went wrong while attempting to take a picture");
        });

    lastTime = millis();
  }
}
