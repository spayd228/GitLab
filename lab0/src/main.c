#define _CRT_SECURE_NO_WARNINGS
#define SIZE 100
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

struct Real_num {
    long double af_comma; //после
    int be_comma; //До
    int b1;
    int b2;
    char num[SIZE];
    int i_comma;
}; //qq

int Find_len (char a[SIZE]){
  int len;
  for (len = 0; a[len] != '\0'; ++len);
  return len;
}

void Chek(struct Real_num *Test) {
    int len = Find_len((*Test).num), numsys = 0, convert_b1 = (*Test).b1;
    if (((*Test).b1 > 16 || (*Test).b1 < 2) || ((*Test).b2 > 16 || (*Test).b2 < 2)) {
        printf("bad input\n");
        exit(0);
    }
    if ((*Test).b1 > 10) {
        convert_b1 += 'A' - 10;
    }
    else {
        convert_b1 += '0';
    }
    int comma = 0;
    for (int i = 0; (*Test).num[i] < len; ++i) {
        (*Test).num[len] = toupper((*Test).num[len]);
        if ((*Test).num[len] == '.') {
          comma++;
          (*Test).i_comma = len;
        }
        if ((*Test).num[len] > numsys) {
            numsys = (*Test).num[len];
        }
    }
    if (numsys >= convert_b1 || comma > 1 || (*Test).num[0] == '.') {
        printf("bad input\n");
        exit(0);
    }
    if (comma == 0){
      (*Test).i_comma = len;
    }
}

void Split(struct Real_num *Output) {
    (*Output).af_comma = 0;
    (*Output).be_comma = 0;
    int len = Find_len((*Output).num);
    bool flag = 1;
    (*Output).i_comma = len;
    for (int i = 0; i < len; ++i) {
        if ((*Output).num[i] == '.') {
            (*Output).i_comma = i;
            flag = 0;
            break;
        }
    }
    for (int i = (*Output).i_comma - 1; i >= 0; --i) {
        (*Output).be_comma += pow(10, (*Output).i_comma - i - 1) * (int)((*Output).num[i] - '0');
    }
    if (!flag) {
      for (int i = (*Output).i_comma + 1; i < len; ++i) {
          (*Output).af_comma += pow(10, (*Output).i_comma - i) * (int)((*Output).num[i] - '0');
      }
    }
}

void Convert_to_dec (struct Real_num *Convert){
    char s_conv[SIZE];
    int len = Find_len((*Convert).num);
    long double after = 0;
    (*Convert).be_comma = 0;
    (*Convert).af_comma = 0;
    for (int i = 0; i < (*Convert).i_comma; ++i){
      (*Convert).be_comma += pow((*Convert).b1, (*Convert).i_comma - i - 1) * ((*Convert).num[i] > '9'? (*Convert).num[i] - 'A' + 10: (*Convert).num[i] - '0');
    }
    for (int i = (*Convert).i_comma + 1; i < len; ++i){
      (*Convert).af_comma += pow((*Convert).b1, (*Convert).i_comma - i) * ((*Convert).num[i] > '9'? (*Convert).num[i] - 'A' + 10: (*Convert).num[i] - '0');
    }
}

void Convert_from_dec_be(struct Real_num Answer) {
    char ans[SIZE], c;
    int remains, i = 0, dec = Answer.be_comma;
    while (dec > 0) {
        remains = dec % Answer.b2;
        if (remains > 9) {
            ans[i] = remains - 10 + 'A';
        }
        else {
            ans[i] = remains + '0';
        }
        dec /= Answer.b2;
        ++i;
    }
    ans[i] = '\0';
    --i;
    for (int j = 0; j < i / 2; ++j) {
        c = ans[i - j];
        ans[i - j] = ans[j];
        ans[j] = c;
    }
    printf("%s", ans);
}

void Convert_from_dec_af_to_be(struct Real_num Answer) {
    char ans[SIZE];
    int integer, i = 0;
    long double dec = Answer.af_comma;
    while (dec > pow(10,-13) && i != 13) {
        dec *= Answer.b2;
        if (dec >= 1) {
            ans[i] = dec > 9 ? dec + 'A' - 10 : dec + '0';
            integer = dec;
            dec -= integer;
        }
        else{
            ans[i] = '0';
        }
        ++i;
    }
    ans[i] = '\0';
    Convert_from_dec_be(Answer);
    if (ans[0] != '\0')
    {
      printf(".%s\n", ans);
    }
}

int main(void) {
    struct Real_num Input;
    scanf("%d %d", &Input.b1, &Input.b2);
    scanf("%s", Input.num);
    Chek(&Input);
    if (Input.b1 == Input.b2){
      printf("%s\n", Input.num);
    }
    else{
      if (Input.b1 != 10){
      Convert_to_dec(&Input);
      }
      if (Input.b2 != 10){
        Split(&Input);
        Convert_from_dec_af_to_be(Input);
      }
      else{
        printf("%.13Lf\n", Input.be_comma + Input.af_comma);
      }
    }
    return 0;
}

//b = b*10 + a[i];
//modf