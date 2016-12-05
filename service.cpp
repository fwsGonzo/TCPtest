// This file is a part of the IncludeOS unikernel - www.includeos.org
//
// Copyright 2015 Oslo and Akershus University College of Applied Sciences
// and Alfred Bratterud
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <service>
#include <net/inet4>
#include <cstdio>
#include "crc32.h"

static const uint16_t TERM_PORT = 6667;

// prevent default serial out
void default_stdout_handlers() {}
#include <hw/serial.hpp>

typedef net::tcp::Connection_ptr Connection_ptr;

template <typename T>
void setup_terminal(T& inet)
{
  // mini terminal
  printf("Setting up terminal on port %u\n", TERM_PORT);
  
  auto& term = inet.tcp().bind(TERM_PORT);
  term.on_connect(
  [] (auto conn) {
    // write a string to change the state
    char BUFFER_CHAR = 'A';
    static CRC32_BEGIN(crc);
    const int LEN = 4096;
    auto* buf = new char[LEN];
    
    for (int i = 0; i < 1000; i++) {
      memset(buf, BUFFER_CHAR, LEN);
      conn->write(buf, LEN,
      [conn, buf, LEN] (int) {
        
        crc = crc32(crc, buf, LEN);
        printf("CRC32: %08x   %s\n", CRC32_VALUE(crc), conn->to_string().c_str());
      });
      
      //BUFFER_CHAR++;
      if (BUFFER_CHAR > 'Z') BUFFER_CHAR = 'A';
    }
  });
}

void Service::start(const std::string&)
{
  // add own serial out after service start
  auto& com1 = hw::Serial::port<1>();
  OS::add_stdout(com1.get_print_handler());

  printf("\n");
  printf("-= Starting LiveUpdate test service =-\n");
  printf("* CPU freq is %f MHz\n", OS::cpu_freq().count());

  auto& inet = net::Inet4::ifconfig<0>(
        { 10,0,0,42 },     // IP
        { 255,255,255,0 }, // Netmask
        { 10,0,0,1 },      // Gateway
        { 10,0,0,1 });     // DNS

  // listen for telnet clients
  setup_terminal(inet);
}
