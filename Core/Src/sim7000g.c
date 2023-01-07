#include "sim7000g.h"
#include "usart.h"
#include "string.h"

const UART_HandleTypeDef *uart = &huart1;

//AT
const char AT[] = "\rAT\r";

// GSM
const char CONFIGURE_GSM_CONNECTION[] = "\rAT+CNCFG=1,1,iot.1nce.net,\"\",\"\",0\r";
const char START_GSM_CONNECTION[] = "\rAT+CNACT=1,1\r";
const char END_GSM_CONNECTION[] = "\rAT+CNACT=1,0\r";

// MQTT CONFIG
const char MQTT_URL[] = "\rAT+SMCONF=\"URL\",144.24.161.226,1883\r";
const char MQTT_KEEPTIME[] = "\rAT+SMCONF=\"KEEPTIME\",60\r";
const char MQTT_USERNAME[] = "\rAT+SMCONF=\"USERNAME\",\"user\"\r";
const char MQTT_PASSWORD[] = "\rAT+SMCONF=\"PASSWORD\",\"P@ssw0rd\"\r";
const char MQTT_QOS[] = "\rAT+SMCONF=\"QOS\",1\r";
const char MQTT_CLEANSS[] = "\rAT+SMCONF=\"CLEANSS\",1\r";
const char MQTT_TOPIC[] = "\rAT+SMCONF=\"TOPIC\",\"demo/one\"\r";
const char MQTT_RETAIN[] = "\rAT+SMCONF=\"RETAIN\",1\r";

// MQTT CONNECTION
const char START_MQTT_CONNECTION[] = "\rAT+SMCONN\r";
const char END_MQTT_CONNECTION[] = "\rAT+SMDISC\r";
const char MQTT_PUBLISH[] = "\rAT+SMPUB=\"demo/one\",%i,1,1\r";

void power_on(void)
{
    HAL_GPIO_WritePin(PWRKEY_GPIO_Port, PWRKEY_Pin, GPIO_PIN_RESET);
    HAL_Delay(2000);
    HAL_GPIO_WritePin(PWRKEY_GPIO_Port, PWRKEY_Pin, GPIO_PIN_SET);
    HAL_Delay(2000);
}

void power_off(void)
{
	power_on();
}

int check_at_response(const char* expected_response)
{
	if (strstr(RX, expected_response) == NULL) {
		memset(RX, 0, strlen(RX));
		return 1;
	}

	memset(RX, 0, strlen(RX));

	return 0;
}


int start_gsm_connection(void)
{
	HAL_UART_Transmit(uart, (uint8_t*)CONFIGURE_GSM_CONNECTION, sizeof(CONFIGURE_GSM_CONNECTION), 2000);
	HAL_Delay(2000);

	HAL_UART_Transmit(uart, (uint8_t *)START_GSM_CONNECTION, sizeof(START_GSM_CONNECTION), 2000);
	HAL_Delay(8000);

	return 0;
}

int end_gsm_connection(void)
{
	HAL_UART_Transmit(uart, (uint8_t *)END_GSM_CONNECTION, sizeof(END_GSM_CONNECTION), 2000);

//	if (check_at_response("APP PDP: DEACTIVE") != 0) {
//		return 1;
//	}

	return 0;
}


void send_at(void)
{
	HAL_UART_Transmit(uart, (uint8_t*)AT, sizeof(AT), 2000);
}

int set_mqtt_config(void)
{
	HAL_UART_Transmit(uart, (uint8_t*)MQTT_URL, sizeof(MQTT_URL), 500);
	HAL_Delay(500);

	HAL_UART_Transmit(uart, (uint8_t*)MQTT_KEEPTIME, sizeof(MQTT_KEEPTIME), 500);
	HAL_Delay(500);

	HAL_UART_Transmit(uart, (uint8_t*)MQTT_USERNAME, sizeof(MQTT_USERNAME), 500);
	HAL_Delay(500);

	HAL_UART_Transmit(uart, (uint8_t*)MQTT_PASSWORD, sizeof(MQTT_PASSWORD), 500);
	HAL_Delay(500);

	HAL_UART_Transmit(uart, (uint8_t*)MQTT_QOS, sizeof(MQTT_QOS), 500);
	HAL_Delay(500);

	HAL_UART_Transmit(uart, (uint8_t*)MQTT_CLEANSS, sizeof(MQTT_CLEANSS), 500);
	HAL_Delay(500);

	HAL_UART_Transmit(uart, (uint8_t*)MQTT_RETAIN, sizeof(MQTT_RETAIN), 500);
	HAL_Delay(500);

	return 0;
}

int start_mqtt_connection(void)
{
	HAL_UART_Transmit(uart, (uint8_t *)START_MQTT_CONNECTION, sizeof(START_MQTT_CONNECTION), 2000);
	HAL_Delay(3000);

	if (check_at_response("OK") != 0) {
		return 1;
	}

	return 0;
}

int end_mqtt_connection(void)
{
	HAL_UART_Transmit(uart, (uint8_t *)END_MQTT_CONNECTION, sizeof(END_MQTT_CONNECTION), 2000);
	HAL_Delay(5000);

	if (check_at_response("OK") != 0) {
		return 1;
	}

	return 0;
}

int mqtt_publish(const char *json, const int size)
{
	const int command_size = snprintf(NULL, 0, MQTT_PUBLISH, size);

	const char mqtt_publish_comm[command_size];
	sprintf(mqtt_publish_comm, MQTT_PUBLISH, size);

	HAL_UART_Transmit(uart, (uint8_t *)mqtt_publish_comm, sizeof(mqtt_publish_comm), 2000);
	HAL_Delay(5000);
	HAL_UART_Transmit(uart, (uint8_t *)json, size, 5000);
	HAL_Delay(10000);
	if (check_at_response("OK") != 0) {
		return 1;
	}

	return 0;
}
