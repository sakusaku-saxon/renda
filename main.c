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
extern int g_WindowWidth;
extern int g_WindowHeight;

//画像関連
#define IMAGE_FILE1 "1.ppm" //タイトル画面
#define IMAGE_FILE2 "2.ppm" //スタート画面
#define IMAGE_FILE3 "3.ppm" //プレイ画面
#define IMAGE_FILE4 "1.ppm" //終了画面
#define IMAGE_FILE5 "2.ppm" //結果発表画面
#define IMAGE_FILE6 "3.ppm" //名前入力画面

int main(int argc, char *argv[]){
  glutInit(&argc, argv); //GLUTの初期化
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //各種設定
  glutInitWindowSize(g_WindowWidth, g_WindowHeight); //最初のウインドウサイズを指定
  glutCreateWindow("RENDA"); //ウインドウのタイトルを指定

  glutDisplayFunc(display); //描画処理用のコールバック関数を指定
  glutIdleFunc(idle); //アイドル時の処理のためのコールバック関数を指定
  glutKeyboardFunc(keyboard); //キーボード入力を受け付けるコールバック関数を指定
  glutMouseFunc(mouse); //マウスがクリックされた時の処理のためのコールバック関数を指定
  glutReshapeFunc(reshape); //画面サイズが変更された時の処理を行うコールバック関数を指定

  glutIgnoreKeyRepeat(GL_TRUE); //キーリピートを無効にする
  init(IMAGE_FILE1); //自分で独自に指定した初期化処理を行う

  glutMainLoop(); //イベント待ち受けループに入る

  return 0;
}
