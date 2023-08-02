/**
 * This example shows what settings are available in ShotSettings structure and how to apply them
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

  Serial.println("Welcome to EasyESP32Camera ShotSettings example");

  Serial.println("Initializing the camera module");
  /**
   * As mentioned in basic and CameraSettings examples, init function takes up to 3 arguments. The third one is ShotSettings
   */

  /**
   * Create ShotSettings structure
   * Every parameter has its default value, but you may change them if you need
   *
   * List of parameters:
   *   +---------------+----------------------+--------------------------------+-------------+--------------------------------------------------------------------------+
   *   |   Data type   |    Parameter name    |          Default value         | Value range |                               Description                                |
   *   +---------------+----------------------+--------------------------------+-------------+--------------------------------------------------------------------------+
   *   | framesize_t   | frameSize            | framesize_t::FRAMESIZE_UXGA    |  QQVGA-UXGA | Do not use sizes above QVGA when pixelFormat is not JPEG                 |
   *   | uint8_t       | quality              | 4                              |   [0 - 63]  | lower - better. Values lower than 4 didn't work for me                   |
   *   | int8_t        | brightness           | 0                              |   [-2 - 2]  |                                                                          |
   *   | int8_t        | contrast             | 0                              |   [-2 - 2]  |                                                                          |
   *   | int8_t        | saturation           | 0                              |   [-2 - 2]  |                                                                          |
   *   | int8_t        | sharpness            | 0                              |   [-2 - 2]  |                                                                          |
   *   | uint8_t       | specialEffect        | 0                              |   [0 - 6]   | (No effect, Negative, Grayscale, Red Tint, Green Tint, Blue Tint, Sepia) |
   *   | bool          | AWB                  | true                           |      -      | Auto White Balance                                                       |
   *   | bool          | AWBGain              | true                           |      -      |                                                                          |
   *   | uint8_t       | WBMode               | 0                              |   [0 - 4]   | (Auto, Sunny, Cloudy, Office, Home) Only when AWBGain enabled            |
   *   | bool          | AEC                  | true                           |      -      | Automatic Exposure Correction                                            |
   *   | bool          | AEC_DSP              | true                           |      -      | Automatic Exposure Correction Digital Signal Processing                  |
   *   | int8_t        | AELevel              | 0                              |   [-2 - 2]  | Automatic Exposure Level                                                 |
   *   | uint16_t      | AECValue             | 0                              |  [0 - 1200] |                                                                          |
   *   | bool          | AGC                  | true                           |      -      | Automatic Gain Control                                                   |
   *   | uint8_t       | AGCGain              | 0                              |   [0 - 30]  | 1x - 31x. Automatic Gain Control Gain. Only when AGC is disabled         |
   *   | gainceiling_t | gainceiling          | gainceiling_t::GAINCEILING_2X  |      -      | 2x - 128x. Only when AGC is enabled                                      |
   *   | bool          | blackPixelCorrection | true                           |      -      |                                                                          |
   *   | bool          | whitePixelCorrection | true                           |      -      |                                                                          |
   *   | bool          | rawGMA               | true                           |      -      | Makes image brighter                                                     |
   *   | bool          | lensCorrection       | true                           |      -      | Corrects brightness fade on the edges of the image caused by lens        |
   *   | bool          | horizontalMirror     | false                          |      -      |                                                                          |
   *   | bool          | verticalFlip         | false                          |      -      |                                                                          |
   *   | bool          | downsize             | false                          |      -      |                                                                          |
   *   | bool          | colorbar             | false                          |      -      |                                                                          |
   *   +---------------+----------------------+--------------------------------+-------------+--------------------------------------------------------------------------+
   *
   *  For more info or some examples visit https://heyrick.eu/blog/index.php?diary=20210418&keitai=0
   */

  ShotSettings shotSettings;

  /**
   * Then provide these settings as third argument to init function
   */
  bool success = camera.init(BoardModel::AI_THINKER, CameraSettings(), shotSettings);
  Serial.print("Initialization ");
  Serial.println(success ? "success" : "fail");

  if (!success) {
    delay(5000);
    ESP.restart();
  }

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