#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/stat.h>
typedef struct{
    int dayType;//åé-ì˙[0-6]
    int time;//xå¿ñ⁄[0-6]
    int noT;//number of time
    char year[5];//îN[1900-precent]
    char duaring;//ëOä˙-å„ä˙[0-1]
}data;
data inparse(char[]);

int main(void){
    data data;
    FILE *fp;
    char filename[11] = {'\0'};
    char str[5] = ".txt";
    char tmp[256] = {'\0'};
    char log[25] = {'\0'};
    int c = 0;
    struct stat statBuf;
    int a = 0;
    time_t t;
    struct tm *localt;
    int inttime,intdate;

    t = time(NULL);
    localt = localtime(&t);
    intdate = localt->tm_mday + (localt->tm_mon + 1) * 100 + (localt->tm_year + 1900) * 10000;
    inttime = localt->tm_sec + localt->tm_min * 100 + localt->tm_hour * 10000;
    sprintf(log, "./%d%d.txt",intdate, inttime);
    printf("%s\n", log);

    if((fp=fopen(log,"w"))==NULL){
        return 0;
    }else {
        fprintf(fp, "%s\n", "â»ñ⁄ñºèoóÕLog");
        fclose(fp);
    }

    while(a == 0){
        data = inparse(log);
        sprintf(filename,"%s%c%c%s", data.year, '_', data.duaring, str);
        printf("%s\n", filename);

        fp = fopen(filename, "r");
        if(fp == NULL){
            printf("ERROR:FileOpenError\n");
        }
        while(c <= (data.dayType * 7 + data.time)){
            if(c == (data.dayType * 7 + data.time)) {
                if (fgets(tmp, 256, fp) != NULL) {
                    printf("%s", tmp);
                }
            }else {
                fgets(tmp, 256, fp);
            }
            c++;
        }
        if((fp=fopen(log,"a"))==NULL){
            return 0;
        }else {
            fprintf(fp, "%s\n", tmp);
            fclose(fp);
        }

        printf("Finish? Yes:1 No:0\n");
        scanf("%d", &a);

//    if (stat("./files/201907311456.txt", &statBuf) == 0) {
//        printf("%s isfound\n", "./files/201907311456.txt");
//    }else {
//        printf("%s isnot\n", "./files/201907311456.txt");
//    }
    }

    return  0;
}

data inparse(char log[]){
    int a, i, intyear, intmonth, intday, intmd, inttime;
    FILE *fp;
    char *name;
    char year[5];
    char month[3];
    char day[3];
    char time[5];
    struct tm strtime;
    data data = {0,0,0,'0'};
    time_t ret;

    name = malloc(sizeof(char) * 256);
    if(name == NULL){
        printf("GetMemoryFailed");
        return data;
    }

    scanf("%s", name);
    if((fp=fopen(log,"a"))==NULL){
    }else {
        fprintf(fp, "%s : ", name);
        fclose(fp);
    }
//	printf("%s\n", name);
    a = 0;
    i = 0;
    while(a < 4){
//        printf("%d  %d\n", a, i);
        year[i] = name[a];
        a++;
        i++;
        if(i == 4){
            year[i] = '\0';
        }
    }
    i = 0;
    while(a < 6){
//        printf("%d %d\n", a, i);
        month[i] = name[a];
        a++;
        i++;
        if(i == 2){
            month[i] = '\0';
        }
    }
    i = 0;
    while(a < 8){
//        printf("%d %d\n", a, i);
        day[i] = name[a];
        a++;
        i++;
        if(i == 2){
            day[i] = '\0';
        }
    }
    i = 0;
    while(a < 12){
//        printf("%d  %d\n", a, i);
//        printf("%c\n", name[a]);
        time[i] = name[a];
//        printf("%c\n", time[i]);
        a++;
        i++;
        if(i == 4){
            time[i] = '\0';
        }
    }
    printf("%s\n", year);
    printf("%s\n", month);
    printf("%s\n", day);
    printf("%s\n", time);

    strncpy(data.year, year,5);
    intyear = atoi(year);
    intmonth = atoi(month);
    intday = atoi(day);
    inttime = atoi(time);
    intmd = intmonth * 100 + intday;

    if(intmd > 401 && intmd < 831){
        data.duaring = '1';
    }
    else{
        data.duaring = '2';
    }
    printf("%c\n", data.duaring);
    strtime.tm_year = intyear - 1900; /* îN[1900îNÇ©ÇÁãNéZ] */
    strtime.tm_mon = intmonth - 1; /* åé[0-11] */
    strtime.tm_mday = intday; /* ì˙[1-31] */
    strtime.tm_hour = 0;/* éû[0-23] */
    strtime.tm_min = 0;/* ï™[0-59] */
    strtime.tm_sec = 0; /* ïb[0-61]â[ïbçló∂ */
    ret = mktime( &strtime );
    if( ret == -1 ) {
        printf( "Error couse : mktime\n" );
        return data;
    }
    data.dayType = strtime.tm_wday;
    data.dayType -= 1;
    if(data.dayType == -1){
        data.dayType = 6;
    }
    printf("%d\n", data.dayType);

    if(inttime > 845 && inttime < 1029){
        data.time = 0;
    }else if(inttime > 1030 && inttime < 1214){
        data.time = 1;
    }else if(inttime > 1215 && inttime < 1444){
        data.time = 2;
    }else if(inttime > 1445 && inttime < 1629){
        data.time = 3;
    }else if(inttime > 1630 && inttime < 1814){
        data.time = 4;
    }else if(inttime > 1815 && inttime < 1954){
        data.time = 5;
    }else if(inttime > 1955 && inttime < 2134){
        data.time = 6;
    }
    data.noT = 0;
    return data;
}