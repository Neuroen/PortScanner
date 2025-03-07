#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

typedef struct
{
    int threadNumber;
    char ipAddress[INET_ADDRSTRLEN];
    int startPort;
    int endPort;
} TargetData;

void* ScanPorts(void* args)
{
    TargetData* targetData = (TargetData*) args;
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    inet_pton(AF_INET, targetData->ipAddress, &target.sin_addr);
    for(int port = targetData->startPort; port <= targetData->endPort; port++)
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0)
        {
            perror("failed to create socket");
            continue;
        }
        target.sin_port = htons(port);
        if(connect(sock, (struct sockaddr*)&target, sizeof(target)) == 0 )
        {
            printf("Port: %d is open\n", port);
        }
        close(sock);
    }
}


int main()
{
    //TODO: Implement a better Performance with Threads
    TargetData targetData;
    printf("Target IP: ");
    scanf("%s", targetData.ipAddress);
    printf("Set Start Port: ");
    scanf("%d", &targetData.startPort);
    printf("Set End Port: ");
    scanf("%d", &targetData.endPort);

    long availableCPUs = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t* threads;
    threads = malloc(sizeof(pthread_t) * availableCPUs);
    int portsForThread = (targetData.endPort - targetData.startPort) / availableCPUs;
    TargetData currentThreadData = targetData;
    currentThreadData.endPort = currentThreadData.startPort + portsForThread;
    TargetData* threadsData = malloc(sizeof(TargetData)*availableCPUs);
    for(int i = 0; i < availableCPUs; i++)
    {
        //printf("Thread %d -> Start: %d | End: %d\n", i, currentThreadData.startPort, currentThreadData.endPort); //Debug
        threadsData[i] = currentThreadData;
        threadsData[i].threadNumber = i;
        pthread_create(&threads[i], NULL, ScanPorts, (void*)&threadsData[i]);
        currentThreadData.startPort = currentThreadData.endPort + 1;
        currentThreadData.endPort += portsForThread;
    }
    while(1)
    {
        //TODO Menu
    }
    return 0;
}