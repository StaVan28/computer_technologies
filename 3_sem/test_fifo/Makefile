all: launch_wr

comp_rd:
	g++ -Wall -Wextra main_rd.c fifo_config.c fifo_config.h -o main_rd

comp_wr:
	g++ -Wall -Wextra main_wr.c fifo_config.c fifo_config.h -o main_wr

launch_wr: comp_wr
	./main_wr ./test.txt
	@ rm ./general_fifo ./secr_fifo 2> /dev/null; exit 0

launch_rd: comp_rd
	./main_rd
	@ rm ./general_fifo ./secr_fifo 2> /dev/null; exit 0

clean:
	rm main_rd main_wr ./general_fifo ./secr_fifo 2> /dev/null; exit 0