all: lsh calc_w cluster search3

search3: search3.cpp input_output.hpp params.hpp
	g++ -g -o search3 search3.cpp

lsh: lsh.cpp input_output.hpp lsh_functions.hpp lsh_hash.hpp image.hpp hash.hpp params.hpp hash_group.hpp quicksort.hpp
	g++ -g -o search2 lsh.cpp

calc_w: calc_w.cpp image.hpp params.hpp
	g++ -g -o calc_w calc_w.cpp

cluster: silhouette.hpp median.hpp assign.hpp centroids.hpp cluster.cpp lsh.cpp input_output.hpp lsh_functions.hpp lsh_hash.hpp hash_group.hpp image.hpp hash.hpp params.hpp quicksort.hpp
	g++ -g -o cluster cluster.cpp

clean:
	rm lsh
	rm calc_w
	rm cluster
	