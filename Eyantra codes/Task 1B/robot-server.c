/*
***********
*
*        		===============================================
*           		Rapid Rescuer (RR) Theme (eYRC 2019-20)
*        		===============================================
*
*  This script is to implement Task 1B of Rapid Rescuer (RR) Theme (eYRC 2019-20).
*  
*  This software is made available on an "AS IS WHERE IS BASIS".
*  Licensee/end user indemnifies and will keep e-Yantra indemnified from
*  any and all claim(s) that emanate from the use of the Software or 
*  breach of the terms of this agreement.
*  
*  e-Yantra - An MHRD project under National Mission on Education using ICT (NMEICT)
*
***********
*/

/*
* Team ID:			[ #466 ]
* Author List:		[AKESH M,LOAHIT K,SATHYA PRAKASH.E,PRAMOTH ARUN]
* Filename:			robot-server.c
* Functions:		receive_from_send_to_client,socket_create
* 					[ Comma separated list of functions in this file ]
* Global variables:	ipv4_addr_str[128],ipv4_addr_str_client[128],listen_sock,ret,
                        csock,l=0,rx_buffer[RX_BUFFER_SIZE],tx_buffer[RX_BUFFER_SIZE],
                        t[200][300],obpos[10],cbpos[10],obcount=0,cbcount=0,k,line_data[MAXCHAR];
* 					[ List of global variables defined in this file ]
*/


// Include necessary header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>



// Constants defined
#define SERVER_PORT 3333
#define RX_BUFFER_SIZE 1024
#define TX_BUFFER_SIZE 1024

#define MAXCHAR 1000				// max characters to read from txt file

// Global variables
struct sockaddr_in dest_addr;
struct sockaddr_in source_addr;

		// buffer to store data from client
	// buffer to store data to be sent to client

char ipv4_addr_str[128];			// buffer to store IPv4 addresses as string
char ipv4_addr_str_client[128];		// buffer to store IPv4 addresses as string

int listen_sock,ret;
int csock,l=0;
char rx_buffer[RX_BUFFER_SIZE];
char tx_buffer[RX_BUFFER_SIZE];
char t[200][300];
int obpos[10],cbpos[10],obcount=0,cbcount=0,k;

char line_data[MAXCHAR];
FILE *input_fp, *output_fp;


/*
* Function Name:	socket_create
* Inputs:			dest_addr [ structure type for destination address ]
* 					source_addr [ structure type for source address ]
* Outputs: 			my_sock [ socket value, if connection is properly created ]
* Purpose: 			the function creates the socket connection with the server
* Example call: 	int sock = socket_create(dest_addr, source_addr);
*/
int socket_create(struct sockaddr_in dest_addr, struct sockaddr_in source_addr){

	int addr_family;
	int ip_protocol;

	dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(SERVER_PORT);
	addr_family = AF_INET;
	ip_protocol = IPPROTO_IP;

	int my_sock;
	my_sock = socket(addr_family, SOCK_STREAM, 0);
         printf("Socket created \n");
	bind(my_sock, (struct sockaddr*) &dest_addr, sizeof(dest_addr));
   
       printf("Socket bound, port 3333\n");
 

	listen(my_sock, 5);
        printf("Socket listening\n"); 
        csock = accept(my_sock, NULL, NULL);
        printf("socket accepted\n");



	return my_sock;
}


/*
* Function Name:	receive_from_send_to_client
* Inputs:			sock [ socket value, if connection is properly created ]
* Outputs: 			None
* Purpose: 			the function receives the data from server and updates the 'rx_buffer'
*					variable with it, sends the obstacle position based on obstacle_pos.txt
*					file and sends this information to the client in the provided format.
* Example call: 	receive_from_send_to_client(sock);
*/


void receive_from_send_to_client(int csock)
{
	int valread,pcnt=0;
    int obcount=0,cbcount=0;
    
	
	valread = recv( csock , rx_buffer, RX_BUFFER_SIZE,0);

	if(valread>0)
	{
        printf("recieved %ld bytes of data: ",strlen(rx_buffer));

	printf("%s\n",rx_buffer);
	
	
	for(int a=0;a<10;a++)
        {

          fgets(line_data,200, input_fp);

          if(strlen(line_data)>8)
           {
            k=(int)line_data[0];
            k=k-48;
            strcpy(t[k],line_data);

           }

        }	
	
	int sr=((int)rx_buffer[0]-48);
	 
	for(int j =0;j<strlen(t[sr]);j++)
	{
	if(t[sr][j]=='(')
	{  
		obpos[obcount] = j;
		
		obcount++;
		 
	}
		
	if(t[sr][j]==')')
	{ 
		cbpos[cbcount] = j;
		
		cbcount++;
	
	}
		
	}
	
   
   
	
	for(int j =0;j<strlen(t[sr]);j++)

	{  

	 if(t[sr][j]=='(')
		{   
			
			for(int k =0;k<((cbpos[pcnt]-obpos[pcnt])+1);k++)
			{  
				tx_buffer[k+1]=t[sr][j+k];    
				tx_buffer[0] = '@';
				tx_buffer[(cbpos[pcnt]-obpos[pcnt])+2] = '@';
			
			}


			pcnt=pcnt+1;
			
			send(csock,tx_buffer,strlen(tx_buffer),0);

                         printf("transmitted  %ld bytes ",strlen(tx_buffer));
			
			printf("%s\n",tx_buffer);
			
			
			
			while(recv( csock , rx_buffer, RX_BUFFER_SIZE,0)<0);
                        printf("recieved %ld bytes of data",strlen(rx_buffer));
                        fputs(rx_buffer, output_fp);
		  	fputs("\n", output_fp);

			printf("%s \n",rx_buffer);
			strncpy(rx_buffer,"",sizeof(rx_buffer));

		}
        
	

	}send(csock,"@$@",3,0);

		



			}
		
     else
       {
        exit(0);}

			

		}

		


int main(){
	
    	char *input_file_name = "obstacle_pos.txt";
	char *output_file_name = "data_from_client.txt";

	// Create socket and accept connection from client
	int sock = socket_create(dest_addr, source_addr);
        

	input_fp = fopen(input_file_name, "r");

	if (input_fp == NULL){
		printf("Could not open file %s\n",input_file_name);
		return 1;
	}



	output_fp = fopen(output_file_name, "w");

	if (output_fp == NULL){
		printf("Could not open file %s\n",output_file_name);
		return 1;
	}
	
	
	while (1) {
		
		
		// Receive and send data from client and get the new shortest path
		receive_from_send_to_client(csock);

        // NOTE: YOU ARE NOT ALLOWED TO MAKE ANY CHANGE HERE
		fputs(rx_buffer, output_fp);
		fputs("\n", output_fp);

	       }

	return 0;
}
