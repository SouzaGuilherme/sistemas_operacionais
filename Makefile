GPLUSPLUS= g++
NAME_OUT="producer_consumer"
NAME_IN="code_producer_consumer.cpp"
FLAGS=-lpthread -g

all:
	$(GPLUSPLUS) $(NAME_IN) -o $(NAME_OUT) $(FLAGS)

clean:
	rm $(NAME_OUT)
