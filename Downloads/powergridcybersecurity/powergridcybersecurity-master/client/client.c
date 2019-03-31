#include "hal_thread.h"
#include "iec61850_server.h"
#include "iec61850_client.h"
#include <stdlib.h>
#include <stdio.h>
#include "static_model.h"

#define NUM_TO_CLIENT 2
#define NUM_TO_SERVER 2

extern IedModel iedModel;
static IedServer iedServer = NULL;

// Array holding the data sent to the client
static float data_to_client[NUM_TO_CLIENT];

// Array holding the data sent to the server
static float data_to_server[NUM_TO_SERVER];

/*
  readFile() reads the data from the client back-end application.
  This is typically the control center.
  Uses the global variable data_to_server.
  Returns 1 on a successful read, 0 otherwise.
 */
int 
readFile () 
{
	// Create file descriptor for the data buffer
	FILE *f = fopen("client_to_server.txt", "r");
	
	if (fscanf(f, "%f %f", &data_to_server[0], &data_to_server[1]) == 0) 
	{
		printf("Error reading from the client_to_server.txt file.\n");
		return 0;
	}

	return 1;
}

/*
  writeFile() writes the data received by the server.
  This data is typically from the power grid simulator.
  Uses the global variable data_to_client.
  Returns 1 on a successful write operation. 0 otherwise.
 */
int 
writeFile () 
{
	// File descriptor for the data buffer
	FILE *f = fopen("server_to_client.txt", "w");

	if (fprintf(f, "%f %f", data_to_client[0], data_to_client[1]) < 0) 
	{
		printf("Error reading from server_to_client.txt. \n");
		return 0;
	}
	
	return 1;
}


int 
main (int argc, char ** argv) 
{

	char * hostname;
	int tcpPort = 102;
	int running = 1;

	IedClientError error;
	IedConnection connection = IedConnection_create();

	// Get the host IP from the user
	if (argc > 1)
	{
		hostname = argv[1];

	}		
	else 
	{
		printf("Enter the host name and try again.");
		exit(1);
	}
	// Get port from the user
	if (argc > 2) 
	{
		tcpPort = atoi(argv[2]);
	}

	// Establish connection with the server
	IedConnection_connect(connection, &error, hostname, tcpPort);

	// Successful connection to the server
	if (error == IED_ERROR_OK) 
	{
		printf("\nClient started..");
		printf("Waiting for server data..\n");
		
		// Main loop
		while (running) 
		{
			IedServer_lockDataModel(iedServer);
			
			// Begin data reception
			if (error == IED_ERROR_OK) 
			{
				IedConnection_getServerDirectory(connection, &error, false);
				
				// Get the first value from the server application.
				data_to_client[0] = IedConnection_readFloatValue(connection, &error, "PowGridSim_SS1CTRL/GGIO1.subVal.f", IEC61850_FC_SV);
				if (error != IED_ERROR_OK) 
				{
					printf("Error reading value from server.");
					exit(1);
				}
				
				// Get the second value from the server
				data_to_client[1] = IedConnection_readFloatValue(connection, &error, "PowGridSim_SS1CTRL/GGIO2.subVal.f", IEC61850_FC_SV);
				if (error != IED_ERROR_OK) 
				{
					printf("Error reading value from server.");
					exit(1);
				}
				
				// Write to the data buffer
				if (writeFile() == 0) 
				{
					printf("\nError occurred while writing to file. Exiting.");
					exit(1);
				}
			}
			else 
			{
				printf("\nAn error occurred while locking the data model.");
				exit(1);
			}

			// Begin data transmission
			if (error == IED_ERROR_OK) 
			{
				// Read data from the control center
				if (readFile() == 0) 
				{
					printf("\n Error reading from file. Exiting.");
					exit(1);
				}
				
				// Write the first value to the server IED model
				IedConnection_writeFloatValue(con, &error, "PowGridSim_SS1CTRL/GGIO3.subVal.f", IEC61850_FC_SV, data_to_server[0]);
				if (error != IED_ERROR_OK) 
				{
					printf("\nError writing first power generation value.");
					exit(1);
				}
				
				// Write the second.
				IedConnection_writeFloatValue(con, &error, "PowGridSim_SS1CTRL/GGIO4.subVal.f", IEC61850_FC_SV, data_to_server[1]);
				if (error != IED_ERROR_OK) 
				{
					printf("\nError writing second power generation value.");
					exit(1);
				}
				
				IedServer_unlockDataModel(iedServer);
			
			}
			Thread_sleep(10000);
		}

	} 
	else
	{
		printf("\nConnection failed.");
	}

	IedConnection_destroy(connection);
}
