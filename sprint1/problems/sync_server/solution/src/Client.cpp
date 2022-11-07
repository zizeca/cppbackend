/**
 * @file Client.cpp
 * @author Enver Kulametov (zizu.meridian@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Client.hpp"

Client::Client(tcp::socket& socket) : m_socket(socket) {}

Client::~Client() {}

void Client::run() { std::cout << "Client handler\n"; }
