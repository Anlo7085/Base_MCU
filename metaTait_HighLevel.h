#ifndef METATAIT_HIGHLEVEL_H_
#define METATAIT_HIGHLEVEL_H_

/* Receive Start Command,  RPM value, and Brightness value */
void transmit(void);

/* Collect Image from SD Card and Send Image Data to Post */
void image_data_send(void);


#endif /* METATAIT_HIGHLEVEL_H_ */
