#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define UNEXPORT	fopen("/sys/class/gpio/unexport","w")
#define EXPORT		fopen("/sys/class/gpio/export","w")
#define GSTRING(s,i,g)  sprintf(s,"/sys/class/gpio/gpio%d%s",i,g)
#define GDIRECTION      "/direction"
#define GVALUE          "/value"
#define GOPEN(s)	fopen((s),"w")

int gpio_cintrol(int pin , int value);
int gpio_init(int pin);
void hello(void)
{
	g_print("Hello World !\n");
}
void destroy(void)
{
	gtk_main_quit();
}

void led_one_on(void){
	gpio_init(26);
	gpio_close(27);
	printf("Led 1 On\n");
}
void led_two_on(void){
	
	printf("led 2 On\n");
	gpio_close(26);
	gpio_init(27);
}
void all_on(void)
{
	printf("All On");
	gpio_init(26);
	gpio_init(27);
}
void all_off(void)
{
	printf("All off \n");
	gpio_close(26);
	gpio_close(27);
}

int main (int argc,char *argv[]){

	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *vbox;
	GtkWidget *btnled1;
	GtkWidget *btnled2;
	GtkWidget *btnledalloff;
	GtkWidget *btnledallon;
	GtkWidget *img;
	gpio_init(26);
	gpio_init(27);
	gtk_init(&argc, &argv);
		
	//////////////////////////////////////////
	//              window			//
	//////////////////////////////////////////

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window),200,300);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	//gtk_signal_connect(GTK_OBJECT(window),"destory",GTK_SIGNAL_FUNC(destroy),NULL);
	gtk_container_border_width(GTK_CONTAINER(window),6);

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
	
	btnled2 = gtk_button_new_with_label("LED2 ON");
	btnled1 = gtk_button_new_with_label("LED1 ON");
	btnledallon = gtk_button_new_with_label("ALL ON");
	btnledalloff = gtk_button_new_with_label("ALL OFF");
	button = gtk_button_new_with_label("Back");
	
	/////////////////////////////////////////
	//		log in                 //
	/////////////////////////////////////////
	
	gtk_box_pack_start(GTK_BOX(vbox),img,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(vbox),btnled1,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(vbox),btnled2,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(vbox),btnledallon,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(vbox),btnledalloff,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(vbox),button,TRUE,TRUE,0);
	
	/////////////////////////////////////////
	//              signal                 //
	///////////////////////////////////////// 

	gtk_signal_connect(GTK_OBJECT(button),"clicked",GTK_SIGNAL_FUNC(hello),NULL);
	gtk_signal_connect_object(GTK_OBJECT(btnled1),"clicked",GTK_SIGNAL_FUNC(led_one_on),NULL);
	gtk_signal_connect_object(GTK_OBJECT(btnled2),"clicked",GTK_SIGNAL_FUNC(led_two_on),NULL);
	gtk_signal_connect_object(GTK_OBJECT(btnledallon),"clicked",GTK_SIGNAL_FUNC(all_on),NULL);
	gtk_signal_connect_object(GTK_OBJECT(btnledalloff),"clicked",GTK_SIGNAL_FUNC(all_off),NULL);
	gtk_signal_connect_object(GTK_OBJECT(button),"clicked",GTK_SIGNAL_FUNC(gtk_widget_destroy),GTK_OBJECT(window));
	//gtk_container_add(GTK_CONTAINER(window),button);
	//gtk_widget_show(button);

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
int gpio_close(int pin)
{
	FILE *p = NULL;
	char gpio[50];
	if((p = UNEXPORT)== NULL){
		printf("Error unexport ! %d\n",pin);
	}
	fprintf(p,"%d",pin);
	fclose(p);
}
int gpio_init(int pin){

	FILE *p = NULL;
	char gpio[50];

	if((p = EXPORT)==NULL){
		printf("Problem opening gpio %d\n",pin);
		return -1;
	}else{
		printf("Opening goip okay %d\n",pin);
	}
	fprintf(p,"%d",pin);
	fclose(p);

	GSTRING(gpio,pin,GDIRECTION);
	if((p = GOPEN(gpio)) == NULL){
		printf("Problem opening gpio %d\n %s \n",pin,gpio);
		return -1;
	}
	else{
		printf("Opening gpio okay %d\n%s\n",pin,gpio);
	}
	fprintf(p,"out");
	fclose(p);

	gpio_cintrol(pin , 1);
}
int gpio_cintrol(int pin , int value)
{
	FILE *p = NULL;
	char gpio[50];
	GSTRING(gpio,pin,GVALUE);
	if((p = GOPEN(gpio)) == NULL)
	{
		printf("Prpblem controlling %d\n",pin);
		return -1;
	}
	fprintf(p,"%d",value);
	pclose(p);
}
