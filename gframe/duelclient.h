#ifndef DUELCLIENT_H
#define DUELCLIENT_H

#include "config.h"
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/thread.h>
#include "network.h"
#include "data_manager.h"
#include "deck_manager.h"

namespace ygo {

class DuelClient {
private:
	static unsigned int connect_state;
	static unsigned char response_buf[64];
	static bool is_responseB;
	static unsigned char response_len;
	static event_base* client_base;
	static bufferevent* client_bev;
	static char duel_client_read[0x2000];
	static char duel_client_write[0x2000];
	static bool is_closing;
public:
	static bool StartClient(unsigned int ip, unsigned short port, bool create_game = true);
	static void StopClient(bool is_exiting = false);
	static void ClientRead(bufferevent* bev, void* ctx);
	static void ClientEvent(bufferevent *bev, short events, void *ctx);
	static int ClientThread(void* param);
	static void HandleSTOCPacketLan(char* data, unsigned int len);
	static void SetResponseI(int respI);
	static void SetResponseB(unsigned char* respB, unsigned char len);
	static void SendPacketToServer(unsigned char proto) {
		char* p = duel_client_write;
		BufferIO::WriteInt16(p, 1);
		BufferIO::WriteInt8(p, proto);
		bufferevent_write(client_bev, duel_client_write, 3);
	}
	template<typename ST>
	static void SendPacketToServer(unsigned char proto, ST& st) {
		char* p = duel_client_write;
		BufferIO::WriteInt16(p, 1 + sizeof(ST));
		BufferIO::WriteInt8(p, proto);
		memcpy(p, &st, sizeof(ST));
		bufferevent_write(client_bev, duel_client_write, sizeof(ST) + 3);
	}
	static void SendBufferToServer(unsigned char proto, void* buffer, size_t len) {
		char* p = duel_client_write;
		BufferIO::WriteInt16(p, 1 + len);
		BufferIO::WriteInt8(p, proto);
		memcpy(p, buffer, len);
		bufferevent_write(client_bev, duel_client_write, len + 3);
	}
};

}

#endif //DUELCLIENT_H
