#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

struct primebin
{
    long int n;
};

struct filedat
{
    char fname[50];
    unsigned long long int d;
    int password;
    long int n;
    long int e;
};

struct RSAval{
    unsigned long long int d;
    long int e;
    long int n;
};

//prototypes for all functions
void encrypt(long int e,long int n,char message[], unsigned long long int d, int pass,int option);
void encrypt_back(char *fname);
void toggleCase(char *fileName, char *fname);
void lowerCase(char *fileName, char *fname);
void upperCase(char *fileName, char *fname);
void replaceWord(char *fileName,char *fname);
void menu_inside_decrypt(char *fname);
int password_gen();
int checkname(char *str);
unsigned long long int gcd(int a, int b);
unsigned long long int modular_expo(long int a,long int b,long int n);
void decrypt();
struct RSAval generate_keys();

void encrypt(long int e,long int n,char message[], unsigned long long int d, int pass,int option)
{
    long int len = strlen(message), i_loop = 0;
    FILE *wptr = fopen("tmp.txt","w");
    char buffer[21];
    for(i_loop = 0 ; i_loop < len ; i_loop++)
    {
        sprintf(buffer,"%llu",modular_expo(message[i_loop],e,n));
        fputs(strcat((buffer),"\n") , wptr);
    }
    fclose(wptr);
    int flag = 0;
    if(option==1)
    {
        do{ 
            printf("Enter the name of the file: ");
            char name[45];
            scanf("%s", name);
            if(checkname(name) == 1 && strcmp(name,"trial") != 0)
            {
                rename("tmp.txt",name);
                FILE *wptrb = fopen("Passwords.bin","ab");
                struct filedat obj;
                strcpy(obj.fname,name);
                obj.d = d;
                obj.password = pass;
                obj.n = n;
                obj.e = e;
                fwrite(&obj, sizeof(struct filedat), 1, wptrb);
                fclose(wptrb);
                flag = 1;
            }
        }while(flag == 0);  
    }
}

void encrypt_back(char *fname)
{
    struct filedat obj;
    FILE *ptr = fopen("Passwords.bin", "rb");
    while(fread(&obj, sizeof(struct filedat), 1, ptr))
    {
        if(strcmp(obj.fname, fname) == 0)
        {
            char ch;
            char msg[50];
            int ctr = 0;
            FILE *p = fopen("trial.txt", "r");
            printf("\nThe message in the file is:\n");
            while(1)
            {
                ch = fgetc(p);
                if( ch == EOF )
                    break;
                msg[ctr] = ch;
                ctr++;
            }
            msg[ctr] = '\0';
            printf("%s\n",msg);
            fclose(p);
            encrypt(obj.e, obj.n, msg, obj.d, obj.password, 0);
            remove(fname);
            rename("tmp.txt", fname);
            remove("tmp.txt");
        }
    }
    fclose(ptr);
}

void toggleCase(char *fileName, char *fname)
{
    FILE *toToggle = fopen(fileName,"r");
    FILE *temp = fopen("temp_to_toggle.txt","w");
    char c;
    if (!temp)
    {
        printf("File error.Could not toggle\n");
        return;
    }
    if (!toToggle)
    {
        printf("File %s could not be opened\n",fileName);
        return;
    }
    while(1)
    {
        c = fgetc(toToggle);
        if (c==EOF)
            break;
        if (islower(c))
            c = toupper(c);
        else if (isupper(c))//only if its a charachter
            c = tolower(c);
        fputc(c,temp);
    }
    fclose(temp);
    fclose(toToggle);
    remove(fileName);
    rename("temp_to_toggle.txt",fileName);
    encrypt_back(fname);
}

void lowerCase(char *fileName, char *fname)
{
    FILE *toLower = fopen(fileName,"r");
    FILE *temp = fopen("temp_to_lower.txt","w");
    char c;
    if (!temp)
    {
        printf("File error.Could not covert to lower case\n");
        return;
    }
    if (!toLower)
    {
        printf("File %s could not be opened\n",fileName);
        return;
    }
    while(1)
    {
        c = fgetc(toLower);
        if(c == EOF)
            break;
        if (isupper(c))
            c = tolower(c);
        fputc(c,temp);
    }
    fclose(temp);
    fclose(toLower);
    remove(fileName);
    rename("temp_to_lower.txt",fileName);
    encrypt_back(fname);
}

void upperCase(char *fileName, char *fname)
{
    FILE *toUpper = fopen(fileName,"r");
    FILE *temp = fopen("temp_to_upper.txt","w");
    char c;
    if (!temp)
    {
        printf("File error.Could not covert to upper case\n");
        return;
    }
    if (!toUpper)
    {
        printf("File %s could not be opened\n\n",fileName);
        return;
    }
    while(1)
    {
        c = fgetc(toUpper);
        if(c == EOF)
            break;
        if (islower(c))
            c = toupper(c);
        fputc(c,temp);
    }
    fclose(temp);
    fclose(toUpper);
    remove(fileName);
    rename("temp_to_upper.txt",fileName);
    encrypt_back(fname);
}

void replaceWord(char *fileName,char *fname)
{
    char original[200], replace[200];  
    printf("Enter word to be replaced: ");
    scanf("%s",original);
    printf("Enter word to replace '%s' with: ",original);
    getchar();
    scanf("%[^\n]s",replace);
    int count = 0;
    FILE *rptr = fopen(fileName,"r");
    FILE *wptr = fopen("temp.txt","w");
    if( !wptr || !rptr )
    {
        printf("File open Error\n");
        return;
    }
    char word[200];
    while(1)
    {   
        fscanf(rptr,"%s",word);
        if(feof(rptr))
        {   
            fclose(wptr);
            fclose(rptr);
            remove(fileName);
            rename("temp.txt",fileName);
            remove("temp.txt");
            printf("\n\nWord '%s' replaced %d time(s)\n\n",original,count);
            break;
        }
        if(strcmp(word,original) == 0)
        {
            fprintf(wptr, "%s", replace);
            fputc(' ',wptr);
            count++;
        }
        else
        {
            fprintf(wptr, "%s", word);
            fputc(' ',wptr);
        }
    }
    encrypt_back(fname);
}

void menu_inside_decrypt(char *fname)
{
    getchar();
    printf("\n\n--------------------------------------------------------------------------------\n\n");
    printf("\nWelcome to the Menu\n\n1. Find and Replace\n2. Toggle Case\n3. Lower Case\n4. Upper Case\n");
    printf("Enter your choice: ");
    char opt;
    opt = getchar();
    FILE *forTrial = fopen("trial.txt","r");
    fclose(forTrial);
    switch(opt)
    {
        case '1':
            replaceWord("trial.txt",fname);
            break;
        case '2':
            toggleCase("trial.txt", fname);
            break;
        case '3':
            lowerCase("trial.txt", fname);
            break;
        case '4':
            upperCase("trial.txt", fname);
            break;
        default:
            return;  
    }
    remove ("trial.txt");
}

int password_gen()
{
    srand(time(0));
    return rand();
}

int checkname(char *str)
{
    struct filedat tempObj;
    FILE *fptr = fopen("Passwords.bin","rb");
    char *t = str;
    strcat(t,".txt");
    while( fread( &tempObj,sizeof(struct filedat),1,fptr ))
    {
        if( strcmp(tempObj.fname, t) == 0 )
        {   
            printf("\nThis file %s is already in use:(\n\n",t);
            fclose(fptr);
            return 0;
        }
    }
    fclose(fptr);
    return 1;
}

unsigned long long int gcd(int a, int b){
    int t;
    while(b != 0){
        t = a;
        a = b;
        b = t%b;
    }
    return a;
}

unsigned long long int modular_expo(long int a,long int b,long int n)//calculating (a^b)modn
{
    unsigned long long int t = 0,final = 1,val = 0;
    val = (a*a)%n;
    for( int i = 0 ; i < b/2 ; i++ )
    {
        final *= val;
        if(final >= n)
            final = final%n;
    }
    if (b%2 == 1)
        final *= a;
    final = final%n;
    return final;
}

void decrypt()
{
    int num_password;
    char name[50];
    FILE *rbptr = fopen("Passwords.bin","r");
    struct filedat obj;
    printf("Enter the name of the file: ");
    scanf("%s", name);
    printf("Enter the password: ");
    scanf("%d",&num_password);
    printf("\n");
    fseek(rbptr, 0, 0);
    while(fread(&obj, sizeof(struct filedat),1,rbptr))
    {
        if((obj.password == num_password) && (strcmp(obj.fname, strcat(name, ".txt")) == 0))
        {
            FILE *rptr;
            FILE *ptr = fopen("trial.txt", "w");
            unsigned long long int d = obj.d;
            long int n = obj.n;
            rptr = fopen(obj.fname,"r");
            if(rptr == NULL)
                printf("Unable to open file %s\n",obj.fname);
            else
            {   
                char ch;
                char msgch;
                char buffer[20];
                printf("The message on the file is:\n");
                while(1)
                {
                    fgets(buffer,20,rptr);
                    if (feof(rptr))
                    {
                        fputc(' ', ptr);
                        break;
                    }
                    if(buffer[strlen(buffer)-1]=='\n')
                    {
                        buffer[strlen(buffer)-1]='\0';
                        msgch = modular_expo(atoi(buffer),d,n);
                        printf("%c",msgch);
                        fputc((char)msgch, ptr);
                    }
                }
                fclose(ptr);
                fclose(rptr);
                fclose(rbptr);
                menu_inside_decrypt(obj.fname);
                return;
            }
        }
    }
    fclose(rbptr);
    printf("File '%s' not found\n",name);
}

struct RSAval generate_keys()
{
   time_t t1 = time(0);
    long int p, q, pline, qline;
    srand(t1);
    FILE *br = fopen("nprimebinary.bin","r");
    struct primebin obj;

    do
    {
        pline = (rand()%262)+1;                                         //Number of prime numbers in the binary file is 262
        fseek(br, sizeof(struct primebin)*pline, 0);
        fread(&obj, sizeof(struct primebin), 1, br);
        p = obj.n;
        qline = (rand()%262)+1;
        fseek(br, sizeof(struct primebin)*qline, 0);                    //Number of prime numbers in the binary file is 262
        fread(&obj, sizeof(struct primebin), 1, br);
        q = obj.n;
    }while(p == q);//keep doing until get distinct ones 

    long int n = p*q;
    long int t = (p-1)*(q-1);
    long int e = 3;
    while(gcd(e, t)!=1)
        e+=2;
    unsigned long long int k = 1;
    unsigned long long int d;
    while(((k*t+1)%e) != 0)
        k+=1;
    d = (k*t+1)/e;
    struct RSAval values;
    values.d=d;
    values.e=e;
    values.n=n;
    return values; 
}

int main()
{  
   char new_msg[100];
   printf("\n    Welcome to the file viewer. Your files are stored in an encrypted form.\n");
    while(1)
    {   int opt;
        char c;
        printf("\n================================================================================\n\n");
        printf("1. Create new file\n");
        printf("2. Work on existing file\n");
        printf("3. Exit\n");
        printf("Enter your choice(1,2 or 3): ");
        scanf("%d",&opt);
        printf("\n");
            
        switch(opt)
        {
            case 1:
                    getchar();
                    printf("Enter the message: ");
                    scanf("%[^\n]s",new_msg);
                    struct RSAval values=generate_keys();
                    int num_password = password_gen();
                    printf("Password is: %d\n",num_password );
                    printf("Please note down your password for future reference.\n");
                    encrypt(values.e,values.n,new_msg,values.d,num_password,1);
                    break;
            case 2:
                    decrypt();
                    break;
            case 3:
                    exit(0);
                    break;
            default :
                    printf("Please enter a valid choice.\n");
        }

    }
    return 0;
}