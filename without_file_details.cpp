// #include <sys/types.h>
#include <sys/stat.h>
// #include <fcntl.h>
#include <bits/stdc++.h>
#include <dirent.h>
#include <termios.h>
#include <unistd.h>

using namespace std;
string ROOT;
void cmd_mode(){
	printf("\033[H\033[J");	
	cout<<":- ";
	
	string tempcmd = "";
	vector<string> cmds;
	while(1){
		char ch = cin.get();
		if(ch == 27){	//ESC
			// cout<<"gya";
			break;
		}
		cout<<ch;
		switch(ch){
			case 32:{	//	SPACE
				cmds.push_back(tempcmd);
				// cout<<"Pushed "<<tempcmd<<endl;
				tempcmd = "";
				break;
			}
			case 127:{	// Backspace
				printf("\033[%dD", (1));
				printf("\033[K");
				if(tempcmd.size() > 0)
					tempcmd.pop_back();
				// cout<<"Deleting\n";
				// cout<<"\b";
				break;
			}
			case 10:{	// ENTER
				cmds.push_back(tempcmd);
				tempcmd = "";

				///	Now command is received in cmds;

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
					string dest_dir = cmds[cmds.size()-1];
					cmds.pop_back();
					for(string s:cmds){
						if(s != "rename"){
							cout<<"Renaming "<<s<<endl;
						}
					}
				}

				else if(cmds[0] == "create_file"){
					if(cmds.size() != 3){
						cout<<"Invalid command\n:- ";
						cmds.clear();					
						break;		
					}
					string dest_dir = cmds[cmds.size()-1];
					cmds.pop_back();
					for(string s:cmds){
						if(s != "create_file"){
							cout<<"Creating "<<s<<endl;
						}
					}	
				}
				else if(cmds[0] == "create_dir"){
					if(cmds.size() != 3){
						cout<<"Invalid command\n:- ";
						cmds.clear();					
						break;		
					}
					string dest_dir = cmds[cmds.size()-1];
					cmds.pop_back();
					for(string s:cmds){
						if(s != "create_dir"){
							cout<<"Creating "<<s<<endl;
						}
					}	
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

				else{
					cout<<"Invalid command\n:- ";
					cmds.clear();					
					break;
				}
				// cout<<"cmds entered \n";
				// for(int i=0;i<cmds.size();i++){
				// 	cout<<cmds[i]<<endl;
				// }

				cout<<":- ";
				cmds.clear();
				break;
			}
			default:{
				tempcmd += ch;
				// cout<<"temp = "<<tempcmd<<endl;
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
	
	struct stat myst;
	mode_t m_temp;


	struct dirent * p = readdir(temp);
	
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
			contents.push_back(modes+"\t\t\t"+(p->d_name));
		}
		p = readdir(temp);
	}
}

void show(vector<string> &contents){
	for(string s: contents){
		cout<<s<<endl;
	}
}

vector<vector<string>> pages;
char normal_mode(){
	char ch;
	vector<string> contents;
	read_curr(contents);
	show(contents);
	int n = contents.size();
	int ptr = n;
	int page_ptr = 0;
	while(1){
		ch = cin.get();
		switch(ch){
			case 10:{		// Enter
					if(ptr >=0 && ptr < n){
						printf("\033[H\033[J");
						// cout<<"ptr = "<<ptr<<endl;
						// pages.push_back(contents);
						// page_ptr++;
						string dest_dir = contents[ptr];
						dest_dir.insert(0,"/");

						char s[100];
						getcwd(s,100);
						dest_dir = s+dest_dir;


						// DIR * new_dir = opendir(dest_dir.c_str());
						int err = chdir(dest_dir.c_str());
						if(err == -1){
						cout<<"dest_dir = "<<dest_dir<<endl;
							// return ':';
						}
						getcwd(s,100);
						cout<<"current dir = "<<s<<endl;


						contents.clear();
						read_curr(contents);
						show(contents);
						n = contents.size();
						ptr = n;
					}
				break;
			}
			case 'A':{		// UP
					printf("\033[1A");
					if(ptr < 0 || ptr > n)
						break;
					ptr--;
				break;
			}
			case 'B':{		// Down
					printf("\033[1B");
					if(ptr < -1 || ptr > n)
						break;
					ptr++;
				break;
			}
			case 'C':{		// Forward
					printf("\033[1C");
				break;
			}
			case 'D':{		// Backward
					printf("\033[1D");
				break;
			}
			case 'x':{
				// cout<<"returned";
				return ch;
			}
			case ':':{
				cout<<"returned";
				return ch;
			}
			// default:{
			// 	cout<<"def "<<ch<<endl;
			// }
		}
	}
	return ch;
}


int main(){
	// cout<<"dkjsfsdjf\n";
	// return 1;
	printf("\033[H\033[J");
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
			printf("\033[H\033[J");
		}
		if(ch == 'x'){
			break;
		}
    }

    cout<<"Pages length "<<pages.size()<<endl;

	termios old,news;
	tcgetattr(STDIN_FILENO, &old);
	news = old;
	news.c_lflag |= (ICANON);
    news.c_lflag |= (ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &news);
	return 0;
}