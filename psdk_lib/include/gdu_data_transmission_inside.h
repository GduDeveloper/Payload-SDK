#ifndef DATA_TRANSMISSION_INSIDE_H
#define DATA_TRANSMISSION_INSIDE_H


typedef struct  __attribute__((packed)) 
{
	uint16_t msg_id;
	void (* fun)(uint8_t *data);
}data_trans_fun_list_t;

typedef  void (*function)(void);

#endif

