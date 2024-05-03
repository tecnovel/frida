/*
 * eventHandler.c
 *
 *  Created on: May 2, 2024
 *      Author: kaestli_a
 */

#include "websrv/mongoose/mongoose.h"


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
		struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
		//mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);

		if(strcmp(wm->data.ptr, "version") == 0){
			mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%m}", MG_ESC("version"), MG_ESC("Version 1"));
		}
  }
}
