#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

#include "student_code.h"
#include "server.h"
#include <stdint.h>

/*
 * This is tested by running two requests at the same time for different length of time.
 * e.g. 2-second and 3-second.  If it takes 5s then they aren't running concurrently
 *   but if it takes 3 seconds then they are.
 * Test a wide range of random times.
 */
void pass_to_client_handler(void* arg) {
  // todo: Convert this to run multi-threaded
  if(DO_CONCURRENT){
    pthread_t thread;
    pthread_create(&thread, NULL, client_handler, arg);
  }else{
    client_handler(arg);
  }
}

/**
 * Using the database lock, make sure that we can safely read, and adjust variables as needed.
 * Note that this requires checking the number of current readers and writers, and using the appropriate condition variable.
 * @param db
 */
/*int sbuf_read(SBUF *sbuf) {
  pthread_mutex_lock(&sbuf->lock);

  while (sbuf->count != 1) {
	pthread_cond_wait(&sbuf->read_go, &sbuf->lock);
  }

  // do the read
  int var = sbuf->buf;
  sbuf->count = 0;

  // let the writer know
  pthread_cond_signal(&sbuf->write_go);

  pthread_mutex_unlock(&sbuf->lock);
  return var;
}
*/
void read_lock(PlayerDatabase* db) {
  //need to lock first
  //cond
  //signal
  //wait inside loop - USE WHILE LOOP
  //if we can read increase readers, but after we loop bc it will be infinite if not
  // todo
  pthread_mutex_lock(db->mutex);
    while (db->writers > 0 || db->readers >= MAX_CONCURRENT_READERS) {
        pthread_cond_wait(db->can_read, db->mutex);
    }
    db->readers++;
    //pthread_cond_signal(&sbuf->write_go);
    //if we have readers, signal we can write?
    pthread_cond_signal(db->can_write);

    pthread_mutex_unlock(db->mutex);
  // Question: What variables do we need to check before we know we can safely read?
}

/**
 * Release the lock and adjust any variables that are necessary
 * Note that this requires checking the number of current readers and writers, and using the appropriate condition variable.
 * @param db
 */
void read_unlock(PlayerDatabase* db) {
  // todo
  //if we are unlocking our readers decrease readerrs 
  //almost seems opposite
  //check while we have readers or writers for all
  pthread_mutex_lock(db->mutex);
    db->readers--;
    if (db->readers == 0) {
        pthread_cond_signal(db->can_write);
    }
    pthread_cond_broadcast(db->can_read);

    pthread_mutex_unlock(db->mutex);
}
/**
 * Using the database lock, make sure that we can safely write, and adjust variables as needed.
 * Note that this requires checking the number of current readers and writers, and using the appropriate condition variable.
 * @param db
 */
void write_lock(PlayerDatabase* db) {
  // todo
  pthread_mutex_lock(db->mutex);
    while (db->writers > 0 || db->readers > 0) {
        pthread_cond_wait(db->can_write, db->mutex);
    }
    db->writers++;

    pthread_mutex_unlock(db->mutex);
}

/**
 * Release the lock and adjust any variables that are necessary
 * Note that this requires checking the number of current readers and writers, and using the appropriate condition variable.
 * @param db
 */
void write_unlock(PlayerDatabase* db) {
  // todo
  pthread_mutex_lock(db->mutex);
    db->writers--;
    pthread_cond_signal(db->can_write);

    pthread_cond_broadcast(db->can_read);

    pthread_mutex_unlock(db->mutex);
}


// Add a new player
int add_player(PlayerDatabase* db, char* player_name) {
  write_lock(db);
  int return_val = _add_player(db, player_name);
  write_unlock(db);
  return return_val;
}

// Update stats for a particular player
int add_player_score(PlayerDatabase* db, char* player_name, int score) {
  // Question: Do we realistically need to lock the entire database for this operation?
  write_lock(db);
  int return_val = _add_player_score(*db, player_name, score);
  write_unlock(db);
  return return_val;
}

// Get stats for a specific player
int get_player_plays(PlayerDatabase* db, char* player_name) {
  read_lock(db);
  int return_val = _get_player_plays(*db, player_name);
  read_unlock(db);
  return return_val;
}

int get_player_high_score(PlayerDatabase* db, char* player_name) {
  read_lock(db);
  int return_val = _get_player_high_score(*db, player_name);
  read_unlock(db);
  return return_val;
}

// Get general statistics
char* get_best_player(PlayerDatabase* db) {
  read_lock(db);
  char* return_val = _get_best_player(*db);
  read_unlock(db);
  return return_val;
}
int get_num_players(PlayerDatabase* db) {
  read_lock(db);
  int return_val = _get_num_players(*db);
  read_unlock(db);
  return return_val;
}
int get_highest_score(PlayerDatabase* db) {
  read_lock(db);
  int return_val = _get_highest_score(*db);
  read_unlock(db);
  return return_val;
}

int get_total_plays(PlayerDatabase* db) {
  read_lock(db);
  int return_val = _get_total_plays(*db);
  read_unlock(db);
  return return_val;
}


