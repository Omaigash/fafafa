#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

void print_intro() {
    printf("Course work for option 4.19, created by Egor Omelyash.\n");
}

// Измененная функция read_text
char** read_text(int* num_sentences, int single_newline) {
    char** text = NULL;
    *num_sentences = 0;
    char buffer[1024];
    char* sentence = NULL;
    int sentence_len = 0;
    int buffer_len = 0;
    int newline_count = 0;

    while (1) {

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        buffer_len = strlen(buffer);

        for (int i = 0; i < buffer_len; ++i) {
            if (buffer[i] == '\n') {
                if (single_newline) {
                    if (sentence_len > 0) {
                        while (sentence_len > 0 && sentence[0] == ' ') {
                            memmove(sentence, sentence + 1, sentence_len);
                            sentence_len--;
                        }
                        sentence = (char*)realloc(sentence, sentence_len + 1);
                        sentence[sentence_len] = '\0';

                        text = (char**)realloc(text, (*num_sentences + 1) * sizeof(char*));
                        text[*num_sentences] = sentence;
                        (*num_sentences)++;
                    }
                    return text;
                } else {
                    newline_count++;
                    if (newline_count == 2) {
                        if (sentence_len > 0) {
                            while (sentence_len > 0 && sentence[0] == ' ') {
                                memmove(sentence, sentence + 1, sentence_len);
                                sentence_len--;
                            }
                            sentence = (char*)realloc(sentence, sentence_len + 1);
                            sentence[sentence_len] = '\0';

                            text = (char**)realloc(text, (*num_sentences + 1) * sizeof(char*));
                            text[*num_sentences] = sentence;
                            (*num_sentences)++;
                        }
                        return text;
                    }
                }
            } else {
                newline_count = 0;
            }

            if (sentence_len == 0 || buffer[i] != '.') {
                sentence = (char*)realloc(sentence, sentence_len + 2);
                sentence[sentence_len] = buffer[i];
                sentence_len++;
            } else {
                sentence = (char*)realloc(sentence, sentence_len + 2);
                sentence[sentence_len] = buffer[i];
                sentence_len++;

                while (sentence_len > 0 && sentence[0] == ' ') {
                    memmove(sentence, sentence + 1, sentence_len);
                    sentence_len--;
                }

                sentence[sentence_len] = '\0';

                text = (char**)realloc(text, (*num_sentences + 1) * sizeof(char*));
                text[*num_sentences] = sentence;
                (*num_sentences)++;

                sentence = NULL;
                sentence_len = 0;
            }
        }
    }

    return text;
}

void print_text(char** text, int num_sentences) {
    for (int i = 0; i < num_sentences; i++) {
        printf("%s\n", text[i]);
    }
}

char** remove_duplicates(char** text, int* num_sentences) {
    char** unique_text = NULL;
    int unique_count = 0;

    for (int i = 0; i < *num_sentences; i++) {
        int is_duplicate = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcasecmp(text[i], unique_text[j]) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        if (!is_duplicate) {
            unique_text = (char**)realloc(unique_text, (unique_count + 1) * sizeof(char*));
            unique_text[unique_count] = (char*)malloc(strlen(text[i]) + 1);
            strcpy(unique_text[unique_count], text[i]);
            unique_count++;
        }
    }

    for (int i = 0; i < *num_sentences; i++)
    {
        free(text[i]);
    }
    free(text);

    *num_sentences = unique_count;
    return unique_text;
}

void task1(char** text, int num_sentences) {
    int first_index = -1;
    int last_index = -1;
    int total_length = 0;

    for (int i = 0; i < num_sentences; i++) {
        total_length += strlen(text[i]);
    }
    char* combined_text = (char*)malloc(total_length + num_sentences);
    combined_text[0] = '\0';

    for (int i = 0; i < num_sentences; i++)
    {
        strcat(combined_text, text[i]);
        if (i < num_sentences - 1)
        {
            strcat(combined_text, " ");
        }
    }

    for (int i = 0; combined_text[i] != '\0'; i++) {
        if (combined_text[i] == '$') {
            if (first_index == -1) {
                first_index = i;
            }
            last_index = i;
        }
    }

    if (first_index == -1) {
        printf("Error: Символ '$' не найден.\n");
        free(combined_text);
        return;
    }

    if (first_index == last_index) {

        char* new_text = (char*)malloc(strlen(combined_text));

        int k = 0;

        for (int i = 0; combined_text[i] != '\0'; i++)
        {
            if (i != first_index)
            {
                new_text[k] = combined_text[i];
                k++;
            }
        }
        new_text[k] = '\0';

        printf("%s\n", new_text);

        free(combined_text);
        free(new_text);
    }
    else {
        char* new_text = (char*)malloc(last_index - first_index + 2);

        int k = 0;
        for (int i = first_index; i <= last_index; i++) {
            new_text[k++] = combined_text[i];
        }
        new_text[k] = '\0';

        printf("%s\n", new_text);

        free(combined_text);
        free(new_text);
    }
}

int count_words(const char* sentence) {
    int count = 0;
    char* copy = (char*)malloc(strlen(sentence) + 1);
    strcpy(copy, sentence);
    char* token = strtok(copy, " .");
    while (token != NULL) {
        count++;
        token = strtok(NULL, " .");
    }
    free(copy);
    return count;
}

int compare_sentences(const void* a, const void* b) {
    const char* sentence_a = *(const char**)a;
    const char* sentence_b = *(const char**)b;
    return count_words(sentence_b) - count_words(sentence_a);
}

void task2(char** text, int num_sentences) {
    qsort(text, num_sentences, sizeof(char*), compare_sentences);
    print_text(text, num_sentences);
}

void task3(char** text, int num_sentences) {
    char symbols[256] = {0};
    int counts[256] = {0};
    int num_unique_symbols = 0;

    for (int i = 0; i < num_sentences; i++) {
        for (int j = 0; text[i][j] != '\0'; j++) {
            char c = text[i][j];

            if (c != ' ' && c != ',' && c != '.' && c != '\n')
            {
                int found = 0;

                for (int k = 0; k < num_unique_symbols; k++) {
                    if (symbols[k] == c) {
                        counts[k]++;
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    symbols[num_unique_symbols] = c;
                    counts[num_unique_symbols] = 1;
                    num_unique_symbols++;
                }
            }
        }
    }

    for (int i = 0; i < num_unique_symbols; i++) {
        printf("%c: %d\n", symbols[i], counts[i]);
    }
}

void task4(char** text, int num_sentences) {
    char** new_text = NULL;
    int new_num_sentences = 0;
    for (int i = 0; i < num_sentences; i++) {
        if (i % 2 == 0) {
            new_text = (char**)realloc(new_text, (new_num_sentences + 1) * sizeof(char*));
            new_text[new_num_sentences] = (char*)malloc(strlen(text[i]) + 1);
            strcpy(new_text[new_num_sentences], text[i]);
            new_num_sentences++;
        }
    }

    print_text(new_text, new_num_sentences);

    for (int i = 0; i < new_num_sentences; i++)
    {
        free(new_text[i]);
    }
    free(new_text);
}

// Функция для дублирования предложений
char* duplicate_sentences(const char* text, int n) {
    // Выделяем память под результирующую строку (с запасом)
    size_t text_len = strlen(text);
    char* result = (char*)malloc(text_len * n * 2 + n + 1); // +n для точек, +1 для завершающего нуля
    if (result == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(1);
    }
    result[0] = '\0'; // Инициализируем пустой строкой

    // Копируем входную строку, чтобы не изменять оригинал
    char* text_copy = strdup(text);
    if (text_copy == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        free(result);
        exit(1);
    }

    char* sentence = strtok(text_copy, "."); // Используем strtok
    while (sentence != NULL) {
        // Убираем пробелы в начале и конце предложения
        while (*sentence == ' ') {
            sentence++;
        }
        size_t len = strlen(sentence);
        while (len > 0 && sentence[len - 1] == ' ') {
            sentence[len - 1] = '\0';
            len--;
        }

        if (len > 0) { // Если предложение не пустое
            // Дублируем предложение n раз
            for (int i = 0; i < n; i++) {
                // Проверяем, достаточно ли места в буфере
                if (strlen(result) + strlen(sentence) + 2 > text_len * n * 2 + n + 1)
                {
                    fprintf(stderr, "Ошибка: недостаточно памяти для конкатенации\n");
                    free(result);
                    free(text_copy);
                    exit(1);
                }
                strcat(result, sentence);

                if (i < n - 1) {
                    strcat(result, " ");
                }
            }
            // Проверяем, достаточно ли места в буфере
            if (strlen(result) + 2 > text_len * n * 2 + n + 1)
            {
                fprintf(stderr, "Ошибка: недостаточно памяти для конкатенации\n");
                free(result);
                free(text_copy);
                exit(1);
            }
            strcat(result, ".");

            char* next_sentence = strtok(NULL, ".");
            if (next_sentence != NULL)
            {
                // Проверяем, достаточно ли места в буфере
                if (strlen(result) + 2 > text_len * n * 2 + n + 1)
                {
                    fprintf(stderr, "Ошибка: недостаточно памяти для конкатенации\n");
                    free(result);
                    free(text_copy);
                    exit(1);
                }
                strcat(result, "\n");
            }
            sentence = next_sentence;
        }
        else {
            sentence = strtok(NULL, ".");
        }
    }

    free(text_copy);
    return result;
}

void task8(char** text, int num_sentences, int n) {
    for (int i = 0; i < num_sentences; i++) {
        char* duplicated_text = duplicate_sentences(text[i], n);
        printf("%s\n", duplicated_text);
        free(duplicated_text);
    }
}

void print_help() {
    printf("Доступные команды:\n");
    printf("0 - Вывести обработанный текст.\n");
    printf("1 - Найти и удалить части текста, основанные на символе '$'.\n");
    printf("2 - Отсортировать предложения по убыванию количества слов.\n");
    printf("3 - Посчитать и отобразить частоту каждого символа.\n");
    printf("4 - Удалить предложения с чётными индексами.\n");
    printf("5 - Вывод справки (это сообшение).\n");
    printf("8 - Дублировать каждое предложение N раз.\n");
}

int main() {
    print_intro();

    int command;
    scanf("%d", &command);

    if (command == 5) {
        print_help();
    }
    else if (command == 8) {
        int n;
        if (scanf("%d\n", &n) != 1) {
            fprintf(stderr, "Ошибка при чтении числа n\n");
            return 1;
        }
        int num_sentences;
        char** text = read_text(&num_sentences, 1); // 1 - single newline

        task8(text, num_sentences, n);

        for (int i = 0; i < num_sentences; i++)
        {
            free(text[i]);
        }
        free(text);
    }
    else {
        getchar();
        int num_sentences;
        char** text = read_text(&num_sentences, 0); // 0 - double newline
        char** processed_text = remove_duplicates(text, &num_sentences);

        if (command == 0) {
            print_text(processed_text, num_sentences);
        }
        else if (command == 1) {
            task1(processed_text, num_sentences);
        }
        else if (command == 2) {
            task2(processed_text, num_sentences);
        }
        else if (command == 3) {
            task3(processed_text, num_sentences);
        }
        else if (command == 4) {
            task4(processed_text, num_sentences);
        }
        else {
            printf("Error: Неверная команда.\n");
        }

        for (int i = 0; i < num_sentences; i++) {
            free(processed_text[i]);
        }
        free(processed_text);
    }

    return 0;
}