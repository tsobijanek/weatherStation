#include "stdint.h"

extern uint8_t RX[30];

int check_at_response(const char* expected_response);

int start_gsm_connection(void);

int end_gsm_connection(void);

void send_at(void);

int set_mqtt_config(void);

int start_mqtt_connection(void);

int end_mqtt_connection(void);

int mqtt_publish(const char *json, const int size);
