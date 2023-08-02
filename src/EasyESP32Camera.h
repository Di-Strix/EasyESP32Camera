#ifndef __EASY_ESP32_CAMERA_H__
#define __EASY_ESP32_CAMERA_H__

#include <Arduino.h>
#include <esp_camera.h>
#include <functional>

enum class BoardModel {
  WROVER_KIT,
  M5STACK_PSRAM,
  M5STACK_WITHOUT_PSRAM,
  AI_THINKER
};

struct CameraSettings {
  int flashLedPin = -1;
  bool flashLedInvert = false;
  pixformat_t pixelFormat = pixformat_t::PIXFORMAT_JPEG;
  byte frameBufferCountWithPSRam = 2;
  byte frameBufferCountWithoutPSRam = 1;
  camera_grab_mode_t grabMode = camera_grab_mode_t::CAMERA_GRAB_LATEST;
  framesize_t maxFrameSizeWithPSRam = framesize_t::FRAMESIZE_UXGA;
  framesize_t maxFrameSizeWithoutPSRam = framesize_t::FRAMESIZE_CIF;
  int xclkFreqHz = 20000000;
  ledc_channel_t ledcChannel = LEDC_CHANNEL_0;
  ledc_timer_t ledcTimer = LEDC_TIMER_0;
};

struct ShotSettings {
  framesize_t frameSize = framesize_t::FRAMESIZE_UXGA; //QQVGA-UXGA Do not use sizes above QVGA when not JPEG
  uint8_t quality = 4; //[0 - 63] lower - better. Values lower than 4 didn't work for me
  int8_t brightness = 0; //[-2 - 2]
  int8_t contrast = 0; //[-2 - 2]
  int8_t saturation = 0; //[-2 - 2]
  int8_t sharpness = 0; //[-2 - 2]
  uint8_t specialEffect = 0; //[0 - 6] (No effect, Negative, Grayscale, Red Tint, Green Tint, Blue Tint, Sepia)
  bool AWB = true; //Auto White Balance
  bool AWBGain = 1;
  uint8_t WBMode = 0; //[0 - 4] (Auto, Sunny, Cloudy, Office, Home) Only when AWBGain enabled
  bool AEC = true; //Automatic Exposure Correction
  bool AEC_DSP = true; //Automatic Exposure Correction Digital Signal Processing
  int8_t AELevel = 0; //[-2 - 2] Automatic Exposure Level
  uint16_t AECValue = 0; //[0 - 1200]
  bool AGC = true; //Automatic Gain Control
  uint8_t AGCGain = 0; //[0 - 30] 1x - 31x. Automatic Gain Control Gain. Only when AGC is disabled
  gainceiling_t gainceiling = gainceiling_t::GAINCEILING_2X; //2x - 128x. Only when AGC is enabled
  bool blackPixelCorrection = true;
  bool whitePixelCorrection = true;
  bool rawGMA = true; //Makes image brighter
  bool lensCorrection = true; //Corrects brightness fade on the edges of the image caused by lens
  bool horizontalMirror = false;
  bool verticalFlip = false;
  bool downsize = false;
  bool colorbar = false;

  uint16_t flashDelay = 1000; //Time to wait after enabling flash before taking photo (ms)
  //For detailed description of each parameter - visit https://heyrick.eu/blog/index.php?diary=20210418&keitai=0
};

class EasyESP32Camera {
  private:
  ShotSettings shotSettings;

  bool flashInverted = false;
  int flashPin = -1;

  bool initialized = false;

  public:
  ~EasyESP32Camera();

  /**
   * @brief Initialises camera module
   * 
   * @param BoardModel
   * @param CameraSettings
   * @param ShotSettings
   * 
   * @return true on successful init
   * @return false on init failure
   */
  bool init(BoardModel, CameraSettings = CameraSettings(), ShotSettings = ShotSettings());

  /**
   * @brief Sets the Shot Settings
   */
  void setShotSettings(ShotSettings);

  /**
   * @brief Gets the Shot Settings structure
   * 
   * @return ShotSettings 
   */
  ShotSettings getShotSettings();

  /**
   * @brief Sets the Flash Led Settings
   * 
   * @param pin Flash LED pin
   * @param invert invert ouput
   */
  void setFlashLedSettings(int pin, bool invert);

  /**
   * @brief Tries to take a picture and depending on the result calls corresponding callback
   * 
   * @param withFlash enable flash LED
   * @param onSuccess called on success
   * @param onFailure called on failure
   */
  void takePicture(bool withFlash, std::function<void(camera_fb_t*)> onSuccess, std::function<void()> onFailure = [](){});
};

#endif //__EASY_ESP32_CAMERA_H__