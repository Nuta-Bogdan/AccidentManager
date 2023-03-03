
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#define _GNU_SOURCE
#define setCol(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x)
typedef struct acc
{
    char loc[30];
    char tip[30];
};
void addacc(struct acc obi[],struct acc x)
{
    // adauga un nou obiet in obi, la primul loc disponibil
    for (int i = 0; i < 100; i++)
        if (strcmp(obi[i].loc,"")==0)
        {
            strcpy(obi[i].loc,x.loc);
            strcpy(obi[i].tip,x.tip);
            break;
        }
}
void delacc(struct acc obi[],struct acc x)
{
    // sterge prima aparitie a obiectului x
    for (int i = 0; i < 100; i++)
        if (!(strcmp(obi[i].loc,x.loc) && strcmp(obi[i].tip,x.tip) ))
        {
            for (int j = i; j < 99; j++)
            {
                strcpy(obi[j].loc,obi[j+1].loc);
                strcpy(obi[j].tip,obi[j+1].tip);
            }
        }
}
char repr(struct acc obi[],char k[][60])
{
    // incarca matricea k cu o reprezentare a listei obi
    for (int i = 0; i<100; i++)
    {
        if (obi[i].loc[0]==0)
            break;
        else
        {
            strcat(k[i],obi[i].loc);
            strcat(k[i]," - ");
            strcat(k[i],obi[i].tip);
        }
    }

    return k;
}
void reprline(struct acc obi[],int i)
{
    // reprezinta un singur obiect din obi
    for  (int j = 0; j<30; j++)
    {
        if (obi[i].loc[j] != NULL)
            printf("%c",obi[i].loc[j]);
        else
            break;
    }
    printf(" - ");
    for  (int k = 0; k<30; k++)
    {
        if (obi[i].tip[k] != NULL)
            printf("%c",obi[i].tip[k]);
        else
            break;
    }

}
char locUnic(struct acc obi[],char boris[100][60])
{
    // il incarca pe boris cu toate locurile unice
    int k = 0;
    for (int i = 0; i< 100; i++)
    {
        bool in = false;
        for(int j = 0; j<100; j++)
        {
            if (strcmp(obi[i].loc,boris[j]) == 0)
            {
                in = true;
                break;
            }
        }
        if (!in)
        {
            strcpy(boris[k],obi[i].loc);
            k++;
        }
    }

    return boris;
}
char tipUnic(struct acc obi[],char boris[100][60])
{
    // il incarca pe boris cu toate tipurile unice
    int k = 0;
    for (int i = 0; i< 100; i++)
    {
        bool in = false;
        for(int j = 0; j<100; j++)
        {
            if (strcmp(obi[i].tip,boris[j]) == 0)
            {
                in = true;
                break;
            }
        }
        if (!in)
        {
            strcpy(boris[k],obi[i].tip);
            k++;
        }
    }
    return boris;
}
int pmenu(char menu[][60],int len,bool withLogic,char before[60],char after[60])
{
    // afisare de meniu. argumente: matricea de afisat, numarul de linii cu ceva pe ele,
    // daca trebuie sau nu sa fie navigabil, daca are antet, default = "", subsol, idem
    int currentLine=1;
    setCol(0x0A);
    while (true)
    {
        if (strcmp(before,"")!=0)
            printf("%s\n\n",before);

        for (int i = 0; i<len; i++)
        {
            if (withLogic && (i == currentLine-1))
                    setCol(0xA0);
            else
                    setCol(0x0A);
                for (int j = 0; j < 60; j++)
                {
                    if (menu[i][j]!=NULL)

                        printf("%c",menu[i][j]);
                    else
                        break;
                }
            if (withLogic && (i == currentLine-1)){
               setCol(0xA0);
                printf(" <<");
              setCol(0x0A);;}

            printf("\n");
        }

        if (strcmp(after,"")!=0)
            printf("\n\n%s",after);

        if (withLogic)
        {
            char comanda = getch();
            system("cls");
            currentLine=logic(currentLine,comanda,len);
            if (comanda == 13)
                return currentLine;
            // pentru enter
            if (comanda == 27)
                return -2;
            // pentru escape
        }
        else
        {
            printf("\n\n");
            system("pause");
            system("cls");
            return 0;
            // doar afisare
        }

    }
}
void shutdwn(struct acc obi[])
{
    // scrie obiectele in data.txt
    // format: loc,tip
    // in loc de space folosim *, pentru ca C-ul ma uraste, imi regret existenta, imi e dor de python
    FILE* fp;
    fp=fopen("data.txt","w");
    for (int i =0; i<100; i++)
    {
        if (strcmp(obi[i].loc,"")!=0)
        {
            for (int j = 0; j< 30; j ++)
            {
                if (obi[i].loc[j]!=0)
                    if(obi[i].loc[j]!=32)
                        fprintf(fp,"%c",obi[i].loc[j]);
                    else
                        fprintf(fp,"*");
                else
                    break;
            }
            // scriere loc

            fprintf(fp,",");

            for (int j = 0; j< 30; j ++)
            {
                if (obi[i].tip[j]!=0)
                    if(obi[i].tip[j]!=32)
                        fprintf(fp,"%c",obi[i].tip[j]);
                    else
                        fprintf(fp,"*");
                else
                    break;
            }
            // scriere tip

            fprintf(fp,"%c",10);
            // \n
        }
        else
            break;
    }
    fclose(fp);
}
void startup(struct acc obi[])
{
    // citire obiecte din data.txt si incarcare in memorie
    FILE* fp;
    fp = fopen("data.txt","r");
    char lin;
    char line[60];
    while (fscanf(fp,"%s",&line)!=EOF)
    {
        char a[30];
        char b[30];

        int vala=0;
        int valb=0;
        int i=0;
        bool virgula = false;

        while (line[i] != 0)
        {
            if (line[i] == 44)
            {
                virgula = true;
                i+=1;
                continue;
            }

            if (!virgula)
            {
                if(line[i] != 42)
                    a[vala]=line[i];
                else
                    a[vala]=32;
                vala+=1;
            }
            // scrie in a, transforma * in " "
            else
            {
                if(line[i] != 42)
                    b[valb]=line[i];
                else
                    b[valb]=32;
                valb+=1;
            }
            // idem, cu b
            i++;
        }
        struct acc temp;
        strcpy(temp.loc,a);
        strcpy(temp.tip,b);
        addacc(obi,temp);

        for(int i =0 ; i<30; i++)
        {
            a[i]=0;
            b[i]=0;
        }
        // cleanup a si b

    }
    system("cls");
    fclose(fp);
}
int logic(int value,int comanda,int lim)
{
    // logica din meniuri, stiu ca n-aveam nevoie,
    // am facut-o la inceput, refuz principial s-o sterg
    if(comanda == 115)
        value ++;
    if (comanda == 119)
        value --;
    if (value == 0)
        value = lim;
    if (value == lim+1)
        value = 1;
    return value;
}
int size(char k[][60])
{
    // returneaza pe cate randuri e ceva de afisat in matricea k
    for (int i = 0; i < 100; i++)
    {
        if (k[i][0] == 0)
            return i;
    }
}
void clean (char menu[][60])
{
    // umple matricea de \0
    for (int i = 0; i< 100; i++)
        for (int j = 0; j< 60; j++)
            menu[i][j] = 0;
}
int main()
{

    struct acc obi[100];  //tine toate obiectele
    char comanda;         //preia logica w,s,enter
    int currentLine=1;
    for (int i = 0; i<100; i++)
    {
        strcpy(obi[i].loc,"");
        strcpy(obi[i].tip,"");
    }
    // curatare lista obiecte

    startup(obi);
    // incarcare obiecte in memorie

    char menu1[3][60]= {{"Raport Nou"},{"Afisare accidente"},{"Exit"}};
    char menu2[4][60]= {{"Arata toate accidentele"},{"Filter"},{"Delete"},{"Exit"}};
    char menu3[2][60]= {{"Dupa Locatie"},{"Dupa Tip"}};

    while (true)
    {

        currentLine = pmenu(menu1,3,true,"---Main Menu---","");
        if (currentLine == 1)
            //adaugare accident nou
        {
            system("cls");

            char a[30];
            char b[30];

            printf("Locul Accidentului: ");
            gets(a);
            printf("Tipul Accidentului: ");
            gets(b);

            struct acc nou;
            strcpy(nou.loc,a);
            strcpy(nou.tip,b);
            addacc(obi,nou);

            system("cls");
        }

        if (currentLine == 2)
            //afisare accidente
        {
            while (true)
            {
                system("cls");
                currentLine = pmenu(menu2,4,true,"","");

                // afisare meniu 2
                if (currentLine == 1)
                {
                    //toate accidentele
                    char menu4[100][60];

                    clean(menu4);
                    repr(obi,menu4);

                    pmenu(menu4,size(menu4),false,"Locatie / Tip","");

                }
                if (currentLine == 2)
                    // filtre
                {
                    currentLine = 1;
                    system("cls");
                    while (true)
                    {
                        currentLine=pmenu(menu3,2,true,"","");
                        bool didThingie=false;

                        if (currentLine == 1)
                        {
                            //filtrare dupa loc

                            didThingie=true;

                            char boris[100][60];
                            clean(boris);
                            locUnic(obi,boris);
                            currentLine=1;

                            while (true)
                            {
                                currentLine = pmenu(boris,size(boris),true,"Selecteaza locul accidentului","");
                                system("cls");

                                for (int i = 0; i<100; i++)
                                {
                                    if (strcmp(obi[i].loc,boris[currentLine-1])==0)
                                    {
                                        reprline(obi,i);
                                        printf("\n");
                                    }
                                }
                                printf("\n");
                                printf("\n");
                                system("pause");
                                system("cls");
                                break;
                            }
                        }
                        else if (currentLine == 2)
                        {
                            //filtrare dupa tip

                            didThingie=true;

                            char boris[100][60];
                            clean(boris);
                            tipUnic(obi,boris);
                            currentLine=1;

                            while (true)
                            {
                                system("cls");
                                currentLine = pmenu(boris,size(boris),true,"Selecteaza tipul accidentului","");
                                system("cls");
                                for (int i = 0; i<100; i++)
                                {
                                    if (strcmp(obi[i].tip,boris[currentLine-1])==0)
                                    {
                                        reprline(obi,i);
                                        printf("\n");
                                    }
                                }
                                printf("\n");
                                printf("\n");
                                system("pause");
                                system("cls");
                                break;
                            }
                        }

                        if (didThingie)
                            break;
                    }

                }
                if (currentLine == 3)
                {
                    // delete
                    while (true)
                    {
                        currentLine=1;
                        char menu5[100][60];
                        clean(menu5);
                        repr(obi,menu5);
                        currentLine=pmenu(menu5,size(menu5),true,"Alege ce incident sa stergi","Press ESC to exit the Matrix");

                        if (currentLine!=-2)
                            delacc(obi,obi[currentLine-1]);
                        else
                            break;
                    }
                }
                if (currentLine == 4 || currentLine==-2)
                    //intoarcere la meniu 1
                {
                    currentLine = 1;
                    system("cls");
                    break;
                }
            }
        }
        if(currentLine == 3 || currentLine==-2)
            break;


    }
    shutdwn(obi);
    return 0;

}
