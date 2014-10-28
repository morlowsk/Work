#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct cache_line {
	    int valid;
	    long int tag;
	    int timestamp;
};
typedef struct cache_line cache_line_t;

typedef struct set{
        cache_line_t *lines;
};
typedef struct set set_t;

typedef struct cache {
        set_t *sets;
        int num_lines;
        int associativity;
        int block_size;
	int num_sets;
	int lines_per_set;
};
typedef struct cache cache_t; 

cache_t *make_cache();
set_t *make_set(int numlines_per_set);
cache_line_t *make_lines(int num_of_lines);

int get_index_bits(int numlines_lines_per_set );
long int get_index(int index_bits, long addr_req, int start_of_index);
int get_offset_bits(int num_blocks);
long int get_offset(int offset_bits, long addr_req);
long int get_tag(long addr_req, int start_of_tag);
void free_cache(cache_t *cache, int numlines, int lines_per_set);

int main (int argc, char *argv[]) {
	
    int num_lines = atoi(argv[1]), //total number of lines
    lines_per_set = atoi(argv[2]), 
    bytes_per_block = atoi(argv[3]);

    char line[80];

    long addr_req;

    printf("Simulating cache with:\n");
    printf(" - Total lines   = %d\n", num_lines);
    printf(" - Lines per set = %d\n", lines_per_set);
    printf(" - Block size    = %d bytes\n", bytes_per_block);

    cache_t *cache = make_cache(); //malloc used
    cache->sets = make_set((num_lines / lines_per_set)); //malloc used
    cache->num_lines = num_lines; //total number of lines in the cache
    if (lines_per_set == 1) {
    	cache->associativity = 0;// direct mapping ftm (0 is no associativity, else 1)
    }
    else { 
	cache->associativity = 1;
    }
    cache->block_size = bytes_per_block;
    cache->num_sets = num_lines / lines_per_set; //the number of sets and the number of lines per set are the same (right...?)
    cache->lines_per_set = lines_per_set;  
    
    int k;
    for (k = 0; k < (num_lines/lines_per_set); k++){
	cache->sets[k].lines = make_lines(lines_per_set);
	int j;
	for (j = 0; j < lines_per_set; j++){
		cache->sets[k].lines[j].valid = 0; 
		cache->sets[k].lines[j].tag = -1;	
		cache->sets[k].lines[j].timestamp = 0;	
	}
    }

    int hits = 0;
    int misses = 0;
    float hit_rate = 0.0;
    float miss_rate = 0.0;
   
   int num_offset_bits = get_offset_bits(bytes_per_block);//number of blocks...
   int num_index_bits = get_index_bits((num_lines / lines_per_set));

 

    while (fgets(line, 80, stdin)) {
        
	addr_req = strtol(line, NULL, 0);
	
	long int offset;
    	int start_of_index;
	
    	if (num_offset_bits != 0){
		offset = get_offset(num_offset_bits, addr_req);//will get the actual block to goto
		start_of_index = num_offset_bits;
    	}
    	else {
	    start_of_index = 0;
    	}
    
    	int start_of_tag;
    	long int index = get_index(num_index_bits, addr_req, start_of_index);

    	if (num_index_bits > 1) {
	    start_of_tag = start_of_index + num_index_bits;
    	}
    	else {
	    start_of_tag = num_index_bits;
    	}
    	long int tagg = get_tag(addr_req, start_of_tag);

        /* simulate cache fetch with address `addr_req` */
        printf("Processing request: 0x%lX\n", addr_req);
	//this is code for a set associative mapping
       if (cache->associativity == 1) {		
		int c = 0;
		int counter = 0;
		for (c = 0; c<lines_per_set; c++) {
			if (cache->sets[index].lines[c].tag == tagg && cache->sets[index].lines[c].valid == 1) { //there never will be a -1 tag
				//printf("Hit!\n");
				cache->sets[index].lines[c].timestamp++;  
				hits++;
				break;
			}	      
			counter++;
		}
		if (counter == lines_per_set) { //if you miss, you need to find the smallest timestamp in order to follow LRU
			//printf("Miss!\n");
			int loc = 0;
			int minimum = cache->sets[index].lines[loc].timestamp;	
			for (c = 1; c < lines_per_set; c++){
				if (cache->sets[index].lines[c].timestamp < minimum){
					minimum = cache->sets[index].lines[c].timestamp;
					loc = c;	
				}
			}
			cache->sets[index].lines[loc].tag = tagg;
			cache->sets[index].lines[loc].timestamp++;
		        cache->sets[index].lines[loc].valid = 1; 	
			misses++;
		}
	}
	
       else if (cache->associativity == 0) { //direct mapping
		if (cache->sets[index].lines->tag == tagg && cache->sets[index].lines->valid == 1) {
			//printf("Hit!\n");
			//cache->sets[index].lines->timestamp++;  
			hits++;
		}	

		else { 
			//printf("Miss!\n");
			cache->sets[index].lines->tag = tagg;
			//cache->sets[index].lines.timestamp++; //will we really need this?
		        cache->sets[index].lines->valid = 1; 	
			misses++;
		}
	}
    }		
    hit_rate = 100*((float) hits / (hits + misses));
    miss_rate = 100*((float) misses / (hits + misses));
    printf("Hit rate: %.02f | Miss rate: %.02f\n", hit_rate, miss_rate);

    free_cache(cache, num_lines, lines_per_set);
    return 0;
}

    void free_cache(cache_t *cache, int num_lines, int lines_per_set) {
	int k;    
    	for (k = 0; k < (num_lines/lines_per_set); k++){//free each set
		free(cache->sets[k].lines);
	}
	       	free(cache->sets);
		free(cache);
	}	

    int get_index_bits(int numlines_lines_per_set) {
	/* float f_lines = (float) numlines_lines_per_set;
	int index_bits = (int) log2f(f_lines); */
	int index_bits = 0;
       	int remainder = numlines_lines_per_set;
	while (1){ //was rem != 1

		remainder = remainder / 2;
		if (remainder == 0) {
			break;
		}
		else index_bits++;
	}
        	return index_bits;
	}

    long get_index(int index_bits, long addr_req, int start_of_index){
	    /* int i = 0; //counter
	    char arr[index_bits];
	    int length = index_bits - 1;
	    long mask = (1 >> i);
	    long addr_req_bit = (addr_req >> start_of_index);
	    int BLEGH;
	    for (i = 0; i < index_bits; i++) {	    
		   BLEGH = (addr_req_bit >> i); //was addr_req,was i
		   long result = BLEGH & mask;
		   if (result == 0) {
			arr[length] = '0';
		   }
		   else if (result == 1) {
			arr[length] = '1';
		   }
		   length--;
	    } 
	    long index = strtol(arr,NULL,2); */
	    long addr_req_bit = (addr_req >> start_of_index);
	    long index = addr_req_bit % (1 << index_bits);
	    return index;
    }

    long int get_tag(long addr_req, int start_of_tag){
	long int tag = (addr_req >> start_of_tag);
 	return tag;		
    }


    int get_offset_bits(int num_blocks) {
	/*float f_blocks = (float) num_blocks;
	int offset_bits = (int) log2f(f_blocks);*/
	int offset_bits = 0;
       	int remainder = num_blocks;
	while (1){
		remainder = remainder / 2;
		if (remainder == 0) {
			break;
		}
		else offset_bits++;
	}
		return offset_bits;
	}

    long int get_offset(int offset_bits, long addr_req){
	    /* int i = 0; //counter
	    char arr[offset_bits];
	    int length = offset_bits - 1;
	    long mask = (1 >> i);
	    for (i = 0; i < offset_bits; i++) {	    
		   long addr_req_bit = (addr_req >> i);
		   int result = addr_req_bit & mask;
		   if (result == 0) {
			arr[length] = '0';
		   }
		   else if (result == 1) {
			arr[length] = '1';
		   }
		   length--;
	    } 
	    long int offset = strtol(arr,NULL,2); */
	    // long addr_req_bit = (addr_req >> offset_bits); DON'T NEED
	    long offset = addr_req % (1 << offset_bits);
	    return offset;
    }

    cache_t *make_cache() {
	    return malloc(sizeof(cache_t));
	}

    set_t *make_set(int numlines_lines_per_set ) {
	    return malloc(sizeof(set_t)*(numlines_lines_per_set));
	}
    cache_line_t *make_lines(int lines_per_set) {
	    return malloc(sizeof(cache_line_t)*lines_per_set);
	}

