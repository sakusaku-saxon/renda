#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "GL/glut.h"
#include "GenericTimer.h"
#include "ImageData.h"
#include "ImageIO.h"
#include "main.h"

//ウインドウサイズ
int g_WindowWidth = 512;
int g_WindowHeight = 512;

//文字入力関連
int g_MessageStartX = 20;
int g_MessageStartY = 260;
#define BUFSIZE 5
char g_MessageBuffer[5];

//画像関連
#define IMAGE_FILE1 "1.ppm" //タイトル画面
#define IMAGE_FILE2 "2.ppm" //スタート画面
#define IMAGE_FILE3 "3.ppm" //プレイ画面
#define IMAGE_FILE4 "4.ppm" //終了画面
#define IMAGE_FILE5 "5.ppm" //結果発表画面
#define IMAGE_FILE6 "6.ppm" //ランキング画面
ImageData g_Image;
int g_ImagePosX = 0;
int g_ImagePosY = 0;
int g_CursorDelta = 10;

//ゲーム内容関連
int count=0; //スペースキーが押された回数
int counting=0; //スペースキーが押された回数のカウント中かどうか

//ハイスコア保存のための構造体
typedef struct{
  char name[5];
  int score;
} Result;

Result result[10];

int LoadStringFromFile(const char *filename, char *buf, int bufsize)
{
  int nReadTotal = 0;
  FILE *fp = fopen(filename, "r");

  if (fp == NULL)
    {
      fprintf(stderr, "LoadStringFromFile: cannot open %s\n", filename);
      return 0;
    }

  while ( ! feof(fp) && nReadTotal < bufsize)
    {
      fgets(&buf[nReadTotal], bufsize-nReadTotal, fp);
      nReadTotal += strlen(&buf[nReadTotal]);
    }

  fclose(fp);

  return 1;
}

/*自分で独自に指定した初期化処理を行う*/
void init(const char *filename){
  glClearColor(1,1,1,1);

  InitImageData(&g_Image);

  if( LoadPPMImage(filename, &g_Image) ){
    FlipImageData(&g_Image);
  }

  if ( ! LoadStringFromFile("hiscore.txt", g_MessageBuffer, BUFSIZE))
    sprintf(g_MessageBuffer, "Error: cannot open hiscore.txt");
}

/*描画処理用のコールバック関数を指定*/
void display(void){
  glClear(GL_COLOR_BUFFER_BIT);

  //画像のメモリが確保されているか確認し、画像を描画
  if(IsImageDataAllocated(&g_Image)){
    DrawImageData(&g_Image, g_ImagePosX, g_ImagePosY);
  }

  glutSwapBuffers(); /*ダブルバッファの切り替え*/

}

/*アイドル時(何も入力を受け付けていない時)の処理のためのコールバック関数を指定*/
void idle(void){
  display();
}

/*キーボード入力を受け付けるコールバック関数を指定*/
void keyboard(unsigned char key, int x, int y){
//  printf("key '%c' pressed at (%d, %d)\n", (char)key, x, y);

  switch(key){
  case 's': //ゲームのリスタート
    play();
    break;
  case 'q': //ゲーム終了
    exit(0);
    break;
  case ' ': //カウント
    if(counting == 1) count++;
    break;
  }
}

/*マウスクリック時に呼びだされるコールバック関数*/
void mouse(int button, int state, int x, int _y){
  int y = g_WindowHeight - _y;
  //printf("mouse clicked at (%d, %d)\n", x, y); //debug
}

/*文字列を表示する*/
void DisplayString(const char *str, int xi, int yi){
  int i, yStart = yi;

  glRasterPos2i(xi, yStart);

  for (i=0; str[i] != '\0'; i++){
    if(str[i] =='\n'){
      yStart -=24;
      glRasterPos2i(xi, yStart);
    } else {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }
  }
  glRasterPos2i(0,0);
}

/*画面サイズが変更された時の処理を行うコールバック関数を指定*/
void reshape(int width, int height){
  g_WindowWidth = width;
  g_WindowHeight = height;
}

void play(void){
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);

  init(IMAGE_FILE2); //スタート画面
  display();

  sleep(1);

  //カウント開始
  init(IMAGE_FILE3); //ゲーム画面
  display();
  if(counting != 1)
  {
    //カウント中でなければカウントを始める
    puts("start counting...");
    count = 0; //カウント回数をリセット
    counting = 1; //カウントしている状態にする
    glutTimerFunc(10 * 1000, timer, 0); //10 * 1000[ms]後にtimer()関数を呼び出す
  }

  sleep(2);
  resultpresent();
}

//カウント開始から10秒後に呼び出される関数
void timer(int value){
	counting = 0; //カウント終了
  init(IMAGE_FILE4); //終了画面
  display;
}

void resultpresent(void){
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);

  init(IMAGE_FILE5);
  display();
  printf("space key was pressed %d times\n", count);

  //ハイスコア1位から順に数値を比較、今回の結果が上回った場合はセーブさせる
  hiscoreload();
  for(int i=0; i<10; i++){
    if(count > result[i].score){
      hiscoresave(i);
      break;
    }
  }
  glColor3ub(0,0,0);
  DisplayString(g_MessageBuffer, g_MessageStartX, g_MessageStartY);
}

void hiscoreload(void){
  FILE *fp;
  int i;

  //result初期化
  for(i=0; i<10; i++){
    result[i].name == '\0';
    result[i].score = 0;
  }

  if((fp = fopen("hiscore.txt", "r")) == NULL){ //これまでの結果をオープン
    fprintf(stderr, "Cannot open hiscore.txt\n");
    exit(2);
  }
   for(i=0; i<10; i++){
     fscanf(fp, "%s %d\n", result[i].name, &result[i].score);
     printf("%s %d\n", result[i].name, result[i].score);
  }

   fclose(fp);
}

void hiscoresave(int i){ //i番目のハイスコアを更新
  FILE *fp;
  int j;
  char name[5];

  printf("Input your name: ");
  scanf("%s", name);
  putchar('\n');


  //ハイスコア更新のため場所をあける
  for(j=9; j>i;--j){
    result[j] = result[j-1];
  }
  //ハイスコアをコピー
  strcpy(result[i].name, name);
  result[i].score = count;

  //ファイルに書き込み
  if((fp = fopen("hiscore.txt", "w")) == NULL){
      fprintf(stderr, "Cannnot open hiscore.txt\n");
      exit(3);
  }
  for(j=0; j<10; j++){
    fprintf(fp, "%s %d\n", result[j].name, result[j].score);
    printf("%s %d\n", result[j].name, result[j].score);
  }

  fclose(fp);
}
