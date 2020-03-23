# fichier binaire :

g++ main.c -o main

# librairie dynamique :

g++ -c main.c # create helloworld.o
g++ -o main.so -shared main.o  
