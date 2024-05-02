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
		if (mg_http_match_uri(hm, "/api/debug")){
			int level = mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
			mg_log_set(level);
			mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
		}else{
			struct mg_http_serve_opts opts = {
				.root_dir = "/www",
				.fs = &mg_fs_fat
			};
			mg_http_serve_dir(c, ev_data, &opts);
		}
		break;

  }
}
