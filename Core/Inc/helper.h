#ifndef INC_HELPER_H_
#define INC_HELPER_H_


typedef enum
{
	LED_ON_TIME,
	LED_OFF_TIME,
	BAUDRATE
}command_t;

typedef struct
{
	uint8_t		rx_data[50]	  	;		// usart receiver data
	uint8_t		tx_data[50]	  	;		// usart transmitter data
	uint8_t		pck_lenght		;		// receive packet lenght

	uint16_t	led_on_time		;		// led high time variable
	uint16_t	led_off_time	;		// led low time variable
	uint16_t	tmr_cnt			;		// led timer counter
	uint8_t		led_step	:1	;		// led run step

	uint8_t		new_data  		:1;		// usart receiver new data flag

	struct {
				command_t	command		;		// new command parameters
				uint32_t	cmd_val		;		// read command parameter value
				uint8_t		new_cmd_flg	:1;		// new command flag
				uint8_t		invalid_flg	:1;		// invalid command flag send to "Invalid Command" message
	}cmd;

}run_t;

typedef struct
{
    const char command_key[8];
    int     lenght;

}command_key_t;



void Command_Parser();
void Send_Message(char *str);

#endif /* INC_HELPER_H_ */
