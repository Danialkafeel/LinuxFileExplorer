#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

int
main(int argc, char *argv[])
{


	if(fork() == 0){
		// child
		char * args[3];
		string command = "subl",t = "/home/danial/2020201069/main.cpp";
		args[0] = (char *)command.c_str();
		args[1] = (char *)t.c_str();
		args[2] = NULL;
		if(execvp(args[0],args) == -1){
			perror("exec");
		}
	}
	else{
		wait(0);
	}

	return 1;

	vector<string> contents[4];

	contents[0].push_back("fssdfs");

    struct stat sb;

   // if (argc != 2) {
   //      fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
   //      exit(EXIT_FAILURE);
   //  }

   // if (stat(argv[1], &sb) == -1) {
   //      perror("stat");
   //      exit(EXIT_FAILURE);
   //  }
    // cout<<"argv[1] = "<<argv[1]<<endl;

    stat("/home/danial/2020201069/canon.cpp",&sb);
   printf("File type:                ");
   mode_t temp = sb.st_mode;
   cout<<(temp & S_IXUSR)<<endl;
   // return 1;

   switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
    }

   printf("I-node number:            %ld\n", (long) sb.st_ino);

   printf("Mode:                     %lo (octal)\n",
            (unsigned long) sb.st_mode);

   printf("Link count:               %ld\n", (long) sb.st_nlink);
    printf("Ownership:                UID=%ld   GID=%ld\n",
            (long) sb.st_uid, (long) sb.st_gid);

   printf("Preferred I/O block size: %ld bytes\n",
            (long) sb.st_blksize);
    printf("File size:                %lld bytes\n",
            (long long) sb.st_size);
    printf("Blocks allocated:         %lld\n",
            (long long) sb.st_blocks);

   printf("Last status change:       %s", ctime(&sb.st_ctime));
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));

   exit(EXIT_SUCCESS);
}