/*
 * eventHandler.c
 *
 *  Created on: May 2, 2024
 *      Author: kaestli_a
 */

#include "eventHandler.h"

static struct websocket_connection *ws_connections = NULL;

void add_connection(struct mg_connection *conn) {
  struct websocket_connection *new_conn = (struct websocket_connection *)malloc(sizeof(struct websocket_connection));
  new_conn->conn = conn;
  new_conn->next = ws_connections;
  ws_connections = new_conn;
}

void remove_connection(struct mg_connection *conn) {
  struct websocket_connection **current = &ws_connections;
  while (*current) {
    if ((*current)->conn == conn) {
      struct websocket_connection *to_free = *current;
      *current = (*current)->next;
      free(to_free);
      return;
    }
    current = &(*current)->next;
  }
}

void broadcast_message(const char *message) {
  struct websocket_connection *current = ws_connections;
  while (current) {
    mg_ws_send(current->conn, message, strlen(message), WEBSOCKET_OP_TEXT);
    current = current->next;
  }
}

#define BUFFER_SIZE 128
char input_buffer[BUFFER_SIZE];
int buffer_start = 0, buffer_end = 0;

void buffer_push_char(char c) {
    if ((buffer_end + 1) % BUFFER_SIZE != buffer_start) { // Check if buffer is not full
        input_buffer[buffer_end] = c;
        buffer_end = (buffer_end + 1) % BUFFER_SIZE;
    }
}

int buffer_pop_char() {
    int c = EOF;
    if (buffer_start != buffer_end) {
        c = input_buffer[buffer_start];
        buffer_start = (buffer_start + 1) % BUFFER_SIZE;
    }
    return c;
}

void handle_websocket_message(struct mg_connection *c, struct mg_ws_message *wm) {
    for (size_t i = 0; i < wm->data.len; i++) {
        buffer_push_char(wm->data.ptr[i]);
    }
}


// website handler
void fn(struct mg_connection *c, int ev, void *ev_data) {


	switch(ev){
	case MG_EV_HTTP_MSG:
		struct mg_http_message *hm = (struct mg_http_message *) ev_data;
		if (mg_http_match_uri(hm, "/ws"))
		{
			// Upgrade to websocket. From now on, a connection is a full-duplex
			// Websocket connection, which will receive MG_EV_WS_MSG events.
			mg_ws_upgrade(c, hm, NULL);
		}
		else
		if (mg_http_match_uri(hm, "/api/debug")){
			int level = mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
			mg_log_set(level);
			mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
		} else if (mg_match(hm->uri, mg_str("/api"), NULL)) {
			double num1 = 0.0, num2 = 0.0;
			mg_json_get_num(hm->body, "$.name", &num1);  // Extract first number
			mg_json_get_num(hm->body, "$.email", &num2);  // Extract second number
			mg_http_reply(c, 200, "Content-Type: application/json\r\n",
					"{%m:%g}\n", MG_ESC("result"), num1 + num2);
		}else{
			struct mg_http_serve_opts opts = {
				.root_dir = "/www",
				.fs = &mg_fs_fat
			};
			mg_http_serve_dir(c, ev_data, &opts);
		}
		break;
	case MG_EV_WS_MSG:
		//struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
		//mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);
		handle_websocket_message(c, (struct mg_ws_message *) ev_data);
		/*if(strcmp(wm->data.ptr, "version") == 0){
			mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%m}", MG_ESC("version"), MG_ESC("Version 1"));
		}*/
		break;
	case MG_EV_WS_OPEN:
		add_connection(c);
		break;
	case MG_EV_CLOSE:
		remove_connection(c);
		break;

  }
}
