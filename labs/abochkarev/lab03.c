int main(int argc, char* argv[], char* env[]) {
    printf("--------------------LAB3--------------------\n");
    printf("%d\n",argc);
 -  // execl(argv[1], argv[1], argv[0], 0); 
 -  execl(argv[1], argv[1], argv[0]); 
 +  execl(argv[1], argv[1], argv[0], 0); 
    printf("Ошибка передачи управления\n");
    printf("--------------------LAB3END--------------------\n");
 -  exit(0);
 +  exit(-1);
  }
