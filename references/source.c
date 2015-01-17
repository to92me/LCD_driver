// BU HahaHA hHAHAH Hhahaha :D :D :D 

#include <stdio.h> 
#include <getopt.h> 
#include <stdlib.h> 
#include <pthread.h>  // dva treda jedan za key jedan za led :) zamisljeno da tasterima upravljamo pwm dim .. 
#include <string.h> 

#define tprintf(arg...) printf(arg) //svidja mi se fora :D 

#define file_dim "/home/drive/f_drive/C_linux/tmp/dim"
#define file_led "/home/drive/f_drive/C_linux/tmp/led"
#define file_key "/home/drive/f_drive/C_linux/tmp/key"
#define file_pwm "/home/drive/f_drive/C_linux/tmp/blink"

#define led_no_default 1
#define dim_default 1
#define pwm_default 1
#define buff_size 4 

const char* program_name; 

pthread_mutex_t led_mutex; 
int led = led_no_default; 
pthread_mutex_t pwm_mutex; 
int pwm = pwm_default; 
pthread_mutex_t dim_mutex; 
int dim = dim_default; 

int thread_key_unlock;
char key_buffer[buff_size];
char led_buffer[buff_size]; 
char pwm_buffer[buff_size]; 
char dim_buffer[buff_size]; 
int key_len, led_len, len_pwm, len_dim; 
int until_loop = 1;
unsigned char led_data, pwm_data, dim_data, key_data ;   
int some_change = 1; 
int thread_status_1, thread_status_2, thread_status_3 ;

	FILE* f_led; 
 	FILE* f_pwm; 
 	FILE* f_dim; 
 	FILE* f_key; 

 pthread_t thread_led_ID; 
 pthread_t thread_key_ID; 
 pthread_t thread_write_ID; 


void PrintUsage ( FILE* stream, int exit_code);
void* write_process();
void* led_process ();
void* key_process();




//###########################################################################################################
//#															                                            	#
//#													MAIN 													#
//#																											#
//###########################################################################################################


int main(int argc, char **argv)
 {
 	
 
 	int next_option;
 	const char* const short_options  = "l:d:p:rkh";
 	const struct option long_options[] = {
 		{"led", 1, NULL, 'l'}, 
 		{"dim", 1, NULL, 'd'},
 		{"pwm", 1, NULL, 'p'},
 		{"procentual", 0, NULL, 'r'}, 
 		{"key", 0, NULL, 'k'},
 		{"help", 0, NULL, 'h'}, 
 		{NULL, 0, NULL, 0}
 	};

 	const char* output_filename = NULL; 
 	program_name = argv[0];

 	do{ 
 		next_option = getopt_long ( argc, argv, short_options, long_options, NULL ); 
 		tprintf("case loop \n");

 		switch (next_option){ 
 			case 'l': { 
 				led = atoi(optarg);
 				break; 
 			}
 			case 'd': {
 				dim = atoi(optarg); 
 				break; 
 			}
 			case 'p': { 
 				
 				pwm = atoi(optarg); 
 				break; 
 			}
 			case 'r':{
 				break; 
 			}
 			case 'k':{
 				thread_key_unlock = 1; 
 				break; 
 			}
 			case 'h':{ 
 				PrintUsage(stderr, 1); 
 				break; 
 			}
 			case '?':{ 
 				PrintUsage(stderr, 1); 
 				break; 
 			}
 			case -1: {
				break; 
			}
 			default: {
				abort(); 
			}
 		}
 	}while(next_option != -1); 

 	thread_status_3 =  pthread_create(&thread_key_ID, NULL, &key_process, NULL);
 	thread_status_2 =  pthread_create(&thread_write_ID, NULL, &write_process, NULL); 


 	if (thread_status_2) {
 		fprintf(stderr, "err while creating thread \n" );
 		return 0; 
 	}
 	
 	

 	
 	if (thread_key_unlock){ 
 		tprintf("key_thread");
		thread_status_3 =  pthread_create(&thread_key_ID, NULL, &key_process, NULL);
		if (thread_status_3) { 
			fprintf(stderr, "err while creating thread \n" );
			 return 0; 
			}
		
	}
	pthread_join(thread_write_ID, NULL); 

	if (thread_key_unlock){ 
		pthread_join(thread_key_ID, NULL); 

	}
 
 	

 	return 0;
 }








 void PrintUsage ( FILE* stream, int exit_code){ 
	fprintf(stream, "Usage: %s options [inputfile... ] \n ", program_name ); 
	fprintf(stream,
		"\n"
		" -h	        --help			Display this usage information.\n"
		" -l<LedNumber> --led<LedNumber>  	Chose led diode to use (  1 2 3 4 ). \n"
		" -d<DimNumber> --dim<DimNumber> 	Chose dim procetage or number to calculate\n"
		" -p<PwmNumber> --pwm<PwmNumber> 	Chose Dim procetage or number to calculate\n"
		" -"); 
	exit(exit_code); 
}


void* write_process(){ 
	printf("thread 2 \n" );
	while (until_loop){ 
		//printf("until_loop_tome\n");
		if (some_change != 0){ 
			tprintf("F_open_led\n");
			some_change = 0; 
			
			f_led = fopen(file_led, "r+w"); 
			if (f_led == NULL ) { 
				fprintf(stderr, " faild to open %s\n Fatal error exiting program \n", file_led);
				pthread_cancel(thread_led_ID); 
				pthread_cancel(thread_key_ID); 
				until_loop = 0; 
				break; 
			}
			
			printf("F_open_pwm\n");
			
			f_pwm = fopen(file_pwm, "r+w"); 
			if ( f_pwm == NULL ) { 
				fprintf(stderr, " faild to open %s\n Fatal error exiting program \n", file_led);
				pthread_cancel(thread_led_ID); 
				pthread_cancel(thread_key_ID); 
				//fclose(f_led); 
				until_loop = 0; 
				break;
			}
			tprintf("F_open_dim\n");
			f_dim = fopen ( file_dim, "r+w"); 
			if ( f_dim == NULL ){ 
				fprintf(stderr, " faild to open %s\n Fatal error exiting program \n", file_led);
				pthread_cancel(thread_led_ID); 
				pthread_cancel(thread_key_ID); 
				//fclose(f_led); 
				//fclose(f_pwm); 
				until_loop = 0; 
				break;
			}
			
			// LED 
			led_len = fread(&led_buffer, 1, 3 , f_led); 
			led_buffer[led_len] = '\0'; 

			led_data = (unsigned char)strtol(led_buffer, NULL, 0); 
			
			switch (led){ 
				case 0: { 
					led_data ^= 0x01; 
					break; 
				}
				case 1: { 
					led_data ^= 0x04; 
					break; 
				}
				case 2:{ 
					led_data ^= 0x10; 
					break; 
				}
				case 3:{ 
					led_data ^= 0x40; 
					break; 
				}
			}
			
			sprintf(led_buffer, "%d", led_data); 
			fwrite(led_buffer,1,strlen(led_buffer), f_led); 
			printf("tome 3\n");
			fclose(f_led);

			pwm_data = 152/pwm-1; 
			sprintf(pwm_buffer, "%d", pwm_data); 
			fwrite(pwm_buffer, 1, strlen(pwm_buffer), f_pwm);
			fclose(f_pwm); 
			
			dim_data = 256*dim/100; 
			sprintf(dim_buffer, "%d", dim_data); 
			fwrite(dim_buffer, 1, strlen(dim_buffer),f_dim); 
			fclose(f_dim);
			
			if(thread_key_unlock){ 
		 		
 			}else{
 				until_loop = 0; 
 			}
		}
	}
}

void* key_process(){ 

	
	int flag1,  flag2; 
	while(until_loop){ 
		tprintf("KEY OPEN  \n ");
		f_key = fopen(file_key, "r+w" ); 
		if (f_key == NULL){
				fprintf( stderr, " faild to open localton %s \n Err status, exiting key process \n ", file_key );
				break; 	
			} 
		key_len = fread(&key_buffer, 1 , 3, f_key ); 
		if (f_key != NULL ){ 
			tprintf("CLOSE F_KEY\n"); 
		 	//fclose(f_key);
		} 
		key_buffer[key_len] = '\0'; 
		key_data = (unsigned char)strtol(key_buffer, NULL, 0);
		 
		if (((key_data & 0x01) == 0 ) && (flag1 == 0 )){ 
			led_data ^= 0x01; 
			some_change = 1; 
			flag1 = 1; 
		}else{
			flag1 = 0; 
		}

		if (((key_data &0x02) == 0 ) && (flag2 == 0)){
			led_data ^=0x04;
			some_change = 1;
			flag2 = 1; 
		}else { 
			flag2 = 0; 
		}
		tprintf("key_data = %x\n", key_data ); 
		if ((key_data & 0x03) == 0){
			until_loop = 0; 
			led_data = 0x00; 
		}
		
		usleep(300000);
	} 
}