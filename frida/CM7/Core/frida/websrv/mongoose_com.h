/*
 * mongoose_com.h
 *
 *  Created on: May 2, 2024
 *      Author: kaestli_a
 */

#ifndef FRIDA_WEBSRV_MONGOOSE_COM_H_
#define FRIDA_WEBSRV_MONGOOSE_COM_H_


#include "mongoose/mongoose.h"

/**
  * @brief Callback function to receive network data.
  * @param buf Pointer to the received data buffer.
  * @param len Length of the received data.
  * @note This function is called when network data is received. It processes
  *       the received data and returns a boolean value indicating success.
  * @retval true if data is received successfully, false otherwise.
  */
bool tud_network_recv_cb(const uint8_t *buf, uint16_t len);

/**
  * @brief Callback function to initialize the network.
  * @note This function is called to initialize the network.
  * @retval None
  */
void tud_network_init_cb(void);

/**
  * @brief Initializes the MAC address for the network.
  * @note This function initializes the MAC address for the network interface.
  * @retval None
  */
void init_tud_network_mac_address(void);

/**
  * @brief Callback function to transmit network data.
  * @param dst Pointer to the destination buffer.
  * @param ref Reference pointer.
  * @param arg Argument indicating the length of data.
  * @note This function is called to transmit network data.
  * @retval Length of transmitted data.
  */
uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg);

/**
  * @brief Logs a character with optional parameters.
  * @param ch The character to log.
  * @param param Additional parameter.
  * @note This function logs a character with optional parameters.
  * @retval None
  */
void log_fn(char ch, void *param);

/**
  * @brief Sends data over USB.
  * @param buf Pointer to the data buffer.
  * @param len Length of the data buffer.
  * @param ifp Pointer to the network interface.
  * @note This function sends data over USB.
  * @retval Number of bytes sent.
  */
size_t usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp);

/**
  * @brief Checks if USB is up.
  * @param ifp Pointer to the network interface.
  * @note This function checks if USB is up.
  * @retval true if USB is up, false otherwise.
  */
bool usb_up(struct mg_tcpip_if *ifp);

/**
  * @brief Retrieves the current time in milliseconds.
  * @note This function retrieves the current time in milliseconds.
  * @retval Current time in milliseconds.
  */
uint64_t mg_millis(void);


#endif /* FRIDA_WEBSRV_MONGOOSE_COM_H_ */
