#ifndef DFU_H
#define DFU_H

#include <stdbool.h>
#include <stdint.h>
#include <zip.h>

bool dfu_ping(void);
bool dfu_set_packet_receive_notification(uint16_t prn);
bool dfu_get_serial_mtu(void);
bool dfu_select_object(uint8_t type);
bool dfu_create_object(uint8_t type, uint32_t size);
bool dfu_object_write(zip_file_t* zf, size_t size);
uint32_t dfu_get_crc(void);
bool dfu_object_execute(void);
bool dfu_object_write_procedure(uint8_t type, zip_file_t* zf, size_t sz);

#endif
