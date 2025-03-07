#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

typedef struct
{
    char ipAddress[INET_ADDRSTRLEN];
    int startPort;
    int endPort;
} TargetData;

void ScanPorts(TargetData args)
{
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    inet_pton(AF_INET, args.ipAddress, &target.sin_addr);
    for(int port = args.startPort; port <= args.endPort; port++)
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
    TargetData targetData;
    printf("Target IP: ");
    scanf("%s", targetData.ipAddress);
    printf("Set Start Port: ");
    scanf("%d", &targetData.startPort);
    printf("Set End Port: ");
    scanf("%d", &targetData.endPort);
    ScanPorts(targetData);
    return 0;
}