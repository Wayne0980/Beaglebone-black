#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void hello(void)
{
	g_print("Hello World !\n");
}
void destroy(void)
{
	gtk_main_quit();
}
void *hellothread(void){
	while(1){
		gdk_threads_enter();
		sleep(1);
		printf("Hello Gtkthread \n");
		gdk_threads_leave();
	}
}
int main (int argc,char *argv[]){
	
	printf("Program Start !\n");
	printf("Gtk initial thread finish!\n");
	if(!g_thread_supported()) g_thread_init(NULL);
		gdk_threads_init();
	gtk_init(& argc,&argv);
	g_thread_create((GThreadFunc)hellothread,NULL,FALSE,NULL);
	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();
	return 0;
}

