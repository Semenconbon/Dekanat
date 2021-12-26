#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <locale.h>

/*
База данных “Деканат”.
Пополнение базы (новая группа, новый студент, новая дисциплина) .
Редактирование базы (ввод и изменение оценок).
Удаление записей.
Вывод сведений в алфавитном порядке
Вывод сведений в порядке убывания среднего балла.
Вывод списка для начисления стипендии, либо на пересдачу (с указанием экзамена), либо на отчисление
*/
#define M 100
#define N 20
int selection;

//таблицы
struct student
{
	int id;
	int group_id;
	char surname[20];
	char name[20];
};
struct group
{
	int id;
	char name[20];
};
struct discipline
{
	int id;
	char name[20];
};
struct mark
{
	int id;
	int mark;
	int group_id;
	int student_id;
	int discipline_id;
};
struct group_discipline
{
	int id;
	int discipline_id;
	int group_id;
};


struct student students[M];
struct group groups[M];
struct discipline disciplines[M];
struct mark marks[M];
struct group_discipline gd[M];

struct summark{
	int student_id;
	int sum_mark;
};
struct summark sum_marks[M];

int number_of_group = 0;
int number_of_student = 0;
int number_of_discipline = 0;
int number_of_marks = 0;
int number_of_gd = 0;

int number_of_sum_marks = 0;



int Menu()/* Функция Меню */
{
	printf("\n");
	printf(
		"1 : Добавить (студентов, группы, оценки, дисциплины)\n"
		"2 : Вывести (студентов, группы, оценки, дисциплины)\n"
		"3 : Вывести списки на отчисления\n"
		"4 : Вывести списки на начисление стипендии\n"
		"5 : Вывести список на пересдачу\n"
		"6 : Удаление или изменение записей\n"
		"0 : выход\n"
		">");
	do
	{
		scanf("%d", &selection);
	} while (selection < 0 || selection > 6);
	printf("\n");
	return selection;
}
void menu_print(){
	printf("0 : назад\n"
		"1 : Вывести студентов из определенной группы\n"
		"2 : Вывести группы\n"
		"3 : Вывести оценки\n"
		"4 : Вывести дисциплины\n"
		">");
	do{
		scanf("%d", &selection);
	} while (selection < 0 || selection > 4);
	printf("\n");
	switch (selection)
	{
	case 1:
		print_students();
		break;
	case 2:
		print_group();
		break;
	case 3:
		menu_marks();
		break;
	case 4:
		menu_discipline();
		break;
	default:
		break;
	}
}
//Функции вывода
int print_students(){
	int i = 0;
	int id;
	char name_group[N]; int group_id;
	printf("Введите название группы (введите * если хотите вывести всех студентов): ");
	getchar(); gets(name_group);
	if (name_group[0] == '*'){
		for (i = 0; i <= number_of_student; i++){
			printf("%s", students[i].surname);
			printf(" %s", students[i].name);
			printf("\n");
		}
	}
	else{
		while (i <= number_of_group){
			if (memcmp(name_group, groups[i].name, 4) == 0){ // предполагается, что имя группы состоит из 4 байтов
				group_id = groups[i].id;
				break;
			}
			i++;
			if (i == number_of_group) { printf("Такой группы нет :(\n"); return 0; }
		}
		for (i = 0; i <= number_of_student; i++){
			if (group_id == students[i].group_id){
				printf("%s", students[i].surname);
				printf(" %s", students[i].name);
				printf("\n");
			}
		}

		i++;
	}
	return 0;
}
int print_group(){
	for (int i = 0; i <= number_of_group; i++){
		printf("%s\n", groups[i].name);
	}
	return 0;
}
int menu_discipline(){
	printf("0 : назад\n"
		"1 : Вывести дисциплины группы\n"
		"2 : Вывести все дисциплины\n"
		">");
	do{
		scanf("%d", &selection);
	} while (selection < 0 || selection > 2);
	printf("\n");
	switch (selection)
	{
	case 1:
		print_discipline_group();
		break;
	case 2:
		print_disciplines();
		break;
	default:
		break;
	}
	return 0;
}
int print_disciplines(){
	for (int i = 0; i <= number_of_discipline; i++){
		printf("%s\n", disciplines[i].name);
	}
	return 0;
}
int print_discipline_group(){
	char name_group[5]; int group_id = 0;
	printf("Введите название группы: ");
	getchar(); gets(name_group);
	for (int i = 0; i <= number_of_group; i++){
		if (memcmp(name_group,
			groups[i].name, 4) == 0){
			group_id = groups[i].id;
			break;
		}
	}
	if (group_id == 0) { printf("\nУверенны, что есть такая группа?\n"); }
	else{
		for (int i = 0; i <= number_of_gd; i++){
			if (gd[i].group_id == group_id){
				int discipline_id = gd[i].discipline_id;
				printf("%s ", disciplines[discipline_id].name);
			}
		}

	}
	return 0;
}
//Здесь мы разабираем оценки
int menu_marks(){
	printf("0 : назад\n"
		"1 : Вывести оценки студента\n"
		"2 : Вывести оценки группы\n"
		"3 : Вывести оценки по дисциплине...\n"
		">");
	do{
		scanf("%d", &selection);
	} while (selection < 0 || selection > 3);
	printf("\n");
	switch (selection)
	{
	case 1:
		print_marks_student();
		break;
	case 2:
		print_marks_groups();
		break;
	default:
		break;
	}
	return 0;
}
int print_marks_student(){
	char name[N];
	char surname[N];
	int student_id = -1;
	printf("Фамилия студента: ");
	getchar(); gets(surname);
	int n_surname = strlen(surname, N);
	printf("Имя студента: ");
	gets(name);
	int n_name = strlen(name, N);
	for (int i = 0; i <= number_of_student; i++){ // поиск нужной фамилии
		if (memcmp(surname, students[i].surname, n_surname) == 0){
			if (memcmp(name, students[i].name, n_name) == 0){
				student_id = students[i].id;
				break;
			}
		}
	}
	if (student_id == -1){ printf("Уверены, что такой студент есть?\n"); return 0; }
	for (int i = 0; i < number_of_discipline; i++){
		for (int j = 0; j < number_of_marks; j++){
			if (student_id == marks[j].student_id && marks[j].discipline_id == disciplines[i].id){
				printf("%s %d\n", disciplines[i].name, marks[j].mark);
			}
		}
	}
	return 0;
}
int print_marks_groups(){
	char name_group[N]; int group_id; int numbers[M];
	printf("Введите название группы (введите * если хотите вывести все группы): ");
	getchar(); gets(name_group);
	if (name_group[0] == '*'){
		for (int i = 0; i < number_of_discipline; i++){ // дисциплина
			printf("\n%s\n", disciplines[i].name);
			for (int j = 0; j < number_of_student; j++){ // студент
				printf("\t%s %s ", students[j].surname, students[j].name);
				for (int k = 0; k < number_of_marks; k++){ // оценки
					if (students[j].id == marks[k].student_id && disciplines[i].id == marks[k].discipline_id)
						printf("%d ", marks[k].mark);
				}
				printf("\n");
			}
		}
	}
	else{
		for (int i = 0; i <= number_of_group; i++){
			if (memcmp(name_group, groups[i].name, 4) == 0){
				group_id = groups[i].id;
			}
		}
		for (int j = 0; j < number_of_discipline; j++){// идём по дисциплинам
			printf("\t%s\n", disciplines[j].name);
			for (int k = 0; k < number_of_student; k++){ // идём по студентам
				if (students[k].group_id == group_id){
					printf("\t\t%s %s ", students[k].surname, students[k].name);
					for (int l = 0; l < number_of_marks; l++){// идём по оценкам
						if (marks[l].group_id == group_id &&
							marks[l].student_id == students[k].id &&
							marks[l].discipline_id == disciplines[j].id){
							printf("%d", marks[l].mark);
						}
					}
					printf("\n");
				}
			}
		}
	}
	return 0;
}
//int print_marks_diminution(){
// int i = 0; int j = 0; int max = 0;
// while (i <= number_of_sum_marks){
// while (j <= number_of_sum_marks){
// if (sum_marks[j].sum_mark > max){
// max = sum_marks[i].sum_mark;
// break;
// }
// }
// i++;
// }
// return 0;
//}
int print_list_deduction(){
	int counter;
	printf("Список студентов на отчисление: ");
	for (int i = 0; i <= number_of_student; i++){
		counter = 0;
		for (int j = 0; j <= number_of_marks; j++){
			if (marks[j].student_id == students[i].id && marks[j].mark <= 2){
				counter++;
			}
			if (counter >= 3){
				printf("\n\t%s %s", students[i].surname, students[i].name);
				break;
			}
		}
	}
	return 0;
}
int print_list_scholarship(){
	int counter;
	printf("Студентs на начисление стипендии: ");
	for (int i = 0; i < number_of_student; i++){
		counter = 0;
		int number_group_in_disc =
			how_many_disciplines_in_group(students[i].group_id);
		for (int j = 0; j < number_group_in_disc; j++){
			if (marks[j].student_id == students[i].id && marks[j].mark > 3 && j == number_group_in_disc - 1){
				printf("\n\t%s %s", students[i].surname, students[i].name);
				break;
			}
		}
	}
	return 0;
}
int print_list_retake(){
	int counter; int counter_n;
	printf("Список студентов на пересдачу: ");
	for (int i = 0; i < number_of_student; i++){
		counter = 0; counter_n = 0;
		int discipline_id_1 = 0; int discipline_id_2 = 0;
		int number_dics = how_many_disciplines_in_group(students[i].group_id);
		for (int j = 0; j < number_of_marks; j++){
			if (marks[j].group_id == students[i].group_id){
				counter_n++;
				if (marks[j].student_id == students[i].id && marks[j].mark <= 2){
					counter++;
					if (discipline_id_1 == 0) discipline_id_1 = marks[j].discipline_id;
					else discipline_id_2 = marks[j].discipline_id;
				}
			}
			if (counter_n == number_dics && counter <= 2 && counter != 0){
				(counter == 2) ? printf("\n\t%s %s на пересдачу по %s %s", students[i].surname, students[i].name, disciplines[discipline_id_1].name, disciplines[discipline_id_2].name) :
					printf("\n\t%s %s на пересдачу по %s", students[i].surname, students[i].name, disciplines[discipline_id_1].name);
				break;
			}
		}
	}
	return 0;
}

int how_many_disciplines_in_group(int group_id){
	int counter = 0;
	for (int i = 0; i <= number_of_gd; i++){
		if (gd[i].group_id == group_id){
			counter++;
		}
	}
	return counter;
}
//здесь мы что-то добавляем
void menu_add(){
	printf("0 : назад\n"
		"1 : Добавить студента для определенной группы\n"
		"2 : Добавить группу\n"
		"3 : Добавить дисциплину\n"
		"4 : Поставить оценку\n"
		"5 : Добавить дисциплину к группе\n"
		">");
	do{
		scanf("%d", &selection);
	} while (selection < 0 || selection > 5);
	printf("\n");
	switch (selection)
	{
	case 1:
		add_student();
		printf("Рекомендуется перезагрузить программу");
		break;
	case 2:
		add_group();
		printf("Рекомендуется перезагрузить программу");
		break;
	case 3:
		add_discipline();
		printf("Рекомендуется перезагрузить программу");
		break;
	case 4:
		add_mark();
		printf("Рекомендуется перезагрузить программу");
		break;
	case 5:
		add_discipline_to_group();
		printf("Рекомендуется перезагрузить программу");
		break;
	default:
		break;
	}
}
int add_student(){
	char group[5]; int group_id; char new_surname[N]; char new_name[N];
	printf("Введите группу: ");
	getchar(); gets(group);
	for (int i = 0; i <= number_of_group; i++){
		if (memcmp(group, groups[i].name, 4) == 0){
			group_id = groups[i].id;
		}
	}
	printf("Введите фамилию: ");
	gets(new_surname);
	printf("Введите имя: ");
	gets(new_name);
	number_of_student++;
	students[number_of_student].id = number_of_student;
	strcpy(students[number_of_student].surname, new_surname);
	strcpy(students[number_of_student].name, new_name);

	FILE* fl;
	if ((fl = fopen("tables\\students.txt", "a+")) == NULL){
		printf("ERROR"); return 0;
	}
	fprintf(fl, "\n%d %d %s %s", number_of_student, group_id, new_surname, new_name);
	fclose(fl);
	return 0;
}
int add_group(){
	char new_name[5];
	printf("Введите название группы: ");
	getchar(); gets(new_name);
	FILE* fl;
	if ((fl = fopen("tables\\groups.txt", "a+")) == NULL){
		printf("ERROR"); return 0;
	}
	number_of_group++;
	fprintf(fl, "\n%d %s", number_of_group, new_name);
	fclose(fl);
	return 0;
}
int add_discipline(){
	char new_discipline[N];
	printf("Введите название дисциплины: ");
	getchar(); gets(new_discipline);
	FILE* fl;
	if ((fl = fopen("tables\\disciplines.txt", "a+")) == NULL){
		printf("ERROR"); return 0;
	}
	number_of_discipline++;
	fprintf(fl, "\n%d %s", number_of_discipline, new_discipline);
	fclose(fl);
	return 0;
}
int add_mark(){
	char name[N];
	char surname[N];
	int student_id = -1; int group_id;
	printf("Фамилия студента: ");
	getchar(); gets(surname);
	int n_surname = strlen(surname, N);
	printf("Имя студента: ");
	gets(name);
	int n_name = strlen(name, N);
	for (int i = 0; i <= number_of_student; i++){ // поиск нужной фамилии
		if (memcmp(surname, students[i].surname, n_surname) == 0){
			if (memcmp(name, students[i].name, n_name) == 0){
				student_id =
					students[i].id;
				group_id = students[i].group_id;
				break;
			}
		}
	}
	if (student_id == -1){ printf("Уверены, что такой студент есть?\n"); return 0; }
	int discipline_id = -1; char discipline[N];
	int n_disc = strlen(discipline, N);
	printf("Введите дисциплину: "); gets(discipline);
	for (int i = 0; i <= number_of_discipline; i++){ // поиск нужной дисциплины
		if (memcmp(discipline, disciplines[i].name, n_disc) == 0){
			discipline_id = disciplines[i].id;
			break;
		}
	}
	if (student_id == -1){ printf("Уверены, что такой студент есть?\n"); return 0; }
	FILE* fl;
	if ((fl = fopen("tables\\marks.txt", "a+")) == NULL){
		printf("ERROR"); return 0;
	}
	number_of_marks++;
	printf("Введите оценку: ");
	int new_mark;
	scanf("%d", &new_mark);
	fprintf(fl, "\n%d %d %d %d %d", number_of_marks, new_mark, group_id, student_id, discipline_id);
	fclose(fl);
	return 0;
}
int add_discipline_to_group(){
	char discipline[N]; char group[N]; int discipline_id = -1; int group_id = -1;
	printf("Введите название дисциплины: ");
	getchar(); gets(discipline);
	int n_discipline = strlen(discipline, N);
	for (int i = 0; i <= number_of_discipline; i++){ // поиск нужной фамилии
		if (memcmp(discipline, disciplines[i].name, n_discipline) == 0){
			discipline_id = disciplines[i].name;
			break;
		}
	}
	printf("Введите название группы: ");
	getchar(); gets(group);
	int n_group = strlen(group, N);
	for (int i = 0; i < number_of_discipline; i++){
		if (memcmp(group, groups[i].name, n_group) == 0){
			group_id = disciplines[i].name;
			break;
		}
	}
	if (group_id == -1 || discipline_id == -1){ printf("Что-то ввели не то?\n"); return 0; }
	for (int i = 0; i < number_of_gd; i++){//проверка на повторение
		if (gd[i].group_id == group_id && gd[i].discipline_id == discipline_id){
			printf("Дисциплина %s в группе %s уже есть", disciplines[i].name, groups[i].name);
			return 0;
		}
	}
	FILE* fl;
	if ((fl = fopen("tables\\groups_disciplines.txt", "a+")) == NULL){
		printf("ERROR"); return 0;
	}
	number_of_gd++;
	fprintf(fl, "\n%d %d %d", number_of_gd, group_id, discipline_id);
	fclose(fl);
	return 0;
}

int menu_delete(){
	printf("1 : Удалить студента\n"
		"2 : Удалить группу\n"
		"3 : Удалить дисциплину\n"
		"4 : Удалить оценку\n"
		"0 : назад\n"
		">");
	do{
		scanf("%d", &selection);
	} while (selection < 0 || selection > 4);
	printf("\n");
	switch (selection)
	{
	case 1:
		delete_student();
		printf("Рекомендуется перезагрузить программу");
		break;
	case 2:
		delete_group();
		printf("Рекомендуется перезагрузить программу");
		break;
	case 3:
		delete_discipline();
		printf("Рекомендуется перезагрузить программу");
		break;
	case 4:
		delete_mark();
		printf("Рекомендуется перезагрузить программу");
		break;
	default:
		break;
	}
	return 0;
}
int delete_student(){
	char name[N];
	char surname[N];
	int student_id = -1;
	printf("Фамилия студента: ");
	getchar(); gets(surname);
	int n_surname = strlen(surname, N);
	printf("Имя студента: ");
	gets(name);
	int n_name = strlen(name, N);
	for (int i = 0; i <= number_of_student; i++){ // поиск нужной фамилии
		if (memcmp(surname, students[i].surname, n_surname) == 0){
			if (memcmp(name, students[i].name, n_name) == 0){
				student_id = students[i].id;
				break;
			}
		}
	}
	FILE* fl;
	if ((fl = fopen("tables\\students_test.txt", "w")) == NULL){
		printf("ERROR"); return 0;
	}
	if (student_id == -1){ printf("Уверены, что такой студент есть?\n"); return 0; }
	for (int i = 0; i < number_of_student; i++){
		if (student_id != students[i].id){
			fprintf(fl, "\n%d %d %s %s", students[i].id, students[i].group_id, students[i].surname, students[i].name);
		}
	}
	number_of_student--;
	fclose(fl);

	return 0;
}
int delete_group(){
	char name_group[5]; int group_id = -1;
	printf("Введите название группы: ");
	getchar(); gets(name_group);
	for (int i = 0; i < number_of_group; i++){
		if (memcmp(name_group, groups[i].name, 5) == 0){
			group_id = groups[i].id;
			break;
		}
	}
	if (group_id == -1) { printf("Неверные данные\n"); return 0; }
	FILE* fl;
	if ((fl = fopen("tables\\groups.txt", "w")) == NULL){
		printf("ERROR"); return 0;
	}
	for (int i = 0; i < number_of_group; i++){
		if (groups[i].id == group_id)
			continue;
		fprintf(fl, "\n%d %s", groups[i].id, groups[i].name);
	}
	fclose(fl);
	number_of_group--;
	return 0;
}
int delete_discipline(){
	char name_discipline[5]; int discipline_id = -1;
	printf("Введите название дисциплины: ");
	getchar(); gets(name_discipline);
	int n_discipline = strlen(name_discipline, N);
	for (int i = 0; i < number_of_discipline; i++){
		if (memcmp(name_discipline, disciplines[i].name, n_discipline) == 0){
			discipline_id = disciplines[i].id;
			break;
		}
	}
	if (discipline_id == -1) { printf("Неверные данные\n"); return 0; }
	FILE* fl;
	if ((fl = fopen("tables\\disciplines.txt", "w")) == NULL){
		printf("ERROR"); return 0;
	}
	for (int i = 0; i < number_of_discipline; i++){
		if (disciplines[i].id == discipline_id)
			continue;
		fprintf(fl, "\n%d %s", disciplines[i].id, disciplines[i].name);
	}
	fclose(fl);
	number_of_discipline--;
	return 0;
}
int delete_mark(){
	char name[N]; char surname[N];
	int student_id = -1;
	printf("Фамилия студента: ");
	getchar(); gets(surname);
	int n_surname = strlen(surname, N);
	printf("Имя студента: ");
	gets(name);
	int n_name = strlen(name, N);

	for (int i = 0; i <= number_of_student; i++){ // поиск нужной фамилии
		if (memcmp(surname, students[i].surname, n_surname) == 0){
			if (memcmp(name, students[i].name, n_name) == 0){
				student_id = students[i].id;
				break;
			}
		}
	}
	char name_discipline[5]; int discipline_id = -1;
	printf("Введите название дисциплины: ");
	gets(name_discipline);
	int n_discipline = strlen(name_discipline, N);
	for (int i = 0; i < number_of_group; i++){
		if (memcmp(name_discipline, disciplines[i].name, n_discipline) == 0){
			discipline_id = disciplines[i].id;
			break;
		}
	}
	if (student_id == -1 || discipline_id == -1) { printf("Неверные данные\n"); return 0; }
	FILE* fl;
	if ((fl = fopen("tables\\marks.txt", "w")) == NULL){
		printf("ERROR"); return 0;
	}
	for (int i = 0; i < number_of_marks; i++){
		if (marks[i].discipline_id == discipline_id && marks[i].student_id == student_id)
			continue;
		fprintf(fl, "\n%d %d %d %d %d", marks[i].id, marks[i].mark, marks[i].group_id, marks[i].student_id, marks[i].discipline_id);
	}
	fclose(fl);
	number_of_marks--;
	return 0;
	return 0;
}

// стартовые инициализации - загрузка из файла в структуры
void StartIndStudent(){
	FILE* fl;
	if ((fl = fopen("tables\\students.txt", "rt")) == NULL){
		printf("ERROR"); return 0;
	}
	char line[100];
	int i = 0;
	char tester = '0';
	while (!feof(fl)){
		fscanf(fl, "%d", &students[i].id);
		fscanf(fl, "%d", &students[i].group_id);
		fscanf(fl, "%s", students[i].surname);
		fscanf(fl, "%s", students[i].name);
		number_of_student++;
		i++;
	}
	fclose(fl);
}
void StartIndGroup(){
	FILE* fl;
	if ((fl = fopen("tables\\groups.txt", "rt")) == NULL){
		printf("ERROR"); return 0;
	}
	char line[100];
	int i = 0;
	while (!feof(fl)){
		fscanf(fl, "%d", &groups[i].id);
		fscanf(fl, "%s", groups[i].name);
		number_of_group++;
		i++;
	}
	fclose(fl);
}
void StartIndDiscipline(){
	FILE* fl;
	if ((fl = fopen("tables\\disciplines.txt", "rt")) == NULL){
		printf("ERROR"); return 0;
	}
	char line[100];
	int i = 0;
	while (!feof(fl)){
		fscanf(fl, "%d", &disciplines[i].id);
		fscanf(fl, "%s", disciplines[i].name);
		number_of_discipline++;
		i++;
	}
	fclose(fl);
}
void StartIndMarks(){
	FILE* fl;
	if ((fl = fopen("tables\\marks.txt", "rt")) == NULL){
		printf("ERROR"); return 0;
	}
	char line[100];
	int i = 0;
	while (!feof(fl)){
		fscanf(fl, "%d", &marks[i].id);
		fscanf(fl, "%d", &marks[i].mark);
		fscanf(fl, "%d", &marks[i].group_id);
		fscanf(fl, "%d", &marks[i].student_id);
		fscanf(fl, "%d", &marks[i].discipline_id);
		number_of_marks++;
		i++;
	}
	fclose(fl);
}
void StartIndGroup_discipline(){
	FILE* fl;
	if ((fl = fopen("tables\\groups_disciplines.txt", "rt")) == NULL){
		printf("ERROR"); return 0;
	}
	char line[100];
	int i = 0;
	while (!feof(fl)){
		fscanf(fl, "%d", &gd[i].id);
		fscanf(fl, "%d", &gd[i].group_id);
		fscanf(fl, "%d", &gd[i].discipline_id);
		number_of_gd++;
		i++;
	}
	fclose(fl);
}
void StartAvengereMarks(){
	for (int i = 0; i <= number_of_student; i++){
		int mid_mark = 0;
		for (int j = 0; j <= number_of_marks; j++){
			if (marks[j].student_id == students[i].id){
				mid_mark += marks[j].discipline_id;
			}
		}
		sum_marks[i].sum_mark = mid_mark / number_of_discipline;
		sum_marks[i].student_id = students[i].id;
		number_of_sum_marks++;
	}
}
//main
int main(void){
	char *locale = setlocale(LC_ALL, "");
	StartIndStudent();
	StartIndGroup();
	StartIndDiscipline();
	StartIndMarks();
	StartIndGroup_discipline();
	//StartAvengereMarks();
	while ((selection = Menu()) != 0){
		switch (selection)
		{
		case 1:
			menu_add();
			break;
		case 2:
			menu_print();
			break;
		case 3:
			print_list_deduction();
			break;
		case 4:
			print_list_scholarship();
			break;
		case 5:
			print_list_retake();
			break;
		case 6:
			menu_delete();
			break;
		}
	}
	getchar();
	return 0;
}