#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <io.h>

/*
 * ���Ǝ��̋L�^�t�@�C�����Ȗږ��Ɠ��t�Ɏ����Ń��l�[������v���O�����i�����t�@�C���Ή��j
 * �N���t�����ɉ����āA�����ł��炩���ߍ쐬�������Ԋ��\��I�����A�Ή������Ȗڂ��擾�B�Ȗږ��Ɠ��t�ɕύX����B
 * �����ɂȂ���͍̂Ō�Ɂin�j������B�in��1����̐����ŏd�������邲�Ƃɒl���P������B
 * ���Ԋ��\�͂P����V���ڂ܂ŁB���Ԃ͋ߑ�̎��Ԋ�����ɁA�O�̋x�ݎ��Ԃ�����Ƃ̏I�������܂łŋ�؂�B
 * ���Ԋ��\�̖����K���͔N_�����B�N�͐���4���B�����͂P���O���A�Q������B
 * ���Ԋ��\�͌��j�̂P���ڂ��P�s�ڂ���n�܂�A�Q�s�ڌ��j�Q���ځA�R�s�ڌ��j�R����...�Ƒ����A�ŏI�s�͓��j�V���ځB
 * ���Ԋ��\�̓��[�U�[���쐬�i��o����2019_1.txt = 2019�N�O���p�̃t�@�C����Y�t�j
 * �T�u�f�B���N�g���͌��Ȃ��E�ύX���Ȃ��i�X�L�b�v����j
 * ���t���Ȃ��t�@�C�����̃t�@�C���́AError�Ƃ���Log�Ɏc���t�@�C�����͏㏑�����Ȃ��B
 * �ύX�̓��O�ɋL�^�����Blogs�t�H���_�Ɏ��s���̓s�s���t����.txt�ō쐬����B
 * log�̓��e�͂ցu�ύX�O�̖��O�F�ύX��̖��O�v�B���O���s���̎��͕ύX�オError�ŋL�^�����B
 * ���l�[������t�@�C����datadir�t�H���_�֓����B���ڕύX����̂ŁA�t�@�C���̃o�N�A�b�v�𐄏��B
 * �ύX��datadir�t�H���_����t�@�C�������o���ΏI���B
 * �g���q�͉��ł��Ή��B�������A��x�̎��s�ň�����̂́A���ނ̊g���q�̂݁B�ŏ��Ɉ����g���q����͂���B
 *
 * ���j
 * Log�̈�ڂ̃f�[�^����ڂ̃��l�[�����s�͉B���t�@�C���������B
 * ���s���ʂ�C�t�@�C���̃l�[���O�Ȃ̂ŁAv01Shiftjis�����s�t�@�C�����ɂȂ��Ă���B
 * ��o����datadir�ɂ͓Y�t�̎��s���ʂ��o�͂����ۂ̌��̃t�@�C����Y�t�B�i���s��̃t�@�C���ł͂Ȃ��j
 * ���s��̃t�@�C���ɂ��Ă͌���datadir�t�H���_���Q�ƁB�i���ۂ̏������ɂ͊֌W���Ȃ��j
 */

typedef struct{
    int dayType;//��-��[0-6]
    int time;//x����[0-6]
    char year[5];//�N[1900-precent]
    char duaring;//�O��-���[0-1]
    char date[5];
}data;
const char *DATADIR = "./datadir";

data inparse(char[],char[]);

int main(void){
    data data;
    FILE *fp;
    char filename[11] = {'\0'};
    char str[5] = ".txt";
    char tmp[256];
    char log[25] = {'\0'};
    int classFileLoadFlag = 0;
    int MainLoopFlag = 0;
    int nameErrorFlag = 0;
    time_t t;
    struct tm *localt;
    int inttime,intdate;
    //�t�@�C������
    char path[_MAX_PATH];
    struct _finddata_t fdata;
    intptr_t fh;
    int n = 1;
    char *fileNew;
    char kakutyousi[10];
    int count = 0;

    printf("�g�p����t�@�C���̊g���q����͂��Ă��������B�@Ex�j�@txt\n");
    scanf("%s", kakutyousi);getchar();
    t = time(NULL);
    localt = localtime(&t);
    intdate = localt->tm_mday + (localt->tm_mon + 1) * 100 + (localt->tm_year + 1900) * 10000;
    inttime = localt->tm_sec + localt->tm_min * 100 + localt->tm_hour * 10000;
    sprintf(log, "./logs\\%d%d.txt",intdate, inttime);
    printf("���O�o�� %s\n", log);

    if((fp=fopen(log,"w"))==NULL){
        return 0;
    }else {
        fprintf(fp, "%s\n", "�Ȗږ��o��Log");
        fclose(fp);
    }

    //renameFileLoad (firstfile)
    sprintf(path, "%s\\*", DATADIR);
    if ((fh = _findfirst(path,&fdata)) == -1) return 1;

    do {
        //renameFileLoad
        if (n != 1) {
            do {
                if (_findnext(fh, &fdata) == 0) {
                    if ((fdata.attrib & _A_SUBDIR) == 0) {
                        sprintf(path, "%s\\%s", DATADIR, fdata.name);
                        printf("%d: %s\n", n, path);
                    }
                } else {
                    printf("�Ō�̃t�@�C���܂ŏI�����܂���\n");
                    MainLoopFlag = 1;
                }
            } while ((fdata.attrib & _A_SUBDIR) != 0);
        }
        if (MainLoopFlag == 1) {
            break;
        }
        n++;

        //inputFilename & parse
        data = inparse(log, fdata.name);
        sprintf(filename, "%s%c%c%s", data.year, '_', data.duaring, str);
        printf("%s\n", filename);

        //classNameLoad
        fp = fopen(filename, "r");
        if (fp == NULL) {
            printf("ERROR:FileOpenError\n");
            nameErrorFlag = 1;
            if ((fp = fopen(log, "a")) == NULL) {
            } else {
                fprintf(fp, "  Error noRename\n");
                fclose(fp);
            }
        }
        tmp[0] = '\0';
        while (classFileLoadFlag <= (data.dayType * 7 + data.time)) {
            if (classFileLoadFlag == (data.dayType * 7 + data.time)) {
                if (fgets(tmp, 256, fp) != NULL) {
                    printf("%s", tmp);
                }
            } else {
                fgets(tmp, 256, fp);
            }
            classFileLoadFlag++;
        }
        fclose(fp);
        classFileLoadFlag = 0;
        tmp[strlen(tmp) - 1] = '\0';

        //rename
        if(nameErrorFlag == 0) {
            do {
                fileNew = malloc(sizeof(char) * 256);
            } while (fileNew == NULL);
            fileNew[0] = '\0';
            sprintf(fileNew, "%s\\%s_%s.%s", DATADIR, tmp, data.date, kakutyousi);
            if (rename(path, fileNew) != 0) {
                do {
                    count++;
                    sprintf(fileNew, "%s\\%s_%s(%d).%s", DATADIR, tmp, data.date, count, kakutyousi);
                    if (count >= 100) {
                        printf("%s��%s�Ƀ��l�[���ł��܂���ł����B\n", path, fileNew);
                        if ((fp = fopen(log, "a")) == NULL) {
                        } else {
                            fprintf(fp, "can't Rename\n");
                            fclose(fp);
                        }
                        break;
                    }
                } while (rename(path, fileNew) != 0);
                if (count < 100) {
                    if ((fp = fopen(log, "a")) == NULL) {
                    } else {
                        fprintf(fp, "%s_%s(%d).%s\n", tmp, data.date, count, kakutyousi);
                        fclose(fp);
                    }
                }
            } else {
                if ((fp = fopen(log, "a")) == NULL) {
                } else {
                    fprintf(fp, "%s_%s(%d).%s\n", tmp, data.date, count, kakutyousi);
                    fclose(fp);
                }
            }
            if (count != 100) {
                printf("%s��%s�Ƀ��l�[�����܂����B\n", path, fileNew);
            }
            free(fileNew);
            count = 0;
        }
        nameErrorFlag = 0;
    }while(MainLoopFlag == 0);

    _findclose(fh);
    return  0;
}

//data convert
data inparse(char log[], char path[]){
    int a, i, intyear, intmonth, intday, intmd, inttime;
    FILE *fp;
    char *name;
    char year[5];
    char month[3];
    char day[3];
    char time[5];
    struct tm strtime;
    data data = {0,0,'0','0','0'};
    time_t ret;

    name = malloc(sizeof(char) * 256);
    if(name == NULL){
        printf("GetMemoryFailed");
        return data;
    }

    name = path;
    if((fp=fopen(log,"a"))==NULL){
    }else {
        fprintf(fp, "%s : ", name);
        fclose(fp);
    }
    a = 0;
    i = 0;
    while(a < 4){
        year[i] = name[a];
        a++;
        i++;
        if(i == 4){
            year[i] = '\0';
        }
    }
    i = 0;
    while(a < 6){
        month[i] = name[a];
        a++;
        i++;
        if(i == 2){
            month[i] = '\0';
        }
    }
    i = 0;
    while(a < 8){
        day[i] = name[a];
        a++;
        i++;
        if(i == 2){
            day[i] = '\0';
        }
    }
    i = 0;
    while(a < 12){
        time[i] = name[a];
        a++;
        i++;
        if(i == 4){
            time[i] = '\0';
        }
    }
    free(name);

//    printf("%s\n", year);
//    printf("%s\n", month);
//    printf("%s\n", day);
//    printf("%s\n", time);

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
//    printf("%c\n", data.duaring);
    strcat(data.date, month);
    strcat(data.date, day);
//    printf("%s : data.date\n", data.date);
    strtime.tm_year = intyear - 1900; /* �N[1900�N����N�Z] */
    strtime.tm_mon = intmonth - 1; /* ��[0-11] */
    strtime.tm_mday = intday; /* ��[1-31] */
    strtime.tm_hour = 0;/* ��[0-23] */
    strtime.tm_min = 0;/* ��[0-59] */
    strtime.tm_sec = 0; /* �b[0-61]�[�b�l�� */
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
//    printf("%d\n", data.dayType);

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
    return data;
}