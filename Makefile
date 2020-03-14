TARGET=renda

$(TARGET): GenericTimer.o ImageData.o ImageIO.o main.o func.o
	cc -o $(TARGET) GenericTimer.o ImageData.o ImageIO.o main.o func.o -L. -lglut -lGLU -lGL -lXi -lXrandr -lm

.c.o:
	cc -c $< -I. -Wall

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o *.c~ *.h~ $(TARGET)
