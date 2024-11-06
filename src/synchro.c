#include "ensitheora.h"
#include "synchro.h"

/* les variables pour la synchro, ici */
pthread_mutex_t mS = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cTF = PTHREAD_COND_INITIALIZER;
pthread_cond_t cFT = PTHREAD_COND_INITIALIZER;
pthread_cond_t cC = PTHREAD_COND_INITIALIZER;
pthread_cond_t cD = PTHREAD_COND_INITIALIZER;

bool bTF = false;
bool bFT = false;

int nb_tex;

/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    pthread_mutex_lock(&mS);

    windowsx = buffer[0].width;
    windowsy = buffer[0].height;
    bTF = true;
    pthread_cond_signal(&cTF);

    pthread_mutex_unlock(&mS);
}

void attendreTailleFenetre() {
    pthread_mutex_lock(&mS);
    while (!bTF){    
        pthread_cond_wait(&cTF,&mS);
    }
    bTF = false;

    pthread_mutex_unlock(&mS);
}

void signalerFenetreEtTexturePrete() {
    pthread_mutex_lock(&mS);

    bFT = true;
    pthread_cond_signal(&cFT);

    pthread_mutex_unlock(&mS);
    
}

void attendreFenetreTexture() {
    pthread_mutex_lock(&mS);
    while (!bFT){    
        pthread_cond_wait(&cFT,&mS);
    }
    bFT = false;

    pthread_mutex_unlock(&mS);
}

void debutConsommerTexture() {
    pthread_mutex_lock(&mS);

    while (nb_tex == 0) {
        pthread_cond_wait(&cC, &mS);
    }

    pthread_mutex_unlock(&mS);

}

void finConsommerTexture() {
    pthread_mutex_lock(&mS);

    nb_tex += -1;
    pthread_cond_signal(&cD);

    pthread_mutex_unlock(&mS);

}

void debutDeposerTexture() {
    pthread_mutex_lock(&mS);

    while (nb_tex == NBTEX) {
        pthread_cond_wait(&cD, &mS);
    }

    pthread_mutex_unlock(&mS);

}

void finDeposerTexture() {
    pthread_mutex_lock(&mS);

    nb_tex += 1;
    pthread_cond_signal(&cD);

    pthread_mutex_unlock(&mS);

}
