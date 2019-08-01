#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <io.h>

/*
 * 授業時の記録ファイルを科目名と日付に自動でリネームするプログラム（複数ファイル対応）
 * 年日付時刻に応じて、自動であらかじめ作成した時間割表を選択し、対応した科目を取得。科目名と日付に変更する。
 * 同名になるものは最後に（n）をつける。（nは1からの整数で重複があるごとに値が１増える。
 * 時間割表は１から７限目まで。時間は近代の時間割を基準に、前の休み時間から授業の終了時刻までで区切る。
 * 時間割表の命名規則は年_数字。年は西暦4桁。数字は１が前期、２が後期。
 * 時間割表は月曜の１限目が１行目から始まり、２行目月曜２限目、３行目月曜３限目...と続き、最終行は日曜７限目。
 * 時間割表はユーザーが作成（提出時は2019_1.txt = 2019年前期用のファイルを添付）
 * サブディレクトリは見ない・変更しない（スキップする）
 * 日付がないファイル名のファイルは、ErrorとしてLogに残しファイル名は上書きしない。
 * 変更はログに記録される。logsフォルダに実行時の都市日付時刻.txtで作成さる。
 * logの内容はへ「変更前の名前：変更後の名前」。名前が不正の時は変更後がErrorで記録される。
 * リネームするファイルはdatadirフォルダへ入れる。直接変更するので、ファイルのバクアップを推奨。
 * 変更後datadirフォルダからファイルを取り出せば終了。
 * 拡張子は何でも対応。ただし、一度の実行で扱えるのは、一種類の拡張子のみ。最初に扱う拡張子を入力する。
 *
 * 注）
 * Logの一つ目のデータや一回目のリネーム失敗は隠しファイルが原因。
 * 実行結果はCファイルのネーム前なので、v01Shiftjisが実行ファイル名になっている。
 * 提出時のdatadirには添付の実行結果を出力した際の元のファイルを添付。（実行後のファイルではない）
 * 実行後のファイルについては結果datadirフォルダを参照。（実際の処理時には関係しない）
 */

typedef struct{
    int dayType;//月-日[0-6]
    int time;//x限目[0-6]
    char year[5];//年[1900-precent]
    char duaring;//前期-後期[0-1]
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
    //ファイル制御
    char path[_MAX_PATH];
    struct _finddata_t fdata;
    intptr_t fh;
    int n = 1;
    char *fileNew;
    char kakutyousi[10];
    int count = 0;

    printf("使用するファイルの拡張子を入力してください。　Ex）　txt\n");
    scanf("%s", kakutyousi);getchar();
    t = time(NULL);
    localt = localtime(&t);
    intdate = localt->tm_mday + (localt->tm_mon + 1) * 100 + (localt->tm_year + 1900) * 10000;
    inttime = localt->tm_sec + localt->tm_min * 100 + localt->tm_hour * 10000;
    sprintf(log, "./logs\\%d%d.txt",intdate, inttime);
    printf("ログ出力 %s\n", log);

    if((fp=fopen(log,"w"))==NULL){
        return 0;
    }else {
        fprintf(fp, "%s\n", "科目名出力Log");
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
                    printf("最後のファイルまで終了しました\n");
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
                        printf("%sを%sにリネームできませんでした。\n", path, fileNew);
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
                printf("%sを%sにリネームしました。\n", path, fileNew);
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
    strtime.tm_year = intyear - 1900; /* 年[1900年から起算] */
    strtime.tm_mon = intmonth - 1; /* 月[0-11] */
    strtime.tm_mday = intday; /* 日[1-31] */
    strtime.tm_hour = 0;/* 時[0-23] */
    strtime.tm_min = 0;/* 分[0-59] */
    strtime.tm_sec = 0; /* 秒[0-61]閏秒考慮 */
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