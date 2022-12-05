#include "main.h"

command_key_t command_keywords[] = {
		{ "ledon", 		5 },
		{ "ledoff", 	6 },
		{ "baudrate", 	8 },
};

void Command_Parser() {				// Parse function of data coming from uart
	uint8_t index = 0;
	char *buff;
	for (int r = 0; r < 3; r++) {
		for (int f = 0; f < command_keywords[r].lenght; f++) {
			if (run.rx_data[f] == command_keywords[r].command_key[f]) {
				index++;
			} else {
				index = 0;
				run.cmd.invalid_flg = 1;
				break;
			}
		}
		if (index == command_keywords[r].lenght) {
			uint8_t f = (run.pck_lenght - (command_keywords[r].lenght + 1));
			buff = (char*) malloc(f * sizeof(char));
			if (buff != NULL) {
				for (int s = 0; s < f; s++) {
					char c = run.rx_data[s + index + 1];
					if (c >= 48 && c <= 57) { 				// is digit control
						buff[s] = c;
						run.cmd.invalid_flg = 0;
					} else {
						run.cmd.invalid_flg = 1;
						break;
					}
				}

				if (!run.cmd.invalid_flg) {
					run.cmd.cmd_val = atoi(buff);
					run.cmd.command = r;
					run.cmd.new_cmd_flg = 1;
					memset(buff, 0xFF, (f * sizeof(char)));
				}
			}
			free(buff);
			break;
		}
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {	// USART Interrupt ReceiveToIdle ISR
	huart->Instance->CR1 &= ~(1 << 4);	// receiver to idle interrupt flag reset
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, run.rx_data, 50);
	run.pck_lenght = Size;
	run.new_data = 1;
}

void Send_Message(char *str) {
	HAL_UART_Transmit(&huart1, (uint8_t*) str, strlen(str), 100);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {// 1msn led on-off ISR

	run.tmr_cnt++;
	if (run.led_step) {
		if (run.tmr_cnt < run.led_on_time) {
			PIN_HIGH(LED);
		} else {
			run.led_step = 0;
			run.tmr_cnt = 0;
		}
	} else {
		if (run.tmr_cnt < run.led_off_time) {
			PIN_LOW(LED);
		} else {
			run.led_step = 1;
			run.tmr_cnt = 0;
		}
	}
}
