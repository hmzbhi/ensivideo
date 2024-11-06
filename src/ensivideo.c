#include <SDL2/SDL.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "oggstream.h"
#include "stream_common.h"

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid_theora,tid_vorbis;

int main(int argc, char *argv[]) {
  int res;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE", argv[0]);
    exit(EXIT_FAILURE);
  }
  assert(argc == 2);

  // Initialisation de la SDL
  res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
  atexit(SDL_Quit);
  assert(res == 0);

  // Your code HERE
  // start the two stream readers (theoraStreamReader and vorbisStreamReader)
  // each in a thread
  pthread_create(&tid_theora,NULL,theoraStreamReader,(void*)argv[1]);
  pthread_create(&tid_vorbis,NULL,vorbisStreamReader,(void*)argv[1]);
  
  // wait for vorbis thread
  pthread_join(tid_vorbis,NULL);

  // 1 seconde of sound in advance, thus wait 1 seconde
  // before leaving
  sleep(1);

  // Wait for theora and theora2sdl threads

  pthread_cancel(tid_theora);
  pthread_join(tid_theora,NULL);
  
  pthread_cancel(tid_theora2sdl);
  pthread_join(tid_theora2sdl,NULL);

  // TODO
  /* liberer des choses ? */
  pthread_cancel(tid_vorbis);

  exit(EXIT_SUCCESS);
}