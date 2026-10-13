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
                else if(contexts[i].state == DONE){
                        first_available_idx = i; 
                        break; 
                }
        }
        
        // check if its possible to create worker
        if(first_available_idx == -1) return 1; 
        if(first_available_idx == current_context_idx) return 1;

        contexts[first_available_idx].state = VALID;
        // link to main context
        contexts[first_available_idx].context.uc_link = &contexts[0].context; 

        // create context with function and args
        makecontext(&contexts[first_available_idx].context, 
                (void (*)(void))foo, 2, arg1, arg2); 

        return 0; 
}       

int32_t t_yield(void)
{
        // TODO

        // Find next valid context to yield to
        int next_valid_idx = -1;
        for(int i = 1; i < NUM_CTX; i++){
                if(contexts[i].state == VALID && i != current_context_idx){
                        next_valid_idx = i; 
                        break; 
                }
        }
        if(next_valid_idx == current_context_idx) return 1;
        if(next_valid_idx == -1) return 1;

        // save callers context into current context
        contexts[current_context_idx].context.uc_link = &contexts[0].context;
        swapcontext(&contexts[next_valid_idx].context,
                &contexts[current_context_idx].context);
        // restore context from next


        return 0; 

}

void t_finish(void)
{
        // TODO

        // free the entry in slot current
        contexts[current_context_idx].state = INVALID;  
        
        // find next valid slot 
        int next_valid_idx = -1;
        for(int i = 1; i < NUM_CTX; i++){
                if(contexts[i].state == VALID && i != current_context_idx){
                        next_valid_idx = i; 
                        break; 
                }
        }
        if(next_valid_idx == current_context_idx) return;
        if(next_valid_idx == -1) return;

        // set context with proctable[next] 
        setcontext(&contexts[next_valid_idx].context);
}
