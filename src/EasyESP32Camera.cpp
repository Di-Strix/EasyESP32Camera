#include "EasyESP32Camera.h"

bool EasyESP32Camera::init(BoardModel boardModel, CameraSettings cameraSettings, ShotSettings shotSettings)
{
  camera_config_t cameraConfig;
  switch (boardModel) {
  case BoardModel::WROVER_KIT:
    cameraConfig.pin_pwdn = -1;
    cameraConfig.pin_reset = -1;
    cameraConfig.pin_xclk = 21;
    cameraConfig.pin_sccb_sda = 26;
    cameraConfig.pin_sccb_scl = 27;

    cameraConfig.pin_d7 = 35;
    cameraConfig.pin_d6 = 34;
    cameraConfig.pin_d5 = 39;
    cameraConfig.pin_d4 = 36;
    cameraConfig.pin_d3 = 19;
    cameraConfig.pin_d2 = 18;
    cameraConfig.pin_d1 = 5;
    cameraConfig.pin_d0 = 4;
    cameraConfig.pin_vsync = 25;
    cameraConfig.pin_href = 23;
    cameraConfig.pin_pclk = 22;
    break;

  case BoardModel::M5STACK_PSRAM:
    cameraConfig.pin_pwdn = -1;
    cameraConfig.pin_reset = 15;
    cameraConfig.pin_xclk = 27;
    cameraConfig.pin_sccb_sda = 25;
    cameraConfig.pin_sccb_scl = 23;

    cameraConfig.pin_d7 = 19;
    cameraConfig.pin_d6 = 36;
    cameraConfig.pin_d5 = 18;
    cameraConfig.pin_d4 = 39;
    cameraConfig.pin_d3 = 5;
    cameraConfig.pin_d2 = 34;
    cameraConfig.pin_d1 = 35;
    cameraConfig.pin_d0 = 32;
    cameraConfig.pin_vsync = 22;
    cameraConfig.pin_href = 26;
    cameraConfig.pin_pclk = 21;
    break;

  case BoardModel::M5STACK_WITHOUT_PSRAM:
    cameraConfig.pin_pwdn = -1;
    cameraConfig.pin_reset = 15;
    cameraConfig.pin_xclk = 27;
    cameraConfig.pin_sccb_sda = 25;
    cameraConfig.pin_sccb_scl = 23;

    cameraConfig.pin_d7 = 19;
    cameraConfig.pin_d6 = 36;
    cameraConfig.pin_d5 = 18;
    cameraConfig.pin_d4 = 39;
    cameraConfig.pin_d3 = 5;
    cameraConfig.pin_d2 = 34;
    cameraConfig.pin_d1 = 35;
    cameraConfig.pin_d0 = 17;
    cameraConfig.pin_vsync = 22;
    cameraConfig.pin_href = 26;
    cameraConfig.pin_pclk = 21;
    break;

  case BoardModel::AI_THINKER:
    cameraConfig.pin_pwdn = 32;
    cameraConfig.pin_reset = -1;
    cameraConfig.pin_xclk = 0;
    cameraConfig.pin_sccb_sda = 26;
    cameraConfig.pin_sccb_scl = 27;

    cameraConfig.pin_d7 = 35;
    cameraConfig.pin_d6 = 34;
    cameraConfig.pin_d5 = 39;
    cameraConfig.pin_d4 = 36;
    cameraConfig.pin_d3 = 21;
    cameraConfig.pin_d2 = 19;
    cameraConfig.pin_d1 = 18;
    cameraConfig.pin_d0 = 5;
    cameraConfig.pin_vsync = 25;
    cameraConfig.pin_href = 23;
    cameraConfig.pin_pclk = 22;
    break;
  };

  cameraConfig.ledc_channel = cameraSettings.ledcChannel;
  cameraConfig.ledc_timer = cameraSettings.ledcTimer;
  cameraConfig.xclk_freq_hz = cameraSettings.xclkFreqHz;
  cameraConfig.pixel_format = cameraSettings.pixelFormat;
  cameraConfig.frame_size = psramFound() ? cameraSettings.maxFrameSizeWithPSRam : cameraSettings.maxFrameSizeWithoutPSRam;
  cameraConfig.fb_count = psramFound() ? cameraSettings.frameBufferCountWithPSRam : cameraSettings.frameBufferCountWithoutPSRam;
  cameraConfig.fb_location = psramFound() ? camera_fb_location_t::CAMERA_FB_IN_PSRAM : camera_fb_location_t::CAMERA_FB_IN_DRAM;
  cameraConfig.grab_mode = cameraSettings.grabMode;

  this->cameraConfig = cameraConfig;

  esp_err_t err = esp_camera_init(&this->cameraConfig);
  if (err != ESP_OK) {
    ESP_LOGE("EasyESP32Camera", "Camera init failed with error 0x%x", err);
    return false;
  }

  this->setShotSettings(shotSettings);
  this->setFlashLedSettings(cameraSettings.flashLedPin, cameraSettings.flashLedInvert);

  this->initialized = true;

  return true;
}

void EasyESP32Camera::setShotSettings(ShotSettings shotSettings)
{
  sensor_t* sensor = esp_camera_sensor_get();

  if (!this->initialized || this->shotSettings.frameSize != shotSettings.frameSize)
    sensor->set_framesize(sensor, std::min(this->cameraConfig.frame_size, shotSettings.frameSize));

  if (!this->initialized || this->shotSettings.quality != shotSettings.quality)
    sensor->set_quality(sensor, shotSettings.quality);

  if (!this->initialized || this->shotSettings.brightness != shotSettings.brightness)
    sensor->set_brightness(sensor, shotSettings.brightness);

  if (!this->initialized || this->shotSettings.contrast != shotSettings.contrast)
    sensor->set_contrast(sensor, shotSettings.contrast);

  if (!this->initialized || this->shotSettings.saturation != shotSettings.saturation)
    sensor->set_saturation(sensor, shotSettings.saturation);

  if (!this->initialized || this->shotSettings.sharpness != shotSettings.sharpness)
    sensor->set_sharpness(sensor, shotSettings.sharpness);

  if (!this->initialized || this->shotSettings.specialEffect != shotSettings.specialEffect)
    sensor->set_special_effect(sensor, shotSettings.specialEffect);

  if (!this->initialized || this->shotSettings.AWB != shotSettings.AWB)
    sensor->set_whitebal(sensor, shotSettings.AWB);

  if (!this->initialized || this->shotSettings.AWBGain != shotSettings.AWBGain)
    sensor->set_awb_gain(sensor, shotSettings.AWBGain);

  if (!this->initialized || this->shotSettings.WBMode != shotSettings.WBMode)
    sensor->set_wb_mode(sensor, shotSettings.WBMode);

  if (!this->initialized || this->shotSettings.AEC != shotSettings.AEC)
    sensor->set_exposure_ctrl(sensor, shotSettings.AEC);

  if (!this->initialized || this->shotSettings.AEC_DSP != shotSettings.AEC_DSP)
    sensor->set_aec2(sensor, shotSettings.AEC_DSP);

  if (!this->initialized || this->shotSettings.AELevel != shotSettings.AELevel)
    sensor->set_ae_level(sensor, shotSettings.AELevel);

  if (!this->initialized || this->shotSettings.AECValue != shotSettings.AECValue)
    sensor->set_ae_level(sensor, shotSettings.AECValue);

  if (!this->initialized || this->shotSettings.AGC != shotSettings.AGC)
    sensor->set_gain_ctrl(sensor, shotSettings.AGC);

  if (!this->initialized || this->shotSettings.AGCGain != shotSettings.AGCGain)
    sensor->set_agc_gain(sensor, shotSettings.AGCGain);

  if (!this->initialized || this->shotSettings.gainceiling != shotSettings.gainceiling)
    sensor->set_gainceiling(sensor, shotSettings.gainceiling);

  if (!this->initialized || this->shotSettings.blackPixelCorrection != shotSettings.blackPixelCorrection)
    sensor->set_bpc(sensor, shotSettings.blackPixelCorrection);

  if (!this->initialized || this->shotSettings.whitePixelCorrection != shotSettings.whitePixelCorrection)
    sensor->set_wpc(sensor, shotSettings.whitePixelCorrection);

  if (!this->initialized || this->shotSettings.rawGMA != shotSettings.rawGMA)
    sensor->set_raw_gma(sensor, shotSettings.rawGMA);

  if (!this->initialized || this->shotSettings.lensCorrection != shotSettings.lensCorrection)
    sensor->set_lenc(sensor, shotSettings.lensCorrection);

  if (!this->initialized || this->shotSettings.horizontalMirror != shotSettings.horizontalMirror)
    sensor->set_hmirror(sensor, shotSettings.horizontalMirror);

  if (!this->initialized || this->shotSettings.verticalFlip != shotSettings.verticalFlip)
    sensor->set_vflip(sensor, shotSettings.verticalFlip);

  if (!this->initialized || this->shotSettings.downsize != shotSettings.downsize)
    sensor->set_dcw(sensor, shotSettings.downsize);

  if (!this->initialized || this->shotSettings.colorbar != shotSettings.colorbar)
    sensor->set_colorbar(sensor, shotSettings.colorbar);

  this->shotSettings = shotSettings;
}

ShotSettings EasyESP32Camera::getShotSettings()
{
  return this->shotSettings;
}

void EasyESP32Camera::setFlashLedSettings(int pin, bool invert)
{
  this->flashPin = pin;
  this->flashInverted = invert;

  if (flashPin >= 0) {
    pinMode(flashPin, OUTPUT);
    digitalWrite(this->flashPin, this->flashInverted);
  }
}

void EasyESP32Camera::takePicture(bool withFlash, std::function<void(camera_fb_t*)> onSuccess, std::function<void()> onFailure)
{
  if (!this->initialized) {
    ESP_LOGE("EasyESP32Camera", "Failed to take picture before camera initialization");
    return;
  }

  camera_fb_t* frameBuffer = nullptr;

  if (withFlash && this->flashPin >= 0) {
    digitalWrite(this->flashPin, !this->flashInverted);
    delay(this->shotSettings.flashDelay);
    frameBuffer = esp_camera_fb_get();
    digitalWrite(this->flashPin, this->flashInverted);
  } else {
    frameBuffer = esp_camera_fb_get();
  }

  if (!frameBuffer) {
    ESP_LOGE("EasyESP32Camera", "Error taking picture");
    onFailure();
    return;
  }

  onSuccess(frameBuffer);

  esp_camera_fb_return(frameBuffer);
}

EasyESP32Camera::~EasyESP32Camera()
{
  esp_camera_deinit();
}