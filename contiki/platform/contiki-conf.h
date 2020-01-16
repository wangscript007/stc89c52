#ifndef CONTIKI_CONF_H_
#define CONTIKI_CONF_H_

#include "8051def.h"
#include "sys/cc.h"
#include <string.h>

/* Include Project Specific conf */
#ifdef PROJECT_CONF_H
#include "project-conf.h"
#endif /* PROJECT_CONF_H */

/*
 * Build for SmartRF05EB+CC2530EM by default.
 * This define needs to have its final value before inclusion of models.h
 */
#ifndef MODELS_CONF_CC2531_USB_STICK
#define MODELS_CONF_CC2531_USB_STICK 0
#endif


/*
 * Define this as 1 to poll the etimer process from within main instead of from
 * the clock ISR. This reduces the ISR's stack usage and may prevent crashes.
 */
#ifndef CLOCK_CONF_STACK_FRIENDLY
#define CLOCK_CONF_STACK_FRIENDLY 1
#endif

#ifndef STACK_CONF_DEBUGGING
#define STACK_CONF_DEBUGGING  0
#endif

/* Energest Module */
#ifndef ENERGEST_CONF_ON
#define ENERGEST_CONF_ON      0
#endif

/* Verbose Startup? Turning this off saves plenty of bytes of CODE in HOME */
#ifndef STARTUP_CONF_VERBOSE
#define STARTUP_CONF_VERBOSE  0
#endif

/* More CODE space savings by turning off process names */
#define PROCESS_CONF_NO_PROCESS_NAMES 1

/*
 * USARTs:
 *   SmartRF RS232 -> USART0 / Alternative 1 (UART)
 *   SmartRF LCD   -> USART1 / Alternative 2 (SPI)
 */
#define UART_ON_USART     0

#define UART1_CONF_ENABLE 0

#ifndef UART0_CONF_ENABLE
#define UART0_CONF_ENABLE  1
#endif
#ifndef UART0_CONF_WITH_INPUT
#define UART0_CONF_WITH_INPUT 0
#endif

#ifndef UART0_CONF_HIGH_SPEED
#define UART0_CONF_HIGH_SPEED 0
#endif

/* USB output buffering enabled by default (relevant to cc2531 builds only) */
#ifndef USB_SERIAL_CONF_BUFFERED
#define USB_SERIAL_CONF_BUFFERED 1
#endif

#define SLIP_RADIO_CONF_NO_PUTCHAR 1

#if defined (UIP_FALLBACK_INTERFACE) || defined (CMD_CONF_OUTPUT)
#define SLIP_ARCH_CONF_ENABLE      1
#endif


/* Code Shortcuts */
/*
 * When set, this directive also configures the following bypasses:
 *   - process_post_synch() in tcpip_input() (we call packet_input())
 *   - process_post_synch() in tcpip_uipcall (we call the relevant pthread)
 *   - mac_call_sent_callback() is replaced with sent() in various places
 *
 * These are good things to do, they reduce stack usage and prevent crashes
 */
#define NETSTACK_CONF_SHORTCUTS   0

/*
 * By default we read our MAC from the (read-only) Information Page (primary
 * location). In order to have a user-programmable mac, define this as 0 to
 * use the secondary location (addresses 0xFFE8 - 0xFFEF on the last flash page)
 */
#ifndef CC2530_CONF_MAC_FROM_PRIMARY
#define CC2530_CONF_MAC_FROM_PRIMARY 0
#endif

/* Interrupt Number 6: Shared between P2 Inputs, I2C and USB
 * A single ISR handles all of the above. Leave this as is if you are not
 * interested in any of the above. Define as 1 (e.g. in project-conf.h) if
 * at least one of those interrupt sources will need handled */
#ifndef PORT_2_ISR_ENABLED
#define PORT_2_ISR_ENABLED 0
#endif

/*
 * Sensors
 * It is harmless to #define XYZ 1
 * even if the sensor is not present on our device
 */
#ifndef BUTTON_SENSOR_CONF_ON
#define BUTTON_SENSOR_CONF_ON   0  /* Buttons */
#endif

/* B2 on the cc2531 USB stick can be a reset button or a general-purpose one */
#ifndef CC2531_CONF_B2_REBOOTS
#define CC2531_CONF_B2_REBOOTS        0  /* General Purpose by default */
#endif

/* ADC - Turning this off will disable everything below */
#ifndef ADC_SENSOR_CONF_ON
#define ADC_SENSOR_CONF_ON      0
#endif
#define TEMP_SENSOR_CONF_ON     0  /* Temperature */
#define VDD_SENSOR_CONF_ON      0  /* Supply Voltage */
#define BATTERY_SENSOR_CONF_ON  0  /* Battery */

/* Low Power Modes - We only support PM0/Idle and PM1 */
#ifndef LPM_CONF_MODE
#define LPM_CONF_MODE         0 /* 0: no LPM, 1: MCU IDLE, 2: Drop to PM1 */
#endif


#ifndef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM                    6
#endif


#endif /* CONTIKI_CONF_H_ */
