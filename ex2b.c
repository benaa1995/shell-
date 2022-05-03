#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define LEN 510

//2 functions that print to us and one function that will execute the command of the process.
void printUserAndCurrentDir();
void printAllData(int, int, double);
void exeCommand(char *);

//Ex1.
int howManyWords(char *);
int howManyLetters(char *);

//In Main I perform the creation of processes, and use our functions, the only thing I use in Main is print scheduler.
int main() {
    pid_t son, son1;
    char string[LEN];
    int numOfCommands = 0;
    int lengthOfAllCommands = 0;
    double averageLengthOfAllCommands;

    while (1) {
        printUserAndCurrentDir();
        fgets(string, LEN, stdin);
        if (howManyWords(string) == 0) {
            continue;
        } else {
            numOfCommands++;
            lengthOfAllCommands += howManyLetters(string);
        }
        if (strcmp(string, "done\n") == 0) {
            averageLengthOfAllCommands = (double) lengthOfAllCommands / (double) numOfCommands;
            printAllData(numOfCommands, lengthOfAllCommands, averageLengthOfAllCommands);
            return 0;
        }
        son = fork();
        if (son == -1) {
            printf("Error creating process (Fork)\n");
            return 1;
        }
        if (son == 0) {
            son1 = fork();
            if (son1 == -1) {
                printf("Error creating process (Fork)\n");
                return 1;
            }
            if (son1 == 0) {
                char *commandInformation[3];
                commandInformation[0] = (char *) malloc(4 * sizeof(char));
                commandInformation[0] = "cat";
                commandInformation[1] = (char *) malloc(20 * sizeof(char));
                sprintf(commandInformation[1], "/proc/%d/sched", getppid());
                commandInformation[2] = NULL;
                execvp(commandInformation[0], commandInformation);
                free(commandInformation[0]);
                free(commandInformation[1]);
            } else {
                wait(NULL);
                exeCommand(string);
            }
        } else {
            wait(NULL);
        }
    }
}

//This function receives the user's data by software commands and prints them to us
void printUserAndCurrentDir() {
    struct passwd *pw;
    pw = getpwuid(getuid());

    if (pw == NULL) {
        printf("Error getting information from user\n");
        exit(1);
    }
    printf("%s@%s>", pw->pw_name, pw->pw_dir);
}

//A function that will help us print the calculations of the end, but this function only prints
void printAllData(int num, int length, double average) {
    printf("Num of commands: %d\nTotal length of all commands: %d\nAverage length of all commands: %f\n", num, length,
           average);
    printf("See you next time !");
}

//This function will receive the complete string, we will build an array of offers and allocate memory in favor of the income of a divided string.
//Of course we will not forget to release memory.
void exeCommand(char *str) {
    if (strcmp(str, "cd\n") == 0) {
        printf("command not supported (Yet)\n");
        return;
    }
    int i;
    int counterWord = howManyWords(str);
    int index = 0;
    int counterOfLetters = 0;
    char **command;
    command = (char **) malloc((counterWord + 1) * sizeof(char *));
    if (command == NULL) {
        printf("error malloc\n");
        exit(1);
    }
    command[counterWord] = NULL;

    for (i = 0; i < strlen(str); i++) {
        if (str[i] != '\n' && str[i] != '\0' && str[i] != ' ') {
            counterOfLetters++;
        }
        if (str[i] == ' ' || str[i] == '\n') {
            if (counterOfLetters > 0) {
                command[index] = (char *) malloc(counterOfLetters * sizeof(char));
                if (command[index] == NULL) {
                    printf("Error!!\nType of problem: Allocation of memory");
                    exit(1);
                }
                strncpy(command[index], &str[i - counterOfLetters], counterOfLetters);
                counterOfLetters = 0;
                index++;
            }
        }
    }
    if (execvp(command[0], command) == -1) {
        printf("command not found\n");
    }
    for (int d = 0; d < counterWord; d++) {
        free(command[d]);
    }
    free(command);
}

//Function from Ex1
int howManyWords(char *sentences) {
    int i;
    int counterOfWords = 0;
    int counterOfLetters = 0;
    for (i = 0; i < strlen(sentences); i++) {
        if (sentences[i] != '\n' && sentences[i] != '\0' && sentences[i] != ' ') {
            counterOfLetters++;
        }
        if (sentences[i] == ' ' || sentences[i] == '\n') {
            if (counterOfLetters > 0) {
                counterOfWords++;
                counterOfLetters = 0;
            }
        }
    }
    return counterOfWords;
}
int howManyLetters(char *sentences) {
    int i;
    int counterOfLetters = 0;
    int counter = 0;
    char *data;

    for (i = 0; i < strlen(sentences); i++) {
        if (sentences[i] != '\n' && sentences[i] != '\0' && sentences[i] != ' ') {
            counterOfLetters++;
        }
        if (sentences[i] == ' ' || sentences[i] == '\n') {
            if (counterOfLetters > 0) {
                data = (char *) malloc(counterOfLetters * sizeof(char));
                if (data == NULL) {
                    printf("Error!!\nType of problem: Allocation of memory");
                    exit(1);
                }
                strncpy(data, &sentences[i - counterOfLetters], counterOfLetters);
                counter += counterOfLetters;
                data = NULL;
                free(data);
                counterOfLetters = 0;
            }
        }
    }
    counter += howManyWords(sentences) - 1;
    return counter;
}
