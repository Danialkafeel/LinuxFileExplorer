// #include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <bits/stdc++.h>
#include <dirent.h>
#include <termios.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#define clr printf("\033[H\033[J");

using namespace std;

string ROOT;
stack<string> back;
stack<string> forw;

void mypush(stack<string> &stk, char * str){
	if(stk.empty()){
		stk.push(str);
		return;
	}
	if(stk.top() != str){
		stk.push(str);
	}
}

void cmd_mode(){
	clr;
	cout<<":- ";
	
	string rawcmd = "";
	vector<string> cmds;
	while(1){
		char ch = cin.get();
		if(ch == 27){	//ESC
			// cout<<"gya";
			break;
		}
		cout<<ch;
		switch(ch){

			case 127:{	// Backspace
				if(rawcmd.size() > 0){
					rawcmd.pop_back();
					printf("\033[%dD", (1));
					printf("\033[K");
				}
				break;
			}
			case 10:{	// ENTER
				// cmds.push_back(rawcmd);
				// rawcmd = "";
				int i = 0;
				for(;i<rawcmd.size();i++){
					if(rawcmd[i] != ' ')
						break;
				}
				cout<<"rawcmd = "<<rawcmd<<endl;

				string tempcmd = "";
				for(;i<rawcmd.size();i++){
					if(rawcmd[i] == ' '){
						cmds.push_back(tempcmd);
						tempcmd = "";
					}
					else{
						tempcmd += rawcmd[i];
					}
				}
				if(tempcmd != "")
					cmds.push_back(tempcmd);

				///	Now command is received in cmds;

				cout<<"cmds ==>  ";
				for(string s: cmds){
					cout<<s<<" ";
				}
				cout<<endl;
				rawcmd = "";

				if(cmds.size() < 2 || cmds[0] == ""){
					cout<<"Invalid command\n:- ";
					cmds.clear();					
					break;
				}

				else if(cmds[0] == "copy"){
					if(cmds.size() < 3){
						cout<<"Invalid command\n:- ";
						cmds.clear();					
						break;		
					}
					string dest_dir = cmds[cmds.size()-1];
					cmds.pop_back();
					for(string s:cmds){
						if(s != "copy"){
							cout<<"Copying "<<s<<endl;
						}
					}
				}
				else if(cmds[0] == "move"){
					if(cmds.size() < 3){
						cout<<"Invalid command\n:- ";
						cmds.clear();					
						break;		
					}
					string dest_dir = cmds[cmds.size()-1];
					cmds.pop_back();
					for(string s:cmds){
						if(s != "move"){
							cout<<"Moving "<<s<<endl;
						}
					}	
				}
				else if(cmds[0] == "rename"){
					if(cmds.size() != 3){
						cout<<"Invalid command\n:- ";
						cmds.clear();					
						break;
					}
					string old_name, new_name;
					old_name = cmds[1];
					new_name = cmds[2];
					if(rename(old_name.c_str(),new_name.c_str()) == -1)
						perror("rename");
				}

				else if(cmds[0] == "create_file"){
					if(cmds.size() != 3){
						cout<<"Invalid command\n:- ";
						cmds.clear();					
						break;		
					}
					string dest_dir = cmds[2];
					if(dest_dir[0] == '~')
						dest_dir.erase(0,1);

					dest_dir = ROOT + dest_dir;
					if(cmds[2] == "."){
						char s[100];
						getcwd(s,100);
						dest_dir = s;
					}
					if(dest_dir[dest_dir.size()-1] != '/')
						dest_dir += "/";
					dest_dir += cmds[1];

					mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
					creat(dest_dir.c_str(),mode);
				}
				else if(cmds[0] == "create_dir"){
					if(cmds.size() != 3){
						cout<<"Invalid command\n:- ";
						cmds.clear();					
						break;		
					}
					string dest_dir = cmds[2];
					if(dest_dir[0] == '~')
						dest_dir.erase(0,1);

					dest_dir = ROOT + dest_dir;
					if(cmds[2] == "."){
						char s[100];
						getcwd(s,100);
						dest_dir = s;
					}
					if(dest_dir[dest_dir.size()-1] != '/')
						dest_dir += "/";

					dest_dir += cmds[1];
					mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
					mkdir(dest_dir.c_str(),mode);	
				}

				else if(cmds[0] == "delete_file"){
					if(cmds.size() != 2){
						cout<<"Invalid command\n:- ";
						cmds.clear();					
						break;		
					}
					string dest_dir = cmds[cmds.size()-1];
					cmds.pop_back();
					for(string s:cmds){
						if(s != "delete_file"){
							cout<<"Deleting "<<s<<endl;
						}
					}	
				}
				else if(cmds[0] == "delete_dir"){
					if(cmds.size() != 2){
						cout<<"Invalid command\n:- ";
						cmds.clear();					
						break;		
					}
					string dest_dir = cmds[cmds.size()-1];
					cmds.pop_back();
					for(string s:cmds){
						if(s != "delete_dir"){
							cout<<"Deleting "<<s<<endl;
						}
					}	
				}
				else if(cmds[0] == "goto"){
					if(cmds.size() != 2){
						cout<<"Invalid command\n:- ";
						cmds.clear();					
						break;
					}
					string dest_dir = cmds[1];
					if(dest_dir[0] == '~')
						dest_dir.erase(0,1);

					char s[100];
					getcwd(s,100);

					dest_dir = ROOT+dest_dir;
					chdir(dest_dir.c_str());
					mypush(back,s);
				}

				else{
					cout<<"Invalid command\n:- ";
					cmds.clear();					
					break;
				}

				cout<<":- ";
				cmds.clear();
				break;
			}
			default:{
				rawcmd += ch;
				// cout<<"rawcmd = "<<rawcmd<<endl;
			}
		}
	}
}

void read_curr(vector<string> &contents){
	char s[100];
	getcwd(s,100);
	if(s != ROOT){
		contents.push_back("..");
	}
	contents.push_back(".");
	DIR * temp = opendir(".");
	struct dirent * p = readdir(temp);
	
	while(p != NULL){
		if(p->d_name[0] != '.'){
			contents.push_back((p->d_name));
		}
		p = readdir(temp);
	}
}

void show_curr(){
	DIR * temp = opendir(".");
	struct stat myst;
	mode_t m_temp;
	struct dirent * p = readdir(temp);
	
	char s[100];
	getcwd(s,100);

	cout<<"Current Directory => "<<s<<endl;

	if(s != ROOT){
		cout<<"..\n";
	}
	cout<<".\n";
	while(p != NULL){
		stat(p->d_name,&myst);
		m_temp = myst.st_mode;
		string modes = "";
		if(p->d_name[0] != '.'){
			modes += ((m_temp & S_IFDIR) ? 'D' : 'F');
			modes += ((m_temp & S_IRUSR) ? 'R' : '-');
			modes += ((m_temp & S_IWUSR) ? 'W' : '-');
			modes += ((m_temp & S_IXUSR) ? 'X' : '-');
			modes += ((m_temp & S_IRGRP) ? 'R' : '-');
			modes += ((m_temp & S_IWGRP) ? 'W' : '-');
			modes += ((m_temp & S_IXGRP) ? 'X' : '-');
			modes += ((m_temp & S_IROTH) ? 'R' : '-');
			modes += ((m_temp & S_IWOTH) ? 'W' : '-');
			modes += ((m_temp & S_IXOTH) ? 'X' : '-');
			
			string mod_time = ctime(&myst.st_mtime);
			mod_time.pop_back();
			string name = p->d_name;
			double sizef = (myst.st_size)/1024.0;
			struct passwd *owner_u = getpwuid(myst.st_uid);
			struct group * owner_g = getgrgid(myst.st_gid);


			cout<<fixed<<setprecision(2);
			cout<<left<<setw(20)<<modes;
			cout<<setw(5)<<sizef<<setw(20)<<" KBytes"<<setw(10)<<owner_u->pw_name<<setw(10)<<owner_g->gr_name<<setw(10)<<mod_time<<"\t"<<name;
			cout<<endl;
		}
		p = readdir(temp);
	}
}


char normal_mode(){
	char ch;
	vector<string> contents;
	read_curr(contents);
	show_curr();
	int n = contents.size();
	int ptr = n;
	while(1){
		ch = cin.get();
		switch(ch){
			case 10:{		// Enter
					if(ptr >=0 && ptr < n){
						clr;
						// cout<<"ptr = "<<ptr<<endl;
						struct stat mst;
						stat(contents[ptr].c_str(),&mst);
						bool isDir;
						(mst.st_mode & S_IFDIR)?isDir = 1: isDir = 0;

						if(isDir){
							string dest_dir = contents[ptr];
							dest_dir.insert(0,"/");

							char s[100];
							getcwd(s,100);
							dest_dir = s+dest_dir;

							if(contents[ptr] == ".."){
								mypush(forw,s);
							}
							else{
								mypush(back,s);
							}

							// DIR * new_dir = opendir(dest_dir.c_str());
							int err = chdir(dest_dir.c_str());
							if(err == -1){
								cout<<"Cannot open "<<dest_dir<<endl;
								// return ':';
							}
							getcwd(s,100);

							contents.clear();
							read_curr(contents);
							show_curr();
							n = contents.size();
							ptr = n;
						}
						else{
							cout<<"file\n";
							if(fork() == 0){
								// child
								char * args[3];
								string file = contents[ptr];
								string command = "vi";
								// cout<<file.substr(file.size()-3,file.size()-1)<<endl;
								string f_type = file.substr(file.size()-3,file.size()-1);
								// if(f_type=="pdf" || f_type=="jpg" || f_type=="png" || f_type=="tml"){
								command = "xdg-open";
								// }
								args[0] = (char *)command.c_str();
								args[1] = (char *)file.c_str();
								args[2] = NULL;
								if(execvp(args[0],args) == -1){
									perror("exec");
								}
							}
							else{
								// parent
								wait(0);
							}
							return 'r';
						}
					}
				break;
			}
			case 'h':{					
					// while(!back.empty())
					//     back.pop();
					while(!forw.empty())
					    forw.pop();
					clr;
					chdir(ROOT.c_str());
					contents.clear();
					read_curr(contents);
					show_curr();
					n = contents.size();
					ptr = n;
				break;
			}
			case 127:{
					char s[100];
					getcwd(s,100);
					if(s != ROOT){
						clr;

						string dest_dir = s;
						while(dest_dir[dest_dir.size()-1] != '/')
							dest_dir.pop_back();
						dest_dir.pop_back();
						mypush(forw,s);

						chdir(dest_dir.c_str());
						contents.clear();
						read_curr(contents);
						show_curr();
						n = contents.size();
						ptr = n;
					}
				break;
			}
			case 'A':{		// UP
					if(ptr == 0)
						break;
					ptr--;
					printf("\033[1A");
				break;
			}
			case 'B':{		// Down
					if(ptr >= n)
						break;
					ptr++;
					printf("\033[1B");
				break;
			}
			case 'C':{		// forward
					// printf("\033[1C");
					if(!forw.empty()){
						char s[100];
						getcwd(s,100);
						mypush(back,s);
						clr;
						string dest_dir = forw.top();
						forw.pop();
						chdir(dest_dir.c_str());
						contents.clear();
						read_curr(contents);
						show_curr();
						n = contents.size();
						ptr = n;
					}
				break;
			}
			case 'D':{		// Backward
					// printf("\033[1D");
					if(!back.empty()){
						char s[100];
						getcwd(s,100);
						mypush(forw,s);

						clr;
						string dest_dir = back.top();
						back.pop();
						chdir(dest_dir.c_str());
						contents.clear();
						read_curr(contents);
						show_curr();
						n = contents.size();
						ptr = n;
					}
				break;
			}
			case 'x':{
				return ch;
			}
			case ':':{
				return ch;
			}
		}
	}
	return ch;
}


int main(){
	clr;

	struct termios old_tio, new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON);
    new_tio.c_lflag &= (~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    char s[100];
    getcwd(s,100);
    ROOT = s;
	// hardstatus alwayslastline
	// hardstatus string '%{= kG}[ %{G}%H %{g}][%= %{=kw}%?%-Lw%?%{r}(%{W}%n*%f%t%?(%u)%?%{r})%{w}%?%+Lw%?%?%= %{g}][%{B}%Y-%m-%d %{W}%c %{g}]'
    while(1){
		char ch = normal_mode();
		// char ch = cin.get();
		if(ch == ':'){
			cmd_mode();
			clr;
		}
		if(ch == 'x'){
			break;
		}
    }

	termios old,news;
	tcgetattr(STDIN_FILENO, &old);
	news = old;
	news.c_lflag |= (ICANON);
    news.c_lflag |= (ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &news);
    // printf("\033[J");
    clr;
    cout<<"\n\n";
    cout<<left<<setw(100)<<""<<"See you Later !!\n\n";
	return 0;
}