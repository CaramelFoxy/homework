#include <iostream>
#include <string>
#include <fstream>

// Структура, що містить назву завдання та його пріоритет (чим менше число – тим вищий пріоритет)
struct Task {
    std::string name;
    int priority;
};

class TaskManager {
private:
    Task* tasks;       // Вказівник на динамічний масив завдань
    int count;         // Поточна кількість завдань
    int capacity;      // Поточна ємність масиву

    // Функція для збільшення розміру масиву при досягненні ємності (подвоєння ємності)
    void resizeArray() {
        capacity *= 2;
        Task* newTasks = new Task[capacity];
        for (int i = 0; i < count; i++) {
            newTasks[i] = tasks[i];
        }
        delete[] tasks;
        tasks = newTasks;
    }

public:
    // Конструктор: ініціалізація з малою ємністю
    TaskManager() : count(0), capacity(2) {
        tasks = new Task[capacity];
    }

    // Деструктор: звільнення динамічно виділеної пам'яті
    ~TaskManager() {
        delete[] tasks;
    }

    // Функція для інтерактивного додавання завдання з бажаним пріоритетом.
    // Якщо існують завдання з пріоритетом >= desiredPriority, то їх пріоритет збільшується.
    void addTask(const std::string& name, int desiredPriority) {
        // Зміщення: для всіх завдань з пріоритетом >= desiredPriority збільшуємо значення на 1.
        for (int i = 0; i < count; i++) {
            if (tasks[i].priority >= desiredPriority) {
                tasks[i].priority++;
            }
        }

        if (count == capacity) {
            resizeArray();
        }

        tasks[count].name = name;
        tasks[count].priority = desiredPriority;
        std::cout << "Task added: [Priority: " << desiredPriority << "] " << name << std::endl;
        count++;
    }

    // Функція для додавання завдання з файлу без зміщення пріоритетів.
    void addTaskFromFile(const std::string& name, int priority) {
        if (count == capacity) {
            resizeArray();
        }
        tasks[count].name = name;
        tasks[count].priority = priority;
        count++;
    }

    // Функція для виконання завдання з найвищим пріоритетом (мінімальне значення пріоритету)
    void completeTask() {
        if (count == 0) {
            std::cout << "No tasks to complete." << std::endl;
            return;
        }
        // Знаходження індексу завдання з найвищим пріоритетом
        int highestPriorityIndex = 0;
        for (int i = 1; i < count; i++) {
            if (tasks[i].priority < tasks[highestPriorityIndex].priority) {
                highestPriorityIndex = i;
            }
        }
        std::cout << "Task completed: [Priority: "
            << tasks[highestPriorityIndex].priority << "] "
            << tasks[highestPriorityIndex].name << std::endl;
        // Видалення завдання шляхом зсуву елементів в масиві
        for (int i = highestPriorityIndex; i < count - 1; i++) {
            tasks[i] = tasks[i + 1];
        }
        count--;
    }

    // Функція для виведення всіх завдань, відсортованих за пріоритетом, без зміни порядку в оригінальному масиві
    void showTasks() {
        if (count == 0) {
            std::cout << "Task list is empty." << std::endl;
            return;
        }
        // Копіювання завдань у тимчасовий динамічний масив для їх сортування
        Task* sortedTasks = new Task[count];
        for (int i = 0; i < count; i++) {
            sortedTasks[i] = tasks[i];
        }
        // Проста бульбашкова сортування за пріоритетом (за зростанням)
        for (int i = 0; i < count; i++) {
            for (int j = i + 1; j < count; j++) {
                if (sortedTasks[j].priority < sortedTasks[i].priority) {
                    Task temp = sortedTasks[i];
                    sortedTasks[i] = sortedTasks[j];
                    sortedTasks[j] = temp;
                }
            }
        }
        std::cout << "Task list:" << std::endl;
        for (int i = 0; i < count; i++) {
            std::cout << " - [Priority: "
                << sortedTasks[i].priority << "] "
                << sortedTasks[i].name << std::endl;
        }
        delete[] sortedTasks;
    }

    // Функція для отримання поточної кількості завдань
    int getCount() const {
        return count;
    }

    // Функція для завантаження завдань з файлу
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return; // Файл не існує або не вдалося відкрити
        }
        int n;
        file >> n;
        file.ignore(); // Пропустити символ нового рядка
        for (int i = 0; i < n; i++) {
            std::string name;
            int priority;
            std::getline(file, name);
            file >> priority;
            file.ignore(); // Пропустити символ нового рядка
            addTaskFromFile(name, priority);
        }
        file.close();
    }

    // Функція для збереження завдань у файл
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        file << count << std::endl;
        for (int i = 0; i < count; i++) {
            file << tasks[i].name << std::endl;
            file << tasks[i].priority << std::endl;
        }
        file.close();
    }
};

int main() {
    TaskManager manager;

    // Спроба завантажити завдання з файлу "tasks.txt"
    manager.loadFromFile("tasks.txt");

    int choice;

    // Меню програми
    do {
        std::cout << "\n=== Task Manager Menu ===" << std::endl;
        std::cout << "1. Add task" << std::endl;
        std::cout << "2. Complete task" << std::endl;
        std::cout << "3. Show tasks" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        // Обробка введення користувача
        switch (choice) {
        case 1: {
            std::cin.ignore(); // Очищення буферу вводу
            std::string taskName;
            int taskPriority;
            std::cout << "Enter task name: ";
            std::getline(std::cin, taskName);
            std::cout << "Enter task priority (lower number = higher priority): ";
            std::cin >> taskPriority;
            manager.addTask(taskName, taskPriority);
            break;
        }
        case 2:
            manager.completeTask();
            break;
        case 3:
            manager.showTasks();
            break;
        case 0:
            std::cout << "Exiting program." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 0);

    // Збереження завдань у файл перед виходом
    manager.saveToFile("tasks.txt");

    return 0;
}
