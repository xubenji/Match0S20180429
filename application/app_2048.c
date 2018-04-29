#include "application/application.h"
#include "driver/console.h"
#include "kernel/memory.h"

/**
2048GAME copyright HU zhiheng
*/
void swap(int *a, int *b);
void up_remove_blank();
void down_remove_blank();
void left_remove_blank();
void right_remove_blank();
void left();
void right();
void up();
void down();
void output(int window);
void output_bc(int window);
void setsand(int num);
void gameover();
void gamewin();
void reset_game(int window);

int a[4][4] = {
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
}; //申请一个4*4数组，并初始化
int n; //用来表示是什么操作
int game_win = 0;
int game_over = 0;
		
void app_2048()
{
	int window;
	uint16_t *window_buf;
	int key;
	int score = 0;
	int run = 0;
	int i,j, k;
	int flags = 0;
	//分配内存保存图层
	window_buf = (int16_t *)malloc(284*320*2);
	
	//打开一个窗口
	window = creatWindow(window_buf, 284, 320, 0xf0f0, "2048");
	
	//绘制ui
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){  
			a[i][j] = 0;
		}
	}
	game_win = 0;
	game_over = 0;

	output_bc(window);
	output(window);
	
	for(;;){
		//从键盘获取一个数
		milliDelay(100);
		key = getChar();
		if(key != -1 ){
			if(key == 'q')break;
			if(game_win == 0 || game_over == 0){
				if(key == 'a' || key == '4' ){
					setsand(2);
					n = 4;//left
				} 
				if(key == 'd' || key == '6' ){
					setsand(2);
					n = 6;//left
				} 
				if(key == 'w' || key == '8' ){
					setsand(2);
					n = 8;//left
				} 
				if(key == 's' || key == '2' ){
					setsand(2);
					n = 2;//left
				} 
				if(n == 4){//左   
					left_remove_blank();
					left();
				}else if(n == 6){//右   
					right_remove_blank();  
					right();  
				}else if(n==8){//上   
					up_remove_blank();
					up();  
				} else if(n==2){//下   
					down_remove_blank();  
					down();
				}
				output_bc(window);
				output(window);	
				
				gamewin();
				gameover();
			}
			
				if(key == ' '){
					for(i=0;i<4;i++){
						for(j=0;j<4;j++){  
							a[i][j] = 0;
						}
					}
					game_win = 0;
					game_over = 0;
					output_bc(window);
					output(window);
					/*
					set_syscall_xy(4, 320-20);
					set_syscall_size(284-8, 16);
					set_syscall_color(COLOR_WHITE);
					window_square(window);
					*/
					fillColor(COLOR_WHITE);
					drawSquare(window, 4, 320-20, 284-8, 16);
					
				}
			
			if(game_win){
				/*set_syscall_string("Game Win");
				set_syscall_xy(4, 320-20);
				set_syscall_color(COLOR_PURPLE);
				window_string(window);
				*/
				fontColor(COLOR_PURPLE);
				drawString(window, 4, 320-20, "Game Win");
					
				
				/*set_syscall_string("Space Replay");
				set_syscall_xy(150+4*8, 320-20);
				set_syscall_color(COLOR_BLACK);
				window_string(window);*/
				
				fontColor(COLOR_BLACK);
				drawString(window, 150+4*8, 320-20, "Space Replay");
					
			}else if(game_over){
				/*set_syscall_string("Game Over");
				set_syscall_xy(4, 320-20);
				set_syscall_color(COLOR_RED);
				window_string(window);
				*/
				fontColor(COLOR_RED);
				drawString(window, 4, 320-20, "Game Over");
				
				/*set_syscall_string("Space Replay");
				set_syscall_xy(150+4*8, 320-20);
				set_syscall_color(COLOR_BLACK);
				window_string(window);
				*/
				fontColor(COLOR_BLACK);
				drawString(window, 150+4*8, 320-20, "Space Replay");
				
				
			}
		}
		key = -1;
	}
	for(i=0;i<4;i++){
        for(j=0;j<4;j++){  
			
			a[i][j] = 0;
			
        }
    }
	mfree(window_buf);
	closeWindow(window);
	closeTask();
}

/*将两个变量的值进行交换*/
void swap(int *a, int *b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

void up_remove_blank()
{  
    int i,j,k;  
    for(j=0;j<4;j++){  
        for(i=1;i<4;i++){  
            k=i;  
            while(k-1>=0&&a[k-1][j]==0){//上面的那个为空   
                swap(&a[k][j],&a[k-1][j]);  
                k--;
                  
            }  
        }  
    }  
}  
void down_remove_blank()
{  
    int i,j,k;  
    for(j=0;j<4;j++){  
        for(i=2;i>=0;i--){  
            k=i;  
            while(k+1<=3&&a[k+1][j]==0){//上面的那个为空   
                swap(&a[k][j],&a[k+1][j]);  
                k++;      
            }  
        }  
    }  
}  
void left_remove_blank()
{  
    int i,j,k;  
    for(i=0;i<4;i++){  
        for(j=1;j<4;j++){  
            k=j;  
            while(k-1>=0&&a[i][k-1]==0){//上面的那个为空   
                swap(&a[i][k],&a[i][k-1]);  
                k--;  
            }     
        }  
    }     
}   
void right_remove_blank()
{  
    int i,j,k;  
    for(i=0;i<4;i++){  
        for(j=2;j>=0;j--){  
            k=j;  
            while(k+1<=3&&a[i][k+1]==0){//上面的那个为空   
                swap(&a[i][k],&a[i][k+1]);  
                k++;      
            }     
        }  
    }     
}  
void left()
{  
    int i,j;  
    for(i=0;i<4;i++){  
        for(j=0;j<3;j++){  
            if(a[i][j]==a[i][j+1]){  
                a[i][j]+=a[i][j+1];  
                a[i][j+1]=0;  
                left_remove_blank();  
            }  
        }  
    }  
}  
void right()
{  
        int i,j;  
    for(i=0;i<4;i++){  
        for(j=3;j>=1;j--){  
            if(a[i][j]==a[i][j-1]){  
                a[i][j]+=a[i][j-1];  
                a[i][j-1]=0;  
                right_remove_blank();  
            }  
        }  
    }  
}  
void up()
{
    int i,j;  
    for(j=0;j<4;j++){//每一列   
        for(i=0;i<3;i++){  
            if(a[i][j]==a[i+1][j]){  
                a[i][j]=a[i][j]+a[i+1][j];  
                a[i+1][j]=0;  
                //移除空格  
                up_remove_blank();   
            }  
        }   
    }
}  
void down()
{  
    int i,j;  
    for(j=0;j<4;j++){//每一列   
        for(i=3;i>=1;i--){  
            if(a[i][j]==a[i-1][j]){  
                a[i][j]=a[i][j]+a[i-1][j];  
                a[i-1][j]=0;  
                //移除空格  
                down_remove_blank();   
            }  
        }   
    }  
}
void output_bc(int window)
{  
    int i,j;
	//set_syscall_size(64, 64);
    for(i=0;i<4;i++){  
        for(j=0;j<4;j++){
			/*set_syscall_xy(4+4*(j+1)+(j)*64, 24+4*(i+1)+(i)*64);
			set_syscall_color(COLOR_GREEN);				
			window_square(window);*/
			fillColor(COLOR_GREEN);
			drawSquare(window, 4+4*(j+1)+(j)*64, 24+4*(i+1)+(i)*64, 64, 64);
        }
    }  
} 

  
void output(int window)
{  
    int i,j;
	
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){  
			/*set_syscall_xy(4+4*(j+1)+(j)*64, 24+24+4*(i+1)+(i)*64);
			set_syscall_color(COLOR_BLACK);
			set_syscall_integer(a[i][j]);
			window_hex(window);*/
			fontColor(COLOR_BLACK);
			drawInt(window, 4+4*(j+1)+(j)*64+8*3, 24+24+4*(i+1)+(i)*64, a[i][j]);
        }
    }  
} 

void setsand(int num)
{  
    int i,j;
	
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){  
			if(a[i][j] == 0){
				a[i][j] = num;
				return;
			}
        }
    }
} 

void gameover()
{  
    int i,j;
	int count = 0;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){  
			if(a[i][j] != 0){
				count++;
			}
        }
    }
	if(count == 16){
		game_over = 1;
		
	}
} 

void gamewin()
{  
    int i,j;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){  
			if(a[i][j] >= 2048){
				game_win = 1;
			}
        }
    }
} 

