#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"
#include "parson.c"
#include <ctype.h>
#include <stdbool.h>

bool isNumeric(char* input) {
    for (int i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i])) {
            return false;
        }
    }
    return true;
}

bool containsSpace(const char *str) {
    while (*str != '\0') {
        if (*str == ' ') {
            return true;
        }
        str++;
    }
    return false;
}

typedef struct {
    int id;
    char title[100];
} Item;


int main(int argc, char *argv[])
{
    char *message;
    char *response;
    char *payload;
    char *cookie =  malloc(300 * sizeof(char));
    char *token = malloc(250 * sizeof(char));
    int sockfd;
    char str[100];
    char username[100];
    char password[100];
  

    // HOST: 34.254.242.81 
    // PORT: 8080
   while(1) {
    printf("\nTasteaza :");
    fgets(str, 100, stdin);

    printf("\nAi tastat : ");
    printf("%s\n", str);

    if(strcmp(str,"register\n") == 0) {
        
        printf("usernameForRegister: ");
        fgets(username, sizeof(username), stdin);

        // Eliminăm caracterul newline (\n) de la sfârșitul username-ului
        username[strcspn(username, "\n")] = '\0';

        while (containsSpace(username)) {
            printf("Username nu trebuie sa contina spatii. Incercati din nou: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';
        }

        printf("passwordForRegister: ");
        fgets(password, sizeof(password), stdin);

        // Eliminăm caracterul newline (\n) de la sfârșitul parolei
        password[strcspn(password, "\n")] = '\0';

        while (containsSpace(password)) {
            printf("Parola nu trebuie sa contina spatii. Incercati din nou: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0';
        }
        if(containsSpace(username) || containsSpace(password)) {
            printf("Numele de utilizator sau parola nu respecta formatul.");
        } else {
            JSON_Value *root_value = json_value_init_object();
            JSON_Object *root_object = json_value_get_object(root_value);
            json_object_set_string(root_object, "username", username);
            json_object_set_string(root_object, "password", password);

            sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
            char *body_data = json_serialize_to_string_pretty(root_value);
        
            message = compute_post_request("34.254.242.81", "/api/v1/tema/auth/register", "application/json", &body_data, 1, NULL, 0, NULL);
            send_to_server(sockfd, message);
            
            char *response = receive_from_server(sockfd);
            // extrag codul actiunii
            char *code = strtok(response, " ");
            code = strtok(NULL, " ");
            
            if(code[0] == '2') {
                printf("Utilizator inregistrat cu succes!\n");
            } else {
                printf("Numele de utilizator este deja folosit.\n");
            }
            close(sockfd);
            json_free_serialized_string(body_data);
            json_value_free(root_value);
        }  
    } else if(strcmp(str,"login\n") == 0) {
            printf("usernameForLogin: ");
            fgets(username, sizeof(username), stdin);

            // Eliminăm caracterul newline (\n) de la sfârșitul username-ului
            username[strcspn(username, "\n")] = '\0';

            while (containsSpace(username)) {
                printf("Username nu trebuie sa contina spatii. Incercati din nou: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0';
            }

            printf("passwordForLogin: ");
            fgets(password, sizeof(password), stdin);

            // Eliminăm caracterul newline (\n) de la sfârșitul parolei
            password[strcspn(password, "\n")] = '\0';

            while (containsSpace(password)) {
                printf("Parola nu trebuie sa contina spatii. Incercati din nou:  ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = '\0';
            }
        if(containsSpace(username) || containsSpace(password)) {
            printf("Numele de utilizator sau parola nu respecta formatul.");
        } else {
            JSON_Value *root_value = json_value_init_object();
            JSON_Object *root_object = json_value_get_object(root_value);
            json_object_set_string(root_object, "username", username);
            json_object_set_string(root_object, "password", password);
            
            sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
            char *body_data = json_serialize_to_string_pretty(root_value);
            message = compute_post_request("34.254.242.81", "/api/v1/tema/auth/login", "application/json", &body_data, 1, NULL, 0, NULL);
            send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            char *copyOfResponse = malloc(strlen(response) + 1);
            memcpy(copyOfResponse, response, strlen(response) + 1);
            char *code = strtok(response, " ");
            code = strtok(NULL, " ");
            
            if(code[0] == '2') {
                printf("Utilizator logat cu succes!\n");
                cookie = strtok(strstr(copyOfResponse, "connect.sid="),";");
                printf("%s", cookie);
            } else {
                printf("error:Niciun cont cu acest nume de utilizator.\n");
            }
            json_free_serialized_string(body_data);
            json_value_free(root_value);
            close(sockfd);
        }
    } else if(strcmp(str,"enter_library\n") == 0) {
        sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
        if(strcmp(cookie, "") != 0 )
        {
            message = compute_get_request("34.254.242.81","/api/v1/tema/library/access", NULL, cookie, 1, NULL);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            char *copyOfResponse = malloc(strlen(response) + 1);
            memcpy(copyOfResponse, response, strlen(response) + 1);
            char *code = strtok(response, " ");
            code = strtok(NULL, " ");
            if(code[0] == '2') {
                printf("\nAcces permis!\n");
                char *token_start = strstr(copyOfResponse, "\"token\":\"");
                if (token_start) {
                token_start += strlen("\"token\":\"");
                char *token_end = strchr(token_start, '"');
                    if (token_end) {
                        char token_value[256];
                        size_t token_len = token_end - token_start;
                        strncpy(token_value, token_start, token_len);
                        token_value[token_len] = '\0';
                        printf("Valoarea tokenului este: %s\n", token_value);
                        memcpy(token, token_value, strlen(token_value) + 1);
                    } 
                }   			
            } else {
                printf("Nu aveti acces in librarie, mai intai autentificati-va!.\n");
            }
        } else {
            printf("Nu aveti acces in librarie, mai intai autentificati-va!.\n");
        }
        close(sockfd);
    } else if(strcmp(str,"get_books\n") == 0) {
        sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
         if(strcmp(token, "") != 0 ) {
            message = compute_get_request("34.254.242.81","/api/v1/tema/library/books", NULL, cookie, 1, token);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            char *copyOfResponse = malloc(strlen(response) + 1);
            memcpy(copyOfResponse, response, strlen(response) + 1);
            // extrag codul actiunii
            char *code = strtok(response, " ");
            code = strtok(NULL, " ");
            if(code[0] == '2') {
                printf("Acces permis in biblioteca!\n");
                char *books_start = strchr(copyOfResponse, '[');
                char *books_end = strrchr(copyOfResponse, ']');
                if (books_start && books_end) {
                    size_t books_len = books_end - books_start + 1;
                    char books[books_len + 1];
                    strncpy(books, books_start, books_len);
                    books[books_len] = '\0';
                    Item items[5];
                    int count = 0;
                    char *info = strtok(books, "[]{},\":");
                    while (info != NULL) {
                    if (strcmp(info, "id") == 0) {
                        info = strtok(NULL, "[]{},\":");
                        items[count].id = atoi(info);
                    } else if (strcmp(info, "title") == 0) {
                        info = strtok(NULL, "[]{},\":");
                        memcpy(items[count].title, info, strlen(info) + 1);
                        count++;
                    }
                    info = strtok(NULL, "[]{},\":");
                }
                // Afișarea fiecărui obiect
                for (int i = 0; i < count; i++) {
                    printf("id = %d\n", items[i].id);
                    printf("title = %s\n", items[i].title);
                    printf("\n");
                }           
                }
            } else {
                printf("Acces nepermis in biblioteca!\n");
            }
         } else {
            printf("Mai intai intrati in biblioteca!\n");
         }
        close(sockfd);
    } else if(strcmp(str,"get_book\n") == 0) {
        sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
        if(strcmp(token, "") != 0 ) {
            char id[100], url[50];
            printf("Id ul cartii: ");
            scanf("%s", id);
            memcpy(url, "/api/v1/tema/library/books/",28);
            strcat(url, id);
            message = compute_get_request("34.254.242.81", url, NULL, cookie, 1, token);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            char *copyOfResponse = malloc(strlen(response) + 1);
            memcpy(copyOfResponse, response, strlen(response) + 1);
            // extrag codul actiunii
            char *code = strtok(response, " ");
            code = strtok(NULL, " ");
            if(code[0] == '2') {
                char book[256];
                char *books_start = strchr(copyOfResponse, '{');
                char *books_end = strrchr(copyOfResponse, '}');
                if (books_start && books_end) {
                size_t books_len = books_end - books_start + 1;
                char book[books_len + 1];
                strncpy(book, books_start, books_len);
                book[books_len] = '\0';
                // Return de tip JSON
                 for(int i = 0; book[i]; i++){
                    book[i] = (book[i]=='{' || book[i]=='}' || book[i]=='"' || book[i]==':') ? ' ' : book[i];
                }
                char *info = strtok(book, ",");
                printf("{\n");
                while (info != NULL) {
                printf("\t%s,\n", info);
                info = strtok(NULL, ",");
                }
                printf("}\n");
                }
            } else {
                printf("Cartea cu id ul furnizat nu exista.");
            }
        } else {
            printf("Mai intai intrati in biblioteca.");
        }
        close(sockfd);
    } else if(strcmp(str,"add_book\n") == 0) {
        sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
        if(strcmp(token, "") != 0 ) {
            JSON_Value *root_value = json_value_init_object();
            JSON_Object *root_object = json_value_get_object(root_value);
            char title[150], author[150], genre[150], publisher[150];
            char page_count[15];
            printf("titlu: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0';
            printf("autor: ");
            fgets(author, sizeof(author), stdin);
            author[strcspn(author, "\n")] = '\0';
            printf("gen: ");
            fgets(genre, sizeof(genre), stdin);
            genre[strcspn(genre, "\n")] = '\0';
            printf("publisher: ");
            fgets(publisher, sizeof(publisher), stdin);
            publisher[strcspn(publisher, "\n")] = '\0';
            printf("numar pagini: ");
            fgets(page_count, sizeof(page_count), stdin);
            page_count[strcspn(page_count, "\n")] = '\0';
            if (isNumeric(page_count) && atoi(page_count) >= 0) {
                json_object_set_string(root_object, "title", title);
                json_object_set_string(root_object, "author", author);
                json_object_set_string(root_object, "genre", genre); 
                json_object_set_number(root_object, "page_count", atoi(page_count));
                json_object_set_string(root_object, "publisher", publisher);
            
                payload = json_serialize_to_string_pretty(root_value);
                message = compute_post_request("34.254.242.81", "/api/v1/tema/library/books", "application/json",
                    &payload, 1, NULL, 0, token);
               
                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);
                char *code = strtok(response, " ");
                code = strtok(NULL, " ");
         
                if(code[0] == '2') {
                    printf("Carte adaugata cu succes!\n");
                } else {
                    printf("Informaţiile introduse sunt incomplete sau nu respectă formatarea!\n");
                }
                json_free_serialized_string(payload);
                json_value_free(root_value);   
            } else {
                printf("Informaţiile introduse sunt incomplete sau nu respectă formatarea!\n");
            }
            
        } else {
            printf("Mai intai intrati in biblioteca.");
         }
        close(sockfd);

    } else if(strcmp(str,"delete_book\n") == 0) {
        sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
        if(strcmp(token, "") != 0 ) {
            char id[100], url[50];
            printf("Id ul cartii: ");
            scanf("%s", id);
            memcpy(url, "/api/v1/tema/library/books/",28);
            strcat(url, id);
            message = compute_delete_request("34.254.242.81", url, NULL, cookie, 1, token);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            char *copyOfResponse = malloc(strlen(response) + 1);
            memcpy(copyOfResponse, response, strlen(response) + 1);
            // extrag codul actiunii
            char *code = strtok(response, " ");
            code = strtok(NULL, " ");
            if(code[0] == '2') {
                printf("Am sters cartea cu succes.");
            } else {
                printf("Id-ul furnizat nu se afla in lista cartilor");
            } 
        } else {
            printf("Mai intai intrati in biblioteca.");
        }
        close(sockfd);
    } else if(strcmp(str,"logout\n") == 0) {
        sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
    
        message = compute_get_request("34.254.242.81","/api/v1/tema/auth/logout", NULL, cookie, 1, NULL);
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);
        char *code = strtok(response, " ");
        code = strtok(NULL, " ");
        memcpy(cookie, "", 1);
        memcpy(token, "", 1);
        if(code[0] == '2') {
            printf("Delogarea s-a realizat cu succes!\n");
        } else {
            printf("Nu avem niciun user logat.\n");
        }
        close(sockfd);
    } else if(strcmp(str,"exit\n") == 0) {
        break;
    }
   }
    // free the allocated data at the end!
    free(cookie);
    free(token);
    return 0;
}
