/**
 * This Library was originally written by Olivier Van den Eede (4ilo) in 2016.
 * Some refactoring was done and SPI support was added by Aleksander Alekseev (afiskon) in 2018.
 *
 * https://github.com/afiskon/stm32-ssd1306
 */

#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <stddef.h>
#include "my_lpc1114fn28.h"
#include "ssd1306_fonts.h"

#ifdef SSD1306_X_OFFSET
#define SSD1306_X_OFFSET_LOWER (SSD1306_X_OFFSET & 0x0F)
#define SSD1306_X_OFFSET_UPPER ((SSD1306_X_OFFSET >> 4) & 0x07)
#else
#define SSD1306_X_OFFSET_LOWER 0
#define SSD1306_X_OFFSET_UPPER 0
#endif

// SSD1306 OLED height in pixels
#define SSD1306_HEIGHT          64
// SSD1306 width in pixels
#define SSD1306_WIDTH           128

#define SSD1306_BUFFER_SIZE   SSD1306_WIDTH * SSD1306_HEIGHT / 8

/* Display Control Macros */
#define SSD1306_CS_SET()	  Chip_GPIO_SetPinState(LPC_GPIO, 0, 2, true);
#define SSD1306_CS_CLR()	  Chip_GPIO_SetPinState(LPC_GPIO, 0, 2, false);
#define SSD1306_RES_SET()  Chip_GPIO_SetPinState(LPC_GPIO, 1, 5, true);
#define SSD1306_RES_CLR()  Chip_GPIO_SetPinState(LPC_GPIO, 1, 5, false);
#define SSD1306_DC_SET()	  Chip_GPIO_SetPinState(LPC_GPIO, 1, 4, true);
#define SSD1306_DC_CLR()	  Chip_GPIO_SetPinState(LPC_GPIO, 1, 4, false);

enum SSD1306_Commands {
// Fundamental Command
	SSD1306_COMMAND_SET_CONTRAST_CONTROL = 0x81,
	SSD1306_COMMAND_DISPLAY_ALL_ON_RESUME = 0xA4,
	SSD1306_COMMAND_SET_ENTIRE_DISPLAY_ON = 0xA5,
	SSD1306_COMMAND_SET_NORMAL_DISPLAY = 0xA6,
	SSD1306_COMMAND_SET_INVERT_DISPLAY = 0xA7,
	SSD1306_COMMAND_SET_DISPLAY_OFF = 0xAE,
	SSD1306_COMMAND_SET_DISPLAY_ON = 0xAF,
// Scrolling Command
	SSD1306_COMMAND_HORIZONTAL_SCROLL_RIGHT = 0x26,
	SSD1306_COMMAND_HORIZONTAL_SCROLL_LEFT = 0x27,
	SSD1306_COMMAND_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29,
	SSD1306_COMMAND_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A,
	SSD1306_COMMAND_DEACTIVATE_SCROLL= 0x2E,
	SSD1306_COMMAND_ACTIVATE_SCROLL = 0x2F,
	SSD1306_COMMAND_SET_VERTICAL_SCROLL_AREA = 0xA3,
// Addressing Setting Command
	// 10~1F : Set Higher Column Start Address for Page Addressing Mode
	SSD1306_COMMAND_SET_MEMORY_MODE = 0x20,
	SSD1306_COMMAND_SET_COLUMN_ADDR = 0x21,
	SSD1306_COMMAND_SET_PAGE_ADDR = 0x22,
	// B0~B7 : Set Page Start Address for Page Addressing Mode
// Hardware Configuration (Panel resolution & layout related) Command
	// 40~F7 : Set Display Start Line
	SSD1306_COMMAND_SET_START_LINE = 0x40,
	SSD1306_COMMAND_SET_SEG_REMAP0 = 0xA0,
	SSD1306_COMMAND_SET_SEG_REMAP1 = 0xA1,
	SSD1306_COMMAND_SET_MULTIPLEX_RATIO = 0xA8,
	SSD1306_COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL = 0xC0,
	SSD1306_COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION_REMAPPED = 0xC8,
	SSD1306_COMMAND_SET_DISPLAY_OFFSET = 0xD3,
	SSD1306_COMMAND_SET_COM_PINS_HARDWARE_CONFIGURATION = 0xDA,
// Timing & Driving Scheme Setting Command
	SSD1306_COMMAND_SET_DISPLAY_CLOCK_DIVIDE_RATIO = 0xD5,
	SSD1306_COMMAND_SET_PRECHARGE_PERIOD = 0xD9,
	SSD1306_COMMAND_SET_VCOMH_DESELECT_LEVEL = 0xDB,
	SSD1306_COMMAND_NOP = 0xE3,
// Advance Graphic Command
	SSD1306_COMMAND_SET_FADE_OUT_AND_BLINKING = 0x23,
	SSD1306_COMMAND_SET_ZOOM_IN = 0xD6,
// Charge Pump Command
	SSD1306_COMMAND_CHARGE_PUMP = 0x8D,
};

/**
 * @brief ssd1306 address pin enumeration definition
 */
typedef enum
{
    SSD1306_ADDR_SA0_0 = 0x78,        /**< address pin GND */
    SSD1306_ADDR_SA0_1 = 0x7A,        /**< address pin VCC */
} ssd1306_address_t;

/**
 * @brief ssd1306 memory addressing mode enumeration definition
 */
typedef enum
{
    SSD1306_MEMORY_ADDRESSING_MODE_HORIZONTAL = 0x00,        /**< horizontal addressing mode */
    SSD1306_MEMORY_ADDRESSING_MODE_VERTICAL   = 0x01,        /**< vertical addressing mode */
    SSD1306_MEMORY_ADDRESSING_MODE_PAGE       = 0x02,        /**< page addressing mode */
} ssd1306_memory_addressing_mode_t;

/**
 * @brief ssd1306 fade blinking mode enumeration definition
 */
typedef enum
{
    SSD1306_FADE_BLINKING_MODE_DISABLE  = 0x00,        /**< disable fade blinking mode */
    SSD1306_FADE_BLINKING_MODE_FADE_OUT = 0x02,        /**< fade out fade blinking mode */
    SSD1306_FADE_BLINKING_MODE_BLINKING = 0x03,        /**< blinking fade blinking mode */
} ssd1306_fade_blinking_mode_t;

/**
 * @brief ssd1306 scroll frame enumeration definition
 */
typedef enum
{
    SSD1306_SCROLL_FRAME_2    = 0x07,        /**< scroll frame 2 */
    SSD1306_SCROLL_FRAME_3    = 0x04,        /**< scroll frame 3 */
    SSD1306_SCROLL_FRAME_4    = 0x05,        /**< scroll frame 4 */
    SSD1306_SCROLL_FRAME_5    = 0x00,        /**< scroll frame 5 */
    SSD1306_SCROLL_FRAME_25   = 0x06,        /**< scroll frame 25 */
    SSD1306_SCROLL_FRAME_64   = 0x01,        /**< scroll frame 64 */
    SSD1306_SCROLL_FRAME_128  = 0x02,        /**< scroll frame 128 */
    SSD1306_SCROLL_FRAME_256  = 0x03,        /**< scroll frame 256 */
} ssd1306_scroll_frame_t;

/**
 * @brief ssd1306 charge pump enumeration definition
 */
typedef enum
{
    SSD1306_CHARGE_PUMP_DISABLE = 0x00,        /**< charge pump disable */
    SSD1306_CHARGE_PUMP_ENABLE  = 0x01,        /**< charge pump enable */
} ssd1306_charge_pump_t;

/**
 * @brief ssd1306 segment column remap enumeration definition
 */
typedef enum
{
    SSD1306_SEGMENT_COLUMN_ADDRESS_0   = 0x00,        /**< segment column remap address 0 */
    SSD1306_SEGMENT_COLUMN_ADDRESS_127 = 0x01,        /**< segment column remap address 127 */
} ssd1306_segment_column_remap_t;

/**
 * @brief ssd1306 entire display enumeration definition
 */
typedef enum
{
    SSD1306_ENTIRE_DISPLAY_OFF = 0x00,        /**< entire display off */
    SSD1306_ENTIRE_DISPLAY_ON  = 0x01,        /**< entire display on */
} ssd1306_entire_display_t;

/**
 * @brief ssd1306 display mode enumeration definition
 */
typedef enum
{
    SSD1306_DISPLAY_MODE_NORMAL  = 0x00,        /**< display mode normal */
    SSD1306_DISPLAY_MODE_INVERSE = 0x01,        /**< display mode inverse */
} ssd1306_display_mode_t;

/**
 * @brief ssd1306 display enumeration definition
 */
typedef enum
{
    SSD1306_DISPLAY_OFF = 0x00,        /**< close display */
    SSD1306_DISPLAY_ON  = 0x01,        /**< open display */
} ssd1306_display_t;

/**
 * @brief ssd1306 scan direction enumeration definition
 */
typedef enum
{
    SSD1306_SCAN_DIRECTION_COM0_START   = 0x00,        /**< scan direction com 0 start */
    SSD1306_SCAN_DIRECTION_COMN_1_START = 0x01,        /**< scan direction com N-1 start */
} ssd1306_scan_direction_t;

/**
 * @brief ssd1306 zoom in enumeration definition
 */
typedef enum
{
    SSD1306_ZOOM_IN_DISABLE = 0x00,        /**< disable zoom in */
    SSD1306_ZOOM_IN_ENABLE  = 0x01,        /**< enable zoom in */
} ssd1306_zoom_in_t;

/**
 * @brief ssd1306 pin conf enumeration definition
 */
typedef enum
{
    SSD1306_PIN_CONF_SEQUENTIAL  = 0x00,        /**< pin conf sequential */
    SSD1306_PIN_CONF_ALTERNATIVE = 0x01,        /**< pin conf alternative */
} ssd1306_pin_conf_t;

/**
 * @brief ssd1306 left right remap enumeration definition
 */
typedef enum
{
    SSD1306_LEFT_RIGHT_REMAP_DISABLE = 0x00,        /**< disable left right remap */
    SSD1306_LEFT_RIGHT_REMAP_ENABLE  = 0x01,        /**< enable left right remap */
} ssd1306_left_right_remap_t;

/**
 * @brief ssd1306 deselect level enumeration definition
 */
typedef enum
{
    SSD1306_DESELECT_LEVEL_0P65 = 0x00,        /**< deselect level 0.65 */
    SSD1306_DESELECT_LEVEL_0P77 = 0x02,        /**< deselect level 0.77 */
    SSD1306_DESELECT_LEVEL_0P83 = 0x03,        /**< deselect level 0.83 */
} ssd1306_deselect_level_t;


// Enumeration for screen colors
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01  // Pixel is set. Color depends on OLED
} SSD1306_COLOR;

typedef enum {
    SSD1306_OK = 0x00,
    SSD1306_ERR = 0x01  // Generic error.
} SSD1306_Error_t;

// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Initialized;
    uint8_t DisplayOn;
} SSD1306_t;

typedef struct {
    uint8_t x;
    uint8_t y;
} SSD1306_VERTEX;

// Screenbuffer
extern uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];

// Screen object
extern SSD1306_t SSD1306;

// Procedure definitions
void ssd1306_Init(void);
void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_RefreshScreen(void);
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);

/**
 * @brief Sets the contrast of the display.
 * @param[in] value contrast to set.
 * @note Contrast increases as the value increases.
 * @note RESET = 7Fh.
 */
void ssd1306_SetContrast(const uint8_t value);

/**
 * @brief Set Display ON/OFF.
 * @param[in] on 0 for OFF, any for ON.
 */
void ssd1306_SetDisplayOn(const uint8_t on);

/**
 * @brief Reads DisplayOn state.
 * @return  0: OFF.
 *          1: ON.
 */
uint8_t ssd1306_GetDisplayOn();

// Low-level procedures
void ssd1306_Reset(void);
void ssd1306_SendCommand(uint8_t byte);
void ssd1306_SendData(uint8_t* buffer, size_t buff_size);
SSD1306_Error_t ssd1306_FillBuffer(uint8_t* buf, uint32_t len);


#endif // __SSD1306_H__
