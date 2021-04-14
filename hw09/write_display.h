// System setup register
#define OSCILLATOR_OFF	(0x2<<4)|(0x0)
#define OSCILLATOR_ON	(0x2<<4)|(0x1)

// Display setup register
#define DISPLAY_OFF	(0x8<<4)|(0x0)
#define BLINKING_OFF	(0x8<<4)|(0x1)
#define BLINKING_2HZ	(0x8<<4)|(0x2)
#define BLINKING_1HZ	(0x8<<4)|(0x5)
#define BLINKING_HALFHZ (0x8<<4)|(0x7)

// Digital dimming data input
#define PW_1_16		(0xE<<4)|(0x0)
#define PW_2_16		(0xE<<4)|(0x1)
#define PW_3_16		(0xE<<4)|(0x2)
#define PW_4_16		(0xE<<4)|(0x3)
#define PW_5_16		(0xE<<4)|(0x4)
#define PW_6_16		(0xE<<4)|(0x5)
#define PW_7_16		(0xE<<4)|(0x6)
#define PW_8_16		(0xE<<4)|(0x7)
#define PW_9_16		(0xE<<4)|(0x8)
#define PW_10_16	(0xE<<4)|(0x9)
#define PW_11_16	(0xE<<4)|(0xA)
#define PW_12_16	(0xE<<4)|(0xB)
#define PW_13_16	(0xE<<4)|(0xC)
#define PW_14_16	(0xE<<4)|(0xD)
#define PW_15_16	(0xE<<4)|(0xE)
#define PW_16_16	(0xE<<4)|(0xF)

// Display Integers    (P,G,F,E)|(D,C,B,A)
#define ZERO		(0x3<<4)|(0xF)
#define ONE		(0x3<<4)|(0x0)
#define TWO		(0x5<<4)|(0xB)
#define THREE		(0x4<<4)|(0xF)
#define FOUR		(0x6<<4)|(0x6)
#define FIVE		(0x6<<4)|(0xD)
#define SIX		(0x7<<4)|(0xD)
#define SEVEN		(0x0<<4)|(0x7)
#define EIGHT		(0x7<<4)|(0xF)
#define NINE		(0x6<<4)|(0x7)

// Display error
#define CHAR_E		(0x7<<4)|(0x9)
#define CHAR_R		(0x5<<4)|(0x0)

// Other useful display sequences
#define CLEAR		0x00
#define DEGREE		(0x6<<4)|(0x3)
#define NEGATIVE	(0x4<<4)|(0x0)
#define DECIMAL		(0x8<<4)

int init_display(void);
int write_display(int fd,double value);
int shutdown_display(int fd);
void get_digits(int *buf, double val, int round);
void get_7seg(int *buf);
void rmv_leading_zeros(int *buf);

