G=Prog3group

all:$(G)

clean:
	rm -rf $(G).o
	rm -rf $(G)
	
$(G).o:$(G).cpp
	g++ -c $(G).cpp -o $(G).o

$(G):$(G).o
	g++ $(G).o -o $(G)

