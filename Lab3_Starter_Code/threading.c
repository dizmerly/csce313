#include <threading.h>

void t_init(void)
{
        // TODO
        // set each state to invalid
        for(int i = 0; i < NUM_CTX; i++){
                contexts[i].state = INVALID;
        }
        // set main state to context zero
        contexts[0].state = VALID; 
        current_context_idx = 0; 

}

int32_t t_create(fptr foo, int32_t arg1, int32_t arg2)
{
        // TODO
        // Find first available slot for worker to work in
        int first_available_idx = -1;
        for(int i = 1; i < NUM_CTX; i++){
                if(contexts[i].state == INVALID){
                        first_available_idx = i; 
                        break; 
                }
        }
        
        // check if its possible to create worker
        if(first_available_idx == -1) return 1; 

        
      
}

int32_t t_yield()
{
        // TODO
}

void t_finish()
{
        // TODO
}
