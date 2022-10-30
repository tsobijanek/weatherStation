#include <bme680_add.h>
#include "i2c.h"

void user_delay_us(uint32_t period)
{
	HAL_Delay(period);
}

uint8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	int8_t rslt = 0;

	HAL_I2C_Mem_Read(&hi2c1, dev_id << 1, reg_addr, 1, reg_data, len, 100);

	return rslt;
}

uint8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	int8_t rslt = 0;

	HAL_I2C_Mem_Write(&hi2c1, dev_id << 1, reg_addr, 1, reg_data, len, 100);

	return rslt;
}

uint8_t init_sensor(struct bme680_dev *sensor)
{
	  sensor->dev_id = BME680_I2C_ADDR_PRIMARY;
	  sensor->intf = BME680_I2C_INTF;
	  sensor->read = user_i2c_read;
	  sensor->write = user_i2c_write;
	  sensor->delay_ms = user_delay_us;

	  return bme680_init(sensor);
}

uint8_t set_sensor_settings(struct bme680_dev *sensor)
{
	  sensor->tph_sett.os_hum = BME680_OS_2X;
	  sensor->tph_sett.os_pres = BME680_OS_4X;
	  sensor->tph_sett.os_temp = BME680_OS_8X;
	  sensor->tph_sett.filter = BME680_FILTER_SIZE_3;

	  sensor->gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;

	  sensor->gas_sett.heatr_temp = 320; // degree Celsius
	  sensor->gas_sett.heatr_dur = 150; // milliseconds

	  if (bme680_init(sensor) != BME680_OK) {
		  return 1;
	  }

	  sensor->power_mode = BME680_FORCED_MODE; // power mode
	  uint8_t required_settings_set = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;
	  if (bme680_set_sensor_settings(required_settings_set, sensor) != BME680_OK) {
		  return 2;
	  }

	  if (bme680_set_sensor_mode(sensor) != BME680_OK) { // set power mode
		  return 3;
	  }

	  return BME680_OK;
//	  sensor->tph_sett.os_hum = BME680_OS_2X;
//	  sensor->tph_sett.os_pres = BME680_OS_4X;
//	  sensor->tph_sett.os_temp = BME680_OS_8X;
//	  sensor->tph_sett.filter = BME680_FILTER_SIZE_3;
//
//	  sensor->gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
//
//	  sensor->gas_sett.heatr_temp = 320; /* degree Celsius */
//	  sensor->gas_sett.heatr_dur = 150; /* milliseconds */
//
//	  int8_t rslt = BME680_OK;
//	  rslt = bme680_init(sensor);
//
//	  return rslt;
}
