// 
// This is G-Sensor Demo 
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#define VERSION	          1
#define PrintTitle(...)   printf("\r\n>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n \
		\r>>       G-Sensor \r\n>>       version:%d\r\n     \
		\r>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n\r\n",__VA_ARGS__)
#define PrintSuccess(...) printf("Create %s Success !\r\n",__VA_ARGS__)
#define DEV_ADDR	0x53
#define MAX_FUNC	16

int CreateDax(void);
int CreateHbox(void);
int CreateLb(void);
int CreateThread(void);
int GtkWindowShow(void);
int CreateBtn(void);
int CreateVbox(void);
int InitialI2C(void);
int Get_G_Sensor(void);
int CreateWindow(void);
int SystemUp(void);
int CreateImg(void);
void destroy (void);
void start(void);

typedef struct{
	int (*func)(void);
}Main;

enum{
	wdw,
	vbx,
	img,
	hbx,
	btn,
	lbx,
	lbxdata,
	lby,
	lbydata,
	lbz,
	lbzdata,
	lbG,
	dax
};
char *str_lb[7]={
	"G-Sensor",
	"X Data:",
	"X Value",
	"Y Data:",
	"Y Value",
	"Z Data:",
	"Z Value"
};
char *str[2]={
	"Exit",
	"Start"
};

typedef struct{
	void (*func)(void);
}SingleProc;

SingleProc sig[]={
	destroy,
	start
};


Main _main[]={
	SystemUp,
	InitialI2C,
	CreateWindow,
	CreateVbox,
	
	CreateImg,
	CreateLb,
	CreateHbox,
	CreateLb,
	CreateLb,
	CreateLb,
	CreateLb,
	CreateLb,
	CreateLb,
	CreateBtn,
	
	CreateThread,
	GtkWindowShow,
	Get_G_Sensor
};

int fd;
const char *i2c = "/dev/i2c-2";
char filename[40];
char G_buf[6];
GtkWidget *w[MAX_FUNC];

void *mainthread(void)
{
	sleep(1);
	PrintSuccess("Thread");
	gtk_label_set_text(GTK_LABEL(w[lbx]),str_lb[0]);
	gtk_label_set_text(GTK_LABEL(w[lby]),str_lb[2]);
	char *xdata;
	char *ydata;
	char *zdata;
	
	while(1){
		_main[MAX_FUNC].func();
		sleep(1);
		sprintf(xdata,"%d",G_buf[0]);
		sprintf(ydata,"%d",G_buf[2]);
		sprintf(zdata,"%d",G_buf[4]);
		//*xdata = G_buf[0];
		gtk_label_set_text(GTK_LABEL(w[lbydata]),ydata);
		gtk_label_set_text(GTK_LABEL(w[lbxdata]),xdata);
		gtk_label_set_text(GTK_LABEL(w[lbzdata]),zdata);
	}
}

void start(void)
{
	//TODO: I2C START MEASURE
}
void destroy(void)
{
	printf("Good-Bye\r\n");
	gtk_main_quit();
	gdk_threads_leave();
}
int CreateDax(void)
{
	w[dax] = gtk_drawing_area_new();
	//gtk_widget_set_size_request(GTK_WIDGET(w[dax]),200,200);
	gtk_box_pack_start(GTK_BOX(w[vbx]),w[dax],TRUE,TRUE,0);
	gtk_widget_show(w[dax]);
	PrintSuccess("Drawing area");
	
}
int CreateLb(void)
{	
	static int i = 0;
	w[lbx+i] = gtk_label_new(str_lb[i]);
	//gtk_widget_set_size_request(GTK_WIDGET(w[lbx+i]),600,100);
	if(i==0)
		gtk_box_pack_start(GTK_BOX(w[vbx]),w[lbx],TRUE,TRUE,0);
	else
		gtk_box_pack_start(GTK_BOX(w[hbx]),w[lbx+i],TRUE,TRUE,0);
	PrintSuccess(str_lb[i]);	
	i++;
}

int CreateThread(void)
{
	if(!g_thread_supported()) g_thread_init(NULL);
		gdk_threads_init();
	g_thread_create((GThreadFunc)mainthread,NULL,FALSE,NULL);
	gdk_threads_enter();
}

int CreateBtn(void)
{
	static int i = 0;
	w[btn+i] = gtk_button_new_with_label(str[i]);
	//gtk_widget_set_size_request(GTK_WIDGET(w[btn+i]),600,100);
	gtk_box_pack_start(GTK_BOX(w[vbx]),w[btn+i],TRUE,TRUE,0);
	gtk_signal_connect(GTK_OBJECT(w[btn+i]),"clicked",GTK_SIGNAL_FUNC(sig[i].func),NULL);
	PrintSuccess(str[i]);
	i++;
}

int SystemUp(void)
{
	PrintTitle(VERSION);
}
int CreateWindow(void)
{
	w[wdw]= gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(w[wdw]),200,300);
	gtk_window_set_position(GTK_WINDOW(w[wdw]),GTK_WIN_POS_CENTER);
	gtk_container_border_width(GTK_CONTAINER(w[wdw]),10);
	PrintSuccess("Window");
}
int CreateVbox(void)
{
	w[vbx] = gtk_vbox_new(TRUE,1);
	gtk_container_add(GTK_CONTAINER(w[wdw]),w[vbx]);
	PrintSuccess("VBox");
}
int CreateHbox(void)
{
	w[hbx] = gtk_hbox_new(TRUE,1);
	gtk_box_pack_start(GTK_BOX(w[vbx]),w[hbx],TRUE,TRUE,0);
	PrintSuccess("HBox");
}
int GtkWindowShow(void)
{
	gtk_widget_show_all(w[wdw]);
}
int CreateImg(void)
{
	w[img] = gtk_image_new_from_file("alona.jpg");
	gtk_box_pack_start(GTK_BOX(w[vbx]),w[img],TRUE,TRUE,0);
	PrintSuccess("Image");
}
int Get_G_Sensor(void)
{
	
	G_buf[0]=0x32;
	write(fd,G_buf,1);
	if(read(fd,G_buf,6)<6)
		printf("Read G-Sensor error \r\n");
	else{
		int i=0;
		printf("=============\r\n");
		for(i=0;i<6;i++)
			printf("Data = %d\r\n",G_buf[i]);
		printf("=============\r\n");
	}
}
int InitialI2C(void)
{
	sprintf(filename,i2c);
	fd = open(filename,O_RDWR);
	if(fd < 0)
		printf("file open errir \r\n");
	else{
		if(ioctl(fd,I2C_SLAVE,DEV_ADDR)<0)
			printf("Slaver error \r\n");
		else{
			unsigned char buf[3];
			buf[0] = 0x31;buf[1] = 0x0b;
			write(fd,buf,2);
			buf[0] = 0x2d;buf[1] = 0x08;
			write(fd,buf,2);
			buf[0] = 0x2e;buf[1] = 0x80;
			write(fd,buf,2);
			printf("Device Initial Success \r\n");
		}
	}
}
int main (int argc,char * argv[])
{
	int i=0;
	gtk_init(&argc,&argv);

	for(i=0;i<MAX_FUNC;i++){
		_main[i].func();
			
	}
	gtk_main();
}
