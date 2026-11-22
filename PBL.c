
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student
{
    int id;
    char name[50];
    float fees;
    int paid;
    char course[20];
    char result[20];
    struct student *next;
} STUDENT;

STUDENT *head = NULL;

void saveData()
{
    FILE *fp = fopen("students.dat", "wb");
    STUDENT *p = head;

    if (fp == NULL)
    {
        printf("File error!\n");
        return;
    }

    while (p != NULL)
    {
        fwrite(p, sizeof(STUDENT), 1, fp);
        p = p->next;
    }

    fclose(fp);
}

void loadData()
{
    FILE *fp = fopen("students.dat", "rb");
    STUDENT rec, *p;

    if (fp == NULL)
        return;

    while (fread(&rec, sizeof(STUDENT), 1, fp) == 1)
    {
        p = (STUDENT *)malloc(sizeof(STUDENT));

        if (p == NULL)
            break;

        *p = rec;
        p->next = NULL;

        if (head == NULL)
            head = p;
        else
        {
            STUDENT *q = head;

            while (q->next != NULL)
                q = q->next;

            q->next = p;
        }
    }

    fclose(fp);
}

STUDENT *searchStudent(int id)
{
    STUDENT *p = head;

    while (p != NULL)
    {
        if (p->id == id)
            return p;

        p = p->next;
    }

    return NULL;
}

void registerStudent()
{
    STUDENT *p, *q;
    int id;

    p = (STUDENT *)malloc(sizeof(STUDENT));

    if (p == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d", &id);

    if (searchStudent(id) != NULL)
    {
        printf("Student ID already exists!\n");
        free(p);
        return;
    }

    p->id = id;

    printf("Enter Student Name: ");
    scanf(" %49[^\n]", p->name);

    p->fees = 0;
    p->paid = 0;
    strcpy(p->course, "Not Allocated");
    strcpy(p->result, "Pending");
    p->next = NULL;

    if (head == NULL)
        head = p;
    else
    {
        q = head;

        while (q->next != NULL)
            q = q->next;

        q->next = p;
    }

    saveData();
    printf("Student registered successfully!\n");
}

void payFees()
{
    int id;
    STUDENT *p;

    printf("Enter Student ID: ");
    scanf("%d", &id);

    p = searchStudent(id);

    if (p == NULL)
    {
        printf("Student not found!\n");
        return;
    }

    if (p->paid)
    {
        printf("Fees are already paid.\n");
        return;
    }

    printf("Enter fee amount: ");
    scanf("%f", &p->fees);

    if (p->fees <= 0)
    {
        printf("Invalid fee amount!\n");
        p->fees = 0;
        return;
    }

    p->paid = 1;
    saveData();

    printf("Fees paid successfully!\n");
}

void allocateCourse()
{
    int id, choice;
    STUDENT *p;

    printf("Enter Student ID: ");
    scanf("%d", &id);

    p = searchStudent(id);

    if (p == NULL)
    {
        printf("Student not found!\n");
        return;
    }

    if (!p->paid)
    {
        printf("Please pay fees first!\n");
        return;
    }

    if (strcmp(p->course, "Not Allocated") != 0)
    {
        printf("Course already allocated: %s\n", p->course);
        return;
    }

    printf("\n1. DS\n2. AI\n3. ML\n4. Web\n");
    printf("Choose Course: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            strcpy(p->course, "DS");
            break;
        case 2:
            strcpy(p->course, "AI");
            break;
        case 3:
            strcpy(p->course, "ML");
            break;
        case 4:
            strcpy(p->course, "Web");
            break;
        default:
            printf("Invalid course choice!\n");
            return;
    }

    saveData();
    printf("Course allocated successfully!\n");
}

void updateResult()
{
    int id, choice;
    STUDENT *p;

    printf("Enter Student ID: ");
    scanf("%d", &id);

    p = searchStudent(id);

    if (p == NULL)
    {
        printf("Student not found!\n");
        return;
    }

    if (strcmp(p->course, "Not Allocated") == 0)
    {
        printf("No course allocated to this student!\n");
        return;
    }

    printf("\n1. Excellent\n2. Good\n3. Fail\n");
    printf("Enter Result: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            strcpy(p->result, "Excellent");
            break;
        case 2:
            strcpy(p->result, "Good");
            break;
        case 3:
            strcpy(p->result, "Fail");
            break;
        default:
            printf("Invalid result choice!\n");
            return;
    }

    saveData();
    printf("Result updated successfully!\n");
}

void displayStudents()
{
    STUDENT *p = head;

    if (p == NULL)
    {
        printf("No student records found!\n");
        return;
    }

    printf("\n------------------------------------------------------------\n");
    printf("ID   NAME             FEES    PAYMENT   COURSE    RESULT\n");
    printf("------------------------------------------------------------\n");

    while (p != NULL)
    {
        printf("%-4d %-16s %-8.2f %-9s %-10s %s\n",
               p->id,
               p->name,
               p->fees,
               p->paid ? "Paid" : "Unpaid",
               p->course,
               p->result);

        p = p->next;
    }

    printf("------------------------------------------------------------\n");
}

void searchRecord()
{
    int id;
    STUDENT *p;

    printf("Enter Student ID: ");
    scanf("%d", &id);

    p = searchStudent(id);

    if (p == NULL)
    {
        printf("Student not found!\n");
        return;
    }

    printf("\nStudent ID   : %d\n", p->id);
    printf("Student Name : %s\n", p->name);
    printf("Fees         : %.2f\n", p->fees);
    printf("Payment      : %s\n", p->paid ? "Paid" : "Unpaid");
    printf("Course       : %s\n", p->course);
    printf("Result       : %s\n", p->result);
}

int login()
{
    char username[30], password[30];

    printf("===== LOGIN SYSTEM =====\n");
    printf("Username: ");
    scanf("%29s", username);

    printf("Password: ");
    scanf("%29s", password);

    if (strcmp(username, "admin") == 0 &&
        strcmp(password, "1234") == 0)
    {
        printf("\nLogin successful!\n");
        return 1;
    }

    printf("\nInvalid username or password!\n");
    return 0;
}

int main()
{
    int choice;

    if (!login())
        return 0;

    loadData();

    do
    {
        printf("\n===== STUDENT COURSE MANAGEMENT SYSTEM =====\n");
        printf("1. Register Student\n");
        printf("2. Pay Fees\n");
        printf("3. Allocate Course\n");
        printf("4. Update Result\n");
        printf("5. Display All Students\n");
        printf("6. Search Student\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                registerStudent();
                break;
            case 2:
                payFees();
                break;
            case 3:
                allocateCourse();
                break;
            case 4:
                updateResult();
                break;
            case 5:
                displayStudents();
                break;
            case 6:
                searchRecord();
                break;
            case 7:
                saveData();
                printf("Data saved. Thank you!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 7);

    return 0;
}
