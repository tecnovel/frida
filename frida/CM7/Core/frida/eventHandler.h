/*
 * eventHandler.h
 *
 *  Created on: May 2, 2024
 *      Author: kaestli_a
 */

#ifndef FRIDA_EVENTHANDLER_H_
#define FRIDA_EVENTHANDLER_H_

#include "websrv/mongoose/mongoose.h"

struct websocket_connection {
  struct mg_connection *conn;
  struct websocket_connection *next;
};




void fn(struct mg_connection *c, int ev, void *ev_data);

void broadcast_message(const char *message);
int buffer_pop_char();

#endif /* FRIDA_EVENTHANDLER_H_ */
