#define _DEFAULT_SOURCE

#include "tests.h"
#include "mem.h"
#include "mem_internals.h"
#include "util.h"
static struct block_header* block_get_header(void* contents) {
  return (struct block_header*) (((uint8_t*)contents)-offsetof(struct block_header, contents));
}
static void  heapInitError(char* const test){
        err("%s CANT INITIALIZE HEAP\n\n", test);
}
static void  mallocError(char* const test){
        err("%s CANT ALLOC\n\n", test);
}
static void freeError(char* const test){
        err("%s CANT FREE BLOCK\n\n", test);

}
static void mmapError(char* const test){
        err("%s CANT MMAP\n\n", test);
}
static bool  checkIfBlockIsValid(void* block, size_t expectedSize){
        struct block_header* header = (block_get_header(block));
        return (block&&!header->is_free && (header->capacity).bytes==expectedSize);
}
static bool checkIfBlockIsFree(void* block){
        return(block_get_header(block)->is_free);
}
static void destroy(void* heap, size_t sz) {
    munmap(heap, size_from_capacity((block_capacity) {.bytes = sz}).bytes);
}

static bool test_1(){
        printf("----- TEST 1 -----\n");
        void* heap = heap_init(343);
        if(!heap){
                heapInitError("TEST 1");
                return false;
        }
        debug_heap(stdout,heap);
        void* block = _malloc(321);
       
        if(!checkIfBlockIsValid(block,321)){
                mallocError("TEST 1");
                return false;

        }
        debug_heap(stdout,heap);
        munmap(heap, 8192);
        printf("COMPLETED SUCCESSFULLY\n\n");
        return true;


}
/*Освобождение одного блока из нескольких выделенных.*/
static bool test_2(){
        printf("----- TEST 2 -----\n");
        void* heap = heap_init(3123);
        if(!heap){
                heapInitError("TEST 2");
                return false;
        }
        void* first = _malloc(100);
        void* second = _malloc(100);
        void* third = _malloc(100);
        if (!checkIfBlockIsValid(first,100) || !checkIfBlockIsValid(second,100) || !checkIfBlockIsValid(third,100)){
                mallocError("TEST 2");
                return false;
        }
        debug_heap(stdout,heap);
        _free(second);
        if(!block_get_header(second)->is_free){
                freeError("TEST 2");
                return false;
        }
        debug_heap(stdout,heap);
        munmap(heap, 8192);
        printf("COMPLETED SUCCESSFULLY\n\n");
        return true;

        
}
/*Освобождение двух блоков из нескольких выделенных.*/
static bool test_3(){
        printf("----- TEST 3 -----\n");
        void* heap = heap_init(3123);
        if(!heap){
                heapInitError("TEST 3");
                return false;
        }
        void* first = _malloc(100);
        void* second = _malloc(100);
        void* third = _malloc(100);
        if (!checkIfBlockIsValid(first,100) || !checkIfBlockIsValid(second,100) || !checkIfBlockIsValid(third,100)){
                mallocError("TEST 3");
                return false;
        }
        debug_heap(stdout,heap);
        _free(second);
        if(!checkIfBlockIsFree(second)){
                freeError("TEST 3");
                return false;
        }
        debug_heap(stdout,heap);
        _free(first);
        if(!checkIfBlockIsFree(first)){
                freeError("TEST 3");
                return false;
        }
        debug_heap(stdout,heap);
        munmap(heap, 8192);
        printf("COMPLETED SUCCESSFULLY\n\n");
        return true;

        
}
/*Память закончилась, новый регион памяти расширяет старый.*/
static bool test_4(){
        printf("----- TEST 4 -----\n");
        void* heap = heap_init(3123);
        if(!heap){
                heapInitError("TEST 4");
                return false;
        }
        void* first= _malloc(8192);
         if (!checkIfBlockIsValid(first,8192)){
                mallocError("TEST 4");
                return false;
        }
        debug_heap(stdout,heap);
        void* second = _malloc(100);
        if(!checkIfBlockIsValid(second,100)){
                mallocError("TEST 4");
                return false;
        }
        debug_heap(stdout,heap);
        destroy(heap, (((struct block_header*)heap)->capacity).bytes); 
        printf("COMPLETED SUCCESSFULY\n\n");
        return true;

 }
/*Память закончилась, старый регион памяти не расширить из-за другого выделенного диапазона адресов, новый регион выделяется в другом месте.*/
static bool test_5(){
        printf("----- TEST 5 -----\n");
        void* heap = heap_init(123);
        if(!heap){
                heapInitError("TEST 5");
                return false;
        }
        debug_heap(stdout,heap);
        if(mmap(HEAP_START + 8192, REGION_MIN_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE |MAP_FIXED|MAP_ANONYMOUS, -1, 0)==MAP_FAILED){
                mmapError("TEST 5");
                return false;
        }
        debug_heap(stdout,heap);
        void* second = _malloc(8192+5000);
        if(!checkIfBlockIsValid(second,8192+5000)){
                mallocError("TEST 5");
                return false;
        }
        debug_heap(stdout,heap);
        destroy(heap, (((struct block_header*)heap)->capacity).bytes); 
        printf("COMPLETED SUCCESSFULY\n\n");
        return true;

}

void run_tests(){
        static bool (*tests[])() = {
                test_1,test_2,test_3,test_4,test_5
        };
        int count=0;
        for (size_t i = 0; i < 5; i++)
        {
                if(tests[i]()){
                        count+=1;
                }
                
        }
        printf("SUCCESSFULY COMPLETED %d/5 TESTS\n\n", count);
        


}