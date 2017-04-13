#ifndef LVDS_H_
#define LVDS_H_

#include "includes.h"

#define  Max_LVDS_LEN	100+1048576

void send_to_lvds(INT8U* header,INT32U header_len,INT8U* data,INT32U data_len);
void handle_lvds_data(INT8U *data_from_lvds, INT32U len);
INT32U get_from_lvds(INT8U* data);
INT8U lvds_has_data();

#endif