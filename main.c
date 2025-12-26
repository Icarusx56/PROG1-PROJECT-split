#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define max_lineLength 100
#define max_usernameLength 100
#define max_passwordLength 100
#define dailyLimit 50000.0

typedef struct
{
    int month;
    int year;
} Date;

typedef struct
{
    char name[50];
    char number[11];
    char email[50];
    double balance;
    double dailyWithdrawalTotal;
    time_t lastWithdrawalDate;
    char mobile[12];
    Date date_opened;
    char status[10];
    char transactions[100][100];
    int num_of_transactions;
} Account;

int num_of_accounts = 0, change = 0;
Account acc[100], temp[100];

int validateOverflow(char *input);
int Unique_Email(char *email);
int validateBalance(char *balance);
int validateTransaction(char *amount);
int validateName(char *name);
int validateMobile(char *mobile);
int validateYesNo();
int validateActiveInactive(char *answer);
int validateKeyword(char *keyword);
int searchUsername_Password(char *username, char *password);
int validateEmail(char *email);
int validateACC_NUM(char *acc_num);
int validateChoice(char *choice);
void num_to_month(Account a);
int validateNegativeOne(char *input);
int validateSortChoice(char *input);
int validateDate(char *date);

void MENU();
void returnToMenu();
void GO_TO_MENU();

void PRINT_Unsorted();
int LOGIN();
int LOAD();
void LOAD_REPORTS();
void QUERY_SEARCH();
void ADVANCED_SEARCH();
int checkNUM(char *number);
void ADD();
void DELETE();
void MODIFY();
void CHANGE_STATUS();
int canWithdraw(Account *acc, double amountToWithdraw);
void WITHDRAW();
void DEPOSIT();
void TRANSFER();
void REPORT();
void sortByDate();
void sortByName();
void sortByBalance();
void sortByStatus();
void PRINT_SORT();
int SAVE();
void QUIT();
void DELETE_MULTIPLE();

int main()
{
    LOGIN();
    return 0;
}

void MENU()
{
    while (1)
    {
        char n[100];
        printf("\n======================== MAIN MENU ========================\n");
        printf("1.  Add an account\n");
        printf("2.  Delete an account\n");
        printf("3.  Modify an account \n");
        printf("4.  Search with the account number\n");
        printf("5.  Advanced search\n");
        printf("6.  Change status for an account\n");
        printf("7.  Withdraw money from an account\n");
        printf("8.  Deposit in an account\n");
        printf("9.  Transfer money from an account to another\n");
        printf("10. Report(prints the last five transactions for an account)\n");
        printf("11. Print all the accounts in a sorted manner\n");
        printf("12. Delete multiple accounts (By Date / Inactive)\n");
        printf("13. Save all changes\n");
        printf("0 ==> Quit the program\n");
        printf("============================================================\n");
        while (1)
        {
            printf("Enter your choice (0-13): ");
            fgets(n, sizeof(n), stdin);
            n[strcspn(n, "\n")] = '\0';
            if (!validateChoice(n))
                continue;
            break;
        }
        switch (atoi(n))
        {
        case 0:
            QUIT();
            break;
        case 1:
            ADD();
            break;
        case 2:
            DELETE();
            break;
        case 3:
            MODIFY();
            break;
        case 4:
            QUERY_SEARCH();
            break;
        case 5:
            ADVANCED_SEARCH();
            break;
        case 6:
            CHANGE_STATUS();
            break;
        case 7:
            WITHDRAW();
            break;
        case 8:
            DEPOSIT();
            break;
        case 9:
            TRANSFER();
            break;
        case 10:
            REPORT();
            break;
        case 11:
            PRINT_SORT();
            break;
        case 12:
            DELETE_MULTIPLE();
            break;
        case 13:
            while (1)
            {
                printf("Are you sure you want to save these changes? [Yes/No]: ");
                if (SAVE() == -1)
                    continue;
                break;
            }
            GO_TO_MENU();
        }
        return;
    }
}
void GO_TO_MENU()
{
    char input[100];
    do
    {
        printf("Enter '-1' to return to menu: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        if (input == NULL)
        {
            printf("Error reading input.\n");
            continue;
        }

        if (validateNegativeOne(input))
        {
            returnToMenu();
            return;
        }
    } while (1);
}
void returnToMenu()
{
    printf("\nReturning to Main Menu...\n");
    MENU();
}

int validateOverflow(char *input)
{
    if (strlen(input) == 100 - 1 && input[100 - 2] != '\n')
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        printf("Error: Input too long.\n");
        return 0;
    }
    return 1;
}
int validateYearChoice(char *choice)
{
    if (choice == NULL || strlen(choice) == 0)
    {
        printf("Error: No year entered.\n");
        return 0;
    }
    if (strlen(choice) != 4)
    {
        printf("Error: Year must be exactly 4 digits (YYYY format).\n");
        return 0;
    }
    for (int i = 0; i < 4; i++)
    {
        if (!isdigit(choice[i]))
        {
            printf("Error: Year must contain only digits (0-9).\n");
            return 0;
        }
    }
    for (int i = 0; i < strlen(choice); i++)
    {
        if (isspace(choice[i]))
        {
            printf("Error: Year cannot contain spaces.\n");
            return 0;
        }
    }

    int year = atoi(choice);

    if (year == 0 && choice[0] != '0')
    {
        printf("Error: Invalid year format.\n");
        return 0;
    }

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int currentYear = tm_info->tm_year + 1900;

    if (year > currentYear)
    {
        printf("Error: Year cannot exceed current year (%d).\n", currentYear);
        return 0;
    }
    return 1;
}
int validateMonthChoice(char *choice)
{
    if (choice == NULL || strlen(choice) == 0)
    {
        printf("Error: Input cannot be empty.\n");
        return 0;
    }
    choice[strcspn(choice, "\n")] = '\0';
    if (strchr(choice, ' ') != NULL)
    {
        printf("Error: Input can not contain spaces.\n");
        return 0;
    }
    int i;
    for (i = 0; choice[i] != '\0'; i++)
    {
        if (!isdigit(choice[i]))
        {
            printf("Error: Invalid input.\n");
            return 0;
        }
    }
    int c = atoi(choice);
    if (c < 1 || c > 12)
    {
        printf("Error: Input is outside the range (1-12).\n");
        return 0;
    }
    return 1;
}
int validateDeleteChoice(char *choice)
{
    if (choice == NULL || strlen(choice) == 0)
    {
        printf("Error: Input cannot be empty.\n");
        return 0;
    }
    choice[strcspn(choice, "\n")] = '\0';
    if (strchr(choice, ' ') != NULL)
    {
        printf("Error: Input can not contain spaces.\n");
        return 0;
    }
    int i;
    for (i = 0; choice[i] != '\0'; i++)
    {
        if (!isdigit(choice[i]))
        {
            printf("Error: Invalid input.\n");
            return 0;
        }
    }
    int c = atoi(choice);
    if (c < 1 || c > 2)
    {
        printf("Error: Input is outside the range (1-2).\n");
        return 0;
    }
    return 1;
}
int validateChoice(char *choice)
{
    if (choice == NULL || strlen(choice) == 0)
    {
        printf("Error: Input cannot be empty.\n");
        return 0;
    }
    choice[strcspn(choice, "\n")] = '\0';
    if (strchr(choice, ' ') != NULL)
    {
        printf("Error: Input can not contain spaces.\n");
        return 0;
    }
    int i;
    for (i = 0; choice[i] != '\0'; i++)
    {
        if (!isdigit(choice[i]))
        {
            printf("Error: Invalid input.\n");
            return 0;
        }
    }
    int c = atoi(choice);
    if (c < 0 || c > 13)
    {
        printf("Error: Input is outside the range (0-13).\n");
        return 0;
    }
    return 1;
}
int validateLogin(char *choice)
{
    if (choice == NULL || strlen(choice) == 0)
    {
        printf("Error: Input cannot be empty.\n");
        return 0;
    }
    choice[strcspn(choice, "\n")] = '\0';
    if (strchr(choice, ' ') != NULL)
    {
        printf("Error: Input can not contain spaces.\n");
        return 0;
    }
    int i;
    for (i = 0; choice[i] != '\0'; i++)
    {
        if (!isdigit(choice[i]))
        {
            printf("Error: Invalid input.\n");
            return 0;
        }
    }
    int c = atoi(choice);
    if (c != 0 && c != 1)
    {
        printf("Error: Input is outside the range (0-1).\n");
        return 0;
    }
    return 1;
}
int Unique_Email(char *email)
{
    int i, unique = 0;
    for (i = 0; i < num_of_accounts; i++)
    {
        if (!strcmp(email, acc[i].email))
        {
            unique = 1;
            break;
        }
    }
    if (unique == 1)
    {
        printf("Error: Email already in use");
        return 0;
    }
    return 1;
}
int validateBalance(char *balance)
{
    int i, length = strlen(balance), dot_count = 0;

    if (length == 0)
    {
        printf("Error: Balance cannot be empty.\n");
        return 0;
    }

    if (balance[0] == '-')
    {
        printf("Error: Balance cannot be negative. You must enter a positive amount.\n");
        return 0;
    }

    if (!isdigit(balance[0]))
    {
        printf("Error: Balance must start with a digit.\n");
        return 0;
    }

    if (length > 1 && balance[0] == '0' && balance[1] != '.' && balance[1] != '\0')
    {
        printf("Error: Leading zeros are not allowed.\n");
        return 0;
    }

    for (i = 0; i < length; i++)
    {
        if (!isdigit(balance[i]) && balance[i] != '.')
        {
            printf("Error: Balance can only contain digits and a decimal point.\n");
            return 0;
        }

        if (balance[i] == '.')
        {
            dot_count++;
            if (dot_count > 1)
            {
                printf("Error: Balance can only have one decimal point.\n");
                return 0;
            }
            if (i == length - 1)
            {
                printf("Error: Decimal point cannot be at the end.\n");
                return 0;
            }

            if (i == 0)
            {
                printf("Error: Decimal point cannot be at the beginning.\n");
                return 0;
            }
        }
    }
    return 1;
}
int validateTransaction(char *amount)
{
    int i, length = strlen(amount), dot_count = 0;

    if (length == 1 && amount[0] == '0')
    {
        printf("Error: Amount cannot be zero. You must enter a positive amount.\n");
    }
    if (length == 0)
    {
        printf("Error: Amount cannot be empty.\n");
        return 0;
    }
    if (amount[0] == '-')
    {
        printf("Error: Amount cannot be negative. You must enter a positive amount.\n");
        return 0;
    }
    if (!isdigit(amount[0]))
    {
        printf("Error: Amount must start with a digit.\n");
        return 0;
    }

    if (length > 1 && amount[0] == '0' && amount[1] != '.' && amount[1] != '\0')
    {
        printf("Error: Leading zeros are not allowed.\n");
        return 0;
    }

    for (i = 0; i < length; i++)
    {
        if (!isdigit(amount[i]) && amount[i] != '.')
        {
            printf("Error: Amount can only contain digits and a decimal point.\n");
            return 0;
        }

        if (amount[i] == '.')
        {
            dot_count++;
            if (dot_count > 1)
            {
                printf("Error: Amount can only have one decimal point.\n");
                return 0;
            }
            if (i == length - 1)
            {
                printf("Error: Decimal point cannot be at the end.\n");
                return 0;
            }

            if (i == 0)
            {
                printf("Error: Decimal point cannot be at the beginning.\n");
                return 0;
            }
        }
    }

    return 1;
}
int validateName(char *name)
{
    int i, count_char = 0, count_space = 0;
    int len;

    if (!name || !name[0])
    {
        printf("Error: Name is empty.\n");
        return 0;
    }

    len = strlen(name);

    for (i = 0; i < len; i++)
    {
        if (isalpha(name[i]))
            count_char++;
        else if (name[i] == ' ')
            count_space++;
        else
        {
            printf("Error: Name can only contain letters and spaces.\n");
            return 0;
        }
    }

    if (count_space != 1)
    {
        printf("Error: Name should be in the format (first_name last_name)\n");
        return 0;
    }

    if (count_char == 0)
    {
        printf("Error: Name must contain letters.\n");
        return 0;
    }

    return 1;
}
int validateMobile(char *mobile)
{
    int i;
    if (strlen(mobile) != 11)
    {
        printf("Error: Mobile number must be 11 numbers.\n");
        return 0;
    }
    for (i = 0; i < 11; i++)
    {
        if (!isdigit(mobile[i]))
        {
            printf("Error: Mobile number can only contain digits 0-9.\n");
            return 0;
        }
    }
    if (mobile[0] != '0')
    {
        printf("Error: Mobile number must start with 0.\n");
        return 0;
    }
    if (mobile[1] != '1')
    {
        printf("Error: 2nd digit of mobile number must be 1.\n");
        return 0;
    }
    if (mobile[2] != '0' && mobile[2] != '1' && mobile[2] != '2' && mobile[2] != '5')
    {
        printf("Error: Phone number must start with 010, 011, 012 or 015.\n");
        return 0;
    }
    return 1;
}
int validateNegativeOne(char *input)
{
    if (!input)
    {
        return 0;
    }
    int len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
    {
        input[len - 1] = '\0';
    }
    if (strcmp(input, "-1") == 0)
    {
        return 1;
    }
    if (strstr(input, "-1") != NULL)
    {
        printf("Error: Please enter '-1' exactly (without extra characters/spaces) to return to menu.\n");
    }
    else
    {
        printf("Error: Please enter '-1' to return to menu.\n");
    }

    return 0;
}
int validateYesNo()
{
    char answer[100];
    int i;
    fgets(answer, sizeof(answer), stdin);
    answer[strcspn(answer, "\n")] = '\0';
    if (!validateOverflow(answer))
    {
        return -1;
    }
    for (i = 0; i < strlen(answer); i++)
        answer[i] = tolower(answer[i]);
    if (!strcmp(answer, "yes"))
        return 1;
    else if (!strcmp(answer, "no"))
        return 0;
    else
    {
        printf("Invalid input, try again.\n");
        return -1;
    }
}
int validateActiveInactive(char *answer)
{
    int i;
    for (i = 0; i < strlen(answer); i++)
        answer[i] = tolower(answer[i]); // check the answer
    if (!strcmp(answer, "active"))
        return 1;
    else if (!strcmp(answer, "inactive"))
        return 0;
    else
        return -1;
}
int validateKeyword(char *keyword)
{
    if (!strlen(keyword))
    {
        printf("Error: keyword can not be empty.\n");
        return 0;
    }
    if (strlen(keyword) < 3)
    {
        printf("Keyword must be at least 3 letters.\n");
        return 0;
    }
    int i;
    int length = strlen(keyword);

    for (i = 0; i < length; i++)
    {
        if (!isalpha(keyword[i]) && keyword[i] != ' ')
        {
            printf("Error: Keyword can only contain letters and spaces!\n");
            return 0;
        }
    }
    return 1;
}
int searchUsername_Password(char *username, char *password)
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        printf("Error: Unable to find users.txt file\n");
        return 0;
    }

    char line[max_lineLength];

    while (fgets(line, sizeof(line), file) != NULL)
    {

        line[strcspn(line, "\n")] = '\0';

        char *fileuser = strtok(line, " ");
        char *filepass = strtok(NULL, "");

        if (fileuser && filepass)
        {

            if (strcmp(fileuser, username) == 0 && strcmp(filepass, password) == 0)
            {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}
int validateEmail(char *email)
{
    if (!email || !email[0])
    {
        printf("Error: Email is empty.\n");
        return 0;
    }
    for (int i = 0; email[i] != '\0'; i++)
    {
        if (isspace((unsigned char)email[i]))
        {
            printf("Error: Email cannot contain spaces, tabs, or newlines.\n");
            return 0;
        }
    }
    char *at = strchr(email, '@');
    if (at == NULL)
    {
        printf("Error: Email does not contain '@' symbol.\n");
        return 0;
    }
    if (strchr(at + 1, '@') != NULL)
    {
        printf("Error: Email cannot contain more than one '@' symbol.\n");
        return 0;
    }
    if (at == email)
    {
        printf("Error: Nothing before '@'\n");
        return 0;
    }
    char *dot = strchr(at, '.');
    if (dot == at + 1)
    {
        printf("Error: '.' cannot be immediately after '@'\n");
        return 0;
    }
    if (strstr(email, "..") != NULL)
    {
        printf("Error: Email cannot contain consecutive dots.\n");
        return 0;
    }
    if (dot == NULL)
    {
        printf("Error: Email does not contain '.' \n");
        return 0;
    }
    int length = strlen(email);
    if (email[length - 1] == '.')
    {
        printf("Error: Email cannot end with a dot.\n");
        return 0;
    }
    if (strlen(dot + 1) < 2)
    {
        printf("Error: Invalid domain extension.\n");
        return 0;
    }

    for (int i = 1; dot[i] != '\0'; i++)
    {
        if (!isalpha(dot[i]) && dot[i] != '.')
        {
            printf("Error: Domain extension can only contain letters.\n");
            return 0;
        }
    }
    return 1;
}
int validateACC_NUM(char *acc_num)
{
    if (strlen(acc_num) != 10)
    {
        printf("Error: Account number must be 10 digits!\n");
        return 0;
    }
    int i;
    for (i = 0; i < 10; i++)
    {
        if (!isdigit(acc_num[i]))
        {
            printf("Invalid characters in account number!\n");
            return 0;
        }
    }
    return 1;
}
int validateSortChoice(char *input)
{
    if (input == NULL || strlen(input) == 0)
    {
        printf("Error: Input cannot be empty.\n");
        return 0;
    }
    input[strcspn(input, "\n")] = '\0';
    if (strchr(input, ' ') != NULL)
    {
        printf("Error: Input can not contain spaces.\n");
        return 0;
    }
    int i;
    for (i = 0; input[i] != '\0'; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Error: Invalid input.\n");
            return 0;
        }
    }
    int c = atoi(input);
    if (c < 1 || c > 5)
    {
        printf("Error: Input is outside the range (1-5).\n");
        return 0;
    }
    return 1;
}
void num_to_month(Account a)
{
    switch (a.date_opened.month)
    {
    case 1:
        printf("Date opened: January %d\n", a.date_opened.year);
        break;
    case 2:
        printf("Date opened: February %d\n", a.date_opened.year);
        break;
    case 3:
        printf("Date opened: March %d\n", a.date_opened.year);
        break;
    case 4:
        printf("Date opened: April %d\n", a.date_opened.year);
        break;
    case 5:
        printf("Date opened: May %d\n", a.date_opened.year);
        break;
    case 6:
        printf("Date opened: June %d\n", a.date_opened.year);
        break;
    case 7:
        printf("Date opened: July %d\n", a.date_opened.year);
        break;
    case 8:
        printf("Date opened: August %d\n", a.date_opened.year);
        break;
    case 9:
        printf("Date opened: September %d\n", a.date_opened.year);
        break;
    case 10:
        printf("Date opened: October %d\n", a.date_opened.year);
        break;
    case 11:
        printf("Date opened: November %d\n", a.date_opened.year);
        break;
    case 12:
        printf("Date opened: December %d\n", a.date_opened.year);
        break;
    }
}

int LOGIN()
{
    while (1)
    {
        char n[100], username[max_usernameLength], password[max_passwordLength];
        int logged_in = 0;
        printf("Enter 1 to login or 0 to exit: ");
        fgets(n, sizeof(n), stdin);
        n[strcspn(n, "\n")] = '\0';
        if (!validateOverflow(n))
            continue;
        if (!validateLogin(n))
            continue;
        if (atoi(n))
        {
            printf("_-=========== LOGIN ===========-_\n\n");

            while (!logged_in)
            {
                printf("Username (-1 to exit): ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0';

                if (!strcmp(username, "-1"))
                {
                    printf("Exiting the program.....\n");
                    exit(0);
                }

                printf("Password (-1 to exit): ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = '\0';

                if (!strcmp(password, "-1"))
                {
                    printf("Exiting the program.....\n");
                    exit(0);
                }

                if (searchUsername_Password(username, password))
                {
                    printf("Login Successful. Welcome, %s.", username);
                    logged_in = 1;
                }
                if (!logged_in)
                    printf("Invalid credentials, try again.\n");
            }
            LOAD();
            LOAD_REPORTS();
            MENU();
        }
        else
        {
            QUIT();
            return 0;
        }
    }
}
int LOAD()
{
    FILE *f = fopen("Accounts.txt", "r");
    if (f == NULL)
    {
        printf("Error: file not found.\n");
        num_of_accounts = 0;
        return 0;
    }

    char line[max_lineLength];
    num_of_accounts = 0;

    while (fgets(line, sizeof(line), f) != NULL && num_of_accounts < 100)
    {
        line[strcspn(line, "\n")] = '\0';
        if (!line[0])
            continue;

        Account *a = &acc[num_of_accounts];

        char line_copy[max_lineLength];
        strcpy(line_copy, line);

        strcpy(a->number, strtok(line_copy, ","));
        strcpy(a->name, strtok(NULL, ","));
        strcpy(a->email, strtok(NULL, ","));
        a->balance = strtod(strtok(NULL, ","), NULL);
        strcpy(a->mobile, strtok(NULL, ","));

        char *date = strtok(NULL, ",");
        if (date)
        {
            sscanf(date, "%d-%d", &a->date_opened.month, &a->date_opened.year);
        }
        else
        {
            a->date_opened.month = 1;
            a->date_opened.year = 2024;
        }
        char *status = strtok(NULL, ",");
        if (status)
        {
            strcpy(a->status, status);
        }
        else
        {
            strcpy(a->status, "active");
        }

        num_of_accounts++;
    }
    for (int i = 0; i < num_of_accounts; i++)
    {
        temp[i] = acc[i];
    }
    fclose(f);
    return num_of_accounts;
}
void LOAD_REPORTS()
{
    int i, j, num_of_transactions = 0;
    char line[max_lineLength];

    for (i = 0; i < num_of_accounts; i++)
    {
        FILE *f = fopen(acc[i].number, "r");
        if (f == NULL)
        {
            FILE *new_file = fopen(acc[i].number, "w");
            if (new_file != NULL)
            {
                fprintf(new_file, "=== Transaction History for %s ===\n", acc[i].number);
                fclose(new_file);
            }
        }
        else
        {
            fclose(f);
        }
    }
    for (i = 0; i < num_of_accounts; i++)
    {
        num_of_transactions = 0;
        FILE *f = fopen(acc[i].number, "r");
        if (f != NULL)
        {
            for (j = 0; j < max_lineLength; j++)
            {
                while (fgets(line, sizeof(line), f) != NULL && num_of_transactions < 100)
                {
                    line[strcspn(line, "\n")] = '\0';
                    if (!line[0])
                        continue;
                    strcpy(acc[i].transactions[j], line);
                    num_of_transactions++;
                }
            }
            fclose(f);
        }
        acc[i].num_of_transactions = num_of_transactions;
    }
}
void REVOKE_REPORTS()
{
    int i, j;
    for (i = 0; i < num_of_accounts; i++)
    {
        FILE *f = fopen(acc[i].number, "w");
        if (f != NULL)
        {
            for (j = 0; j < acc[i].num_of_transactions; j++)
            {
                fprintf(f, "%s\n", acc[i].transactions[j]);
            }
            fclose(f);
        }
        else
        {
            printf("Error: Couldn't open file");
            exit(1);
        }
    }
    for (; i < 100; i++)
    {
        if (acc[i].number[0] != '\0')
        {
            if (remove(acc[i].number) == 0)
            {
                acc[i].number[0] = '\0';
            }
        }
    }
}
void QUERY_SEARCH()
{
    char search[100];
    do
    {
        printf("Enter account number to search for account (-1 to return to menu): ");
        fgets(search, sizeof(search), stdin);
        search[strcspn(search, "\n")] = '\0';

        if (!validateOverflow(search))
            continue;

        if (!strcmp(search, "-1"))
        {
            returnToMenu();
            return;
        }

        if (!validateACC_NUM(search))
        {
            continue;
        }
        break;
    } while (1);

    int i;

    for (i = 0; i < num_of_accounts; i++)
    {
        if (!strcmp(search, acc[i].number))
        {
            printf("==================================================\n");
            printf("Account Number: %s\n", acc[i].number);
            printf("Name: %s\n", acc[i].name);
            printf("E-mail: %s\n", acc[i].email);
            printf("Balance: $%.2f \n", acc[i].balance);
            printf("Mobile: %s\n", acc[i].mobile);
            printf("Date opened: %d-%d\n", acc[i].date_opened.month, acc[i].date_opened.year);
            printf("Status: %s\n", acc[i].status);
            printf("\n");
            printf("==================================================\n");
            GO_TO_MENU();
            return;
        }
    }
    printf("Account number %s not found.\n", search);
    QUERY_SEARCH();
}
void ADVANCED_SEARCH()
{
    char keyword[100];
    printf("\n=== ADVANCED SEARCH ===\n");
    do
    {
        printf("Enter keyword to search for account (-1 to return to menu): ");
        fgets(keyword, sizeof(keyword), stdin);
        keyword[strcspn(keyword, "\n")] = '\0';

        if (!validateOverflow(keyword))
        {
            continue;
        }
        if (!strcmp(keyword, "-1"))
        {
            returnToMenu();
        }

        if (!validateKeyword(keyword))
        {
            continue;
        }
        break;
    } while (1);
    int accFound = 0;
    int i, j;

    printf("\nSearch results for '%s':\n", keyword);
    printf("========================================\n");

    for (i = 0; i < num_of_accounts; i++)
    {
        char s1[50];
        strcpy(s1, acc[i].name);
        char s2[50];
        strcpy(s2, keyword);
        for (j = 0; j < 50; j++)
        {
            s1[j] = tolower(s1[j]);
            s2[j] = tolower(s2[j]);
        }

        if (strstr(s1, s2) != NULL)
        {
            accFound++;

            printf("\nAccount Number: %s\n", acc[i].number);
            printf("Name: %s\n", acc[i].name);
            printf("E-mail: %s\n", acc[i].email);
            printf("Balance: $%.2lf\n", acc[i].balance);
            printf("Mobile: %s\n", acc[i].mobile);
            printf("Date opened: %d-%d\n", acc[i].date_opened.month, acc[i].date_opened.year);
            printf("Status: %s\n", acc[i].status);
        }
    }
    if (!accFound)
    {
        printf("\nNo accounts found containing the keyword: %s\n", keyword);
    }
    else
    {
        printf("\n%d account(s) found.\n", accFound);
    }
    GO_TO_MENU();
}
int checkNUM(char *number)
{
    int i;
    for (i = 0; i < num_of_accounts; i++)
    {
        if (!strcmp(acc[i].number, number))
        {
            return 0;
        }
    }
    return 1;
}
void ADD()
{
    if (num_of_accounts >= 100)
    {
        printf("Error: Maximum account limit reached, can not add an account to the system.\n");
        returnToMenu();
        return;
    }
    Account new;
    printf("\n_-=========== ADD NEW ACCOUNT ===========-_\n");
    char input[100];
    // ACCOUNT NUMBER
    do
    {
        printf("Enter account number (10 digits) (-1 to return to menu): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        if (!strcmp(input, "-1"))
        {
            returnToMenu();
        }
        if (!validateACC_NUM(input))
        {
            continue;
        }
        if (!checkNUM(input))
        {
            printf("Error: Account number %s already exists.", input);
            continue;
        }
        strcpy(new.number, input);
        break;
    } while (1);
    // NAME
    do
    {
        printf("Enter account name (-1 to return to menu): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        if (!strcmp(input, "-1"))
        {
            returnToMenu();
        }
        if (!validateName(input))
        {
            continue;
        }
        strcpy(new.name, input);
        break;
    } while (1);
    // EMAIL
    do
    {
        printf("Enter E-mail (-1 to return to menu): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        if (!strcmp(input, "-1"))
        {
            returnToMenu();
        }
        if (!validateEmail(input))
        {
            continue;
        }
        strcpy(new.email, input);
        break;
    } while (1);
    // BALANCE
    do
    {
        printf("Enter initial account balance($) (-1 to return to menu): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        if (!strcmp(input, "-1"))
        {
            returnToMenu();
        }
        if (!validateBalance(input))
        {
            continue;
        }
        new.balance = atof(input);
        break;
    } while (1);
    // MOBILE NUMBER
    do
    {
        printf("Enter mobile number (-1 to return to menu): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        if (!strcmp(input, "-1"))
        {
            returnToMenu();
        }
        if (!validateMobile(input))
        {
            continue;
        }
        strcpy(new.mobile, input);
        break;
    } while (1);
    // DEFAULTS
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    new.date_opened.month = tm_info->tm_mon + 1;
    new.date_opened.year = tm_info->tm_year + 1900;
    strcpy(new.status, " active");

    printf("\n========= ACCOUNT SUMMARY =========\n");
    printf("Account Number: %10s\n", new.number);
    printf("Name: %s\n", new.name);
    printf("E-mail: %s\n", new.email);
    printf("Balance: $%.2f\n", new.balance);
    printf("Mobile: %s\n", new.mobile);
    printf("Date opened: %2d-%4d\n", new.date_opened.month, new.date_opened.year);
    printf("\n");

    do
    {
        printf("Create this account? [Yes/No]: ");
        int x = validateYesNo();
        if (x == 1)
        {
            break;
        }
        else if (!x)
        {
            returnToMenu();
            return;
        }
        else
            continue;
    } while (1);
    acc[num_of_accounts] = new;
    temp[num_of_accounts] = new;
    num_of_accounts++;
    change = 1;
    printf("\nAccount created successfully!\n");
    FILE *f = fopen(acc[num_of_accounts - 1].number, "w");
    if (f != NULL)
    {
        fprintf(f, "=== Transaction History for %s ===\n", acc[num_of_accounts - 1].number);
    }
    fclose(f);
    GO_TO_MENU();
}
void DELETE()
{
    char account_number[100];
    int i, n, delete_index = -1;

    do
    {
        printf("Enter the account number to delete (-1 to go back to menu): ");
        fgets(account_number, sizeof(account_number), stdin);
        account_number[strcspn(account_number, "\n")] = '\0';
        if (!validateOverflow(account_number))
        {
            continue;
        }

        if (!strcmp(account_number, "-1"))
        {
            returnToMenu();
            return;
        }

        if (!validateACC_NUM(account_number))
        {
            continue;
        }

        delete_index = -1;
        for (i = 0; i < num_of_accounts; i++)
        {
            if (!strcmp(account_number, acc[i].number))
            {
                delete_index = i;
                break;
            }
        }

        if (delete_index == -1)
        {
            printf("Account was not found, process terminated.\n");
            GO_TO_MENU();
            return;
        }

        if (acc[delete_index].balance != 0.0)
        {
            printf("Account balance is not zero, process terminated.\n");
            GO_TO_MENU();
            return;
        }

        do
        {
            printf("Are you sure you want to delete account %s? [Yes/No]: ", account_number);
            n = validateYesNo();
            if (n == 1)
            {
                for (i = delete_index; i < num_of_accounts - 1; i++)
                {
                    acc[i] = acc[i + 1];
                }
                num_of_accounts--;
                change = 1;
                printf("Account deletion is successful!\n");
                remove(account_number);
                GO_TO_MENU();
                return;
            }
            else if (n == 0)
            {
                printf("Process termintated.\n");
                returnToMenu();
                return;
            }
            else
                continue;
        } while (1);
    } while (1);
}
void MODIFY()
{
    char input[100];
    do
    {
        printf("Enter the account number (-1 to go back to menu):");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        if (!strcmp(input, "-1"))
        {
            returnToMenu();
            return;
        }
        if (!validateACC_NUM(input))
        {
            continue;
        }
        break;
    } while (1);

    int i, check = 0, n;
    for (i = 0; i < num_of_accounts; i++)
    {
        if (!strcmp(input, acc[i].number))
        {
            check = 1;
            break;
        }
    }

    if (!check)
    {
        printf("Account was not found.\n");
        MODIFY();
        return;
    }
    printf("Current name: %s\nCurrent mobile number: %s\nCurrent email address: %s\n", acc[i].name, acc[i].mobile, acc[i].email);
    do
    {
        printf("Enter the new name (Enter -1 to keep original): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        if (!strcmp(input, "-1"))
            break;
        if (!validateName(input))
        {
            continue;
        }
        strcpy(acc[i].name, input);
        break;
    } while (1);
    do
    {
        printf("Enter the new mobile number (Enter -1 to keep original): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        if (!strcmp(input, "-1"))
            break;
        if (!validateMobile(input))
        {
            continue;
        }
        strcpy(acc[i].mobile, input);
        break;
    } while (1);
    do
    {
        printf("Enter the new email address (Enter -1 to keep original): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        if (!strcmp(input, "-1"))
            break;
        if (!validateEmail(input))
        {
            continue;
        }
        strcpy(acc[i].email, input);
        break;
    } while (1);
    change = 1;
    printf("Account modification was successful!\n");
    GO_TO_MENU();
}
void CHANGE_STATUS()
{
    int i, x, found = 0;
    char search[100];
    while (1)
    {
        do
        {
            printf("Enter the account number (-1 to return to menu): ");
            fgets(search, sizeof(search), stdin);
            search[strcspn(search, "\n")] = '\0';
            if (!validateOverflow(search))
            {
                continue;
            }
            if (atoi(search) == -1)
            {
                returnToMenu();
                return;
            }
            if (!validateACC_NUM(search))
            {
                continue;
            }
            break;
        } while (1);

        for (i = 0; i < num_of_accounts; i++)
        {
            if (!strcmp(search, acc[i].number))
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            printf("Account number %s not found.\n", search);
            continue;
        }
        break;
    }
    while (1)
    {
        printf("The account status is%s, do you want to change it [Yes/No](-1 to go to menu):", acc[i].status);
        x = validateYesNo();
        if (x == 1)
        {
            const char *newStatus = (!strcmp(acc[i].status, " active")) ? " inactive" : " active";
            strcpy(acc[i].status, newStatus);
            change = 1;
            printf("Account status was changed to%s.\n", acc[i].status);
            GO_TO_MENU();
            return;
        }
        else if (x == 0)
        {
            printf("No changes were made, account status is%s.\n", acc[i].status);
            GO_TO_MENU();
            return;
        }
        else
            continue;
        break;
    }
}
int canWithdraw(Account *acc, double amountToWithdraw)
{
    time_t now = time(NULL);
    struct tm currentDay = *localtime(&now);
    struct tm lastDay = *localtime(&acc->lastWithdrawalDate);
    if (currentDay.tm_mday != lastDay.tm_mday || currentDay.tm_mon != lastDay.tm_mon || currentDay.tm_year != lastDay.tm_year)
    {
        acc->dailyWithdrawalTotal = 0.0;
    }
    if (acc->dailyWithdrawalTotal + amountToWithdraw > dailyLimit)
    {
        return 0;
    }
    return 1;
}
void WITHDRAW()
{
    int i, n, found = 0, inactive = 0;
    char search[100], amountS[100];
    double amount;
    while (1)
    {
        do
        {
            printf("Enter the account number (-1 to return to menu): ");
            fgets(search, sizeof(search), stdin);
            search[strcspn(search, "\n")] = '\0';
            if (!validateOverflow(search))
            {
                continue;
            }
            if (atoi(search) == -1)
            {
                returnToMenu();
                return;
            }
            if (!validateACC_NUM)
            {
                continue;
            }
            break;
        } while (1);
        for (i = 0; i < num_of_accounts; i++)
        {
            if (!strcmp(search, acc[i].number))
            {
                found = 1;
                break;
            }
        }
        if (!(strcmp(acc[i].status, " inactive")))
        {
            printf("Transaction Failed: Account is inactive.\n");
            continue;
        }
        if (!found)
        {
            printf("Account number %s not found.\n", search);
            continue;
        }
        while (1)
        {
            amount = acc[i].balance;
            do
            {
                if (amount > acc[i].balance)
                    printf("Transaction Failed: Insufficient funds available to complete this request.\n");
                printf("Enter the amount needed to be withdrew (Maximum Limit: $10,000) (-1 to return to menu): ");
                fgets(amountS, sizeof(amountS), stdin);
                amountS[strcspn(amountS, "\n")] = '\0';
                if (!validateOverflow(amountS))
                {
                    continue;
                }
                if (atoi(amountS) == -1)
                {
                    returnToMenu();
                    return;
                }
                validateTransaction(amountS);
                amount = strtod(amountS, NULL);
            } while (amount > 10000 || amount <= 0 || amount > acc[i].balance);
            if (!canWithdraw(&acc[i], amount))
            {
                printf("Transaction Failed: Daily limit of $%.2f exceeded.\n", dailyLimit);
                continue;
            }
            break;
        }
        do
        {
            printf("Are you sure you want to withdraw $%s from your account? [Yes/No]: ", amountS);
            n = validateYesNo();
        } while (n == -1);
        if (n)
        {
            acc[i].balance -= amount;
            acc[i].dailyWithdrawalTotal += amount;
            acc[i].lastWithdrawalDate = time(NULL);
            change = 1;
            FILE *f = fopen(search, "a");
            if (f)
            {
                fseek(f, 33, SEEK_SET);
                fprintf(f, "\nWithdrew: $%s", amountS);
                fclose(f);
            }
            printf("Withdrawing $%s from your account...\n", amountS);
            printf("Withdrawal processed successfully!\n");
        }
        else
        {
            printf("Withdrawal has been cancelled.\n");
        }
        GO_TO_MENU();
        return;
    }
}
void DEPOSIT()
{
    int i, n, found = 0, inactive = 0;
    char search[100], amount[100];
    while (1)
    {
        do
        {
            printf("Enter the account number (-1 to return to menu): ");
            fgets(search, sizeof(search), stdin);
            search[strcspn(search, "\n")] = '\0';
            if (!validateOverflow(search))
            {
                continue;
            }
            if (atoi(search) == -1)
            {
                returnToMenu();
                return;
            }
            if (!validateACC_NUM(search))
            {
                continue;
            }
            break;
        } while (1);
        for (i = 0; i < num_of_accounts; i++)
        {
            if (!strcmp(search, acc[i].number))
            {
                found = 1;
                break;
            }
        }
        if (!(strcmp(acc[i].status, " inactive")))
        {
            printf("Transaction Failed: Account is inactive.\n");
            continue;
        }
        if (!found)
        {
            printf("Account number %s not found.\n", search);
            continue;
        }
        do
        {
            printf("Enter the amount needed to be deposited (Maximum Limit: $10,000) (-1 to return to menu): ");
            fgets(amount, sizeof(amount), stdin);
            amount[strcspn(amount, "\n")] = '\0';
            if (!validateOverflow(amount))
            {
                continue;
            }
            if (atoi(amount) == -1)
            {
                returnToMenu();
                return;
            }
            validateTransaction(amount);
        } while (strtod(amount, NULL) > 10000 || strtod(amount, NULL) <= 0);
        do
        {
            printf("Are you sure you want to deposit $%s into your account? [Yes/No]: ", amount);
            n = validateYesNo();
        } while (n == -1);
        if (n)
        {
            acc[i].balance += strtod(amount, NULL);
            change = 1;
            FILE *f = fopen(search, "a");
            if (f)
            {
                fseek(f, 33, SEEK_SET);
                fprintf(f, "\nDeposited: $%s", amount);
                fclose(f);
            }
            printf("Depositing $%s into your account...\n", amount);
            printf("Deposit processed successfully!\n");
        }
        else
        {
            printf("Deposit has been cancelled.\n");
        }
        GO_TO_MENU();
        return;
    }
}
void TRANSFER()
{
    int i, j, n, foundSender = 0, foundReceiver = 0;
    double amount;
    char sender[100], receiver[100], amountS[100];
    while (1)
    {
        do
        {
            printf("Enter the sender account number (-1 to return to menu): ");
            fgets(sender, sizeof(sender), stdin);
            sender[strcspn(sender, "\n")] = '\0';
            if (!validateOverflow(sender))
            {
                continue;
            }
            if (atoi(sender) == -1)
            {
                returnToMenu();
                return;
            }
            if (!validateACC_NUM(sender))
            {
                continue;
            }
            break;
        } while (1);
        for (i = 0; i < num_of_accounts; i++)
        {
            if (!strcmp(sender, acc[i].number))
            {
                foundSender = 1;
                break;
            }
        }
        if (!(strcmp(acc[i].status, " inactive")))
        {
            printf("Transaction Failed: Account is inactive.\n");
            continue;
        }
        if (!foundSender)
        {
            printf("Account number %s not found.\n", sender);
            continue;
        }
        break;
    }
    while (1)
    {
        do
        {
            printf("Enter the receiver account number (-1 to return to menu): ");
            fgets(receiver, sizeof(receiver), stdin);
            receiver[strcspn(receiver, "\n")] = '\0';
            if (!validateOverflow(receiver))
            {
                continue;
            }
            if (atoi(receiver) == -1)
            {
                returnToMenu();
                return;
            }
            if (!validateACC_NUM(receiver))
            {
                continue;
            }
            break;
        } while (1);
        for (j = 0; j < num_of_accounts; j++)
        {
            if (!strcmp(receiver, acc[j].number))
            {
                foundReceiver = 1;
                break;
            }
        }
        if (!(strcmp(acc[j].status, " inactive")))
        {
            printf("Transaction Failed: Account is inactive.\n");
            continue;
        }
        if (!foundReceiver)
        {
            printf("Account number %s not found.\n", receiver);
            continue;
        }
        break;
    }
    amount = acc[i].balance;
    do
    {
        if (amount > acc[i].balance)
            printf("Transaction Failed: Insufficient funds available to complete this request.\n");
        printf("Enter the amount needed to be transferred (-1 to return to menu): ");
        fgets(amountS, sizeof(amountS), stdin);
        amountS[strcspn(amountS, "\n")] = '\0';
        if (!validateOverflow(amountS))
        {
            continue;
        }
        if (atoi(amountS) == -1)
        {
            returnToMenu();
            return;
        }
        validateTransaction(amountS);
        amount = strtod(amountS, NULL);
    } while (amount <= 0 || amount > acc[i].balance);
    do
    {
        printf("Are you sure you want to transfer $%s to the receiver (Account Number: %s)? [Yes/No]: ", amountS, receiver);
        n = validateYesNo();
    } while (n == -1);
    if (n)
    {
        acc[i].balance -= amount;
        acc[j].balance += amount;
        change = 1;
        FILE *f = fopen(sender, "a");
        if (f)
        {
            fseek(f, 33, SEEK_SET);
            fprintf(f, "\nTransferred to %s: -$%s", receiver, amountS);
            fclose(f);
        }
        f = fopen(receiver, "a");
        if (f)
        {
            fseek(f, 33, SEEK_SET);
            fprintf(f, "\nTransferred from %s: +$%s", sender, amountS);
            fclose(f);
        }
        printf("Transferring $%s to the receiver (Account Number: %s)...\n", amountS, receiver);
        printf("Transaction has been sucessfully processsed!\n");
    }
    else
    {
        printf("Transaction has been cancelled.\n");
    }
    GO_TO_MENU();
    return;
}
void REPORT()
{
    int i, found = 0;
    char search[100], output[100];
    while (1)
    {
        while (1)
        {
            printf("Enter the account number (-1 to return to menu): ");
            fgets(search, sizeof(search), stdin);
            search[strcspn(search, "\n")] = '\0';
            if (!validateOverflow(search))
            {
                continue;
            }
            if (atoi(search) == -1)
            {
                returnToMenu();
                return;
            }
            if (!validateACC_NUM)
            {
                continue;
            }
            break;
        }
        for (i = 0; i < num_of_accounts; i++)
        {
            if (!strcmp(search, acc[i].number))
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            printf("Account number %s not found.\n", search);
            continue;
        }
        break;
    }
    FILE *f = fopen(search, "r");
    if (f != NULL)
    {
        i = 0;
        fseek(f, 33, SEEK_SET);
        while (fgets(output, sizeof(output), f) != NULL && i++ < 5)
            ;
        printf("\n========= LAST %d TRANSACTIONS SUMMARY =========", i);
        i = 0;
        fseek(f, 33, SEEK_SET);
        while (fgets(output, sizeof(output), f) != NULL && i++ < 5)
            printf("\n%s", output);
        fclose(f);
        GO_TO_MENU();
    }
}
void sortByDate()
{
    int i, j;
    Account t;
    char input[100];
    int n = -1;

    do
    {
        printf("Do you want to sort by Ascending or Descending date ? Please Enter (Ascending/Descending) to choose: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        for (int k = 0; k < strlen(input); k++)
        {
            input[k] = tolower(input[k]);
        }
        if (strcmp(input, "ascending") == 0)
        {
            n = 1;
        }
        else if (strcmp(input, "descending") == 0)
        {
            n = 0;
        }
        else
        {
            printf("Please enter either 'Ascending' or 'Descending'.\n");
        }
    } while (n == -1);

    if (n == 1)
    {
        for (i = 0; i < num_of_accounts - 1; i++)
        {
            for (j = 0; j < num_of_accounts - i - 1; j++)
            {
                if (temp[j].date_opened.year > temp[j + 1].date_opened.year)
                {
                    t = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = t;
                }
                else if (temp[j].date_opened.year == temp[j + 1].date_opened.year)
                {
                    if (temp[j].date_opened.month > temp[j + 1].date_opened.month)
                    {
                        t = temp[j];
                        temp[j] = temp[j + 1];
                        temp[j + 1] = t;
                    }
                }
            }
        }
    }
    else if (n == 0)
    {
        for (i = 0; i < num_of_accounts - 1; i++)
        {
            for (j = 0; j < num_of_accounts - i - 1; j++)
            {
                if (temp[j].date_opened.year < temp[j + 1].date_opened.year)
                {
                    t = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = t;
                }
                else if (temp[j].date_opened.year == temp[j + 1].date_opened.year)
                {
                    if (temp[j].date_opened.month > temp[j + 1].date_opened.month)
                    {
                        t = temp[j];
                        temp[j] = temp[j + 1];
                        temp[j + 1] = t;
                    }
                }
            }
        }
    }
    printf("\nAccounts sorted by date opened (%s):-\n", n ? "Oldest to Newest" : "Newest to Oldest");
    printf("\n%-5s %-15s %-18s %-25s %-12s %-12s %-12s %-10s\n",
           "No.", "Account No.", "Name", "Email", "Mobile", "Balance", "Date", "Status");
    printf("%-5s %-15s %-18s %-25s %-12s %-12s %-12s %-10s\n",
           "----", "-----------", "------------------", "-------------------------",
           "------------", "------------", "------------", "----------");
    for (i = 0; i < num_of_accounts; i++)
    {
        printf("%-5d %-15s %-18s %-25s %-12s %-12.2f %02d/%d     %-10s\n",
               i + 1,
               temp[i].number,
               temp[i].name,
               temp[i].email,
               temp[i].mobile,
               temp[i].balance,
               temp[i].date_opened.month,
               temp[i].date_opened.year,
               temp[i].status);
    }
    GO_TO_MENU();
    return;
}
void sortByName()
{
    int i, j;
    Account t;
    char input[100];
    int n = -1;

    do
    {
        printf("Do you want to sort alphabetically Ascending or Descending? Please Enter (Ascending/Descending) to choose: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        for (int k = 0; k < strlen(input); k++)
        {
            input[k] = tolower(input[k]);
        }
        if (strcmp(input, "ascending") == 0)
        {
            n = 1;
        }
        else if (strcmp(input, "descending") == 0)
        {
            n = 0;
        }
        else
        {
            printf("Please enter either 'Ascending' or 'Descending'.\n");
        }
    } while (n == -1);

    if (n == 1)
    {
        for (i = 0; i < num_of_accounts - 1; i++)
        {
            for (j = 0; j < num_of_accounts - i - 1; j++)
            {
                if (strcmp(temp[j].name, temp[j + 1].name) > 0)
                {
                    t = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = t;
                }
            }
        }
    }
    else if (n == 0)
    {
        for (i = 0; i < num_of_accounts - 1; i++)
        {
            for (j = 0; j < num_of_accounts - i - 1; j++)
            {
                if (strcmp(temp[j].name, temp[j + 1].name) < 0)
                {
                    t = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = t;
                }
            }
        }
    }

    printf("\nAccounts sorted by name (Alphabetically %s):-\n", n ? "Ascending" : "Descending");
    printf("\n%-5s %-15s %-20s %-30s %-12s %-15s %-12s %-10s\n",
           "No.", "Account No.", "Name", "Email", "Mobile", "Balance", "Date Created", "Status");
    printf("%-5s %-15s %-20s %-30s %-12s %-15s %-12s %-10s\n",
           "----", "-----------", "--------------------", "------------------------------",
           "------------", "---------------", "------------", "----------");

    for (i = 0; i < num_of_accounts; i++)
    {
        printf("%-5d %-15s %-20s %-30s %-12s %-15.2f %02d/%d     %-10s\n",
               i + 1,
               temp[i].number,
               temp[i].name,
               temp[i].email,
               temp[i].mobile,
               temp[i].balance,
               temp[i].date_opened.month,
               temp[i].date_opened.year,
               temp[i].status);
    }

    GO_TO_MENU();
    return;
}
void sortByBalance()
{
    int i, j;
    Account t;
    char input[100];
    int n = -1;

    do
    {
        printf("Do you want to sort by Ascending or Descending balance ? Please Enter (Ascending/Descending) to choose: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        for (int i = 0; i < strlen(input); i++)
        {
            input[i] = tolower(input[i]);
        }
        if (strcmp(input, "ascending") == 0)
        {
            n = 1;
        }
        else if (strcmp(input, "descending") == 0)
        {
            n = 0;
        }
        else
        {
            printf("Please enter either 'Ascending' or 'Descending'.\n");
        }
    } while (n == -1);

    if (n == 1)
    {

        for (i = 0; i < num_of_accounts - 1; i++)
        {
            for (j = 0; j < num_of_accounts - i - 1; j++)
            {
                if (temp[j].balance > temp[j + 1].balance)
                {
                    t = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = t;
                }
            }
        }
    }
    if (n == 0)
    {
        for (i = 0; i < num_of_accounts - 1; i++)
        {
            for (j = 0; j < num_of_accounts - i - 1; j++)
            {
                if (temp[j].balance < temp[j + 1].balance)
                {
                    t = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = t;
                }
            }
        }
    }
    printf("\nAccounts sorted by balance (%s):-\n", n ? "Ascending" : "Descending");
    printf("\n%-5s %-15s %-20s %-30s %-12s %-15s %-12s %-10s\n",
           "No.", "Account No.", "Name", "Email", "Mobile", "Balance", "Date", "Status");
    printf("%-5s %-15s %-20s %-30s %-12s %-15s %-12s %-10s\n",
           "----", "-----------", "--------------------", "------------------------------",
           "------------", "---------------", "------------", "----------");

    for (i = 0; i < num_of_accounts; i++)
    {
        printf("%-5d %-15s %-20s %-30s %-12s %-15.2f %02d/%d     %-10s\n",
               i + 1,
               temp[i].number,
               temp[i].name,
               temp[i].email,
               temp[i].mobile,
               temp[i].balance,
               temp[i].date_opened.month,
               temp[i].date_opened.year,
               temp[i].status);
    }

    GO_TO_MENU();
    return;
}
void sortByStatus()
{
    int i, j;
    Account t;
    char input[100];
    int n = -1;

    do
    {
        printf("Do you want active or inactive accounts on the top of the list? (Active/Inactive): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }
        for (int i = 0; i < strlen(input); i++)
        {
            input[i] = tolower(input[i]);
        }
        if (strcmp(input, "active") == 0)
        {
            n = 1;
        }
        else if (strcmp(input, "inactive") == 0)
        {
            n = 0;
        }
        else
        {
            printf("Please enter either 'Active' or 'Inactive'.\n");
        }
    } while (n == -1);

    if (n == 1)
    {
        for (i = 0; i < num_of_accounts; i++)
        {
            for (j = 0; j < num_of_accounts - 1; j++)
            {
                int active1 = strcmp(temp[j].status, " active") == 0;
                int active2 = strcmp(temp[j + 1].status, " active") == 0;
                if (active2)
                {
                    Account x = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = x;
                }
            }
        }
    }
    else if (n == 0)
    {

        for (i = 0; i < num_of_accounts; i++)
        {
            for (j = 0; j < num_of_accounts - 1; j++)
            {
                int active1 = strcmp(temp[j].status, " active") == 0;
                int active2 = strcmp(temp[j + 1].status, " active") == 0;
                if (!active2)
                {
                    Account x = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = x;
                }
            }
        }
    }
    printf("\nAccounts sorted by status:-\n");
    printf("\n%-5s %-15s %-20s %-30s %-12s %-15s %-12s %-10s\n",
           "No.", "Account No.", "Name", "Email", "Mobile", "Balance", "Date Created", "Status");
    printf("%-5s %-15s %-20s %-30s %-12s %-15s %-12s %-10s\n",
           "----", "-----------", "--------------------", "------------------------------",
           "------------", "---------------", "------------", "----------");
    for (i = 0; i < num_of_accounts; i++)
    {
        printf("%-5d %-15s %-20s %-30s %-12s %-15.2f %02d/%d     %-10s\n",
               i + 1,
               temp[i].number,
               temp[i].name,
               temp[i].email,
               temp[i].mobile,
               temp[i].balance,
               temp[i].date_opened.month,
               temp[i].date_opened.year,
               temp[i].status);
    }
    GO_TO_MENU();
    return;
}
void PRINT_Unsorted()
{
    printf("\nAccounts:-\n");
    printf("\n%-5s %-15s %-20s %-30s %-12s %-15s %-12s %-10s\n",
           "No.", "Account No.", "Name", "Email", "Mobile", "Balance", "Date Created", "Status");
    printf("%-5s %-15s %-20s %-30s %-12s %-15s %-12s %-10s\n",
           "----", "-----------", "--------------------", "------------------------------",
           "------------", "---------------", "------------", "----------");
    for (int i = 0; i < num_of_accounts; i++)
    {
        printf("%-5d %-15s %-20s %-30s %-12s %-15.2f %02d/%d     %-10s\n",
               i + 1,
               acc[i].number,
               acc[i].name,
               acc[i].email,
               acc[i].mobile,
               acc[i].balance,
               acc[i].date_opened.month,
               acc[i].date_opened.year,
               acc[i].status);
    }
    GO_TO_MENU();
    return;
}
void PRINT_SORT()
{
    char input[100];
    do
    {
        printf("Enter a number to sort by ...... :-\n");
        printf("1) Name\n");
        printf("2) Balance\n");
        printf("3) Date opened\n");
        printf("4) Status\n");
        printf("5) Unsorted\n");
        printf("Enter your choice (1-5) (-1 to return to menu): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }

        if (!strcmp(input, "-1"))
        {
            returnToMenu();
            return;
        }
        if (!validateSortChoice(input))
        {
            continue;
        }
        break;
    } while (1);
    switch (atoi(input))
    {
    case 1:
    {
        sortByName();
        break;
    }
    case 2:
    {
        sortByBalance();
        break;
    }
    case 3:
    {
        sortByDate();
        break;
    }
    case 4:
    {
        sortByStatus();
        break;
    }
    case 5:
    {
        PRINT_Unsorted();
        break;
    }
    default:
        break;
    }
    return;
}
int SAVE()
{
    while (1)
    {
        int i, x;
        x = validateYesNo();
        if (x == 1)
        {
            FILE *f = fopen("Accounts.txt", "w");
            if (f == NULL)
            {
                printf("Error: couldn't create file");
                exit(1);
            }
            printf("Saving changes...\n");
            change = 0;
            for (i = 0; i < num_of_accounts; i++)
            {
                fprintf(f, "%s,%s,%s,%lf,", acc[i].number, acc[i].name, acc[i].email, acc[i].balance);
                fprintf(f, "%s,%d-%d,%s\n", acc[i].mobile, acc[i].date_opened.month, acc[i].date_opened.year, acc[i].status);
            }
            printf("Changes saved successfully!\n");
            fclose(f);
            return 0;
        }
        else if (x == 0)
        {
            LOAD();
            REVOKE_REPORTS();
            printf("No changes were saved.\n");
            return 0;
        }
        else
            return -1;
    }
}
void QUIT()
{
    while (1)
    {
        if (change)
        {
            printf("You have unsaved changes! Would you like to save before exiting? [Yes/No]: ");
            if (SAVE() == -1)
                continue;
        }
        break;
    }
    printf("Exiting the program.....");
    exit(0);
}
void DELETE_MULTIPLE()
{
    char input[100];
    int delete_count = 0;
    int *account_indices = NULL;
    printf("Choose method of deleting multiple accounts:-\n");
    printf("1. By Date (Delete all accounts created on a specific date whose balance is zero)\n");
    printf("2. By Status (Delete all accounts that are inactive and older than 3 months whose balance is zero)\n");
    do
    {
        printf("Enter your choice (1-2) (-1 to return to menu): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (!validateOverflow(input))
        {
            continue;
        }

        if (!strcmp(input, "-1"))
        {
            returnToMenu();
            return;
        }
        if (!validateDeleteChoice(input))
        {
            continue;
        }
        break;
    } while (1);

    int n = atoi(input);
    if (n == 1)
    {
        int month, year;
        do
        {
            printf("Enter month (1-12) (-1 to return to menu): ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            if (!validateOverflow(input))
            {
                continue;
            }

            if (!strcmp(input, "-1"))
            {
                returnToMenu();
                return;
            }
            if (!validateMonthChoice(input))
            {
                continue;
            }
            break;
        } while (1);
        month = atoi(input);
        do
        {
            printf("Enter year (-1 to return to menu): ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            if (!validateOverflow(input))
            {
                continue;
            }

            if (!strcmp(input, "-1"))
            {
                returnToMenu();
                return;
            }
            if (!validateYearChoice(input))
            {
                continue;
            }
            break;
        } while (1);
        year = atoi(input);
        for (int i = 0; i < num_of_accounts; i++)
        {
            if (acc[i].date_opened.month == month && acc[i].date_opened.year == year && acc[i].balance == 0.0)
            {
                delete_count++;
            }
        }
        if (delete_count == 0)
        {
            printf("No accounts found.\n");
            returnToMenu();
            return;
        }
        account_indices = malloc(delete_count * sizeof(int));
        int delete_index = 0;
        for (int i = 0; i < num_of_accounts; i++)
        {
            if (acc[i].date_opened.month == month && acc[i].date_opened.year == year && acc[i].balance == 0.0)
            {
                account_indices[delete_index++] = i;
            }
        }
        printf("\n%d account(s) found:-\n", delete_count);
        for (int k = 0; k < delete_count; k++)
        {
            int x = account_indices[k];
            printf("  Account %s: %s", acc[x].number, acc[x].name);
            printf(" (Opened: %02d/%04d",
                   acc[x].date_opened.month,
                   acc[x].date_opened.year);
            printf(", Balance: $%.2f)\n", acc[x].balance);
        }
        do
        {
            printf("Confirm deletion? [Yes/No]: ");
            int y = validateYesNo();

            if (y == 1)
            {
                for (int i = 0; i < delete_count - 1; i++)
                {
                    for (int j = 0; j < delete_count - i - 1; j++)
                    {
                        if (account_indices[j] < account_indices[j + 1])
                        {
                            int temp = account_indices[j];
                            account_indices[j] = account_indices[j + 1];
                            account_indices[j + 1] = temp;
                        }
                    }
                }
                for (int i = 0; i < delete_count; i++)
                {
                    int x = account_indices[i];
                    for (int j = x; j < num_of_accounts - 1; j++)
                    {
                        acc[j] = acc[j + 1];
                    }
                    num_of_accounts--;
                }

                printf("Successfully deleted %d account(s).\n", delete_count);
                GO_TO_MENU();
                return;
            }
            else if (y == 0)
            {
                printf("Process terminated, deletion cancelled.\n");
                returnToMenu();
                return;
            }
            else
                continue;
        } while (1);
        free(account_indices);
    }
    else if (n == 2)
    {
        Date current;
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        current.month = tm_info->tm_mon + 1;
        current.year = tm_info->tm_year + 1900;

        for (int i = 0; i < num_of_accounts; i++)
        {
            int acc_ageMonth = (current.year - acc[i].date_opened.year) * 12;
            acc_ageMonth += (current.month - acc[i].date_opened.month);

            if (acc_ageMonth > 3 && !strcmp(acc[i].status, " inactive") && acc[i].balance == 0.0)
            {
                delete_count++;
            }
        }
        if (delete_count == 0)
        {
            printf("No accounts found that meet all the deletion conditions.\n");
            returnToMenu();
            return;
        }

        account_indices = malloc(delete_count * sizeof(int));
        int delete_index = 0;

        for (int i = 0; i < num_of_accounts; i++)
        {
            int acc_ageMonth = (current.year - acc[i].date_opened.year) * 12;
            acc_ageMonth += (current.month - acc[i].date_opened.month);

            if (acc_ageMonth > 3 && !strcmp(acc[i].status, " inactive") && acc[i].balance == 0.0)
            {
                account_indices[delete_index++] = i;
            }
        }
        printf("\n%d inactive account(s) found with zero balance:-\n", delete_count);
        for (int i = 0; i < delete_count; i++)
        {
            int x = account_indices[i];
            printf("  Account %s: %s", acc[x].number, acc[x].name);
            printf(" (Opened: %02d/%04d",
                   acc[x].date_opened.month,
                   acc[x].date_opened.year);
            printf(", Balance: $%.2f)\n", acc[x].balance);
        }
        do
        {
            printf("Confirm deletion? [Yes/No]: ");
            int y = validateYesNo();

            if (y == 1)
            {
                for (int i = 0; i < delete_count - 1; i++)
                {
                    for (int j = 0; j < delete_count - i - 1; j++)
                    {
                        if (account_indices[j] < account_indices[j + 1])
                        {
                            int temp = account_indices[j];
                            account_indices[j] = account_indices[j + 1];
                            account_indices[j + 1] = temp;
                        }
                    }
                }
                for (int i = 0; i < delete_count; i++)
                {
                    int x = account_indices[i];
                    for (int j = x; j < num_of_accounts - 1; j++)
                    {
                        acc[j] = acc[j + 1];
                    }
                    num_of_accounts--;
                }
                change = 1;
                printf("Successfully deleted %d account(s).\n", delete_count);
                GO_TO_MENU();
                return;
            }
            else if (y == 0)
            {
                printf("Process terminated, deletion cancelled.\n");
                returnToMenu();
                return;
            }
            else
                continue;
        } while (1);
        free(account_indices);
    }
}
