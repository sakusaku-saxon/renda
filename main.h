#ifndef __MAIN_H__
#define __MAIN_H__

int LoadStringFromFile(const char *filename, char *buf, int bufsize);
void init(const char *filename);
void display(void);
void idle(void);
void keyboard(unsigned char key, int x, int y);
void inputkeyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int _y);
void DisplayString(const char *str, int xi, int yi);
void reshape(int width, int height);
void play(void);
void resultpresent(void);
void hiscoreload(void);
void hiscoresave(int i);

#endif
