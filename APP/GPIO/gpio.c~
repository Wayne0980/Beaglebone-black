#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define UNEXPORT	fopen("/sys/class/gpio/unexport","w")
#define EXPORT  	fopen("/sys/class/gpio/export","w")
#define GSTRING(s,i,g)  sprintf(s,"/sys/class/gpio/gpio%d%s",i,g)
#define GVALUE		"/value"
#define GDIRECTION      "/direction"
#define GOPEN(s)	fopen((s),"w")

void GtkWindowShow(void);
void CreateImg(void);
void CreateWindow(void);
void CreateVbox(void);
void CreateBtn(void);
void led1_on(void);
void led2_on(void);
void all_on(void);
void all_off(void);
void destroy(void);
int gpio_control(int pin,int value);
int gpio_init(int pin);
int gpio_close(int pin);
int gpio_open(int pin);
void GtkGpioInit(void);
void GtkGpioExit(void);

enum{
	wdw,
	vbx,
	img,
	btn
}; 

char *str[5]={
	"LED1 ON",
	"LED2 ON",
	"LEDALL ON",
	"LEDALL OFF",
	"Exit"
};

typedef struct{
	void (*func)();
}SingelProc;

SingelProc sigproc[]={
	led1_on,
	led2_on,
	all_on,
	all_off,
	destroy
};

typedef struct{
	void (*func)();
}MainProc;

GtkWidget *w[7];
MainProc mainproc[]={
	CreateWindow,
	CreateVbox,
	CreateImg,
	CreateBtn,
	CreateBtn,
	CreateBtn,
	CreateBtn,
	CreateBtn,
	GtkGpioInit,
	GtkWindowShow,
	GtkGpioExit
};
void destroy(void)
{
	printf("Good-bye\r\n");
	mainproc[10].func();
	gtk_main_quit();
}
void led1_on(void)
{
	printf("led1\r\n");
	gpio_open(26);
	gpio_close(27);

}

void led2_on(void)
{
	printf("led2\r\n");
	gpio_open(27);
	gpio_close(26);
}


void all_on(void)
{
	printf("all on \r\n");
	gpio_open(26);
	gpio_open(27);
}


void all_off(void)
{
	printf("all off \n");
	gpio_close(26);
	gpio_close(27);
}
void CreateBtn(void)
{
	static int i = 0;
	w[btn+i] = gtk_button_new_with_label(str[i]);
	gtk_box_pack_start(GTK_BOX(w[vbx]),w[btn+i],TRUE,TRUE,0);
	gtk_signal_connect(GTK_OBJECT(w[btn+i]),"clicked",GTK_SIGNAL_FUNC(sigproc[i].func),NULL);
	printf("Button %d OK \r\n",i);
	i++;
}
void CreateImg(void)
{
	w[img] = gtk_image_new_from_file("alona.jpg");
	gtk_box_pack_start(GTK_BOX(w[vbx]),w[img],TRUE,TRUE,0);
	printf("Image OK!\r\n");
}
void CreateWindow(void)
{
	w[wdw] = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(w[wdw]),200,300);
	gtk_window_set_position(GTK_WINDOW(w[wdw]),GTK_WIN_POS_CENTER);
	gtk_container_border_width(GTK_CONTAINER(w[wdw]),6);
	printf("Create Window Success ! \r\n");
}

void CreateVbox(void)
{
	w[vbx] = gtk_vbox_new(TRUE,1);
	gtk_container_add(GTK_CONTAINER(w[wdw]),w[vbx]);
	printf("Vbox OK !\r\n");
}
int main (int argc,char *argv[]){
		
	gtk_init(&argc,&argv);
	
	int i=0;
	for(i=0;i<10;i++)
		mainproc[i].func();
	gtk_main();
	return 0;
	
}
void GtkWindowShow(void)
{
	gtk_widget_show_all(w[wdw]);
}
void GtkGpioInit(void)
{
	gpio_init(26);
	gpio_init(27);
}
void GtkGpioExit(void)
{
	gpio_exit(26);
	gpio_exit(27);
}
int gpio_exit(int pin)
{
	FILE *p = NULL;
	char gpio[50];
	if((p = UNEXPORT)==NULL){
		printf("Problem closing gpio %d \r\n",pin);
		return -1;
	}else{
		printf("close gpio okay %d\r\n",pin);
	}
	fprintf(p,"%d",pin);
	fclose(p);
}
int gpio_init(int pin)
{
	FILE *p = NULL;
	char gpio[50];
	if((p = EXPORT)==NULL){
		printf("Problem opening gpio %d\r\n",pin);
		return -1;
	}
	else{
		printf("Init Gpio okay %d\r\n",pin);
	}
	fprintf(p,"%d",pin);
	fclose(p);
	GSTRING(gpio,pin,GDIRECTION);
	if((p=GOPEN(gpio))==NULL){
		printf("Problem gpio ready %d\r %s \r\n",pin,gpio);
		return -1;
	}else{
		printf("Gpio Ready%d\r\n%s\r\n",pin,gpio);
	}
	fprintf(p,"out");
	fclose(p);
}
int gpio_open(int pin)
{
	gpio_control(pin,1);
}
int gpio_close(int pin)
{
	gpio_control(pin,0);
}
int gpio_control(int pin ,int value)
{
	FILE *p = NULL;
	char gpio[50];
	GSTRING(gpio,pin,GVALUE);
	if((p = GOPEN(gpio))== NULL){
		printf("Problem controlling %d \r\n",pin);
		return -1;	
	}else{
		if(value)
			printf("Open okay pin %d \r\n",pin);
		else
			printf("Close okay pin %d \r\n",pin);
	}
	fprintf(p,"%d",value);
	pclose(p);
}
