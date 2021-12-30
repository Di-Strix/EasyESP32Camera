/**
 * This example shows what settings are available in CameraSettings structure and how to apply them
 */
#include <EasyESP32Camera.h>

EasyESP32Camera camera;

uint32_t lastTime = 0;

void setup()
{
  delay(5000);
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial.println("Welcome to EasyESP32Camera CameraSettings example");

  Serial.println("Initializing the camera module");
  /**
   * As mentioned in basic example, init function takes up to 3 arguments. The second is CameraSettings itself
   * The third one is ShotSettings. See correspoding example to learn more
   */

  /**
   * Create CameraSettings structure
   * Every parameter has its default value, but you may change them if you need
   * 
   * List of parameters:
   *  int             flashLedPin                     = -1;
   *  bool            flashLedInvert                  = false;
   *  pixformat_t     pixelFormat                     = pixformat_t::PIXFORMAT_JPEG;
   *  byte            frameBufferCountWithPSRam       = 2;
   *  byte            frameBufferCountWithoutPSRam    = 1;
   *  framesize_t     maxFrameSizeWithPSRam           = framesize_t::FRAMESIZE_UXGA;
   *  framesize_t     maxFrameSizeWithoutPSRam        = framesize_t::FRAMESIZE_CIF;
   *  int             xclkFreqHz                      = 20000000;
   *  ledc_channel_t  ledcChannel                     = LEDC_CHANNEL_0;
   *  ledc_timer_t    ledcTimer                       = LEDC_TIMER_0;
   */
  CameraSettings cameraSettings;

  /**
   * Most commonly you may want to setup the flash led, so you can do this by setting the flashLedPin field and invert it 
   * using flashLedInvert if needed
   */
  cameraSettings.flashLedPin = 4;

  /**
   * Then provide these settings as second argument to init function
   */
  bool success = camera.init(BoardModel::AI_THINKER, cameraSettings);
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
        true,
        [](camera_fb_t* frameBuffer) {
          Serial.println("Picture has been taken successfully, frame buffer length: " + String(frameBuffer->len));
        },
        []() {
          Serial.println("Something went wrong while attempting to take a picture");
        });

    lastTime = millis();
  }
}
