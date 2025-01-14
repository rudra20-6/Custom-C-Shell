#include "iMan.h"

void remove_before_html_tag(char input[], char output[])
{
    char *html_start = strstr(input, "<html>");
    if (html_start == NULL)
    {
        // No <html> tag found, copy the whole input as output
        strcpy(output, input);
    }
    else
    {
        // Copy the content from the <html> tag onwards
        strcpy(output, html_start);
    }
}

void remove_html_tags(char input[], char output[])
{
    int in_tag = 0;  // 1 if we're inside a tag
    int in_head = 0; // 1 if we're inside the <head> section
    int src_idx = 0;
    char temp[2]; // Temporary string to hold single character

    output[0] = '\0'; // Initialize the output string

    // Loop through the input string
    while (input[src_idx] != '\0')
    {
        // Detect <head> section and skip it entirely
        if (!in_head && strncmp(&input[src_idx], "<head>", 6) == 0)
        {
            in_head = 1;
            src_idx += 6;
            continue;
        }
        if (in_head && strncmp(&input[src_idx], "</head>", 7) == 0)
        {
            in_head = 0;
            src_idx += 7;
            continue;
        }

        // Skip everything inside the <head> section
        if (in_head)
        {
            src_idx++;
            continue;
        }

        // Detect HTML tags
        if (input[src_idx] == '<')
        {
            in_tag = 1;
            src_idx++; // Skip the opening '<'
        }
        else if (input[src_idx] == '>')
        {
            in_tag = 0;
            src_idx++; // Skip the closing '>'
        }
        else if (!in_tag)
        {
            // Convert the character to a temporary string
            temp[0] = input[src_idx++];
            temp[1] = '\0';

            // Append the single character string to the output
            strcat(output, temp);
        }
        else
        {
            // Skip content inside the tag
            src_idx++;
        }
    }

    // Output the result
    // printf("Processed Output:\n%s\n", output);
}

void displayMan(char command_name[])
{
    char man_page[BUFFER_SIZE] = {0};
    char cleanManPage[BUFFER_SIZE];
    char partiallyCleanManPage[BUFFER_SIZE];


    int sockfd;
    struct sockaddr_in server_addr;
    char request[256];
    char response[BUFFER_SIZE];
    ssize_t bytes_received;


    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Create HTTP GET request
    snprintf(request, sizeof(request),
             "GET /?topic=%s&section=all HTTP/1.1\r\n"
             "Host: man.he.net\r\n"
             "Connection: close\r\n\r\n",
             command_name);

    // Send the request
    if (send(sockfd, request, strlen(request), 0) < 0) {
        perror("send");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Receive the response
    while ((bytes_received = recv(sockfd, response, sizeof(response) - 1, 0)) > 0) {
        response[bytes_received] = '\0';
        // Print the entire response for debugging
        // printf("%s", response);
        strcat(man_page, response);
    }

    if (bytes_received < 0) {
        perror("recv");
    }

    // Close the socket
    close(sockfd);


    // // Remove HTML tags and process the content
    remove_before_html_tag(man_page, partiallyCleanManPage);
    remove_html_tags(partiallyCleanManPage, cleanManPage);

    // // Print cleaned response
    printf("\n%s\n", cleanManPage);

}