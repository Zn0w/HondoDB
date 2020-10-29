#pragma once

#include <memory>
#include <cstdint>
#include <vector>
#include <mutex>
#include <deque>
#include <thread>
#include <iostream>

#define ASIO_STANDALONE

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "../util/string.h"