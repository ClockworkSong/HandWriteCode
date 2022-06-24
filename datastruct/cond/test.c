#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	int len;
	char data[0];
}common_st;

typedef struct {
	char name[20];
	long id;
	int age;
} student_st;

void print_stu(const student_st* stu) {

	printf("name=%s\n", stu->name);
	printf("id=%ld\n", stu->id);
	printf("age=%d\n", stu->age);

	return;
}

int main(int argc, char* argv[]) {
	
	student_st *stu = (student_st*)malloc(sizeof(student_st));
	strcpy(stu->name, "songxd");
	stu->id = 110102113;
	stu->age = 31;

	student_st stu2;
	strcpy(stu2.name, "baoxw");
	stu2.id = 110102114;
	stu2.age = 30;

	common_st* com = (common_st*)malloc(sizeof(common_st)) + sizeof(student_st)*2;
	com->len = sizeof(student_st);
	memcpy(com->data, stu, sizeof(student_st));

	student_st *tmp = (student_st*)com->data;
	print_stu(tmp);

	memcpy(com->data+com->len, &stu2, sizeof(student_st));
	tmp++;
	print_stu(tmp);

	return 0;
}
