#include "iman.h"

int initialize_socket(const char *hostname, int port_number)
{
    struct sockaddr_in server_address;
    struct hostent *resolved_host;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("Error: Unable to create socket");
        return -1;
    }

    resolved_host = gethostbyname(hostname);
    if (resolved_host == NULL)
    {
        fprintf(stderr, "Error: Host not found\n");
        return -1;
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    memcpy(&server_address.sin_addr.s_addr, resolved_host->h_addr, resolved_host->h_length);
    server_address.sin_port = htons(port_number);

    if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Error: Unable to connect to server");
        close(socket_fd);
        return -1;
    }

    return socket_fd;
}

void retrieve_man_page(const char *cmd_name)
{
    const char *server_host = "man.he.net";
    int server_port = 80;
    char request_buffer[BUFFER_SIZE];

    int socket_fd = initialize_socket(server_host, server_port);

    snprintf(request_buffer, sizeof(request_buffer),"GET /?topic=%s&section=all HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",cmd_name, server_host);

    if (send(socket_fd, request_buffer, strlen(request_buffer), 0) < 0)
    {
        perror("Error: Failed to send request");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    int bytes_received;
    while ((bytes_received = recv(socket_fd, request_buffer, sizeof(request_buffer) - 1, 0)) > 0)
    {
        request_buffer[bytes_received] = '\0';

        int idx = 0;
        bool inside_html_tag = false;

        while (idx < bytes_received)
        {
            if (request_buffer[idx] == '<')
            {
                inside_html_tag = true;
            }

            if (!inside_html_tag)
            {
                putchar(request_buffer[idx]);
            }

            if (request_buffer[idx] == '>')
            {
                inside_html_tag = false;
            }

            idx++;
        }
    }

    if (bytes_received < 0)
    {
        perror("Error: Failed to receive data");
    }

    close(socket_fd);
}
