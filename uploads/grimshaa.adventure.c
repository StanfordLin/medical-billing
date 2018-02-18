/*
////////////
// Sources//
////////////
 */
#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#include <dirent.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct listItem {
  struct Room* roomptr;
  struct listItem* next;
};

struct player{
  struct Room* currentRoom;
  struct listItem* head;
};
struct Room{
  char* name;
  char* roomType;
  struct listItem* head;
  int connectionNumber;
};
/**
 * [mostRecent counts number of steps taken by player and prints them]
 * @param x [player struct passed with list of steps taken]
 */
void mostRecent(struct player* x) {
  struct listItem* listIter = x->head;
  int stepCount=1;
  while(x->head != NULL) {
    stepCount++;
    if(listIter->next == NULL) {
      break;
    }
    listIter = listIter->next;
  }
  printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n",stepCount);
  listIter = x->head;
  while(x->head != NULL) {
    printf("%s\n",listIter->roomptr->name);
    if(listIter->next == NULL) {
      break;
    }
    listIter = listIter->next;
  }
  printf("%s\n",x->currentRoom->name);
}
/**
 * [cmpChars tells if 2 strings of equal length contain the same chars]
 * @param  buff [1st string in comparison]
 * @param  name [2nd string in comparison]
 * @return      [returns 0 for valid string, 1 for invalid]
 */
int cmpChars(char* buff,char *name) {
  int k;
  for(k=0; k <= strlen(buff); k++) {
    if(buff[k] != name[k]) {
      return 0;
    }
  }
  return 1;
}
/**
 * [ConnectRoom adds room y to list of rooms pointed to by head]
 * @param head [pointer to list of rooms]
 * @param y    [pointer to a room to add to list]
 */
void ConnectRoom(struct listItem** head, struct Room* y){
  if ((*head) == NULL){
    struct listItem* new = (struct listItem*)malloc(sizeof(struct listItem));
    new->roomptr = y;
    new->next = NULL;

    *head = new;
    return;
  }
  struct listItem* listIter = (*head);
  while(listIter->next != NULL) {
    listIter = listIter->next;
  }

  struct listItem* new = (struct listItem*)malloc(sizeof(struct listItem));
  new->roomptr = y;
  new->next = NULL;
  listIter->next=new;

}
/**
 * [timeKeeper is a thread function that outputs time data to shared
 * resource currentTime.txt]
 * @param  param [void parameter for flexibility]
 * @return       [returns null for flexibility]
 */
void *timeKeeper(void* param) {
  time_t rawtime;
  struct tm *info;
  char buffer[80];
  time( &rawtime );
  info = localtime( &rawtime );
  strftime(buffer,80,"%I:%M%p, %A, %B %d, %Y\n\n", info);
  /*locks execution of main thread so no reading from file before its created*/
  pthread_mutex_lock(&mutex);
  FILE *fp;
  fp = fopen("currentTime.txt", "w+");
  fprintf(fp,"%s",buffer);
  fclose(fp);
  pthread_mutex_unlock(&mutex);

  return NULL;
}
/**
 * [threaderControl if input == time, lock mutex and call thread]
 * @param  buff [string input]
 * @return      [returns 1 if input == time, else 0]
 */
int threaderControl(char *buff) {
  char name[4] = "time";
  if (cmpChars(buff,name)==1) {

    pthread_mutex_lock(&mutex);
    pthread_t tid;
    pthread_create(&tid, NULL, timeKeeper, NULL);
    pthread_mutex_unlock(&mutex);
    pthread_join(tid, NULL);
    return 1;

  }
  return 0;
}
void currentTimeRead() {
  char buff[255];
  FILE *fp;
  fp = fopen("currentTime.txt", "r");
  fgets(buff, 255, (FILE*)fp);
  printf("%s\n",buff);
}
/**
 * [inputPrompt lists locations and prompts player for where to go.
 * Also returns if player has won]
 * @param  myPlayer [player traversing the game with current room ptr
 *                   and list of rooms visited]
 * @param  rooms    [rooms player can connect to]
 * @return          [returns 1 if game is over, else returns 0]
 */
int inputPrompt(struct player* myPlayer,struct Room *rooms) {
  printf("CURRENT LOCATION: %s\n",myPlayer->currentRoom->name);

  printf("POSSIBLE CONNECTIONS: ");
  struct listItem* listIter = myPlayer->currentRoom->head;

  while(listIter->roomptr != NULL) {
    printf("%s",listIter->roomptr->name);
    if(listIter->next == NULL) {
      printf(".\n");
      break;
    }
    printf(", ");
    listIter = listIter->next;
  }
  char input[256];
  printf("WHERE TO? >");
  scanf("%[^\n]%*c", input);
  printf("\n");

  if (threaderControl(input)){
    currentTimeRead();
    return 0;
  }

  listIter = myPlayer->currentRoom->head;
  while(1) {
    if(cmpChars(input,listIter->roomptr->name) == 1) {
      myPlayer->currentRoom = listIter->roomptr;
      if(cmpChars(listIter->roomptr->roomType,"END_ROOM") == 1) {
        printf("YOU REACHED THE END ROOM AND WON!\n");
        return 1;
      }
      ConnectRoom(&myPlayer->head,listIter->roomptr);
      return 0;
    }
    if(listIter->next == NULL) {
      break;
    }
    listIter = listIter->next;
  }
  printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
  return 0;
}
/**
 * [filterDots returns 1 if filename starts with dot]
 * @param  myfile [file with filename to test]
 * @return        [returns 1 if filename starts with dot else 0]
 */
int filterDots(struct dirent *myfile){
  if (myfile->d_name[0] == '.') {
    return 1;
  }
  return 0;
}
/**
 * [findFolder finds most recent folder that is a folder and does
 * not start with a dot]
 * @param dirName [the name of the directory we are passing back]
 */
void findFolder(char **dirName) {
  DIR *mydir;
  mydir = opendir(".");

  struct dirent *myfile;
  struct stat mystat;

  struct dirent *tempdir = NULL;
  long int tempTime = 0;

  while((myfile = readdir(mydir)) != NULL) {
    stat(myfile->d_name, &mystat);
    if(tempTime < mystat.st_mtime && S_ISDIR(mystat.st_mode) == 1 && filterDots(myfile) == 0) {
      tempTime = mystat.st_mtime;
      tempdir = myfile;
    }
  }

  asprintf(&(*dirName),"%s",tempdir->d_name);

  closedir(mydir);
}
/**
 * [getFiles gets files within folder and outputs filenames to room
 * names]
 * @param dirName [name of directory to pull files from]
 * @param rooms   [rooms to name in function]
 */
void getFiles(char *dirName,struct Room* rooms) {
  DIR *mydir;
  struct dirent *myfile;

  mydir = opendir(dirName);

  int roomIndex=0;
  while((myfile = readdir(mydir)) != NULL) {
    if (filterDots(myfile) == 0){
      asprintf(&rooms[roomIndex].name, "%s", myfile->d_name);
      roomIndex++;
    }
  }
  closedir(mydir);
}
/**
 * [parseFiles parses file data into room in rooms with given name]
 * @param dirName [directory to search]
 * @param rooms   [room list to parse info to]
 */
void parseFiles(char *dirName,struct Room* rooms) {
    DIR *mydir;
    struct dirent *myfile;

    mydir = opendir(dirName);
    char buff[255];
    FILE *fp;
    char *fqFileName;

    int roomIndex=0;

    while((myfile = readdir(mydir)) != NULL) {

      if (filterDots(myfile) == 0){

        asprintf(&fqFileName, "%s/%s", dirName, myfile->d_name);

        fp = fopen(fqFileName, "r");

        int i;
        for(i=0; i<3; i++){
          fscanf(fp, "%s", buff);
        }
        while (fscanf(fp, "%s", buff) == 1){
          if(buff[0] >= 'a' && buff[0] <= 'z'){
            int j;
            for(j=0; j<7; j++){
              if (strlen(buff) == strlen(rooms[j].name)){
                if (cmpChars(buff,rooms[j].name) == 1){
                  ConnectRoom(&rooms[roomIndex].head,&rooms[j]);
                }
              }
            }
          }
          int j;
          for(j=0; j<strlen(buff); j++){
            if(buff[j] == '_'){
              asprintf(&rooms[roomIndex].roomType,"%s",buff);
            }
          }
        }
        fclose(fp);
        free(fqFileName);
        roomIndex++;
      }
    }
    closedir(mydir);
}
/**
 * [freeList frees list of room pointers]
 * @param head [head of list to be freed]
 */
void freeList(struct listItem* head) {
  struct listItem* listIter;
  while(head != NULL){
    listIter = head;
    head = head->next;
    free(listIter);
  }
}
/**
 * [cleanup frees rooms and list of connections to rooms]
 * @param rooms [list of rooms to be freed]
 */
void cleanup(struct Room* rooms) {
  int i;
  for(i=0; i<7; i++){
    freeList(rooms[i].head);
    free(rooms[i].roomType);
    free(rooms[i].name);
  }
}


int main() {
/*
  this block of code creates preconditins for functions to run
 */

  srand(time(NULL));
  struct player myPlayer;
  myPlayer.head = NULL;

  struct Room rooms[7];


  int i;
  for(i=0; i<7; i++) {
    rooms[i].connectionNumber = 0;
    rooms[i].head = NULL;
  }
/*
  this block of code moves information from files in folder to structs
 */
  char *dirName;
  findFolder(&dirName);
  getFiles(dirName,rooms);
  parseFiles(dirName,rooms);
  free(dirName);
/*
  This block of code moves the player to the start room
 */
  for(i=0; i<7; i++) {
    if(cmpChars(rooms[i].roomType,"START_ROOM") == 1){
      myPlayer.currentRoom = &rooms[i];
    }
  }
/*
  This block of code runs the game until the player reaches END_ROOM
 */
  while(1){
    if(inputPrompt(&myPlayer,rooms) == 1){
      mostRecent(&myPlayer);
      break;
    }
  }
/*
  This block of code performs heap cleanup, and returns
 */
  cleanup(rooms);
  freeList(myPlayer.head);
  return 0;
}
