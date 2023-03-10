dict3: dict3.o dictionary.o read.o quadtree.o 
	gcc -Wall -o dict3 dict3.o dictionary.o read.o quadtree.o -g

dict3.o: dict3.c dictionary.h read.h quadtree.h
	gcc -Wall -o dict3.o dict3.c -g -c


# Note that because we #include recurd_struct.c we need it in our dependencies.
dictionary.o: dictionary.c dictionary.h record_struct.c record_struct.h
	gcc -Wall -o dictionary.o dictionary.c -g -c

read.o: read.c read.h record_struct.c record_struct.h
	gcc -Wall -o read.o read.c -g -c

quadtree.o: quadtree.c quadtree.h dictionary.c dictionary.h record_struct.c record_struct.h
	gcc -Wall -o quadtree.o quadtree.c -g -c 

result_struct.o: result_struct.c result_struct.h dictionary.c dictionary.h
	gcc -Wall -o result_struct.o result_struct.c -g -c 



dict4: dict4.o dictionary.o read.o quadtree.o result_struct.o
	gcc -Wall -o dict4 dict4.o dictionary.o read.o quadtree.o result_struct.o -g

dict4.o: dict4.c dictionary.h read.h quadtree.h result_struct.h
	gcc -Wall -o dict4.o dict4.c -g -c
