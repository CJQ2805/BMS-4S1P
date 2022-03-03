#include "CRC.h"

//±ê×¼CRC_8  POLY = 0x07
uint8_t CRC8(uint8_t *ptr, uint8_t len)
{
    uint8_t i;
    uint8_t crc = 0;
    while (len-- != 0)
    {
        for (i = 0x80; i != 0; i /= 2)
        {
            if ((crc & 0x80) != 0)
            {
                crc *= 2;
                crc ^= 0x07;
            }
            else
                crc *= 2;

            if ((*ptr & i) != 0)
                crc ^= 0x07;
        }
        ptr++;
    }
    return (crc);
}