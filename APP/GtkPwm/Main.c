#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define EXPORT		fopen("/sys/devices/bone_capemgr.8/slots","w")
#define PIN_EXPORT	fopen("/sys/devices/bone_capemgr.8/slots","w")
#define DUTY		fopen("/sys/devices/ocp.2/pwm_test_P8_13.16/duty","w")
#define PERIOD		fopen("/sys/devices/ocp.2/pwm_test_P8_13.16/period","w")

int gpio_cintrol(int pin , int value);
int gpio_init(void);
GtkWidget *drawarea = NULL;
GdkColor color;

void hello(void)
{
	g_print("Hello World !\n");
}
void destroy(void)
{
	gtk_main_quit();
}
int i_color = 0;
void *btncolor(void){
	int fmode = 1;
	while(1){
		gdk_threads_enter();
			
		printf("thread \n");	
		pwm_duty(i_color);
		if(fmode == 1){
			i_color+=100;
			if(i_color >= 60000){
				fmode = 2;
				
			}
		}else{
			i_color-=100;
			if(i_color <= 200){
				fmode = 1;
			}
				
		}
		color.red = i_color;
		color.blue = i_color;
		color.green = i_color;
		gtk_widget_modify_bg(drawarea,GTK_STATE_NORMAL,&color);
		gtk_widget_set_size_request(GTK_WIDGET(drawarea),200,40);
		gdk_threads_leave();
	}
}
int main (int argc,char *argv[]){

	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *vbox;
	GtkWidget *img;
	gpio_init();
	if(!g_thread_supported()) g_thread_init(NULL);
		gdk_threads_init();
	gtk_init(&argc, &argv);
	drawarea = gtk_drawing_area_new();
	gtk_widget_modify_bg(drawarea,GTK_STATE_NORMAL,&color);
	gtk_widget_set_size_request(GTK_WIDGET(drawarea),200,40);	
	//////////////////////////////////////////
	//              window			//
	//////////////////////////////////////////

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window),200,160);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_border_width(GTK_CONTAINER(window),3);
	
	/////////////////////////////////////////
	//		Layout                 //
	/////////////////////////////////////////
	
	vbox = gtk_vbox_new(TRUE,1);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	
	/////////////////////////////////////////
	//		Image		       //	
	/////////////////////////////////////////
	
	img = gtk_image_new_from_file("Alona.jpeg");

	/////////////////////////////////////////
	//		BUTTON		       //
	/////////////////////////////////////////
	
	//btnled1 = gtk_button_new_with_label(" ");
	button = gtk_button_new_with_label("Back");
	
	/////////////////////////////////////////
	//		log in                 //
	/////////////////////////////////////////
	
	gtk_box_pack_start(GTK_BOX(vbox),img,TRUE,TRUE,0);
	//gtk_box_pack_start(GTK_BOX(vbox),btnled1,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(vbox),drawarea,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(vbox),button,TRUE,TRUE,0);
	/////////////////////////////////////////
	//              signal                 //
	///////////////////////////////////////// 

	gtk_signal_connect(GTK_OBJECT(button),"clicked",GTK_SIGNAL_FUNC(destroy),NULL);	
	
	/////////////////////////////////////////
	//		Thread                 //
	/////////////////////////////////////////
	// create thread
	g_thread_create((GThreadFunc)btncolor,NULL,FALSE,NULL);
	gtk_widget_show_all(drawarea);
	gtk_widget_show_all(window);
	
	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();
	return 0;
}
int pwm_duty(int duty)
{
	FILE *p = NULL;
	if((p = DUTY) == NULL){
		printf("Problem changed Duty\n");
		return -1;
	}else
		printf("Changed Duty :%d\n",duty);
	
	fprintf(p,"%d",duty);
	fclose(p);
	return 0;

}
int gpio_init(void){

	FILE *p = NULL;
	char gpio[50];

	if((p = EXPORT)==NULL){
		printf("Problem opening pwm am33xx_pwm \n");
		return -1;
	}else{
		printf("Opening pwm okay \n");
	}
	fprintf(p,"am33xx_pwm");
	fclose(p);
	if((p = PIN_EXPORT)== NULL){
		printf("Problem openong pwm P8_13 \n ");
		return -1;
	}
	else{
		printf("Opening pwm P8_13 okay\n");
	}
	fprintf(p,"bone_pwm_P8_13");
	fclose(p);

	if((p == DUTY) == NULL)
	{
		printf("Problem open duty \n");
		return -1;
	}
	else{
		printf("Open duty okay\n");
	}
	fprintf(p,"25000");
	fclose(p);

	if((p == PERIOD)== NULL)
	{
		printf("Problem open period\n");
		return -1;
	}
	else
		printf("Open period okay\n");

	fprintf(p,"60000");

	fclose(p);
	return 0;
}	
