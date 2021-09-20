#include "stm32_base.h"
#include "std.h"

/* Function for convert float to char with max 3 digit after point */
void float2char(float number, char *data, int prec) {
    char s[10];
    int decimals, units;
    int scale = 1;

    int i;

    if(prec > 3) {
        prec = 3;
    }

    for(i = 0; i < prec; i++) {
        scale *= 10;
    }

    if(number < 0) {
        decimals = (int)(number * -(prec)) % scale;
        units = (int)(-1 * number);
    } else {
        decimals = (int)(number * scale) % scale;
        units = (int)number;
    }

    for(i = 9; i > (9 - prec); i--) {
        s[i] = (decimals % 10) + '0';
        decimals /= 10;
    }

    if(prec != 0) {
        s[i] = '.';
        i--;
    }

    if(units == 0) {
        s[i] = '0';
        i--;
    }

    for(; (units > 0) && (i > 0); i--) {
        s[i] = (units % 10) + '0';
        units /= 10;
    }

    if(number < 0) s[i] = '-';
    i++;
    for(int j = 0; (s[j + i] != '\0') && (j < 10); j++) {
        *(data + j) = s[j + i];
    }
}

/* Function for convert float to 4 digits for semi indicator */
// TODO work with dot: Think about form variables (bin or dec) and return value or change by link.
void float2digits(float number, uint8_t *dig, uint8_t precision, uint8_t digits) {
    int overflow = 0;
    switch (precision) {
        case 0:
            break;
        case 1:
            number *= 10;
            break;
        case 2:
            number *= 100;
            break;
        case 3:
            number *= 1000;
            break;
        default:
            number *= 10;
            break;
    }

    if((number - (int)number) >= 0.5) {
        number += 1;
    }
    
    dig[3] = ((int)number / 1000);
    dig[2] = ((int)number - dig[3] * 1000) / 100;
    dig[1] = ((int)number - dig[3] * 1000 - dig[2] * 100) / 10;
    dig[0] = ((int)number - dig[3] * 1000 - dig[2] * 100 - dig[1] * 10) / 1;

    if((4 - digits) > 9) {
        dig[0] = -1;
    }
}

/* Function for compare of two strings */
int strcomp(char *str1, char *str2) {
    for(int i = 0; (*(str1 + i) != '\0') || (*(str2 + i) != '\0'); i++) {
        if(*(str1 + i) != *(str2 + i)) {
            return 0;
        }
    }
    return 1;
}