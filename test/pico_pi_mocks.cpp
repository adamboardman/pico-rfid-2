/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <cstring>

#include "pico_pi_mocks.h"

int lastAddress;
int last_length_read;
int last_length_written;
long mock_data_read = 0;
std::vector<uint8_t> mock_read_data;
std::vector<uint8_t> mock_write_data;
bool use_expected_write_data = false;
std::vector<uint8_t> mock_expected_write_data;
int mock_expected_write_read_at;
int waitTime;

void reset_for_test(const i2c_inst_t *i2c) {
	lastAddress = 0;
	last_length_read = 0;
	last_length_written = 0;
	mock_data_read = 0;
	mock_read_data.clear();
	mock_write_data.clear();
	mock_expected_write_data.clear();
	mock_expected_write_read_at = 0;
	use_expected_write_data = false;
}

void set_read_data(std::vector<uint8_t> &data) {
	mock_read_data = data;
}

void set_expected_write_data(std::vector<uint8_t> &data) {
	mock_expected_write_data = data;
	use_expected_write_data = true;
}

int i2c_read_blocking(i2c_inst_t *i2c, uint8_t addr, uint8_t *dst, size_t len, bool nostop) {
	lastAddress = addr;
	for (int i = 0; i < len; i++) {
		try {
			dst[i] = mock_read_data.at(mock_data_read);
		} catch (...) {
			printf("Attempted to read past mock data [%d] - stick a breakpoint here to track it down", i);
		}
		mock_data_read++;
	}
	last_length_read = len;
	return PICO_ERROR_NONE;
}

int i2c_write_blocking(i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len, bool nostop) {
	lastAddress = addr;
	for (int i = 0; i < len; i++) {
		if (use_expected_write_data) {
			try {
				REQUIRE((int)src[i]==(int)mock_expected_write_data.at(mock_expected_write_read_at++));
			} catch (...) {
				printf("Attempted to read past mock data [%d] - stick a breakpoint here to track it down", i);
			}
		}
		mock_write_data.push_back(src[i]);
	}
	last_length_written = len;
	return PICO_ERROR_NONE;
}


void sleep_us(int us) {
	waitTime += us;
}
