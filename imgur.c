#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <curl/curl.h>


#define VERSION   1.0
#define KEY       "b3625162d3418ac51a9ee805b1840452"

char *response;

void removechar(char* str, char *c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != *c);
    }
    *pw = '\0';
}

/* Callback to change return value to string to be parsed. */
void callback(void *buffer, size_t size, size_t nmemb, void *userp) {
    response = buffer;
}


void arguments() {
    printf("Arguments\n");
    exit(1);
}

int main (int argc, char *argv[]) {
    struct stat buf;

    /* Get HOME directory */
    int dirlen = sizeof(getenv("HOME")) + 7;
    char directory[dirlen + 1];
    strcat(directory, getenv("HOME"));
    strcat(directory, "/.imgur");

    /* Create log directory if it doesn't exist */
    if (stat(directory , &buf) == -1) {
        mkdir(directory, 0700);
    }

    if (argc == 1) 
        arguments();

    if (access(argv[1], R_OK) != 0) {
        printf("File '%s' not found!\n", argv[1]);
        exit(1);
    }

    CURL *curl;
    CURLcode res;
    FILE *output = fopen(directory, "wb");

    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;

    curl_global_init(CURL_GLOBAL_ALL);

    /* File to upload */
    curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "image", CURLFORM_FILE, argv[1], CURLFORM_END);

    /* API Key */
    curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "key", CURLFORM_COPYCONTENTS, KEY, CURLFORM_END);


    curl = curl_easy_init();
    if(curl) {
        /* POST URL */
        curl_easy_setopt(curl, CURLOPT_URL, "http://api.imgur.com/2/upload.json");
        /* POST form */
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        /* Make return data a buffer */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

        /* passing the pointer to the response as the callback parameter */
        //curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);

        /* Perform the request */
        res = curl_easy_perform(curl);

        removechar(response, "\\");

        char *link = strstr(response, "http://i.imgur");

        int count = 0;
        while (count < 10000) {
            if (link[count] == '\"') {
                break;
            }
            count++;
        }

        char *real = (char*) malloc(count + 1);
        int t;
        for (t = 0; t < count; t++) {
            real[t] = link[t];
        }

        printf("%s\n", real);


        /* Check for errors */
        /*if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));*/

        curl_easy_cleanup(curl);
        curl_formfree(formpost);
    }
    return 0;
}