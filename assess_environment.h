#ifndef ASSESS_ENVIRONMENT_H
#define ASSESS_ENVIRONMENT_H


//Camera line search constants

#define IMAGE_BUFFER_SIZE		640
#define MIN_LINE_WIDTH			20
#define WIDTH_SLOPE				5

//Function declarations
void assess_environment_start(void);
uint16_t get_line_position(void);
uint16_t get_line_number(void);
void set_line_number(uint16_t value);


#endif /* ASSESS_ENVIRONMENT_H */
