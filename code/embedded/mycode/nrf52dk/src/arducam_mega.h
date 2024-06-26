/* MIT License */

/* Copyright (c) [year] [fullname] */

/* Permission is hereby granted, free of charge, to any person obtaining a copy */
/* of this software and associated documentation files (the "Software"), to deal */
/* in the Software without restriction, including without limitation the rights */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell */
/* copies of the Software, and to permit persons to whom the Software is */
/* furnished to do so, subject to the following conditions: */

/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software. */

/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE */
/* SOFTWARE. */

#ifndef __ARDUCAM_MEGA_H__
#define __ARDUCAM_MEGA_H__

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

#define ARDUCHIP_FRAMES     0x01
#define ARDUCHIP_TEST1      0x00 // TEST register
#define ARDUCHIP_FIFO       0x04 // FIFO and I2C control
#define ARDUCHIP_FIFO_2     0x07 // FIFO and I2C control
#define FIFO_CLEAR_MASK     0x80
#define FIFO_CLEAR_ID_MASK  0x01
#define FIFO_START_MASK     0x02
#define FIFO_RDPTR_RST_MASK 0x10
#define FIFO_WRPTR_RST_MASK 0x20

#define ARDUCHIP_TRIG 0x44 // Trigger source
#define VSYNC_MASK    0x01
#define SHUTTER_MASK  0x02
#define CAP_DONE_MASK 0x04

#define FIFO_SIZE1 0x45 // Camera write FIFO size[7:0] for burst to read
#define FIFO_SIZE2 0x46 // Camera write FIFO size[15:8]
#define FIFO_SIZE3 0x47 // Camera write FIFO size[18:16]

#define BURST_FIFO_READ  0x3C // Burst FIFO read operation
#define SINGLE_FIFO_READ 0x3D // Single FIFO read operation

#define BUF_MAX_LENGTH 255

#define CAPRURE_MAX_NUM 0xff

#define CAM_REG_POWER_CONTROL                      0X02
#define CAM_REG_SENSOR_RESET                       0X07
#define CAM_REG_FORMAT                             0X20
#define CAM_REG_CAPTURE_RESOLUTION                 0X21
#define CAM_REG_BRIGHTNESS_CONTROL                 0X22
#define CAM_REG_CONTRAST_CONTROL                   0X23
#define CAM_REG_SATURATION_CONTROL                 0X24
#define CAM_REG_EV_CONTROL                         0X25
#define CAM_REG_WHILEBALANCE_MODE_CONTROL          0X26
#define CAM_REG_COLOR_EFFECT_CONTROL               0X27
#define CAM_REG_SHARPNESS_CONTROL                  0X28
#define CAM_REG_AUTO_FOCUS_CONTROL                 0X29
#define CAM_REG_EXPOSURE_GAIN_WHILEBALANCE_CONTROL 0X2A
#define CAM_REG_MANUAL_GAIN_BIT_9_8                0X2B
#define CAM_REG_MANUAL_GAIN_BIT_7_0                0X2C
#define CAM_REG_MANUAL_EXPOSURE_BIT_19_16          0X2D
#define CAM_REG_MANUAL_EXPOSURE_BIT_15_8           0X2E
#define CAM_REG_MANUAL_EXPOSURE_BIT_7_0            0X2F
#define CAM_REG_SENSOR_ID                          0x40
#define CAM_REG_YEAR_ID                            0x41
#define CAM_REG_MONTH_ID                           0x42
#define CAM_REG_DAY_ID                             0x43
#define CAM_REG_SENSOR_STATE                       0x44
#define CAM_REG_DEBUG_DEVICE_ADDRESS               0X0A
#define CAM_REG_DEBUG_REGISTER_HIGH                0X0B
#define CAM_REG_DEBUG_REGISTER_LOW                 0X0C
#define CAM_REG_DEBUG_REGISTER_VALUE               0X0D

#define CAM_REG_SENSOR_STATE_IDLE (1 << 1)
#define CAM_SENSOR_RESET_ENABLE   (1 << 6)
#define CAM_FORMAT_BASICS         (0 << 0)
#define CAM_SET_CAPTURE_MODE      (0 << 7)
#define CAM_SET_VIDEO_MODE        (1 << 7)

#define SET_WHILEBALANCE 0X02
#define SET_EXPOSURE     0X01
#define SET_GAIN         0X00

#define CAMERA_TYPE_NUMBER 2

#define CAMERA_OV5640 0
#define CAMERA_OV3640 1

#define FORMAT_JPEG 0X01
#define FORMAT_RGB  0X02
#define FORMAT_YUV  0X03

#define RESOLUTION_160X120   (1 << 0)
#define RESOLUTION_320X240   (1 << 1)
#define RESOLUTION_640X480   (1 << 2)
#define RESOLUTION_800X600   (1 << 3)
#define RESOLUTION_1280X720  (1 << 4)
#define RESOLUTION_1280X960  (1 << 5)
#define RESOLUTION_1600X1200 (1 << 6)
#define RESOLUTION_1920X1080 (1 << 7)
#define RESOLUTION_2048X1536 (1 << 8)
#define RESOLUTION_2592X1944 (1 << 9)
#define RESOLUTION_320x320   (1 << 10)
#define RESOLUTION_128x128   (1 << 11)
#define RESOLUTION_96x96     (1 << 12)

#define SPECIAL_NORMAL    (0 << 0)
#define SPECIAL_BLUEISH   (1 << 0)
#define SPECIAL_REDISH    (1 << 1)
#define SPECIAL_BW        (1 << 2)
#define SPECIAL_SEPIA     (1 << 3)
#define SPECIAL_NEGATIVE  (1 << 4)
#define SPECIAL_GREENISH  (1 << 5)
#define SPECIAL_YELLOWISH (1 << 8)

/**
 * @enum CamStatus
 * @brief Camera status
 */
typedef enum {
	CAM_ERR_SUCCESS = 0,      /**<Operation succeeded*/
	CAM_ERR_NO_CALLBACK = -1, /**< No callback function is registered*/
} CamStatus;

/**
 * @enum CAM_IMAGE_MODE
 * @brief Configure camera resolution
 */
typedef enum {
	CAM_IMAGE_MODE_QQVGA = 0x00,   /**<160x120 */
	CAM_IMAGE_MODE_QVGA = 0x01,    /**<320x240*/
	CAM_IMAGE_MODE_VGA = 0x02,     /**<640x480*/
	CAM_IMAGE_MODE_SVGA = 0x03,    /**<800x600*/
	CAM_IMAGE_MODE_HD = 0x04,      /**<1280x720*/
	CAM_IMAGE_MODE_SXGAM = 0x05,   /**<1280x960*/
	CAM_IMAGE_MODE_UXGA = 0x06,    /**<1600x1200*/
	CAM_IMAGE_MODE_FHD = 0x07,     /**<1920x1080*/
	CAM_IMAGE_MODE_QXGA = 0x08,    /**<2048x1536*/
	CAM_IMAGE_MODE_WQXGA2 = 0x09,  /**<2592x1944*/
	CAM_IMAGE_MODE_96X96 = 0x0a,   /**<96x96*/
				       /// @cond
	CAM_IMAGE_MODE_128X128 = 0x0b, /**<128x128*/
	CAM_IMAGE_MODE_320X320 = 0x0c, /**<320x320*/
	CAM_IMAGE_MODE_12 = 0x0d,      /**<Reserve*/
	CAM_IMAGE_MODE_13 = 0x0e,      /**<Reserve*/
	CAM_IMAGE_MODE_14 = 0x0f,      /**<Reserve*/
	CAM_IMAGE_MODE_15 = 0x10,      /**<Reserve*/
				       /// @endcond
} CAM_IMAGE_MODE;

/**
 * @enum CAM_CONTRAST_LEVEL
 * @brief Configure camera contrast level
 */
typedef enum {
	CAM_CONTRAST_LEVEL_MINUS_3 = 6, /**<Level -3 */
	CAM_CONTRAST_LEVEL_MINUS_2 = 4, /**<Level -2 */
	CAM_CONTRAST_LEVEL_MINUS_1 = 2, /**<Level -1 */
	CAM_CONTRAST_LEVEL_DEFAULT = 0, /**<Level Default*/
	CAM_CONTRAST_LEVEL_1 = 1,       /**<Level +1 */
	CAM_CONTRAST_LEVEL_2 = 3,       /**<Level +2 */
	CAM_CONTRAST_LEVEL_3 = 5,       /**<Level +3 */
} CAM_CONTRAST_LEVEL;

/**
 * @enum CAM_EV_LEVEL
 * @brief Configure camera EV level
 */
typedef enum {
	CAM_EV_LEVEL_MINUS_3 = 6, /**<Level -3 */
	CAM_EV_LEVEL_MINUS_2 = 4, /**<Level -2 */
	CAM_EV_LEVEL_MINUS_1 = 2, /**<Level -1 */
	CAM_EV_LEVEL_DEFAULT = 0, /**<Level Default*/
	CAM_EV_LEVEL_1 = 1,       /**<Level +1 */
	CAM_EV_LEVEL_2 = 3,       /**<Level +2 */
	CAM_EV_LEVEL_3 = 5,       /**<Level +3 */
} CAM_EV_LEVEL;

/**
 * @enum CAM_SATURATION_LEVEL
 * @brief Configure camera saturation  level
 */
typedef enum {
	CAM_SATURATION_LEVEL_MINUS_3 = 6, /**<Level -3 */
	CAM_SATURATION_LEVEL_MINUS_2 = 4, /**<Level -2 */
	CAM_SATURATION_LEVEL_MINUS_1 = 2, /**<Level -1 */
	CAM_SATURATION_LEVEL_DEFAULT = 0, /**<Level Default*/
	CAM_SATURATION_LEVEL_1 = 1,       /**<Level +1 */
	CAM_SATURATION_LEVEL_2 = 3,       /**<Level +2 */
	CAM_SATURATION_LEVEL_3 = 5,       /**<Level +3 */
} CAM_SATURATION_LEVEL;

/**
 * @enum CAM_BRIGHTNESS_LEVEL
 * @brief Configure camera brightness level
 */
typedef enum {
	CAM_BRIGHTNESS_LEVEL_MINUS_4 = 8, /**<Level -4 */
	CAM_BRIGHTNESS_LEVEL_MINUS_3 = 6, /**<Level -3 */
	CAM_BRIGHTNESS_LEVEL_MINUS_2 = 4, /**<Level -2 */
	CAM_BRIGHTNESS_LEVEL_MINUS_1 = 2, /**<Level -1 */
	CAM_BRIGHTNESS_LEVEL_DEFAULT = 0, /**<Level Default*/
	CAM_BRIGHTNESS_LEVEL_1 = 1,       /**<Level +1 */
	CAM_BRIGHTNESS_LEVEL_2 = 3,       /**<Level +2 */
	CAM_BRIGHTNESS_LEVEL_3 = 5,       /**<Level +3 */
	CAM_BRIGHTNESS_LEVEL_4 = 7,       /**<Level +4 */
} CAM_BRIGHTNESS_LEVEL;

/**
 * @enum CAM_SHARPNESS_LEVEL
 * @brief Configure camera Sharpness level
 */
typedef enum {
	CAM_SHARPNESS_LEVEL_AUTO = 0, /**<Sharpness Auto */
	CAM_SHARPNESS_LEVEL_1,        /**<Sharpness Level 1 */
	CAM_SHARPNESS_LEVEL_2,        /**<Sharpness Level 2 */
	CAM_SHARPNESS_LEVEL_3,        /**<Sharpness Level 3 */
	CAM_SHARPNESS_LEVEL_4,        /**<Sharpness Level 4 */
	CAM_SHARPNESS_LEVEL_5,        /**<Sharpness Level 5 */
	CAM_SHARPNESS_LEVEL_6,        /**<Sharpness Level 6 */
	CAM_SHARPNESS_LEVEL_7,        /**<Sharpness Level 7 */
	CAM_SHARPNESS_LEVEL_8,        /**<Sharpness Level 8 */
} CAM_SHARPNESS_LEVEL;

/**
 * @enum CAM_VIDEO_MODE
 * @brief Configure resolution in video streaming mode
 */
typedef enum {
	CAM_VIDEO_MODE_0 = 1, /**< 320x240 */
	CAM_VIDEO_MODE_1 = 2, /**< 640x480 */
} CAM_VIDEO_MODE;

/**
 * @enum CAM_IMAGE_PIX_FMT
 * @brief Configure image pixel format
 */
typedef enum {
	CAM_IMAGE_PIX_FMT_RGB565 = 0x02, /**< RGB565 format */
	CAM_IMAGE_PIX_FMT_JPG = 0x01,    /**< JPEG format */
	CAM_IMAGE_PIX_FMT_YUV = 0x03,    /**< YUV format */
	CAM_IMAGE_PIX_FMT_NONE,          /**< No defined format */
} CAM_IMAGE_PIX_FMT;

/**
 * @enum CAM_WHITE_BALANCE
 * @brief Configure white balance mode
 */
typedef enum {
	CAM_WHITE_BALANCE_MODE_DEFAULT = 0, /**< Auto */
	CAM_WHITE_BALANCE_MODE_SUNNY,       /**< Sunny */
	CAM_WHITE_BALANCE_MODE_OFFICE,      /**< Office */
	CAM_WHITE_BALANCE_MODE_CLOUDY,      /**< Cloudy*/
	CAM_WHITE_BALANCE_MODE_HOME,        /**< Home */
} CAM_WHITE_BALANCE;

/**
 * @enum CAM_COLOR_FX
 * @brief Configure special effects
 */
typedef enum {
	CAM_COLOR_FX_NONE = 0,      /**< no effect   */
	CAM_COLOR_FX_BLUEISH,       /**< cool light   */
	CAM_COLOR_FX_REDISH,        /**< warm   */
	CAM_COLOR_FX_BW,            /**< Black/white   */
	CAM_COLOR_FX_SEPIA,         /**<Sepia   */
	CAM_COLOR_FX_NEGATIVE,      /**<positive/negative inversion  */
	CAM_COLOR_FX_GRASS_GREEN,   /**<Grass green */
	CAM_COLOR_FX_OVER_EXPOSURE, /**<Over exposure*/
	CAM_COLOR_FX_SOLARIZE,      /**< Solarize   */
	CAM_COLOR_FX_YELLOWISH,
} CAM_COLOR_FX;

/**
 * @enum CAM_AUTO_FOCUS
 * @brief Configure auto focus mode
 */
typedef enum {
	CAM_AUTO_FOCUS_DEFAULT = 0, /**< default configuration; continuous auto focus */
	CAM_AUTO_FOCUS_SINGLE,      /**< single auto focus   */
	CAM_AUTO_FOCUS_CONTINUOUS,  /**< continuous auto focus   */
	CAM_AUTO_FOCUS_PAUSE,       /**< pause continuous auto focus   */
	CAM_AUTO_FOCUS_DISABLE,     /**< disable auto focus  */
} CAM_AUTO_FOCUS;

typedef uint8_t (*BUFFER_CALLBACK)(uint8_t *buffer,
				   uint8_t lenght); /**<Callback function prototype  */

struct camera_info {
	char *cameraId;                 /**<Model of camera module */
	int supportResolution;          /**<Resolution supported by the camera module */
	int supportSpecialEffects;      /**<Special effects supported by the camera module
					 */
	unsigned long exposureValueMax; /**<Maximum exposure time supported by the
					   camera module */
	unsigned int exposureValueMin;  /**<Minimum exposure time supported by the
					   camera module */
	unsigned int gainValueMax;      /**<Maximum gain supported by the camera module */
	unsigned int gainValueMin;      /**<Minimum gain supported by the camera module */
	unsigned char supportFocus;     /**<Does the camera module support the focus function */
	unsigned char supportSharpness; /**<Does the camera module support the
					   sharpening function */
	unsigned char deviceAddress;
};

struct arducam_mega_data {
	uint32_t totalLength;            /**< The total length of the picture */
	uint32_t receivedLength;         /**< The remaining length of the picture */
	uint8_t blockSize;               /**< The length of the callback function transmission */
	uint8_t cameraId;                /**< Model of camera module */
	uint8_t cameraDataFormat;        /**< The currently set image pixel format */
	uint8_t burstFirstFlag;          /**< Flag bit for reading data for the first time in
					    burst mode */
	uint8_t previewMode;             /**< Stream mode flag */
	uint8_t currentPixelFormat;      /**< The currently set image pixel format */
	uint8_t currentPictureMode;      /**< Currently set resolution */
	struct camera_info myCameraInfo; /**< Basic information of the current camera */
	const struct CameraOperations *arducamCameraOp; /**< Camera function interface */
	BUFFER_CALLBACK callBackFunction;               /**< Camera callback function */
};

struct arducam_mega_config {
	struct gpio_dt_spec gpio_dt;
	struct spi_dt_spec spi_dt;
	struct spi_config spi_cfg;
	CAM_IMAGE_MODE resolution;
};

int arducam_mega_init();
int arducam_mega_capture_image(CAM_IMAGE_MODE mode, CAM_IMAGE_PIX_FMT pixel_format);
int arducam_mega_save_image(char *filename, const char *mount_point, int image_length);
int arducam_mega_get_id();
CAM_IMAGE_MODE arducam_mega_get_resolution(char *resolution);

CAM_SATURATION_LEVEL arducam_mega_get_saturation(char *saturation);
CAM_CONTRAST_LEVEL arducam_mega_get_contrast(char *contrast);
CAM_BRIGHTNESS_LEVEL arducam_mega_get_brightness(char *brightness);

int arducam_mega_set_saturation(CAM_SATURATION_LEVEL saturation);
int arducam_mega_set_contrast(CAM_CONTRAST_LEVEL contrast);
int arducam_mega_set_brightness(CAM_BRIGHTNESS_LEVEL brightness);
int arducam_mega_set_autofocus(CAM_AUTO_FOCUS autofocus);

#endif /* __ARDUCAM_MEGA_H__ */