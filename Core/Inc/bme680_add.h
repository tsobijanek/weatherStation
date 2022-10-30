/*
 * bme68x_add.h
 *
 *  Created on: Jun 5, 2022
 *      Author: tomek
 */

#ifndef INC_BME680_ADD_H_
#define INC_BME680_ADD_H_

#include "stdio.h"
#include "bme680.h"

uint8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

uint8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

void user_delay_us(uint32_t period);

uint8_t init_sensor(struct bme680_dev *sensor);

uint8_t set_sensor_settings(struct bme680_dev *sensor);

#endif /* INC_BME680_ADD_H_ */
