// Copyright(c) 2019, 2020
// Yuming Meng <mengyuming@hotmail.com>.
// All rights reserved.
//
// Author:  Yuming Meng
// Date:  2020-01-07 10:22
// Description:  No.

#include <sys/types.h>
#include <sys/socket.h>

#include <chrono>  // NOLINT.
#include <thread>  // NOLINT.

#include "websocket/server.h"
#include "websocket/websocket.h"


using libwebsocket::Server;
using libwebsocket::WebSocket;

int main(void) {
  Server server;
  WebSocket websocket;;
  websocket.set_fin(1);
  websocket.set_opcode(1);
  websocket.set_mask(0);
  std::vector<char> payload_content;
  std::vector<char> send_data;
  server.Init("", 8081, 10);  // Set ip="": listen any address.
  // Set the callback function when receiving data.
  server.OnReceived([&](const int &fd, const char *buffer, const int &size) {
    payload_content.clear();
    send_data.clear();
    payload_content.assign(buffer, buffer + size);
    // TODO(mengyuming@hotmail.com): Just return the received data at now.
    websocket.FormDataGenerate(payload_content, &send_data);
    send(fd, send_data.data(), send_data.size(), 0);
  });
  server.Run(3000);  // 3000 ms epoll timeout.
  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
  return 0;
}
