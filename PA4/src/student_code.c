
#include "student_code.h"

void finalize_stats(SCHEDULER_STATS* stats) {
    // todo
    //compute the averages
    //float
    stats->average_turnaround_time = (float)stats->sum_of_turnaround_times / stats->num_processes_started;
    stats->average_response_time = (float)stats->sum_of_response_time / stats->num_processes_completed;
}


void mark_process_start(SCHEDULER_STATS* stats, PROCESS* p, float curr_time, float time_slice) {
    // todo
    p->process_state = PROC_READY;
    stats->num_processes_started++;
    stats->sum_of_response_time += (curr_time - p->entry_time);
    //done
}

void mark_process_run(SCHEDULER_STATS* stats, PROCESS* p, float curr_time, float time_slice) {
    // todo
    p->process_state = PROC_RUNNING;
    p->time_remaining -= time_slice;
    //stats
}

void mark_process_end(SCHEDULER_STATS* stats, PROCESS* p, float end_time, float time_slice_remaining) {
    // todo
    p->process_state = PROC_BLOCKED;
    stats->num_processes_completed++;
    stats->sum_of_turnaround_times += (end_time - p->entry_time);

}

int compare_priority(PROCESS p1, PROCESS p2) {
    return (int)p1.priority - (int)p2.priority;
}

PROCESS* priority_process_selector(PROCESS_LIST* pl) {
    PROCESS *p;
    p = get_minimum(pl, compare_priority);
    return p;
}

int compare_fifo(PROCESS p1, PROCESS p2){
    if (p1.entry_time < p2.entry_time){
        return -1; // if 0 < 2 p1 is smaller
    }else if(p1.entry_time > p2.entry_time){
        return 1;
    }else{
        return 0;
    }
}

PROCESS* fifo_process_selector(PROCESS_LIST* pl) {
    PROCESS * p;
    p = get_minimum(pl, compare_fifo);
    return p;
}

PROCESS* rr_process_selector(PROCESS_LIST* pl) {
    PROCESS* p;
    // todo
    p = get_next(pl);
    return p;
}

int compare_sjf(PROCESS p1, PROCESS p2){
    if (p1.length < p2.length){
        return -1; //shorter length
    }else if( p1.length > p2.length){
        return 1;
    }else{
        return 0;
    }
}

PROCESS* sjf_process_selector(PROCESS_LIST* pl) {
    PROCESS* p;
    p = get_minimum(pl, compare_sjf);
    return p;
 }

int compare_lifo(PROCESS p1, PROCESS p2){
    if (p1.entry_time > p2.entry_time){
        return -1; // if 0 < 2 p1 is smaller
    }else if(p1.entry_time < p2.entry_time){
        return 1;
    }else{
        return 0;
    }
}
PROCESS* lifo_process_selector(PROCESS_LIST* pl) {
    PROCESS* p;
    p = get_minimum(pl, compare_lifo);
    return p;
}

int compare_stcf(PROCESS p1, PROCESS p2){
    if(p1.time_remaining < p2.time_remaining){
        return -1;
    }else if(p1.time_remaining > p2.time_remaining){
        return 1;
    }else{
        return 0;
    }
}
PROCESS* stcf_process_selector(PROCESS_LIST* pl) {
    PROCESS *p;
    p = get_minimum(pl, compare_stcf);
    return p;
    
}

