#include <iostream>
#include <string>
#include <fstream>

// ���������, �� ������ ����� �������� �� ���� �������� (��� ����� ����� � ��� ����� ��������)
struct Task {
    std::string name;
    int priority;
};

class TaskManager {
private:
    Task* tasks;       // �������� �� ��������� ����� �������
    int count;         // ������� ������� �������
    int capacity;      // ������� ������ ������

    // ������� ��� ��������� ������ ������ ��� ��������� ������ (�������� ������)
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
    // �����������: ����������� � ����� ������
    TaskManager() : count(0), capacity(2) {
        tasks = new Task[capacity];
    }

    // ����������: ��������� �������� ������� ���'��
    ~TaskManager() {
        delete[] tasks;
    }

    // ������� ��� �������������� ��������� �������� � ������� ����������.
    // ���� ������� �������� � ���������� >= desiredPriority, �� �� �������� ����������.
    void addTask(const std::string& name, int desiredPriority) {
        // �������: ��� ��� ������� � ���������� >= desiredPriority �������� �������� �� 1.
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

    // ������� ��� ��������� �������� � ����� ��� ������� ���������.
    void addTaskFromFile(const std::string& name, int priority) {
        if (count == capacity) {
            resizeArray();
        }
        tasks[count].name = name;
        tasks[count].priority = priority;
        count++;
    }

    // ������� ��� ��������� �������� � �������� ���������� (�������� �������� ���������)
    void completeTask() {
        if (count == 0) {
            std::cout << "No tasks to complete." << std::endl;
            return;
        }
        // ����������� ������� �������� � �������� ����������
        int highestPriorityIndex = 0;
        for (int i = 1; i < count; i++) {
            if (tasks[i].priority < tasks[highestPriorityIndex].priority) {
                highestPriorityIndex = i;
            }
        }
        std::cout << "Task completed: [Priority: "
            << tasks[highestPriorityIndex].priority << "] "
            << tasks[highestPriorityIndex].name << std::endl;
        // ��������� �������� ������ ����� �������� � �����
        for (int i = highestPriorityIndex; i < count - 1; i++) {
            tasks[i] = tasks[i + 1];
        }
        count--;
    }

    // ������� ��� ��������� ��� �������, ������������ �� ����������, ��� ���� ������� � ������������ �����
    void showTasks() {
        if (count == 0) {
            std::cout << "Task list is empty." << std::endl;
            return;
        }
        // ��������� ������� � ���������� ��������� ����� ��� �� ����������
        Task* sortedTasks = new Task[count];
        for (int i = 0; i < count; i++) {
            sortedTasks[i] = tasks[i];
        }
        // ������ ����������� ���������� �� ���������� (�� ����������)
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

    // ������� ��� ��������� ������� ������� �������
    int getCount() const {
        return count;
    }

    // ������� ��� ������������ ������� � �����
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return; // ���� �� ���� ��� �� ������� �������
        }
        int n;
        file >> n;
        file.ignore(); // ���������� ������ ������ �����
        for (int i = 0; i < n; i++) {
            std::string name;
            int priority;
            std::getline(file, name);
            file >> priority;
            file.ignore(); // ���������� ������ ������ �����
            addTaskFromFile(name, priority);
        }
        file.close();
    }

    // ������� ��� ���������� ������� � ����
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

    // ������ ����������� �������� � ����� "tasks.txt"
    manager.loadFromFile("tasks.txt");

    int choice;

    // ���� ��������
    do {
        std::cout << "\n=== Task Manager Menu ===" << std::endl;
        std::cout << "1. Add task" << std::endl;
        std::cout << "2. Complete task" << std::endl;
        std::cout << "3. Show tasks" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        // ������� �������� �����������
        switch (choice) {
        case 1: {
            std::cin.ignore(); // �������� ������ �����
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

    // ���������� ������� � ���� ����� �������
    manager.saveToFile("tasks.txt");

    return 0;
}
