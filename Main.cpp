#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;

const int SIZE = 20;

enum DepressionType {
    TRENCH = 0,   // желоб
    FRACTURE,     // разлом
    RIDGE,        // хребет
    BASIN         // котловина
};

// Структура для представления информации об океанической впадине
struct Trench {
    char name[40];
    int depth;
    char ocean[30];
    DepressionType type;
};

typedef Trench arr[SIZE];

// Вспомогательная функция для перевода enum обратно в текст
const char* getTypeName(DepressionType t) {
    switch (t) {
    case TRENCH:   return "trench";
    case FRACTURE: return "fracture";
    case RIDGE:    return "ridge";
    case BASIN:    return "basin";
    default:       return "unknown";
    }
}

Trench* init() {
    Trench* trenches = new Trench[SIZE];
    trenches[0] = { "Mariana", 10994, "Pacific", TRENCH };
    trenches[1] = { "Tonga", 10882, "Pacific", BASIN };
    trenches[2] = { "Philippine", 10540, "Pacific", TRENCH };
    trenches[3] = { "Kermadec", 10047, "Pacific", RIDGE };
    trenches[4] = { "Izu-Ogasawara", 9780, "Pacific", RIDGE };
    trenches[5] = { "Kuril-Kamchatka", 10542, "Pacific", TRENCH };
    trenches[6] = { "Puerto_Rico", 8376, "Atlantic", FRACTURE };
    trenches[7] = { "South_Sandwich", 8428, "Atlantic", TRENCH };
    trenches[8] = { "Peru-Chile", 8065, "Pacific", TRENCH };
    trenches[9] = { "Aleutian", 7679, "Pacific", FRACTURE };
    trenches[10] = { "Cayman", 7686, "Atlantic", BASIN };
    trenches[11] = { "Java", 7192, "Indian", TRENCH };
    trenches[12] = { "Diamantina", 8047, "Indian", FRACTURE };
    trenches[13] = { "Romanche", 7760, "Atlantic", FRACTURE };
    trenches[14] = { "Eurasian", 5449, "Arctic", BASIN };
    trenches[15] = { "Litke", 5449, "Arctic", TRENCH };
    trenches[16] = { "Yap", 8527, "Pacific", RIDGE };
    trenches[17] = { "Ryukyu", 7507, "Pacific", BASIN };
    trenches[18] = { "Mid-Atlantic", 3000, "Atlantic", RIDGE };
    trenches[19] = { "Challenger", 10920, "Pacific", FRACTURE };
    return trenches;
}

// Фильтр для поиска только "желобов"
bool checkTypeZhelob(Trench t) {
    return t.type == TRENCH;
}

// Удобно принтуем
void print(Trench t) {
    printf("%-20s %-15s %6i %-15s\n", t.name, t.ocean, t.depth, getTypeName(t.type));
}

// print с фильтром
void print(Trench t, bool (*y)(Trench)) {
    if (y(t)) {
        printf("%-20s %-15s %6i %-15s\n", t.name, t.ocean, t.depth, getTypeName(t.type));
    }
}

// Обмен двух впадин
void swal(Trench& t1, Trench& t2) {
    Trench temp = t1;
    t1 = t2;
    t2 = temp;
}

// Сравнение по глубине
bool checkSortDepthDesc(Trench t1, Trench t2) {
    return t1.depth < t2.depth;
}

// Сортировка пузырьком
Trench* sorted(Trench trenches[], bool (*y)(Trench, Trench), int size) {
    for (int i = 0; i < size; i++) {
        for (int j = size - 1; j > i; j--) {
            if (y(trenches[i], trenches[j])) {
                swal(trenches[i], trenches[j]);
            }
        }
    }
    return trenches;
}

// Функция изменения данных впадины по ее глубине
void modifyByDepth(Trench trenches[], int target_depth, const char* new_name, DepressionType new_type) {
    bool found = false;
    for (int i = 0; i < SIZE; i++) {
        if (trenches[i].depth == target_depth) {
            strcpy_s(trenches[i].name, new_name);
            trenches[i].type = new_type;
            found = true;
        }
    }
    if (found) {
        printf("\n[УСПЕХ] Данные изменены на: %s (%s)\n", new_name, getTypeName(new_type));
    }
    else {
        printf("\n[ОШИБКА] Впадина с глубиной %i не найдена.\n", target_depth);
    }
}

// --- ЗАДАЧА 1: Работа с текстовым файлом ---

// Запись 2 колонок: Название и Тип (enum в виде числа)
void writeToTextFile(Trench trenches[], int size, const char* filename) {
    ofstream fout(filename);
    if (fout.is_open()) {
        for (int i = 0; i < size; i++) {
            fout << trenches[i].name << " " << trenches[i].type << "\n";
        }
        fout.close();
    }
}

// Чтение текстового файла и обновление поля type
void readFromTextFileAndUpdate(Trench trenches[], int size, const char* filename) {
    ifstream fin(filename);
    if (fin.is_open()) {
        string name;
        int type_val;
        while (fin >> name >> type_val) {
            for (int i = 0; i < size; i++) {
                if (strcmp(trenches[i].name, name.c_str()) == 0) {
                    trenches[i].type = (DepressionType)type_val;
                    break;
                }
            }
        }
        fin.close();
    }
}

// --- ЗАДАЧА 2: Работа с бинарным файлом ---

// Запись всего массива структур в бинарный файл
void writeToBinaryFile(Trench trenches[], int size, const char* filename) {
    ofstream fout(filename, ios::binary);
    if (fout.is_open()) {
        fout.write((char*)trenches, sizeof(Trench) * size);
        fout.close();
    }
}

// Чтение всего массива структур из бинарного файла
void readFromBinaryFile(Trench trenches[], int size, const char* filename) {
    ifstream fin(filename, ios::binary);
    if (fin.is_open()) {
        fin.read((char*)trenches, sizeof(Trench) * size);
        fin.close();
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    Trench* trenches = init();

    // Выполнение Задачи 1 (Текстовый файл)
    const char* txt_filename = "trenches.txt";
    writeToTextFile(trenches, SIZE, txt_filename);
    readFromTextFileAndUpdate(trenches, SIZE, txt_filename);

    // Выполнение Задачи 2 (Бинарный файл)
    const char* bin_filename = "trenches.bin";
    writeToBinaryFile(trenches, SIZE, bin_filename);
    readFromBinaryFile(trenches, SIZE, bin_filename);

    int min_depth = trenches[0].depth;
    int max_depth = trenches[0].depth;
    for (int i = 1; i < SIZE; i++) {
        if (trenches[i].depth < min_depth) min_depth = trenches[i].depth;
        if (trenches[i].depth > max_depth) max_depth = trenches[i].depth;
    }
    printf("--- Минимальная и максимальная глубина ---\n");
    printf("Минимальная глубина: %i м\n", min_depth);
    printf("Максимальная глубина: %i м\n\n", max_depth);

    sorted(trenches, checkSortDepthDesc, SIZE);

    printf("--- Желобные впадины (по убыванию глубины) ---\n");
    for (int i = 0; i < SIZE; i++) {
        print(trenches[i], checkTypeZhelob);
    }

    printf("\n--- Океаны 3-х глубочайших впадин ---\n");
    for (int i = 0; i < 3; i++) {
        printf("%i. Океан: %s (Впадина: %s, Глубина: %i)\n", i + 1, trenches[i].ocean, trenches[i].name, trenches[i].depth);
    }

    int target_depth;
    char new_name[40];
    int type_input;

    printf("\n--- Изменение данных впадины ---\n");
    printf("Введите глубину впадины, которую хотите изменить (например, 7192): ");
    cin >> target_depth;
    printf("Введите новое название (на английском, без пробелов): ");
    cin >> new_name;
    printf("Выберите новый тип (0 - trench, 1 - fracture, 2 - ridge, 3 - basin): ");
    cin >> type_input; // Вводим цифру, так как это enum

    // Приводим введенное число к типу enum и вызываем функцию
    modifyByDepth(trenches, target_depth, new_name, (DepressionType)type_input);

    // Делаем поиск по океану
    char target_ocean[30];
    printf("\nВведите название океана (например, Pacific, Atlantic, Indian): ");
    cin >> target_ocean;

    Trench* filtered_trenches = new Trench[SIZE];
    int filtered_count = 0;

    for (int i = 0; i < SIZE; i++) {
        if (!strcmp(trenches[i].ocean, target_ocean)) {
            filtered_trenches[filtered_count] = trenches[i];
            filtered_count++;
        }
    }

    printf("\n--- Впадины океана \"%s\" ---\n", target_ocean);
    if (filtered_count == 0) {
        printf("Впадины в данном океане не найдены.\n");
    }
    else {
        for (int i = 0; i < filtered_count; i++) {
            print(filtered_trenches[i]);
        }
    }

    delete[] trenches;
    delete[] filtered_trenches;

    return 0;
}
