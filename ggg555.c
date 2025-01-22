#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 
 
#define MAX_SENTENCES 100 
#define MAX_LENGTH 256 
#define BUFFER 10 
 
int my_strcasecmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        char c1 = tolower((unsigned char)*s1);
        char c2 = tolower((unsigned char)*s2);
        if (c1 != c2) {
            return c1 - c2;
        }
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

int dublicats(char *text, char **sentences) {
    int count = 0;
    char *token = strtok(text, ".!?");
    while (token != NULL) {
        while (isspace(*token)) token++;
        char *end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) end--;
        *(end + 1) = '\0';
        int is_duplicate = 0;
        for (int i = 0; i < count; i++) {
            if (my_strcasecmp(sentences[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        if (!is_duplicate && count < MAX_SENTENCES) {
            sentences[count] = malloc(strlen(token) + 1);
            if (sentences[count] != NULL) {
                strcpy(sentences[count], token);
                count++;
            }
        }
        token = strtok(NULL, ".!?");
    }
    return count;
}
 
void func1(char* sentence) { 
    int len = strlen(sentence); 
    while (len > 1 && tolower(sentence[0]) == tolower(sentence[len - 1])) { 
        memmove(sentence, sentence + 1, len - 1); 
        sentence[len - 1] = '\0'; 
        len--; 
        sentence[len - 1] = '\0'; 
        len--; 
    } 
} 
int cmp(const void* a, const void* b) { 
    return strcmp(*(const char**)a, *(const char**)b); 
} 
void func2(char* sentence) { 
    char* list[1000]; 
    int n = 0; 
    char* word = strtok(sentence, " ."); 
    while (word != NULL) { 
        list[n++] = word; 
        word = strtok(NULL, " ."); 
    } 
    qsort(list, n, sizeof(char*), cmp); 
    for (int i = 0; i < n; i++) { 
        printf("%s", list[i]); 
        if (i < n - 1) { 
            printf(" "); 
        } 
    } 
    if (n > 0) { 
        printf("."); 
    } 
    printf("\n"); 
} 
int palindrom(const char *word) { 
    int len = strlen(word); 
    for (int i = 0; i < len / 2; i++) { 
        if (tolower(word[i]) != tolower(word[len - 1 - i])) { 
            return 0; 
        } 
    } 
    return 1; 
} 
int func3(char** sentences, int count) {
    int new_count = 0;

    for (int i = 0; i < count; i++) {
        char temp[MAX_LENGTH];
        strncpy(temp, sentences[i], MAX_LENGTH - 1);
        temp[MAX_LENGTH - 1] = '\0';

        char* word = strtok(temp, " ,.-\n");
        int has_palindrome = 0;

        while (word != NULL) {
            if (palindrom(word)) {
                has_palindrome = 1;
                break;
            }
            word = strtok(NULL, " ,.-\n");
        }
        if (!has_palindrome) {
            if (new_count < count) {
                sentences[new_count] = sentences[i];
                new_count++;
            } else {
                fprintf(stderr, "Error: Buffer overflow in func3\n");
                exit(1);
            }
        } else {
            free(sentences[i]);
        }
    }

    return new_count;
}
void func4(char** sentences, int count) { 
    for (int i = 0; i < count; i++) { 
        char* sentence = sentences[i]; 
        char* pos = strstr(sentence, "HiddenTiger"); 
        if (pos != NULL) { 
            if (pos != sentence && isspace(*(pos - 1))) { 
                printf("%s.\n", sentence); 
            } 
        } 
    } 
} 
 
char* read_text() { 
    char* text = calloc(1024, sizeof(char)); 
    if (!text) { 
        perror("Memory alloc error"); 
        exit(1); 
    } 
    char c; 
    size_t index = 0; 
    while ((c = getchar()) != EOF) { 
        if (index >= 1024 - 1) { 
            text = realloc(text, (index + 1024) * sizeof(char)); 
        } 
        text[index++] = c; 
        if (c == '\n' && index > 1 && text[index - 2] == '\n') { 
            break; 
        } 
    } 
    text[index] = '\0'; 
    return text; 
}
void spravka(){
    printf("Доступные функции:\n");
			
    printf("int dublicats(char* text, char** sentences)\n");
    printf("  Функция разбивает текст на предложения и удаляет дубликаты.\n");
    printf("  В процессе разбиения удаляет пробелы в начале и конце предложений, а также проверяет наличие дубликатов.\n");
    printf("  Возвращает количество уникальных предложений.\n\n");

    printf("void func1(char* sentence)\n");
    printf("  Функция удаляет символы с начала и конца предложения, пока первый и последний символы не станут различными (без учета регистра).\n");
    printf("  В процессе удаления использует функцию memmove для сдвига строки влево после удаления первого символа и обнуляет последний символ строки, чтобы уменьшить её длину.\n");
    printf("  Продолжает удалять символы, пока первый и последний символы не станут различными.\n\n");

    printf("void func2(char* sentence)\n");
    printf("  Функция разбивает предложение на слова и сортирует их в лексикографическом порядке.\n");
    printf("  Использует strtok для разделения строки на слова, удаляя знаки препинания и пробелы.\n");
    printf("  Сортирует полученные слова с помощью qsort и выводит их на экран.\n");
    printf("  Каждое отсортированное предложение выводится с пробелами между словами и точкой в конце.\n");
    
    printf("int func3(char **sentences, int count)\n");
    printf("  Функция обрабатывает массив строк (предложений) и удаляет из него те предложения, которые содержат хотя бы одно слово-палиндром.\n");
    printf("  Каждый элемент массива строк проверяется на наличие палиндромов с использованием функции palindrom(). Если предложение не содержит\n");
    printf("  палиндромов, оно сохраняется в массиве.\n");
    printf("  Возвращает количество оставшихся предложений в массиве.\n");
    printf("  Использует strncpy для копирования предложений и strtok для разбиения\n");
    printf("  предложений на слова, удаляя знаки препинания и пробелы.\n");
            
    printf("void func4(char** sentences, int count)\n");
    printf("  Функция выводит все предложения, в которых есть слово 'HiddenTiger' и которое не является первым словом.\n");
    printf("  В процессе поиска использует функцию strstr для нахождения подстроки 'HiddenTiger' в предложении.\n");
    printf("  Проверяет, что 'HiddenTiger' не является первым словом, убедившись, что перед ним есть пробел.\n");
    printf("  Если условие выполняется, выводит предложение.\n\n");
}
void func8 (char** sentences, int count, int n){
    for (int i = 0; i < count; i++){
        char *sentence = sentences[i];
        for (int k = 0; k < n; k++){
            printf("%s", sentence);
            if (k < n - 1){
                printf(" ");
            }
        }
        printf(".\n");
    }
}
int main() {
    printf("Course work for option 4.19, created by Egor Omelyash.\n");
    int command;
    scanf("%d", &command);
    getchar();
    char* text;
    char* sentences[MAX_SENTENCES];
    int sentence_count;
    switch (command) {
        case 0:
            text = read_text();
            sentence_count = dublicats(text, sentences);
            for (int i = 0; i < sentence_count - 1; i++) {
                printf("%s.\n", sentences[i]);
                free(sentences[i]);
            }
            free(text);
            break;
        case 1:
            text = read_text();
            sentence_count = dublicats(text, sentences);
            for (int i = 0; i < sentence_count - 1; i++) {
                func1(sentences[i]);
                printf("%s.\n", sentences[i]);
                free(sentences[i]);
            }
            free(text);
            break;
        case 2:
            text = read_text();
            sentence_count = dublicats(text, sentences);
            for (int i = 0; i < sentence_count; i++) {
                func2(sentences[i]);
                free(sentences[i]);
            }
            free(text);
            break;
        case 3:
            text = read_text();
            sentence_count = dublicats(text, sentences);
            sentence_count = func3(sentences, sentence_count);
            for (int i = 0; i < sentence_count - 1; i++) {
                printf("%s.\n", sentences[i]);
                free(sentences[i]);
            }
            free(text);
            break;
        case 4:
            text = read_text();
            sentence_count = dublicats(text, sentences);
            func4(sentences, sentence_count);
            for (int i = 0; i < sentence_count; i++) {
                free(sentences[i]);
            }
            free(text);
            break;
        case 5:
            spravka();
            break;
	case 8:
            int n;
            scanf("%d", &n);
            getchar();
            text = read_text();
            sentence_count = dublicats(text, sentences);
            func8(sentences, sentence_count - 1, n);
            for (int i = 0; i < sentence_count; i++){
                free(sentences[i]);
            }
            free(text);
            break;
        default:
            printf("Error: Неправильный номер функции\n");
            break;
    }

    return 0;
}
