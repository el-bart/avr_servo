#ifndef INCLUDE_CONFIG_HPP_FILE
#define INCLUDE_CONFIG_HPP_FILE

// units commonly used
#define KHz   (1000L)
#define MHz   (1000L*KHz)

// quarc frequency in [Hz]
#define F_CPU (11059200L)

// USART speed (bps)
#define USART_BAUD (57600L)

// max queues sizes
#define QUEUE_SEND_SIZE (110)
#define QUEUE_RECV_SIZE (120)

// servos count
#define SERVO_COUNT (17)

#endif
