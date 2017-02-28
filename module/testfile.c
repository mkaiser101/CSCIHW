#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  //open
#include <unistd.h> //close
#define SimpleDEVICE "/dev/simple_character_device"
#include <string.h>


int main () {
	char command;
	char buffer[1024];
	int Drivefile = open(SimpleDEVICE, O_RDWR);
	while (1) {
		printf("\nr) Read from device\nw) Write to device\ne) Exit device\nAnything else to continue reading and writing\n\nEnter command: ");
		scanf("%c", &command);
		switch (command) {
			case 'w': 
				printf("Enter data you want to write to the device: ");
				scanf("%s", buffer);
				write(Drivefile, buffer, strlen(buffer));
				while (getchar() != '\n');
				break;
			case 'r': 
				read(Drivefile, buffer, 1024);
				printf("Device output: %s\n", buffer);
				while (getchar() != '\n');
				break;
			case 'e':
				return 0;
			case 'q':
				printf("the game is over");
				printf("the game is over");
				printf("the game is over");
				printf("the game is over");
				printf("the game is over");
				printf("the game is over");
				return 0;
			default:
				while (getchar() != '\n');
		}
	}
	close(Drivefile);
	return 0;
}

