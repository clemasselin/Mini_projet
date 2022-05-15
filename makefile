

# Define project name here
PROJECT = Mini

#Define path to the e-puck2_main-processor folder
GLOBAL_PATH = ../../lib/e-puck2_main-processor

#Source files to include
CSRC += ./main.c \
		./assess_environment.c \
		./control_pannel.c \
		./sign_detection.c \
		./circulation_instructions.c \
		
		
		
		

		

#Header folders to include
INCDIR += 

#Jump to the main Makefile
include $(GLOBAL_PATH)/Makefile 