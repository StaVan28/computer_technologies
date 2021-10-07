all: launch_rd

comp_rd:
	g++ -Wall -Wextra main_rd.c -o main_rd

comp_wr:
	g++ -Wall -Wextra main_wr.c -o main_wr

launch_wr: comp_wr
	./main_wr 
	rm ./general_fifo > /dev/null

launch_rd: comp_rd
	./main_rd ./test.txt
	rm ./general_fifo > /dev/null
